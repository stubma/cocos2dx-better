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
#ifndef __CCTCPSocketHub_h__
#define __CCTCPSocketHub_h__

#include "cocos2d.h"
#include "CCTCPSocket.h"
#include "CCByteBuffer.h"
#include "CCTCPSocketListener.h"

NS_CC_BEGIN

/**
 * It manages a group of sockets and monitor them in every update. The update loop is started
 * after hub is created.
 */
class CC_DLL CCTCPSocketHub : public CCObject {
private:
	typedef vector<CCTCPSocket*> CCTCPSocketList;
	CCTCPSocketList m_lstSocket;
	
	typedef map<int, CCTCPSocketListener*> CCTCPSocketListenerMap;
    CCTCPSocketListenerMap m_listenerMap;
	
protected:
	/// find a listener for a tag
	CCTCPSocketListener* getListener(int tag);
	
public:
    CCTCPSocketHub();
    virtual ~CCTCPSocketHub();
	
	/**
	 * create socket instance and auto add it to hub
	 *
	 * @param hostname host name or ip address, ipv4 only
	 * @param port port
	 * @param tag tag of socket
	 * @param blockSec block time when create this socket, 0 means not block
	 * @param keepAlive true means keep socket alive
	 * @return instance or NULL if failed
	 */
    CCTCPSocket* createSocket(const string& hostname, int port, int tag, int blockSec = kCCSocketDefaultTimeout, bool keepAlive = false);
	
    /// register callback, it doesn't retain the callback
    void registerCallback(int tag, CCTCPSocketListener* callback);
	
    /// add socket to hub
    bool addSocket(CCTCPSocket* socket);
	
	/// remove socket by tag
    bool removeSocket(int tag);
	
    // 断开socket
    void disconnect(int tag);
	
	/// get socket by tag
    CCTCPSocket* getSocket(int tag);
	
	/// send a packet
    bool sendPacket(int tag, CCByteBuffer* packet);
	
	/// update loop
    void update(float delta);
};

NS_CC_END

#endif //__CCTCPSocketHub_h__