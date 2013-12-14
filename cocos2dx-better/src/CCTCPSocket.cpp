#include "CCTCPSocket.h"
#include "support/zip_support/unzip.h"


NS_CC_BEGIN

CCTCPSocket::CCTCPSocket()
{
    // 初始化
    memset(m_bufOutput, 0, sizeof(m_bufOutput));
    memset(m_bufInput, 0, sizeof(m_bufInput));
}

void CCTCPSocket::closeSocket()
{
#ifdef WIN32
    closesocket(m_sockClient);
    WSACleanup();
#else
    close(m_sockClient);
#endif
}

bool CCTCPSocket::Create(const char* pszServerIP, int nServerPort, int tagid, int nBlockSec, bool bKeepAlive /*= FALSE*/)
{
    // 检查参数
    if(pszServerIP == 0 || strlen(pszServerIP) > 15) {
        return false;
    }
	
#ifdef WIN32
    WSADATA wsaData;
    WORD version = MAKEWORD(2, 0);
    int ret = WSAStartup(version, &wsaData);//win sock start up
    if (ret != 0) {
        return false;
    }
#endif
	
    // 创建主套接字
    m_sockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(m_sockClient == INVALID_SOCKET) {
        closeSocket();
        return false;
    }
	
    // 设置SOCKET为KEEPALIVE
    if(bKeepAlive)
    {
        int        optval=1;
        if(setsockopt(m_sockClient, SOL_SOCKET, SO_KEEPALIVE, (char *) &optval, sizeof(optval)))
        {
            closeSocket();
            return false;
        }
    }
	
#ifdef WIN32
    DWORD nMode = 1;
    int nRes = ioctlsocket(m_sockClient, FIONBIO, &nMode);
    if (nRes == SOCKET_ERROR) {
        closeSocket();
        return false;
    }
#else
    // 设置为非阻塞方式
    fcntl(m_sockClient, F_SETFL, O_NONBLOCK);
#endif
	
    unsigned long serveraddr = inet_addr(pszServerIP);
    if(serveraddr == INADDR_NONE)    // 检查IP地址格式错误
    {
        closeSocket();
        return false;
    }
	
    sockaddr_in    addr_in;
    memset((void *)&addr_in, 0, sizeof(addr_in));
    addr_in.sin_family = AF_INET;
    addr_in.sin_port = htons(nServerPort);
    addr_in.sin_addr.s_addr = serveraddr;
    
    if(connect(m_sockClient, (sockaddr *)&addr_in, sizeof(addr_in)) == SOCKET_ERROR) {
        if (hasError()) {
            closeSocket();
            return false;
        }
        else    // WSAWOLDBLOCK
        {
            timeval timeout;
            timeout.tv_sec    = nBlockSec;
            timeout.tv_usec    = 0;
            fd_set writeset, exceptset;
            FD_ZERO(&writeset);
            FD_ZERO(&exceptset);
            FD_SET(m_sockClient, &writeset);
            FD_SET(m_sockClient, &exceptset);
			
            int ret = select(FD_SETSIZE, NULL, &writeset, &exceptset, &timeout);
            if (ret == 0 || ret < 0) {
                closeSocket();
                return false;
            } else    // ret > 0
            {
                ret = FD_ISSET(m_sockClient, &exceptset);
                if(ret)        // or (!FD_ISSET(m_sockClient, &writeset)
                {
                    closeSocket();
                    return false;
                }
            }
        }
    }
	
    m_nInbufLen        = 0;
    m_nInbufStart    = 0;
    m_nOutbufLen    = 0;
	
    struct linger so_linger;
    so_linger.l_onoff = 1;
    so_linger.l_linger = 500;
    setsockopt(m_sockClient, SOL_SOCKET, SO_LINGER, (const char*)&so_linger, sizeof(so_linger));
    m_tag = tagid;
    return true;
}

