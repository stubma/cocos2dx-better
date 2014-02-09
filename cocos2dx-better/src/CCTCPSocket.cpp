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
#include <pthread.h>

NS_CC_BEGIN

CCTCPSocket::CCTCPSocket() :
m_sock(kCCSocketInvalid),
m_alreadyConnected(false),
m_connected(false) {
    memset(m_outBuf, 0, sizeof(m_outBuf));
    memset(m_inBuf, 0, sizeof(m_inBuf));
}

CCTCPSocket::~CCTCPSocket() {
	destroy();
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
    close(m_sock);
}

void* CCTCPSocket::waitingConnectThreadEntry(void* arg) {
	// autorelease pool
	CCThread thread;
	thread.createAutoreleasePool();
	
	// get socket
	CCTCPSocket* s = (CCTCPSocket*)arg;
	
	// select it
	timeval timeout;
	timeout.tv_sec = s->m_blockSec;
	timeout.tv_usec = 0;
	fd_set writeset, exceptset;
	FD_ZERO(&writeset);
	FD_ZERO(&exceptset);
	FD_SET(s->m_sock, &writeset);
	FD_SET(s->m_sock, &exceptset);
	int ret = select(FD_SETSIZE, NULL, &writeset, &exceptset, &timeout);
	if (ret < 0) {
		s->destroy();
	} else {
		ret = FD_ISSET(s->m_sock, &exceptset);
		if(ret) {
			s->destroy();
		} else {
			s->m_connected = true;
		}
	}
	
	// release
	s->autorelease();
	
	return NULL;
}

bool CCTCPSocket::init(const string& hostname, int port, int tag, int blockSec, bool keepAlive) {
	// check
    if(hostname.empty() || hostname.length() > 15) {
        return false;
    }
	
	// create tcp socket
    m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(m_sock == kCCSocketInvalid) {
        closeSocket();
        return false;
    }
	
    // keep alive or not
    if(keepAlive) {
        int optVal = 1;
        if(setsockopt(m_sock, SOL_SOCKET, SO_KEEPALIVE, (char*)&optVal, sizeof(optVal))) {
            closeSocket();
            return false;
        }
    }
	
	// non-block mode
    fcntl(m_sock, F_SETFL, O_NONBLOCK);
	
	// validate host name
    unsigned long serveraddr = inet_addr(hostname.c_str());
    if(serveraddr == INADDR_NONE) {
        closeSocket();
        return false;
    }
	
	// create address
    sockaddr_in addr_in;
    memset((void *)&addr_in, 0, sizeof(addr_in));
    addr_in.sin_family = AF_INET;
    addr_in.sin_port = htons(port);
    addr_in.sin_addr.s_addr = serveraddr;
    
	// connect
	// if has error, close it
	// if success, select it with specified timeout
    if(connect(m_sock, (sockaddr*)&addr_in, sizeof(addr_in)) == kCCSocketError) {
        if (hasError()) {
            closeSocket();
            return false;
        }
		
		// connected
		if(errno != EINPROGRESS) {
			m_connected = true;
		}
    }
	
	// reset buffer
    m_inBufLen = 0;
    m_inBufStart = 0;
    m_outBufLen = 0;
	
	// delay 500ms when socket closed
    struct linger so_linger;
    so_linger.l_onoff = 1;
    so_linger.l_linger = 500;
    setsockopt(m_sock, SOL_SOCKET, SO_LINGER, (const char*)&so_linger, sizeof(so_linger));
	
	// save tag
    m_tag = tag;
	
	// select it, we must retain self to avoid pthread access an released object
	m_blockSec = blockSec;
	CC_SAFE_RETAIN(this);
	pthread_t thread;
	pthread_create(&thread, NULL, waitingConnectThreadEntry, (void*)this);
	
    return true;
}

bool CCTCPSocket::sendData(void* buf, int size) {
	// basic checking
    if(!buf || size <= 0) {
        return false;
    }
    if (m_sock == kCCSocketInvalid) {
        return false;
    }
	
    // buffer size check
    if(m_outBufLen + size > kCCSocketOutputBufferDefaultSize) {
        flush();
        if(m_outBufLen + size > kCCSocketOutputBufferDefaultSize) {
            destroy();
            return false;
        }
    }
	
	// append data to buffer
    memcpy(m_outBuf + m_outBufLen, buf, size);
    m_outBufLen += size;
	
    return true;
}

