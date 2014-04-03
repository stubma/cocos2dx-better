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
#ifndef __CCUDPSocket_h__
#define __CCUDPSocket_h__

#include "cocos2d.h"
#include "CCByteBuffer.h"
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "CCMoreMacros.h"

using namespace std;

NS_CC_BEGIN

class CCUDPSocketHub;
class CCPacket;

/**
 * UDP socket
 */
class CC_DLL CCUDPSocket : public CCObject {
    friend class CCUDPSocketHub;
    
private:
    /// read buffer, it is a loop buffer
    char m_inBuf[kCCSocketInputBufferDefaultSize];
	
	/// available data in read buffer
    ssize_t m_inBufLen;
    
    /// pthread mutex
    pthread_mutex_t m_mutex;
	
    /// block time for waiting socket connection
	int m_blockSec;
    
private:
    // we wait here until socket is connected or failed
	static void* udpThreadEntry(void* arg);
    
	/// has error
    bool hasError();
	
	/// close socket
    void closeSocket();
    
    /// read data from sock
    void recvFromSock();
	
protected:
	/**
	 * init socket
	 *
	 * @param hostname host name or ip address, ipv4 only
	 * @param port port
	 * @param tag tag of socket
	 * @param blockSec block time when create this socket, 0 means not block
	 * @return true means initialization successful
	 */
    bool init(const string& hostname, int port, int tag = -1, int blockSec = kCCSocketDefaultTimeout);
    
public:
	CCUDPSocket();
	virtual ~CCUDPSocket();
	
	/**
	 * create socket instance
	 *
	 * @param hostname host name or ip address, ipv4 only
	 * @param port port
	 * @param tag tag of socket
	 * @param blockSec block time when create this socket, 0 means not block
	 * @return instance or NULL if failed
	 */
	static CCUDPSocket* create(const string& hostname, int port, int tag = -1, int blockSec = kCCSocketDefaultTimeout);
	
	/**
	 * add packet to send queue
	 *
	 * @param p packet
	 */
    void sendPacket(CCPacket* p);
	
	/// socket handle
    CC_SYNTHESIZE_READONLY(int, m_socket, Socket);
    
    /// tag
    CC_SYNTHESIZE_READONLY(int, m_tag, Tag);
    
    /// hub reference
    CC_SYNTHESIZE(CCUDPSocketHub*, m_hub, Hub);
    
    /// connected
    CC_SYNTHESIZE_READONLY_BOOL(m_connected, Connected);
    
    /// stop
    CC_SYNTHESIZE_BOOL(m_stop, Stop);
    
    /// server name
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(string, m_hostname, Hostname);
    
    /// port
    CC_SYNTHESIZE_READONLY(int, m_port, Port);
    
    /// server address
    CC_SYNTHESIZE_PASS_BY_REF(sockaddr_in, m_srvAddr, ServerAddress);
    
    /// send queue
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(CCArray, m_sendQueue, SendQueue);
};

NS_CC_END

#endif /* defined(__CCUDPSocket_h__) */
