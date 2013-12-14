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
#include "CCTCPSocketHub.h"
#include "support/zip_support/unzip.h"

NS_CC_BEGIN

CCTCPSocketHub::CCTCPSocketHub() {
	scheduleUpdate();
};

CCTCPSocketHub::~CCTCPSocketHub() {
	CCScheduler* s = CCDirector::sharedDirector()->getScheduler();
	s->unscheduleUpdateForTarget(this);
	
	for (CCTCPSocketList::iterator iter = m_lstSocket.begin(); iter != m_lstSocket.end(); ++iter) {
		int tag = (*iter)->getTag();
		CC_SAFE_RELEASE(*iter);
		CCTCPSocketListener* l = getListener(tag);
		if(l) {
			l->onTCPSocketDisconnected(tag);
		}
	}
}

CCTCPSocketHub* CCTCPSocketHub::create() {
	CCTCPSocketHub* h = new CCTCPSocketHub();
	return (CCTCPSocketHub*)h->autorelease();
}

CCTCPSocket* CCTCPSocketHub::createSocket(const string& hostname, int port, int tag, int blockSec, bool keepAlive) {
	CCTCPSocket* s = CCTCPSocket::create(hostname, port, tag, blockSec, keepAlive);
	if (s && addSocket(s)) {
		CCTCPSocketListener* l = getListener(tag);
		if(l) {
			l->onTCPSocketConnected(tag);
		}
	}
	
	return s;
}

CCTCPSocketListener* CCTCPSocketHub::getListener(int tag) {
	CCTCPSocketListenerMap::iterator iter = m_listenerMap.find(tag);
	if(iter != m_listenerMap.end()) {
		return iter->second;
	} else {
		return NULL;
	}
}

bool CCTCPSocketHub::addSocket(CCTCPSocket* socket) {
	for(CCTCPSocketList::iterator iter = m_lstSocket.begin(); iter != m_lstSocket.end(); ++ iter) {
		if((*iter)->getSocket() == socket->getSocket())
			return false;
	}
	m_lstSocket.push_back(socket);
	CC_SAFE_RETAIN(socket);
	return true;
}

bool CCTCPSocketHub::removeSocket(int tag) {
	for(CCTCPSocketList::iterator iter = m_lstSocket.begin(); iter != m_lstSocket.end(); ++ iter) {
		if((*iter)->getTag() == tag) {
			CC_SAFE_RELEASE(*iter);
			m_lstSocket.erase(iter);
			return true;
		}
	}
	return false;
}

CCTCPSocket* CCTCPSocketHub::getSocket(int tag) {
	for(CCTCPSocketList::iterator iter = m_lstSocket.begin(); iter != m_lstSocket.end(); ++ iter) {
		if((*iter)->getTag() == tag) {
			return *iter;
		}
	}
	return NULL;
}

void CCTCPSocketHub::update(float delta) {
	for(CCTCPSocketList::iterator iter = m_lstSocket.begin(); iter != m_lstSocket.end(); ++ iter) {
		CCTCPSocket* pSocket = *iter;
		int tag = pSocket->getTag();
		if (!pSocket->hasAvailable()) {
			CCTCPSocketListener* l = getListener(tag);
			if(l)
				l->onTCPSocketDisconnected(tag);
			CC_SAFE_RELEASE(*iter);
			m_lstSocket.erase(iter);
			break;
		}
		
		while (true) {
			char buffer[kCCSocketMaxPacketSize] = {0};
			int read = pSocket->receiveData(buffer, kCCSocketMaxPacketSize);
			if (read <= 0)
				break;
			CCByteBuffer packet;
			packet.write((uint8*)buffer, read);
			
			CCTCPSocketListener* l = getListener(tag);
			if(l) {
				l->onTCPSocketData(tag, packet);
			}
		}
	}
}

void CCTCPSocketHub::registerCallback(int tag, CCTCPSocketListener* callback) {
	m_listenerMap[tag] = callback;
}

bool CCTCPSocketHub::sendPacket(int tag, CCByteBuffer* packet) {
	for(CCTCPSocketList::iterator iter = m_lstSocket.begin(); iter != m_lstSocket.end(); ++ iter) {
		if ((*iter)->getTag() == tag) {
			(*iter)->sendData((void*)packet->getBuffer(), packet->available());
			return (*iter)->flush();
		}
	}
	
	return false;
}

void CCTCPSocketHub::disconnect(int tag) {
	for(CCTCPSocketList::iterator iter = m_lstSocket.begin(); iter != m_lstSocket.end(); ++ iter) {
		if((*iter)->getTag() == tag) {
			(*iter)->destroy();
			CCTCPSocketListener* l = getListener(tag);
			if(l) {
				l->onTCPSocketDisconnected(tag);
			}
			break;
		}
	}
}

NS_CC_END