bool CCTCPSocket::SendMsg(void* pBuf, int nSize)
{
    if(pBuf == 0 || nSize <= 0) {
        return false;
    }
	
    if (m_sockClient == INVALID_SOCKET) {
        return false;
    }
	
    // 检查通讯消息包长度
    int packsize = 0;
    packsize = nSize;
	
    // 检测BUF溢出
    if(m_nOutbufLen + nSize > OUTBUFSIZE) {
        // 立即发送OUTBUF中的数据，以清空OUTBUF。
        Flush();
        if(m_nOutbufLen + nSize > OUTBUFSIZE) {
            // 出错了
            Destroy();
            return false;
        }
    }
    // 数据添加到BUF尾
    memcpy(m_bufOutput + m_nOutbufLen, pBuf, nSize);
    m_nOutbufLen += nSize;
    return true;
}

bool CCTCPSocket::ReceiveMsg(void* pBuf, int& nSize)
{
    //检查参数
    if(pBuf == NULL || nSize <= 0) {
        return false;
    }
    
    if (m_sockClient == INVALID_SOCKET) {
        return false;
    }
	
    // 检查是否有一个消息(小于2则无法获取到消息长度)
    if(m_nInbufLen < 2) {
        //  如果没有请求成功  或者   如果没有数据则直接返回
        if(!recvFromSock() || m_nInbufLen < 2) {        // 这个m_nInbufLen更新了
            return false;
        }
    }
	
    // 计算要拷贝的消息的大小（一个消息，大小为整个消息的第一个16字节），因为环形缓冲区，所以要分开计算
    int packsize = (unsigned char)m_bufInput[m_nInbufStart+2] +
	(unsigned char)m_bufInput[(m_nInbufStart + 3) % INBUFSIZE] * 256; // 注意字节序，高位+低位
	
    // 检测消息包尺寸错误 暂定最大16k
    if (packsize <= 0 || packsize > _MAX_MSGSIZE) {
        m_nInbufLen = 0;        // 直接清空INBUF
        m_nInbufStart = 0;
        return false;
    }
	
    // 检查消息是否完整(如果将要拷贝的消息大于此时缓冲区数据长度，需要再次请求接收剩余数据)
    if (packsize > m_nInbufLen) {
        // 如果没有请求成功   或者    依然无法获取到完整的数据包  则返回，直到取得完整包
        if (!recvFromSock() || packsize > m_nInbufLen) {    // 这个m_nInbufLen已更新
            return false;
        }
    }
	
    // 复制出一个消息
    if(m_nInbufStart + packsize > INBUFSIZE) {
        // 如果一个消息有回卷（被拆成两份在环形缓冲区的头尾）
        // 先拷贝环形缓冲区末尾的数据
        int copylen = INBUFSIZE - m_nInbufStart;
        memcpy(pBuf, m_bufInput + m_nInbufStart, copylen);
		
        // 再拷贝环形缓冲区头部的剩余部分
        memcpy((unsigned char *)pBuf + copylen, m_bufInput, packsize - copylen);
        nSize = packsize;
    } else {
        // 消息没有回卷，可以一次拷贝出去
        memcpy(pBuf, m_bufInput + m_nInbufStart, packsize);
        nSize = packsize;
    }
	
    // 重新计算环形缓冲区头部位置
    m_nInbufStart = (m_nInbufStart + packsize) % INBUFSIZE;
    m_nInbufLen -= packsize;
    return    true;
}

bool CCTCPSocket::hasError()
{
	int err = errno;
	if(err != EINPROGRESS && err != EAGAIN) {
		return true;
	}
	
	return false;
}
	
