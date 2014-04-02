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
#include "CCByteBuffer.h"

NS_CC_BEGIN

#define DEFAULT_SIZE 0x1000

CCByteBuffer::CCByteBuffer() :
m_buffer(NULL),
m_readPos(0),
m_writePos(0),
m_external(false) {
	reserve(DEFAULT_SIZE);
}

CCByteBuffer::CCByteBuffer(size_t res) :
m_buffer(NULL),
m_readPos(0),
m_writePos(0),
m_external(false) {
	reserve(res);
}

CCByteBuffer::CCByteBuffer(const CCByteBuffer& b) :
m_buffer(NULL),
m_readPos(0),
m_writePos(0),
m_external(false) {
	reserve(b.m_bufferSize);
	memcpy(b.m_buffer, m_buffer, b.m_writePos);
	m_readPos = b.m_readPos;
	m_writePos = b.m_writePos;
}

CCByteBuffer::CCByteBuffer(const char* buf, size_t bufSize, size_t dataLen) :
m_buffer((uint8*)buf),
m_readPos(0),
m_writePos(dataLen),
m_external(true),
m_bufferSize(bufSize) {
    
}

CCByteBuffer::~CCByteBuffer() {
    if(!m_external) {
        CC_SAFE_FREE(m_buffer);
    }
}

CCByteBuffer* CCByteBuffer::create() {
	CCByteBuffer* b = new CCByteBuffer();
	return (CCByteBuffer*)b->autorelease();
}

CCByteBuffer* CCByteBuffer::create(size_t res) {
	CCByteBuffer* b = new CCByteBuffer(res);
	return (CCByteBuffer*)b->autorelease();
}

void CCByteBuffer::reserve(size_t res) {
    if(m_external)
        return;
    
	if(m_buffer)
		m_buffer = (uint8*)realloc(m_buffer, res);
	else
		m_buffer = (uint8*)malloc(res);
	
	m_bufferSize = res;
}

void CCByteBuffer::compact() {
    if(m_readPos > 0) {
        memmove(m_buffer, m_buffer + m_readPos, available());
        m_writePos -= m_readPos;
        m_readPos = 0;
    }
}

size_t CCByteBuffer::read(uint8 * buffer, size_t len) {
	if(m_readPos + len > m_writePos)
		len = (m_writePos - m_readPos);
	
	memcpy(buffer, &m_buffer[m_readPos], len);
	m_readPos += len;
	return len;
}

void CCByteBuffer::read(string& dest) {
	readCString(dest);
}

void CCByteBuffer::readCString(string& dest) {
	dest.clear();
	char c;
	while(true)	{
		c = read<char>();
		if(c == 0)
			break;
		dest += c;
	}
}

void CCByteBuffer::readPascalString(string& dest) {
	dest.clear();
	uint16 len = read<uint16>();
	while(len-- > 0) {
		dest += read<char>();
	}
}

void CCByteBuffer::readLine(string& dest) {
	dest.clear();
	char c;
	while(m_readPos < m_bufferSize)	{
		c = read<char>();
		if(c == '\r')
			continue;
		else if(c == 0 || c == '\n')
			break;
		dest += c;
	}
}

void CCByteBuffer::write(const uint8* data, size_t size) {
	size_t new_size = m_writePos + size;
	if(new_size > m_bufferSize) {
        if(m_external) {
            CCLOGWARN("external mode: buffer size is not enough to write");
            return;
        }
		new_size = (new_size / DEFAULT_INCREASE_SIZE + 1) * DEFAULT_INCREASE_SIZE;
		reserve(new_size);
	}
	
	memcpy(&m_buffer[m_writePos], data, size);
	m_writePos += size;
}

void CCByteBuffer::write(const string& value) {
	writeCString(value);
}

void CCByteBuffer::writeCString(const string& value) {
    if(m_writePos + value.length() + 1 > m_bufferSize) {
        if(m_external) {
            CCLOGWARN("external mode: buffer size is not enough to write");
            return;
        }
        ensureCanWrite(value.length() + 1);
    }
    
	memcpy(&m_buffer[m_writePos], value.c_str(), value.length() + 1);
	m_writePos += (value.length() + 1);
}

void CCByteBuffer::writePascalString(const string& value) {
    if(m_writePos + value.length() + sizeof(uint16) > m_bufferSize) {
        if(m_external) {
            CCLOGWARN("external mode: buffer size is not enough to write");
            return;
        }
        ensureCanWrite(value.length() + sizeof(uint16));
    }
	
	write<uint16>(value.length());
	memcpy(&m_buffer[m_writePos], value.c_str(), value.length());
	m_writePos += value.length();
}

void CCByteBuffer::writeLine(const string& value) {
    if(m_writePos + value.length() + 2 * sizeof(char) > m_bufferSize) {
        if(m_external) {
            CCLOGWARN("external mode: buffer size is not enough to write");
            return;
        }
        ensureCanWrite(value.length() + 2 * sizeof(char));
    }
    
	memcpy(&m_buffer[m_writePos], value.c_str(), value.length());
	m_writePos += value.length();
	write<char>('\r');
	write<char>('\n');
}

void CCByteBuffer::skip(size_t len) {
	if(m_readPos + len > m_writePos)
		len = (m_writePos - m_readPos);
	m_readPos += len;
}

void CCByteBuffer::revoke(size_t len) {
    m_readPos -= len;
    m_readPos = MAX(0, m_readPos);
}

void CCByteBuffer::ensureCanWrite(size_t size) {
	size_t new_size = m_writePos + size;
	if(new_size > m_bufferSize) {
		new_size = (new_size / DEFAULT_INCREASE_SIZE + 1) * DEFAULT_INCREASE_SIZE;
		reserve(new_size);
	}
}

NS_CC_END