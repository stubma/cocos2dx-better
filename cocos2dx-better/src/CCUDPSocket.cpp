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
#include "CCUDPSocket.h"
#include "CCPacket.h"
#include "CCUDPSocketHub.h"

NS_CC_BEGIN

CCUDPSocket::CCUDPSocket() :
m_socket(kCCSocketInvalid),
m_tag(-1) {
    pthread_mutex_init(&m_mutex, NULL);
}

CCUDPSocket::~CCUDPSocket() {
    pthread_mutex_destroy(&m_mutex);
}

CCUDPSocket* CCUDPSocket::create(const string& hostname, int port, int tag, int blockSec) {
	CCUDPSocket* s = new CCUDPSocket();
	if(s->init(hostname, port, tag)) {
		return (CCUDPSocket*)s->autorelease();
	}
	
	s->release();
	return NULL;
}

void* CCUDPSocket::udpThreadEntry(void* arg) {
    // autorelease pool
	CCThread thread;
	thread.createAutoreleasePool();
	
	// get socket
	CCUDPSocket* s = (CCUDPSocket*)arg;
	
    // client address
	sockaddr_in my_addr;
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = INADDR_ANY;
	my_addr.sin_port = 0;
	
	// bind
	if(bind(s->m_socket, (sockaddr*)&my_addr, sizeof(my_addr)) == kCCSocketError && s->hasError()) {
        s->closeSocket();
	} else {
     	// select it
        timeval timeout;
        timeout.tv_sec = s->m_blockSec;
        timeout.tv_usec = 0;
        fd_set writeset, exceptset;
        FD_ZERO(&writeset);
        FD_ZERO(&exceptset);
        FD_SET(s->m_socket, &writeset);
        FD_SET(s->m_socket, &exceptset);
        int ret = select(FD_SETSIZE, NULL, &writeset, &exceptset, &timeout);
        if (ret == 0 || ret < 0) {
            s->closeSocket();
        } else {
            ret = FD_ISSET(s->m_socket, &exceptset);
            if(ret) {
                s->closeSocket();
            } else {
                s->m_connected = true;
                if(s->m_hub)
                    s->m_hub->onSocketConnectedThreadSafe(s);
            }
        }
    }
	
	// delay 500ms when socket closed
    struct linger so_linger;
    so_linger.l_onoff = 1;
    so_linger.l_linger = 500;
    setsockopt(s->m_socket, SOL_SOCKET, SO_LINGER, (const char*)&so_linger, sizeof(so_linger));
    
    // read/write loop
    CCPacket* p = NULL;
    while(!s->m_stop && s->m_connected && s->getSocket() != kCCSocketInvalid) {
        s->recvFromSock();
        if(s->m_inBufLen > 0) {
            CCPacket* p = NULL;
            if(s->m_hub) {
                if(s->m_hub->isRawPolicy()) {
                    p = CCPacket::createRawPacket(s->m_inBuf, s->m_inBufLen);
                } else {
                    p = CCPacket::createStandardPacket(s->m_inBuf, s->m_inBufLen);
                }
                s->m_hub->onPacketReceivedThreadSafe(p);
            }
        }
        
        // get packet to be sent
        if(!p && s->m_sendQueue.count() > 0) {
            pthread_mutex_lock(&s->m_mutex);
            p = (CCPacket*)s->m_sendQueue.objectAtIndex(0);
            CC_SAFE_RETAIN(p);
            s->m_sendQueue.removeObjectAtIndex(0);
            pthread_mutex_unlock(&s->m_mutex);
        }
        
        // send current packet
        if(p) {
            ssize_t sent = sendto(s->m_socket, p->getBuffer(), p->getPacketLength(), 0, (sockaddr*)&s->m_srvAddr, sizeof(s->m_srvAddr));
            if(sent >= p->getPacketLength()) {
                CC_SAFE_RELEASE(p);
                p = NULL;
            } else {
                if (s->hasError()) {
                    s->closeSocket();
                }
            }
        }
    }
    
    // end
    if(s->m_connected) {
        if(s->m_hub)
            s->m_hub->onSocketDisconnectedThreadSafe(s);
        s->m_connected = false;
    }
	
	// release
	s->autorelease();
    
    // exit
    pthread_exit(NULL);
	
	return NULL;
}

bool CCUDPSocket::init(const string& hostname, int port, int tag, int blockSec) {
	// check
    if(hostname.empty() || hostname.length() > 15) {
        return false;
    }
	
	// create tcp socket
    m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(m_socket == kCCSocketInvalid) {
        closeSocket();
        return false;
    }
	
	// non-block mode
    fcntl(m_socket, F_SETFL, O_NONBLOCK);
	
	// validate host name
    in_addr_t serveraddr = inet_addr(hostname.c_str());
    if(serveraddr == INADDR_NONE) {
        closeSocket();
        return false;
    }
	
	// server address
    memset((void *)&m_srvAddr, 0, sizeof(m_srvAddr));
    m_srvAddr.sin_family = AF_INET;
    m_srvAddr.sin_port = htons(port);
    m_srvAddr.sin_addr.s_addr = serveraddr;
	
	// save info
    m_hostname = hostname;
    m_port = port;
    m_tag = tag;
    m_blockSec = blockSec;
    m_inBufLen = 0;
	
    // open a thread to process this socket
    // should hold it to avoid wrong pointer
	CC_SAFE_RETAIN(this);
	pthread_t thread;
	pthread_create(&thread, NULL, udpThreadEntry, (void*)this);
    pthread_detach(thread);
    
	return true;
}

void CCUDPSocket::sendPacket(CCPacket* p) {
    pthread_mutex_lock(&m_mutex);
    m_sendQueue.addObject(p);
    pthread_mutex_unlock(&m_mutex);
}

void CCUDPSocket::recvFromSock() {
	socklen_t len = sizeof(m_srvAddr);
	m_inBufLen = recvfrom(m_socket, m_inBuf, kCCSocketInputBufferDefaultSize, 0, (sockaddr*)&m_srvAddr, &len);
}

bool CCUDPSocket::hasError() {
	int err = errno;
	if(err != EINPROGRESS && err != EAGAIN) {
		return true;
	}
	
	return false;
}

void CCUDPSocket::closeSocket() {
    if(m_socket != kCCSocketInvalid) {
        close(m_socket);
        m_socket = kCCSocketInvalid;
    }
}

NS_CC_END