// 从网络中读取尽可能多的数据，实际向服务器请求数据的地方
bool CCTCPSocket::recvFromSock(void)
{
	if (m_nInbufLen >= INBUFSIZE || m_sockClient == INVALID_SOCKET) {
		return false;
	}
	
	// 接收第一段数据
	int    savelen, savepos;            // 数据要保存的长度和位置
	if(m_nInbufStart + m_nInbufLen < INBUFSIZE)    {    // INBUF中的剩余空间有回绕
		savelen = INBUFSIZE - (m_nInbufStart + m_nInbufLen);        // 后部空间长度，最大接收数据的长度
	} else {
		savelen = INBUFSIZE - m_nInbufLen;
	}
	
	// 缓冲区数据的末尾
	savepos = (m_nInbufStart + m_nInbufLen) % INBUFSIZE;
	//CHECKF(savepos + savelen <= INBUFSIZE);
	int inlen = recv(m_sockClient, m_bufInput + savepos, savelen, 0);
	if(inlen > 0) {
		// 有接收到数据
		m_nInbufLen += inlen;
		
		if (m_nInbufLen > INBUFSIZE) {
			return false;
		}
		
		// 接收第二段数据(一次接收没有完成，接收第二段数据)
		if(inlen == savelen && m_nInbufLen < INBUFSIZE) {
			int savelen = INBUFSIZE - m_nInbufLen;
			int savepos = (m_nInbufStart + m_nInbufLen) % INBUFSIZE;
			//CHECKF(savepos + savelen <= INBUFSIZE);
			inlen = recv(m_sockClient, m_bufInput + savepos, savelen, 0);
			if(inlen > 0) {
				m_nInbufLen += inlen;
				if (m_nInbufLen > INBUFSIZE) {
					return false;
				}
			} else if(inlen == 0) {
				Destroy();
				return false;
			} else {
				// 连接已断开或者错误（包括阻塞）
				if (hasError()) {
					Destroy();
					return false;
				}
			}
		}
	} else if(inlen == 0) {
		Destroy();
		return false;
	} else {
		// 连接已断开或者错误（包括阻塞）
		if (hasError()) {
			Destroy();
			return false;
		}
	}
	
	return true;
}

bool CCTCPSocket::Flush(void)        //? 如果 OUTBUF > SENDBUF 则需要多次SEND（）
{
	if (m_sockClient == INVALID_SOCKET) {
		return false;
	}
	
	if(m_nOutbufLen <= 0) {
		return true;
	}
	
	// 发送一段数据
	int    outsize;
	outsize = send(m_sockClient, m_bufOutput, m_nOutbufLen, 0);
	if(outsize > 0) {
		// 删除已发送的部分
		if(m_nOutbufLen - outsize > 0) {
			memcpy(m_bufOutput, m_bufOutput + outsize, m_nOutbufLen - outsize);
		}
		
		m_nOutbufLen -= outsize;
		
		if (m_nOutbufLen < 0) {
			return false;
		}
	} else {
		if (hasError()) {
			Destroy();
			return false;
		}
	}
	
	return true;
}

bool CCTCPSocket::Check(void)
{
	// 检查状态
	if (m_sockClient == INVALID_SOCKET) {
		return false;
	}
	
	char buf[1];
	int    ret = recv(m_sockClient, buf, 1, MSG_PEEK);
	if(ret == 0) {
		Destroy();
		return false;
	} else if(ret < 0) {
		if (hasError()) {
			Destroy();
			return false;
		} else {    // 阻塞
			return true;
		}
	} else {    // 有数据
		return true;
	}
	
	return true;
}

void CCTCPSocket::Destroy(void)
{
	// 关闭
	struct linger so_linger;
	so_linger.l_onoff = 1;
	so_linger.l_linger = 500;
	int ret = setsockopt(m_sockClient, SOL_SOCKET, SO_LINGER, (const char*)&so_linger, sizeof(so_linger));
	
	closeSocket();
	
	m_sockClient = INVALID_SOCKET;
	m_nInbufLen = 0;
	m_nInbufStart = 0;
	m_nOutbufLen = 0;
	
	memset(m_bufOutput, 0, sizeof(m_bufOutput));
	memset(m_bufInput, 0, sizeof(m_bufInput));
}

CCTCPSocketManager *CCTCPSocketManager::mSingleton = 0;
CCTCPSocket *CCTCPSocketManager::createSocket(const char* pszServerIP,    // IP地址
										  int nServerPort,            // 端口
										  int _tag,                    // 标识ID
										  int nBlockSec,            // 阻塞时间ms
										  bool bKeepAlive)
{
	CCTCPSocket *pSocket = new CCTCPSocket();
	if (pSocket->Create(pszServerIP, nServerPort, _tag, nBlockSec, bKeepAlive))
	{
		assert(addSocket(pSocket));
		if (_pOnConnect){
			_pOnConnect(_tag);
		}
		return pSocket;
	}
	
	delete pSocket;
	return NULL;
}

