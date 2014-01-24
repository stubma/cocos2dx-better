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
#include "CCUDPSocketHub.h"

NS_CC_BEGIN

CCUDPSocketHub::CCUDPSocketHub() {
    CCScheduler* s = CCDirector::sharedDirector()->getScheduler();
    s->scheduleSelector(schedule_selector(CCUDPSocketHub::mainLoop), this, 0, false);
};

CCUDPSocketHub::~CCUDPSocketHub() {
	CCScheduler* s = CCDirector::sharedDirector()->getScheduler();
	s->unscheduleSelector(schedule_selector(CCUDPSocketHub::mainLoop), this);
	
	for (CCUDPSocketList::iterator iter = m_lstSocket.begin(); iter != m_lstSocket.end(); ++iter) {
		int tag = (*iter)->getTag();
		CC_SAFE_RELEASE(*iter);
		CCUDPSocketListener* l = getListener(tag);
		if(l) {
			l->onUDPSocketClosed(tag);
		}
	}
}

CCUDPSocketHub* CCUDPSocketHub::create() {
	CCUDPSocketHub* h = new CCUDPSocketHub();
	return (CCUDPSocketHub*)h->autorelease();
}

CCUDPSocket* CCUDPSocketHub::createSocket(const string& hostname, int port, int tag, int blockSec) {
	CCUDPSocket* s = CCUDPSocket::create(hostname, port, tag, blockSec);
	if (s && addSocket(s)) {
		CCUDPSocketListener* l = getListener(tag);
		if(l) {
			l->onUDPSocketBound(tag);
		}
	}
	
	return s;
}

CCUDPSocketListener* CCUDPSocketHub::getListener(int tag) {
	CCUDPSocketListenerMap::iterator iter = m_listenerMap.find(tag);
	if(iter != m_listenerMap.end()) {
		return iter->second;
	} else {
		return NULL;
	}
}

bool CCUDPSocketHub::addSocket(CCUDPSocket* socket) {
	for(CCUDPSocketList::iterator iter = m_lstSocket.begin(); iter != m_lstSocket.end(); ++ iter) {
		if((*iter)->getSocket() == socket->getSocket())
			return false;
	}
	m_lstSocket.push_back(socket);
	CC_SAFE_RETAIN(socket);
	return true;
}

bool CCUDPSocketHub::removeSocket(int tag) {
	for(CCUDPSocketList::iterator iter = m_lstSocket.begin(); iter != m_lstSocket.end(); ++ iter) {
		if((*iter)->getTag() == tag) {
			CC_SAFE_RELEASE(*iter);
			m_lstSocket.erase(iter);
			return true;
		}
	}
	return false;
}

CCUDPSocket* CCUDPSocketHub::getSocket(int tag) {
	for(CCUDPSocketList::iterator iter = m_lstSocket.begin(); iter != m_lstSocket.end(); ++ iter) {
		if((*iter)->getTag() == tag) {
			return *iter;
		}
	}
	return NULL;
}

void CCUDPSocketHub::mainLoop(float delta) {
	for(CCUDPSocketList::iterator iter = m_lstSocket.begin(); iter != m_lstSocket.end(); ++ iter) {
		CCUDPSocket* pSocket = *iter;
		int tag = pSocket->getTag();
		
		while (true) {
			int read = pSocket->receiveData(m_buffer, kCCSocketMaxPacketSize);
			if (read <= 0)
				break;
			CCByteBuffer packet;
			packet.write((uint8*)m_buffer, read);
			
			CCUDPSocketListener* l = getListener(tag);
			if(l) {
				l->onUDPSocketData(tag, packet);
			}
		}
	}
}

void CCUDPSocketHub::registerCallback(int tag, CCUDPSocketListener* callback) {
	m_listenerMap[tag] = callback;
}

void CCUDPSocketHub::unregisterCallback(int tag) {
    CCUDPSocketListenerMap::iterator iter = m_listenerMap.find(tag);
    if(iter != m_listenerMap.end()) {
        m_listenerMap.erase(iter);
    }
}

bool CCUDPSocketHub::sendPacket(int tag, CCByteBuffer* packet) {
	for(CCUDPSocketList::iterator iter = m_lstSocket.begin(); iter != m_lstSocket.end(); ++ iter) {
		if ((*iter)->getTag() == tag) {
			(*iter)->sendData((void*)packet->getBuffer(), packet->available());
		}
	}
	
	return false;
}

void CCUDPSocketHub::disconnect(int tag) {
	for(CCUDPSocketList::iterator iter = m_lstSocket.begin(); iter != m_lstSocket.end(); ++ iter) {
		if((*iter)->getTag() == tag) {
			(*iter)->destroy();
			CCUDPSocketListener* l = getListener(tag);
			if(l) {
				l->onUDPSocketClosed(tag);
			}
			break;
		}
	}
}

NS_CC_END