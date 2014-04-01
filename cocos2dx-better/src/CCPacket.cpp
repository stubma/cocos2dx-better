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
#include "CCPacket.h"
#include "CCByteBuffer.h"
#include "CCTCPSocketHub.h"
#include "ccMoreTypes.h"

NS_CC_BEGIN

CCPacket::CCPacket() :
m_body(NULL),
m_packetLength(0),
m_raw(false) {
    memset(&m_header, 0, sizeof(Header));
}

CCPacket::~CCPacket() {
    CC_SAFE_FREE(m_body);
}

CCPacket* CCPacket::createStandardPacket(const char* buf, int len) {
    if(len < kCCPacketHeaderLength)
        return NULL;
    
    CCPacket* p = new CCPacket();
    if(p->initWithStandardBuf(buf, len)) {
        return (CCPacket*)p->autorelease();
    }
    CC_SAFE_RELEASE(p);
    return NULL;
}

CCPacket* CCPacket::createRawPacket(const char* buf, int len) {
    CCPacket* p = new CCPacket();
    if(p->initWithRawBuf(buf, len)) {
        return (CCPacket*)p->autorelease();
    }
    CC_SAFE_RELEASE(p);
    return NULL;
}

bool CCPacket::initWithStandardBuf(const char* buf, int len) {
    // quick check
    if(len < kCCPacketHeaderLength) {
        return false;
    }
    
    // header
    CCByteBuffer bb(buf, len);
    m_header.magic[0] = bb.read<char>();
    m_header.magic[1] = bb.read<char>();
    m_header.magic[2] = bb.read<char>();
    m_header.magic[3] = bb.read<char>();
    m_header.protocolVersion = betoh32(bb.read<int>());
    m_header.serverVersion = betoh32(bb.read<int>());
    m_header.command = betoh32(bb.read<int>());
    m_header.encryptAlgorithm = betoh32(bb.read<int>());
    m_header.length = betoh32(bb.read<int>());
    
    // body
    if(bb.available() >= m_header.length) {
        if(CCTCPSocketHub::getDecryptFunc()) {
            // read body and try to decode
            char* body = (char*)malloc(m_header.length * sizeof(char));
            bb.read((uint8*)body, m_header.length);
            int plainLen;
            char* plain = (char*)(*CCTCPSocketHub::getDecryptFunc())(body, m_header.length, &plainLen);
            if(plain != body)
                free(body);
            
            // copy to body
            allocateBody(plainLen + 1); // one more 0 bytes make it a c string
            memcpy(m_body, plain, plainLen);
            m_header.length = plainLen;
            free(plain);
        } else {
            allocateBody(m_header.length + 1);
            bb.read((uint8*)m_body, m_header.length);
        }
    } else {
        return false;
    }
    
    // init other
    m_raw = false;
    m_packetLength = m_header.length + kCCPacketHeaderLength;
    
    return true;
}

bool CCPacket::initWithRawBuf(const char* buf, int len) {
    if(CCTCPSocketHub::getDecryptFunc()) {
        // read body and try to decode
        int plainLen;
        char* plain = (char*)(*CCTCPSocketHub::getDecryptFunc())(buf, len, &plainLen);
        
        // copy to body
        allocateBody(plainLen + 1); // one more 0 bytes make it a c string
        memcpy(m_body, plain, plainLen);
        m_header.length = plainLen;
        if(plain != buf)
            free(plain);
    } else {
        m_header.length = len;
        allocateBody(len + 1);
        memcpy(m_body, buf, len);
    }
    
    // other
    m_raw = true;
    m_packetLength = m_header.length;
    
    return true;
}

void CCPacket::allocateBody(size_t len) {
    m_body = (char*)calloc(len, sizeof(char));
}

NS_CC_END