bool    CCTCPSocketManager::addSocket(CCTCPSocket *pSocket)
{
	std::list<CCTCPSocket*>::iterator iter, enditer = m_lstSocket.end();
	for (iter = m_lstSocket.begin(); iter != enditer; ++ iter)
	{
		if ((*iter)->GetSocket() == pSocket->GetSocket())
			return false;
	}
	m_lstSocket.push_back(pSocket);
	return true;
}

// 删除socket
bool    CCTCPSocketManager::removeSocket(int _tag)
{
	std::list<CCTCPSocket*>::iterator iter, enditer = m_lstSocket.end();
	for (iter = m_lstSocket.begin(); iter != enditer; ++ iter)
	{
		if ((*iter)->getTagID() == _tag)
		{
			(*iter)->Destroy();
			m_lstSocket.erase(iter);
			return true;
		}
	}
	return false;
}

CCTCPSocket *CCTCPSocketManager::GetSocket(int _tag)
{
	std::list<CCTCPSocket*>::iterator iter, enditer = m_lstSocket.end();
	for (iter = m_lstSocket.begin(); iter != enditer; ++ iter)
	{
		if ((*iter)->getTagID() == _tag)
		{
			return *iter;
		}
	}
	return NULL;
}

void    CCTCPSocketManager::update(float delta)
{
	std::list<CCTCPSocket*>::iterator iter, enditer = m_lstSocket.end();
	for (iter = m_lstSocket.begin(); iter != enditer; ++ iter)
	{
		CCTCPSocket *pSocket = *iter;
		int _tag = pSocket->getTagID();
		if (!pSocket->Check())
		{// 掉线了
			_OnDisconnect(_tag);
			m_lstSocket.erase(iter);
			break;
		}
		
		while (true)
		{
			char buffer[_MAX_MSGSIZE] = {0};
			int nSize = sizeof(buffer);
			char *pbufMsg = buffer;
			if (!pSocket->ReceiveMsg(pbufMsg, nSize))
				break;
			CCByteBuffer packet;
			uint16  _cmd, _length;
			packet.write((uint8*)pbufMsg, nSize);
			_cmd = packet.read<uint16>();
			_length = packet.read<uint16>();
			if (_cmd & 0x8000)
			{
				Byte uncompr[1024] = {0};
				uLong uncomprLen;
				_cmd &= 0x7fff;
				uncompress(uncompr, &uncomprLen, packet.getBuffer()+4, packet.available()-4);
			}
			
			if (_pProcess == 0 || _pProcess(pSocket->getTagID(), _cmd, packet))
			{// 分发数据
				std::map<uint16, ProFunc>::iterator mapi = _mapProcess.find(_cmd);
				if (mapi == _mapProcess.end())
					continue;
				mapi->second(pSocket->getTagID(), packet);
			}
		}
	}
}

void    CCTCPSocketManager::register_process(const uint16 &entry, ProFunc callback)
{
	_mapProcess[entry] = callback;
}

bool    CCTCPSocketManager::SendPacket(int _tag, CCByteBuffer *packet)
{
	std::list<CCTCPSocket*>::iterator iter, enditer = m_lstSocket.end();
	for (iter = m_lstSocket.begin(); iter != enditer; ++ iter)
	{
		if ((*iter)->getTagID() == _tag)
		{
			(*iter)->SendMsg((void *)packet->getBuffer(), packet->available());
			return (*iter)->Flush();
		}
	}
	
	return false;
}

void    CCTCPSocketManager::disconnect(int _tag)
{
	std::list<CCTCPSocket*>::iterator iter, enditer = m_lstSocket.end();
	for (iter = m_lstSocket.begin(); iter != enditer; ++ iter)
	{
		if ((*iter)->getTagID() == _tag)
		{
			(*iter)->Destroy();
			if (_OnDisconnect){
				_OnDisconnect(_tag);
			}
			break;
		}
	}
}

NS_CC_END