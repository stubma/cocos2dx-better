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

NS_CC_BEGIN

static CC_MULTI_ENCRYPT_FUNC sEncryptFunc = NULL;
static CC_MULTI_DECRYPT_FUNC sDecryptFunc = NULL;

CCTCPSocketHub::CCTCPSocketHub() :
m_rawPolicy(false) {
    pthread_mutex_init(&m_mutex, NULL);
    
    // start main loop
    CCScheduler* s = CCDirector::sharedDirector()->getScheduler();
    s->scheduleSelector(schedule_selector(CCTCPSocketHub::mainLoop), this, 0, false);
}

CCTCPSocketHub::~CCTCPSocketHub() {
    // release
    pthread_mutex_destroy(&m_mutex);
}

CCTCPSocketHub* CCTCPSocketHub::create(CC_MULTI_ENCRYPT_FUNC encryptFunc, CC_MULTI_DECRYPT_FUNC decryptFunc) {
	CCTCPSocketHub* h = new CCTCPSocketHub();
    sEncryptFunc = encryptFunc;
    sDecryptFunc = decryptFunc;
	return (CCTCPSocketHub*)h->autorelease();
}

CC_MULTI_ENCRYPT_FUNC CCTCPSocketHub::getEncryptFunc() {
    return sEncryptFunc;
}

CC_MULTI_DECRYPT_FUNC CCTCPSocketHub::getDecryptFunc() {
    return sDecryptFunc;
}

void CCTCPSocketHub::stopAll() {
    // release socket
    CCObject* obj;
    CCARRAY_FOREACH(&m_sockets, obj) {
        CCTCPSocket* s = (CCTCPSocket*)obj;
        if(s->isConnected()) {
            s->m_connected = false;
            s->closeSocket();
            CCNotificationCenter::sharedNotificationCenter()->postNotification(kCCNotificationTCPSocketDisconnected, s);
        }
        s->setStop(true);
        s->setHub(NULL);
    }
    m_sockets.removeAllObjects();
    
    // stop update
    CCScheduler* s = CCDirector::sharedDirector()->getScheduler();
	s->unscheduleSelector(schedule_selector(CCTCPSocketHub::mainLoop), this);
}

CCTCPSocket* CCTCPSocketHub::createSocket(const string& hostname, int port, int tag, int blockSec, bool keepAlive) {
	CCTCPSocket* s = CCTCPSocket::create(hostname, port, tag, blockSec, keepAlive);
	if(s)
        addSocket(s);
	return s;
}

void CCTCPSocketHub::onSocketConnectedThreadSafe(CCTCPSocket* s) {
    pthread_mutex_lock(&m_mutex);
    m_connectedSockets.addObject(s);
    pthread_mutex_unlock(&m_mutex);
}

void CCTCPSocketHub::onSocketDisconnectedThreadSafe(CCTCPSocket* s) {
    pthread_mutex_lock(&m_mutex);
    m_disconnectedSockets.addObject(s);
    pthread_mutex_unlock(&m_mutex);
}

void CCTCPSocketHub::onPacketReceivedThreadSafe(CCPacket* packet) {
    pthread_mutex_lock(&m_mutex);
    m_packets.addObject(packet);
    pthread_mutex_unlock(&m_mutex);
}

bool CCTCPSocketHub::addSocket(CCTCPSocket* socket) {
    CCObject* obj;
    CCARRAY_FOREACH(&m_sockets, obj) {
        CCTCPSocket* s = (CCTCPSocket*)obj;
        if(s->getSocket() == socket->getSocket()) {
            return false;
        }
    }
    m_sockets.addObject(socket);
    socket->setHub(this);
	return true;
}

CCTCPSocket* CCTCPSocketHub::getSocket(int tag) {
    CCObject* obj;
    CCARRAY_FOREACH(&m_sockets, obj) {
        CCTCPSocket* s = (CCTCPSocket*)obj;
        if(s->getTag() == tag) {
            return s;
        }
    }
	return NULL;
}

void CCTCPSocketHub::mainLoop(float delta) {
    pthread_mutex_lock(&m_mutex);
    
    // notification center
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    
    // connected events
    CCObject* obj;
    CCARRAY_FOREACH(&m_connectedSockets, obj) {
        CCTCPSocket* s = (CCTCPSocket*)obj;
        nc->postNotification(kCCNotificationTCPSocketConnected, s);
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
        CCTCPSocket* s = (CCTCPSocket*)obj;
        nc->postNotification(kCCNotificationTCPSocketDisconnected, s);
        m_sockets.removeObject(s);
    }
    m_disconnectedSockets.removeAllObjects();
    
    pthread_mutex_unlock(&m_mutex);
}

void CCTCPSocketHub::sendPacket(int tag, CCPacket* packet) {
    CCObject* obj;
    CCARRAY_FOREACH(&m_sockets, obj) {
        CCTCPSocket* s = (CCTCPSocket*)obj;
        if(s->getTag() == tag) {
            s->sendPacket(packet);
            break;
        }
    }
}

void CCTCPSocketHub::disconnect(int tag) {
    CCObject* obj;
    CCARRAY_FOREACH(&m_sockets, obj) {
        CCTCPSocket* s = (CCTCPSocket*)obj;
        if(s->getTag() == tag) {
            s->setStop(true);
            break;
        }
    }
}

NS_CC_END