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

NS_CC_BEGIN

CCUDPSocket::CCUDPSocket() :
m_sock(kCCSocketInvalid),
m_tag(-1) {
}

CCUDPSocket::~CCUDPSocket() {
}

CCUDPSocket* CCUDPSocket::create(const string& hostname, int port, int tag, int blockSec) {
	CCUDPSocket* s = new CCUDPSocket();
	if(s->init(hostname, port, tag)) {
		return (CCUDPSocket*)s->autorelease();
	}
	
	s->release();
	return NULL;
}

bool CCUDPSocket::init(const string& hostname, int port, int tag, int blockSec) {
	// check
    if(hostname.empty() || hostname.length() > 15) {
        return false;
    }
	
	// create tcp socket
    m_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(m_sock == kCCSocketInvalid) {
        closeSocket();
        return false;
    }
	
	// non-block mode
    fcntl(m_sock, F_SETFL, O_NONBLOCK);
	
	// validate host name
    unsigned long serveraddr = inet_addr(hostname.c_str());
    if(serveraddr == INADDR_NONE) {
        closeSocket();
        return false;
    }
	
	// server address
    memset((void *)&m_srvAddr, 0, sizeof(m_srvAddr));
    m_srvAddr.sin_family = AF_INET;
    m_srvAddr.sin_port = htons(port);
    m_srvAddr.sin_addr.s_addr = serveraddr;
	
	// client address
	sockaddr_in my_addr;
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = INADDR_ANY;
	my_addr.sin_port = 0;
	
	// bind
	if(bind(m_sock, (sockaddr*)&my_addr, sizeof(my_addr)) == kCCSocketError) {
		if (hasError()) {
            closeSocket();
            return false;
        }
	}
	
	// select it
	timeval timeout;
	timeout.tv_sec = blockSec;
	timeout.tv_usec = 0;
	fd_set writeset, exceptset;
	FD_ZERO(&writeset);
	FD_ZERO(&exceptset);
	FD_SET(m_sock, &writeset);
	FD_SET(m_sock, &exceptset);
	int ret = select(FD_SETSIZE, NULL, &writeset, &exceptset, &timeout);
	if (ret == 0 || ret < 0) {
		closeSocket();
		return false;
	} else {
		ret = FD_ISSET(m_sock, &exceptset);
		if(ret) {
			closeSocket();
			return false;
		}
	}
	
	// delay 500ms when socket closed
    struct linger so_linger;
    so_linger.l_onoff = 1;
    so_linger.l_linger = 500;
    setsockopt(m_sock, SOL_SOCKET, SO_LINGER, (const char*)&so_linger, sizeof(so_linger));
	
	// save tag
    m_tag = tag;
	
	return true;
}

bool CCUDPSocket::sendData(void* buf, int size) {
	ssize_t sent = sendto(m_sock, buf, size, 0, (sockaddr*)&m_srvAddr, sizeof(m_srvAddr));
	if(sent < size) {
		return false;
	} else {
		return true;
	}
}

int CCUDPSocket::receiveData(void* buf, int size) {
	socklen_t len = sizeof(m_srvAddr);
	int recv = recvfrom(m_sock, buf, size, 0, (sockaddr*)&m_srvAddr, &len);
	return recv;
}

bool CCUDPSocket::hasError() {
	int err = errno;
	if(err != EINPROGRESS && err != EAGAIN) {
		return true;
	}
	
	return false;
}

void CCUDPSocket::closeSocket() {
    close(m_sock);
}

void CCUDPSocket::destroy() {
	// ensure not destroy again
	if(m_sock == kCCSocketInvalid)
		return;
	
	// close
	closeSocket();
}

NS_CC_END