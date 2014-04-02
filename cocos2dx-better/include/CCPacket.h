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
#ifndef __CCPacket__
#define __CCPacket__

#include "cocos2d.h"
#include "CCMoreMacros.h"
#include "CCJSONObject.h"

using namespace std;

NS_CC_BEGIN

#define kCCPacketHeaderLength 24

/**
 * A general packet definition, it will have a header. However, it provides
 * methods to create a raw packet which has no header.
 */
class CC_DLL CCPacket : public CCObject {
public:
    typedef struct {
        char magic[4];
        int protocolVersion;
        int serverVersion;
		int command;
		int encryptAlgorithm;
        int length; // data length after header
    } Header;
    
protected:
    CCPacket();
    
    virtual bool initWithStandardBuf(const char* buf, size_t len);
    virtual bool initWithRawBuf(const char* buf, size_t len, int algorithm);
    virtual bool initWithJson(const string& magic, int command, CCJSONObject* json, int protocolVersion, int serverVersion, int algorithm);
    
    // allocate buffer
    void allocate(size_t len);
    
    // write header
    void writeHeader();
    
public:
    virtual ~CCPacket();
    
    /**
     * create a standard format packet which has a header
     */
    static CCPacket* createStandardPacket(const char* buf, size_t len);
    
    /**
     * create a raw packet which doesn't have header and the body is raw data received from
     * socket. you can set algorithm if data is encrypted, by default it is -1 which means no
     * decryption will be performed
     */
    static CCPacket* createRawPacket(const char* buf, size_t len, int algorithm = -1);
    
    /**
     * create a standard packet which body content is from a json object
     */
    static CCPacket* createStandardPacket(const string& magic, int command, CCJSONObject* json, int algorithm = -1);
    
    /**
     * create a standard packet which body content is from a json object
     */
    static CCPacket* createStandardPacket(const string& magic, int command, CCJSONObject* json, int protocolVersion, int serverVersion, int algorithm = -1);
    
    // body length
    int getBodyLength() { return m_header.length; }
    
    // get body pointer
    const char* getBody();
    
    CC_SYNTHESIZE_PASS_BY_REF_NC(Header, m_header, Header);
    CC_SYNTHESIZE(char*, m_buffer, Buffer);
    CC_SYNTHESIZE_READONLY(size_t, m_packetLength, PacketLength);
    CC_SYNTHESIZE_READONLY(bool, m_raw, Raw);
};

NS_CC_END

#endif /* defined(__CCPacket__) */