int CCTCPSocket::receiveData(void* buf, int size) {
	// basic checking
    if(buf == NULL || size <= 0) {
        return -1;
    }
    if (m_sock == kCCSocketInvalid) {
        return -1;
    }
	
	// read can be more than buffer size
	size = MIN(size, kCCSocketInputBufferDefaultSize);
	
	// ensure the packet is complete, if not, read again
    if (size > m_inBufLen) {
        if (!recvFromSock()) {
            return -1;
        }
    }
	
	// check available
	size = MIN(size, m_inBufLen);
	
	// if the packet is looped, need copy two times
    if(m_inBufStart + size > kCCSocketInputBufferDefaultSize) {
        int copylen = kCCSocketInputBufferDefaultSize - m_inBufStart;
        memcpy(buf, m_inBuf + m_inBufStart, copylen);
        memcpy((unsigned char *)buf + copylen, m_inBuf, size - copylen);
    } else {
        memcpy(buf, m_inBuf + m_inBufStart, size);
    }
	
	// reposition read pos
    m_inBufStart = (m_inBufStart + size) % kCCSocketInputBufferDefaultSize;
    m_inBufLen -= size;
	
    return size;
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
	if (m_inBufLen >= kCCSocketInputBufferDefaultSize || m_sock == kCCSocketInvalid) {
		return false;
	}
	
	// max byte can be hold by read buffer and the write position
	// for the first read operation
	int savelen, savepos;
	if(m_inBufStart + m_inBufLen < kCCSocketInputBufferDefaultSize){
		savelen = kCCSocketInputBufferDefaultSize - (m_inBufStart + m_inBufLen);
	} else {
		savelen = kCCSocketInputBufferDefaultSize - m_inBufLen;
	}
	
	// read to buffer end
	savepos = (m_inBufStart + m_inBufLen) % kCCSocketInputBufferDefaultSize;
	int inlen = recv(m_sock, m_inBuf + savepos, savelen, 0);
	if(inlen > 0) {
		m_inBufLen += inlen;
		if (m_inBufLen > kCCSocketInputBufferDefaultSize) {
			return false;
		}
		
		// read second if has more
		if(inlen == savelen && m_inBufLen < kCCSocketInputBufferDefaultSize) {
			int savelen = kCCSocketInputBufferDefaultSize - m_inBufLen;
			int savepos = (m_inBufStart + m_inBufLen) % kCCSocketInputBufferDefaultSize;
			inlen = recv(m_sock, m_inBuf + savepos, savelen, 0);
			if(inlen > 0) {
				m_inBufLen += inlen;
				if (m_inBufLen > kCCSocketInputBufferDefaultSize) {
					return false;
				}
			} else if(inlen == 0) {
				destroy();
				return false;
			} else {
				if (hasError()) {
					destroy();
					return false;
				}
			}
		}
	} else if(inlen == 0) {
		destroy();
		return false;
	} else {
		if (hasError()) {
			destroy();
			return false;
		}
	}
	
	return true;
}

bool CCTCPSocket::flush() {
	// basic checking
	if (m_sock == kCCSocketInvalid) {
		return false;
	}
	if(m_outBufLen <= 0) {
		return true;
	}
	
	// send
	int outsize;
	outsize = send(m_sock, m_outBuf, m_outBufLen, 0);
	if(outsize > 0) {
		// move cursor
		if(m_outBufLen - outsize > 0) {
			memcpy(m_outBuf, m_outBuf + outsize, m_outBufLen - outsize);
		}
		m_outBufLen -= outsize;
		
		// any more?
		if (m_outBufLen < 0) {
			return false;
		}
	} else {
		if (hasError()) {
			destroy();
			return false;
		}
	}
	
	return true;
}

bool CCTCPSocket::hasAvailable() {
	// basic check
	if (m_sock == kCCSocketInvalid) {
		return false;
	}
	
	char buf[1];
	int ret = recv(m_sock, buf, 1, MSG_PEEK);
	if(ret == 0) {
		destroy();
		return false;
	} else if(ret < 0) {
		if (hasError()) {
			destroy();
			return false;
		} else {
			return true;
		}
	} else {
		return true;
	}
}

void CCTCPSocket::destroy() {
	// ensure not destroy again
	if(m_sock == kCCSocketInvalid)
		return;
	
	// close
	closeSocket();
	
	// reset
	m_sock = kCCSocketInvalid;
	m_connected = false;
	m_inBufLen = 0;
	m_inBufStart = 0;
	m_outBufLen = 0;
	memset(m_outBuf, 0, sizeof(m_outBuf));
	memset(m_inBuf, 0, sizeof(m_inBuf));
}

NS_CC_END