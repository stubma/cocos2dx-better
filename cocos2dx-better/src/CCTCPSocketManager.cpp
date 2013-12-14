/****************************************************************************
 Author: Luma (stubma@gmail.com)
 
 https://github.com/stubma/cocos2dx-better
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#include "CCTCPSocketManager.h"
#include "support/zip_support/unzip.h"

NS_CC_BEGIN

CCTCPSocketManager *CCTCPSocketManager::mSingleton = 0;

CCTCPSocketManager::~CCTCPSocketManager() {
	for (list<CCTCPSocket*>::iterator iter = m_lstSocket.begin(); iter != m_lstSocket.end(); ++iter) {
		CC_SAFE_RELEASE(*iter);
	}
}

CCTCPSocket *CCTCPSocketManager::createSocket(const string& hostname, int port, int tag, int blockSec, bool keepAlive)
{
	CCTCPSocket *pSocket = CCTCPSocket::create(hostname, port, tag, blockSec, keepAlive);
	if (pSocket)
	{
		assert(addSocket(pSocket));
		if (_pOnConnect){
			_pOnConnect(tag);
		}
	}
	
	return pSocket;
}

bool    CCTCPSocketManager::addSocket(CCTCPSocket *pSocket)
{
	for (list<CCTCPSocket*>::iterator iter = m_lstSocket.begin(); iter != m_lstSocket.end(); ++ iter)
	{
		if ((*iter)->getSocket() == pSocket->getSocket())
			return false;
	}
	m_lstSocket.push_back(pSocket);
	CC_SAFE_RETAIN(pSocket);
	return true;
}

// 删除socket
bool    CCTCPSocketManager::removeSocket(int _tag)
{
	std::list<CCTCPSocket*>::iterator iter, enditer = m_lstSocket.end();
	for (iter = m_lstSocket.begin(); iter != enditer; ++ iter)
	{
		if ((*iter)->getTag() == _tag)
		{
			CC_SAFE_RELEASE(*iter);
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
		if ((*iter)->getTag() == _tag)
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
		int _tag = pSocket->getTag();
		if (!pSocket->hasAvailable())
		{// 掉线了
			_OnDisconnect(_tag);
			m_lstSocket.erase(iter);
			break;
		}
		
		while (true)
		{
			char buffer[kCCSocketMaxPacketSize] = {0};
			int nSize = sizeof(buffer);
			char *pbufMsg = buffer;
			if (!pSocket->receiveData(pbufMsg, nSize))
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
			
			if (_pProcess == 0 || _pProcess(pSocket->getTag(), _cmd, packet))
			{// 分发数据
				std::map<uint16, ProFunc>::iterator mapi = _mapProcess.find(_cmd);
				if (mapi == _mapProcess.end())
					continue;
				mapi->second(pSocket->getTag(), packet);
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
		if ((*iter)->getTag() == _tag)
		{
			(*iter)->sendData((void *)packet->getBuffer(), packet->available());
			return (*iter)->flush();
		}
	}
	
	return false;
}

void    CCTCPSocketManager::disconnect(int _tag)
{
	std::list<CCTCPSocket*>::iterator iter, enditer = m_lstSocket.end();
	for (iter = m_lstSocket.begin(); iter != enditer; ++ iter)
	{
		if ((*iter)->getTag() == _tag)
		{
			(*iter)->destroy();
			if (_OnDisconnect){
				_OnDisconnect(_tag);
			}
			break;
		}
	}
}

NS_CC_END