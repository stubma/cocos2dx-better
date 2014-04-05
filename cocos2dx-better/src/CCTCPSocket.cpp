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
#include "CCTCPSocket.h"
#include "CCPacket.h"
#include "CCTCPSocketHub.h"

NS_CC_BEGIN

CCTCPSocket::CCTCPSocket() :
m_socket(kCCSocketInvalid),
m_connected(false),
m_stop(false) {
    pthread_mutex_init(&m_mutex, NULL);
    memset(m_inBuf, 0, sizeof(m_inBuf));
}

CCTCPSocket::~CCTCPSocket() {
    closeSocket();
    pthread_mutex_destroy(&m_mutex);
}

CCTCPSocket* CCTCPSocket::create(const string& hostname, int port, int tag, int blockSec, bool keepAlive) {
	CCTCPSocket* s = new CCTCPSocket();
	if(s->init(hostname, port, tag, blockSec, keepAlive)) {
		return (CCTCPSocket*)s->autorelease();
	}
	
	s->release();
	return NULL;
}

void CCTCPSocket::closeSocket() {
    if(m_socket != kCCSocketInvalid) {
        close(m_socket);
        m_socket = kCCSocketInvalid;
    }
}

void CCTCPSocket::compactInBuf(int consumed) {
    if(consumed < m_inBufLen) {
        memmove(m_inBuf, m_inBuf + consumed, m_inBufLen - consumed);
        m_inBufLen -= consumed;
    } else {
        m_inBufLen = 0;
    }
}

void CCTCPSocket::sendPacket(CCPacket* p) {
    pthread_mutex_lock(&m_mutex);
    m_sendQueue.addObject(p);
    pthread_mutex_unlock(&m_mutex);
}

void* CCTCPSocket::tcpThreadEntry(void* arg) {
	// autorelease pool
	CCThread thread;
	thread.createAutoreleasePool();
	
	// get socket
	CCTCPSocket* s = (CCTCPSocket*)arg;
	
	// create address
    sockaddr_in addr_in;
    memset((void *)&addr_in, 0, sizeof(addr_in));
    addr_in.sin_family = AF_INET;
    addr_in.sin_port = htons(s->m_port);
    addr_in.sin_addr.s_addr = inet_addr(s->m_hostname.c_str());;
    
	// connect
	// if has error, close it
	// if success, select it with specified timeout
    if(connect(s->m_socket, (sockaddr*)&addr_in, sizeof(addr_in)) == kCCSocketError && s->hasError()) {
        s->closeSocket();
    } else {
     	// delay 500ms when socket closed
        struct linger so_linger;
        so_linger.l_onoff = 1;
        so_linger.l_linger = 500;
        setsockopt(s->m_socket, SOL_SOCKET, SO_LINGER, (const char*)&so_linger, sizeof(so_linger));
        
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
        if (ret < 0) {
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
    
    // read/write loop
    CCPacket* p = NULL;
    ssize_t sent = 0;
    while(!s->m_stop && s->m_connected && s->getSocket() != kCCSocketInvalid) {
        s->recvFromSock();
        if(s->m_inBufLen > 0) {
            CCPacket* p = NULL;
            if(s->m_hub->isRawPolicy()) {
                p = CCPacket::createRawPacket(s->m_inBuf, s->m_inBufLen);
            } else {
                p = CCPacket::createStandardPacket(s->m_inBuf, s->m_inBufLen);
            }
            s->compactInBuf(p->getPacketLength());
            if(s->m_hub)
                s->m_hub->onPacketReceivedThreadSafe(p);
        }
        
        // get packet to be sent
        if(!p && s->m_sendQueue.count() > 0) {
            pthread_mutex_lock(&s->m_mutex);
            p = (CCPacket*)s->m_sendQueue.objectAtIndex(0);
            CC_SAFE_RETAIN(p);
            s->m_sendQueue.removeObjectAtIndex(0);
            sent = 0;
            pthread_mutex_unlock(&s->m_mutex);
        }
        
        // send current packet
        if(p) {
            ssize_t outsize = send(s->m_socket, p->getBuffer() + sent, p->getPacketLength() - sent, 0);
            if(outsize > 0) {
                // move cursor
                sent += outsize;
                
                // any more?
                if (sent >= p->getPacketLength()) {
                    CC_SAFE_RELEASE(p);
                    p = NULL;
                }
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

bool CCTCPSocket::init(const string& hostname, int port, int tag, int blockSec, bool keepAlive) {
	// check
    if(hostname.empty() || hostname.length() > 15) {
        return false;
    }
	
	// create tcp socket
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(m_socket == kCCSocketInvalid) {
        closeSocket();
        return false;
    }
	
    // keep alive or not
    if(keepAlive) {
        int optVal = 1;
        if(setsockopt(m_socket, SOL_SOCKET, SO_KEEPALIVE, (char*)&optVal, sizeof(optVal))) {
            closeSocket();
            return false;
        }
    }
    
    // validate host name
    in_addr_t serveraddr = inet_addr(hostname.c_str());
    if(serveraddr == INADDR_NONE) {
        closeSocket();
        return false;
    }
    
    // nonblock
    fcntl(m_socket, F_SETFL, O_NONBLOCK);
    
	// save info
    m_hostname = hostname;
    m_port = port;
    m_tag = tag;
	m_blockSec = blockSec;
    
    // reset buffer
    m_inBufLen = 0;
    
    // open a thread to process this socket
    // should hold it to avoid wrong pointer
	CC_SAFE_RETAIN(this);
    pthread_t thread;
	pthread_create(&thread, NULL, tcpThreadEntry, (void*)this);
	pthread_detach(thread);
    
    return true;
}

bool CCTCPSocket::hasError() {
	int err = errno;
	if(err != EINPROGRESS && err != EAGAIN) {
		return true;
	}
	
	return false;
}

bool CCTCPSocket::recvFromSock() {
	// basic check
	if (m_inBufLen >= kCCSocketInputBufferDefaultSize || m_socket == kCCSocketInvalid) {
		return false;
	}
	
	// max byte can be hold by read buffer and the write position
	int savelen = kCCSocketInputBufferDefaultSize - m_inBufLen;
	
	// read to buffer end
	int savepos = m_inBufLen;
	ssize_t inlen = recv(m_socket, m_inBuf + savepos, savelen, 0);
	if(inlen > 0) {
		m_inBufLen += inlen;
		if (m_inBufLen > kCCSocketInputBufferDefaultSize) {
			return false;
		}
	} else if(inlen == 0) {
        closeSocket();
		return false;
	} else {
		if (hasError()) {
            closeSocket();
			return false;
		}
	}
	
	return true;
}

bool CCTCPSocket::hasAvailable() {
	// basic check
	if (m_socket == kCCSocketInvalid) {
		return false;
	}
	
	char buf[1];
	ssize_t ret = recv(m_socket, buf, 1, MSG_PEEK);
	if(ret == 0) {
		closeSocket();
		return false;
	} else if(ret < 0) {
		if (hasError()) {
			closeSocket();
			return false;
		} else {
			return true;
		}
	} else {
		return true;
	}
}

NS_CC_END