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

using namespace std;

NS_CC_BEGIN

#define kCCPacketHeaderLength 24

typedef enum {
    NONE,
    NOT
} ccPacketEncryptAlgorithm;

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
    
    virtual bool initWithStandardBuf(const char* buf, int len);
    virtual bool initWithRawBuf(const char* buf, int len);
    
public:
    virtual ~CCPacket();
    static CCPacket* createStandardPacket(const char* buf, int len);
    static CCPacket* createRawPacket(const char* buf, int len);
    
    // allocate memory for body
    void allocateBody(size_t len);
    
    // body length
    int getBodyLength() { return m_header.length; }
    
    CC_SYNTHESIZE_PASS_BY_REF_NC(Header, m_header, Header);
    CC_SYNTHESIZE(char*, m_body, Body);
    CC_SYNTHESIZE_READONLY(int, m_packetLength, PacketLength);
    CC_SYNTHESIZE_READONLY(bool, m_raw, Raw);
};

NS_CC_END

#endif /* defined(__CCPacket__) */
