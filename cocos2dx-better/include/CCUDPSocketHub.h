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
#ifndef __CCUDPSocketHub_h__
#define __CCUDPSocketHub_h__

#include "cocos2d.h"
#include "CCUDPSocket.h"
#include "CCByteBuffer.h"
#include "CCPacket.h"
#include "ccMoreTypes.h"
#include <pthread.h>

NS_CC_BEGIN

/// object is a connected socket
#define kCCNotificationUDPSocketConnected "kCCNotificationUDPSocketConnected"

/// object is disconnected socket
#define kCCNotificationUDPSocketDisconnected "kCCNotificationUDPSocketDisconnected"

/// object is packet
#define kCCNotificationPacketReceived "kCCNotificationPacketReceived"

/**
 * It manages a group of sockets and monitor them in every update. The update loop is started
 * after hub is created.
 */
class CC_DLL CCUDPSocketHub : public CCObject {
    friend class CCUDPSocket;
    
private:
    /// pthread mutex
    pthread_mutex_t m_mutex;
    
    /// connected sockets
    CCArray m_connectedSockets;
    
    /// disconnected sockets
    CCArray m_disconnectedSockets;
    
    /// packet array
    CCArray m_packets;
	
protected:
	CCUDPSocketHub();
    
    /// add socket to hub
    bool addSocket(CCUDPSocket* socket);
	
    /// listen on socket, read and write if necessary
    void mainLoop(float delta);
    
    /// called by tcp socket when it is connected
    void onSocketConnectedThreadSafe(CCUDPSocket* s);
    
    /// called by tcp socket when it is disconnected
    void onSocketDisconnectedThreadSafe(CCUDPSocket* s);
    
    /// called when a socket want to deliver a packet
    void onPacketReceivedThreadSafe(CCPacket* packet);
    
public:
    virtual ~CCUDPSocketHub();
	static CCUDPSocketHub* create(CC_MULTI_ENCRYPT_FUNC encryptFunc = NULL, CC_MULTI_DECRYPT_FUNC decryptFunc = NULL);
    static CC_MULTI_ENCRYPT_FUNC getEncryptFunc();
    static CC_MULTI_DECRYPT_FUNC getDecryptFunc();
	
	/**
	 * create socket instance and auto add it to hub
	 *
	 * @param hostname host name or ip address, ipv4 only
	 * @param port port
	 * @param tag tag of socket
	 * @param blockSec block time when create this socket, 0 means not block
	 * @return instance or NULL if failed
	 */
    CCUDPSocket* createSocket(const string& hostname, int port, int tag, int blockSec = kCCSocketDefaultTimeout);
	
    /// stop all
    void stopAll();
	
    /// disconnect socket
    void disconnect(int tag);
	
	/// get socket by tag
    CCUDPSocket* getSocket(int tag);
	
	/// send a packet
    void sendPacket(int tag, CCPacket* packet);
    
    /// socket array
    CC_SYNTHESIZE_PASS_BY_REF(CCArray, m_sockets, Sockets);
    
    /// raw policy means packet don't have header, just contains a piece of bytes
    /// so it will be developer's responsibility to parse the packet
    CC_SYNTHESIZE_BOOL(m_rawPolicy, RawPolicy);
};

NS_CC_END

#endif //__CCUDPSocketHub_h__