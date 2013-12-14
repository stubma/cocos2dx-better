#include "CCTCPSocketManager.h"
#include "support/zip_support/unzip.h"


NS_CC_BEGIN

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