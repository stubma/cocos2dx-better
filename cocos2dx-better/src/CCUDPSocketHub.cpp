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

static CC_MULTI_ENCRYPT_FUNC sEncryptFunc = NULL;
static CC_MULTI_DECRYPT_FUNC sDecryptFunc = NULL;

CCUDPSocketHub::CCUDPSocketHub() :
m_rawPolicy(false) {
    pthread_mutex_init(&m_mutex, NULL);
    
    CCScheduler* s = CCDirector::sharedDirector()->getScheduler();
    s->scheduleSelector(schedule_selector(CCUDPSocketHub::mainLoop), this, 0, false);
};

CCUDPSocketHub::~CCUDPSocketHub() {
    // release
    pthread_mutex_destroy(&m_mutex);
}

CCUDPSocketHub* CCUDPSocketHub::create(CC_MULTI_ENCRYPT_FUNC encryptFunc, CC_MULTI_DECRYPT_FUNC decryptFunc) {
	CCUDPSocketHub* h = new CCUDPSocketHub();
    sEncryptFunc = encryptFunc;
    sDecryptFunc = decryptFunc;
	return (CCUDPSocketHub*)h->autorelease();
}

CC_MULTI_ENCRYPT_FUNC CCUDPSocketHub::getEncryptFunc() {
    return sEncryptFunc;
}

CC_MULTI_DECRYPT_FUNC CCUDPSocketHub::getDecryptFunc() {
    return sDecryptFunc;
}

CCUDPSocket* CCUDPSocketHub::createSocket(const string& hostname, int port, int tag, int blockSec) {
	CCUDPSocket* s = CCUDPSocket::create(hostname, port, tag, blockSec);
	if(s)
        addSocket(s);
	return s;
}

void CCUDPSocketHub::stopAll() {
    // release socket
    CCObject* obj;
    CCARRAY_FOREACH(&m_sockets, obj) {
        CCUDPSocket* s = (CCUDPSocket*)obj;
        if(s->isConnected()) {
            s->m_connected = false;
            s->closeSocket();
            CCNotificationCenter::sharedNotificationCenter()->postNotification(kCCNotificationUDPSocketDisconnected, s);
        }
        s->setStop(true);
        s->setHub(NULL);
    }
    m_sockets.removeAllObjects();
    
    // stop update
    CCScheduler* s = CCDirector::sharedDirector()->getScheduler();
	s->unscheduleSelector(schedule_selector(CCUDPSocketHub::mainLoop), this);
}

void CCUDPSocketHub::onSocketConnectedThreadSafe(CCUDPSocket* s) {
    pthread_mutex_lock(&m_mutex);
    m_connectedSockets.addObject(s);
    pthread_mutex_unlock(&m_mutex);
}

void CCUDPSocketHub::onSocketDisconnectedThreadSafe(CCUDPSocket* s) {
    pthread_mutex_lock(&m_mutex);
    m_disconnectedSockets.addObject(s);
    pthread_mutex_unlock(&m_mutex);
}

void CCUDPSocketHub::onPacketReceivedThreadSafe(CCPacket* packet) {
    pthread_mutex_lock(&m_mutex);
    m_packets.addObject(packet);
    pthread_mutex_unlock(&m_mutex);
}

bool CCUDPSocketHub::addSocket(CCUDPSocket* socket) {
    CCObject* obj;
    CCARRAY_FOREACH(&m_sockets, obj) {
        CCUDPSocket* s = (CCUDPSocket*)obj;
        if(s->getSocket() == socket->getSocket()) {
            return false;
        }
    }
    m_sockets.addObject(socket);
    socket->setHub(this);
	return true;
}

CCUDPSocket* CCUDPSocketHub::getSocket(int tag) {
    CCObject* obj;
    CCARRAY_FOREACH(&m_sockets, obj) {
        CCUDPSocket* s = (CCUDPSocket*)obj;
        if(s->getTag() == tag) {
            return s;
        }
    }
	return NULL;
}

void CCUDPSocketHub::mainLoop(float delta) {
    pthread_mutex_lock(&m_mutex);
    
    // notification center
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    
    // connected events
    CCObject* obj;
    CCARRAY_FOREACH(&m_connectedSockets, obj) {
        CCUDPSocket* s = (CCUDPSocket*)obj;
        nc->postNotification(kCCNotificationUDPSocketConnected, s);
    }
    m_connectedSockets.removeAllObjects();
    
    // data event
    CCARRAY_FOREACH(&m_packets, obj) {
        CCPacket* p = (CCPacket*)obj;
        nc->postNotification(kCCNotificationPacketReceived, p);
    }
    m_packets.removeAllObjects();
    
    // disconnected event
    CCARRAY_FOREACH(&m_disconnectedSockets, obj) {
        CCUDPSocket* s = (CCUDPSocket*)obj;
        nc->postNotification(kCCNotificationUDPSocketDisconnected, s);
        m_sockets.removeObject(s);
    }
    m_disconnectedSockets.removeAllObjects();
    
    pthread_mutex_unlock(&m_mutex);
}

void CCUDPSocketHub::sendPacket(int tag, CCPacket* packet) {
    CCObject* obj;
    CCARRAY_FOREACH(&m_sockets, obj) {
        CCUDPSocket* s = (CCUDPSocket*)obj;
        if(s->getTag() == tag) {
            s->sendPacket(packet);
            break;
        }
    }
}

void CCUDPSocketHub::disconnect(int tag) {
    CCObject* obj;
    CCARRAY_FOREACH(&m_sockets, obj) {
        CCUDPSocket* s = (CCUDPSocket*)obj;
        if(s->getTag() == tag) {
            s->setStop(true);
            break;
        }
    }
}

NS_CC_END