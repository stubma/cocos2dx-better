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
m_buffer(NULL),
m_packetLength(0),
m_raw(false) {
    memset(&m_header, 0, sizeof(Header));
}

CCPacket::~CCPacket() {
    CC_SAFE_FREE(m_buffer);
}

CCPacket* CCPacket::createStandardPacket(const char* buf, size_t len) {
    if(len < kCCPacketHeaderLength)
        return NULL;
    
    CCPacket* p = new CCPacket();
    if(p->initWithStandardBuf(buf, len)) {
        return (CCPacket*)p->autorelease();
    }
    CC_SAFE_RELEASE(p);
    return NULL;
}

CCPacket* CCPacket::createRawPacket(const char* buf, size_t len, int algorithm) {
    CCPacket* p = new CCPacket();
    if(p->initWithRawBuf(buf, len, algorithm)) {
        return (CCPacket*)p->autorelease();
    }
    CC_SAFE_RELEASE(p);
    return NULL;
}

CCPacket* CCPacket::createStandardPacket(const string& magic, int command, CCJSONObject* json, int algorithm) {
    CCPacket* p = new CCPacket();
    if(p->initWithJson(magic, command, json, 0, 0, algorithm)) {
        return (CCPacket*)p->autorelease();
    }
    CC_SAFE_RELEASE(p);
    return NULL;
}

CCPacket* CCPacket::createStandardPacket(const string& magic, int command, CCJSONObject* json, int protocolVersion, int serverVersion, int algorithm) {
    CCPacket* p = new CCPacket();
    if(p->initWithJson(magic, command, json, protocolVersion, serverVersion, algorithm)) {
        return (CCPacket*)p->autorelease();
    }
    CC_SAFE_RELEASE(p);
    return NULL;
}

bool CCPacket::initWithJson(const string& magic, int command, CCJSONObject* json, int protocolVersion, int serverVersion, int algorithm) {
    // check magic
    if(magic.length() < 4)
        return false;
    
    // magic
    m_header.magic[0] = magic.at(0);
    m_header.magic[1] = magic.at(1);
    m_header.magic[2] = magic.at(2);
    m_header.magic[3] = magic.at(3);
    
    // protocol version
    m_header.protocolVersion = protocolVersion;
    
    // server version
    m_header.serverVersion = serverVersion;
    
	// command id
    m_header.command = command;
	
	// no encrypt
    m_header.encryptAlgorithm = algorithm;
    
    // body
	string bodyStr = json->toString();
    char* plain = (char*)bodyStr.c_str();
    if(CCTCPSocketHub::getEncryptFunc() && algorithm != -1) {
        size_t bodyLen = bodyStr.length();
        size_t encLen;
        const char* enc = (*CCTCPSocketHub::getEncryptFunc())(plain, bodyLen, &encLen, algorithm);
        allocate(encLen + kCCPacketHeaderLength + 1);
        memcpy(m_buffer + kCCPacketHeaderLength, enc, encLen);
        if(enc != plain) {
            free((void*)enc);
        }
        
        // length
        m_header.length = (int)encLen;
        m_packetLength = encLen + kCCPacketHeaderLength;
    } else {
        m_header.length = (int)bodyStr.length();
        allocate(bodyStr.length() + kCCPacketHeaderLength + 1);
        memcpy(m_buffer + kCCPacketHeaderLength, plain, bodyStr.length());
        m_packetLength = bodyStr.length() + kCCPacketHeaderLength;
    }
    
    // write header
    writeHeader();
    
    // init
    m_raw = false;
    
    return true;
}

bool CCPacket::initWithStandardBuf(const char* buf, size_t len) {
    // quick check
    if(len < kCCPacketHeaderLength) {
        return false;
    }
    
    // header
    CCByteBuffer bb(buf, len, len);
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
            size_t plainLen;
            char* plain = (char*)(*CCTCPSocketHub::getDecryptFunc())(body, m_header.length, &plainLen, m_header.encryptAlgorithm);
            if(plain != body)
                free(body);
            
            // copy to body
            allocate(plainLen + kCCPacketHeaderLength + 1); // one more 0 bytes make it a c string
            memcpy(m_buffer + kCCPacketHeaderLength, plain, plainLen);
            m_header.length = (int)plainLen;
            free(plain);
        } else {
            allocate(m_header.length + kCCPacketHeaderLength + 1);
            bb.read((uint8*)m_buffer + kCCPacketHeaderLength, m_header.length);
        }
    } else {
        return false;
    }
    
    // init other
    m_raw = false;
    m_packetLength = m_header.length + kCCPacketHeaderLength;
    
    // write header
    writeHeader();
    
    return true;
}

bool CCPacket::initWithRawBuf(const char* buf, size_t len, int algorithm) {
    if(CCTCPSocketHub::getDecryptFunc() && algorithm != -1) {
        // read body and try to decode
        size_t plainLen;
        char* plain = (char*)(*CCTCPSocketHub::getDecryptFunc())(buf, len, &plainLen, algorithm);
        
        // copy to body
        allocate(plainLen + 1); // one more 0 bytes make it a c string
        memcpy(m_buffer, plain, plainLen);
        m_header.length = (int)plainLen;
        if(plain != buf)
            free(plain);
    } else {
        m_header.length = (int)len;
        allocate(len + 1);
        memcpy(m_buffer, buf, len);
    }
    
    // other
    m_raw = true;
    m_packetLength = m_header.length;
    
    return true;
}

void CCPacket::writeHeader() {
    CCByteBuffer bb(m_buffer, m_packetLength, 0);

    // magic
    bb.write<char>(m_header.magic[0]);
    bb.write<char>(m_header.magic[1]);
    bb.write<char>(m_header.magic[2]);
    bb.write<char>(m_header.magic[3]);
    
    // protocol version
    bb.write<int>(htobe32(m_header.protocolVersion));
    
    // server version
    bb.write<int>(htobe32(m_header.serverVersion));
    
	// command id
    bb.write<int>(htobe32(m_header.command));
	
	// no encrypt
	bb.write<int>(htobe32(m_header.encryptAlgorithm));
    
    // body length
    bb.write<int>(htobe32(m_header.length));
}

const char* CCPacket::getBody() {
    if(m_raw)
        return m_buffer;
    else
        return m_buffer + kCCPacketHeaderLength;
}

void CCPacket::allocate(size_t len) {
    if(!m_buffer)
        m_buffer = (char*)calloc(len, sizeof(char));
}

NS_CC_END