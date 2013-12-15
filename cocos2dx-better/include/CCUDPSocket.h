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

using namespace std;

NS_CC_BEGIN

/**
 * UDP socket
 */
class CC_DLL CCUDPSocket : public CCObject {
private:
	/// socket handle
    int m_sock;
	
	/// tag of this socket
    int m_tag;
	
	/// server address
	sockaddr_in m_srvAddr;
	
private:
	/// has error
    bool hasError();
	
	/// close socket
    void closeSocket();
	
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
	 * send data in a buffer
	 *
	 * @param buf buffer
	 * @param size data to be sent
	 * @return operation success or failed
	 */
    bool sendData(void* buf, int size);
	
	/**
	 * receive data and put into a buffer
	 *
	 * @param buf buffer large enough to hold data
	 * @param size wanted data length
	 * @return actual read data size, or -1 if fail to read a complete packet
	 */
    int receiveData(void* buf, int size);
	
	/// destroy socket
    void destroy();
	
	/// get socket handle
    int getSocket() const { return m_sock; }
    
	/// get tag
    int getTag() { return m_tag; }
};

NS_CC_END

#endif /* defined(__CCUDPSocket_h__) */
