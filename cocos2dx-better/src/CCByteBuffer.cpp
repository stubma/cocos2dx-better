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
#define DEFAULT_INCREASE_SIZE 200

CCByteBuffer::CCByteBuffer() :
m_buffer(NULL),
m_readPos(0),
m_writePos(0) {
	reserve(DEFAULT_SIZE);
}

CCByteBuffer::CCByteBuffer(size_t res) :
m_buffer(NULL),
m_readPos(0),
m_writePos(0) {
	reserve(res);
}

CCByteBuffer::CCByteBuffer(const CCByteBuffer& b) :
m_buffer(NULL),
m_readPos(0),
m_writePos(0) {
	reserve(b.m_bufferSize);
	memcpy(b.m_buffer, m_buffer, b.m_writePos);
	m_readPos = b.m_readPos;
	m_writePos = b.m_writePos;
}

CCByteBuffer::~CCByteBuffer() {
	CC_SAFE_FREE(m_buffer);
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
	if(m_buffer)
		m_buffer = (uint8*)realloc(m_buffer, res);
	else
		m_buffer = (uint8*)malloc(res);
	
	m_bufferSize = res;
}

template<typename T>
T CCByteBuffer::read() {
	if(m_readPos + sizeof(T) > m_writePos)
		return (T)0;
	T ret = *(T*)&m_buffer[m_readPos];
	m_readPos += sizeof(T);
	return ret;
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

template<typename T>
size_t CCByteBuffer::readVector(size_t vsize, vector<T>& v) {
	v.clear();
	while(vsize--) {
		T t = read<T>();
		v.push_back(t);
	}
	return v.size();
}

template<typename T>
size_t CCByteBuffer::writeVector(const vector<T>& v) {
	for(typename vector<T>::const_iterator i = v.begin(); i != v.end(); i++) {
		write<T>(*i);
	}
	return v.size();
}

template<typename T>
size_t CCByteBuffer::readList(size_t vsize, list<T>& v) {
	v.clear();
	while(vsize--) {
		T t = read<T>();
		v.push_back(t);
	}
	return v.size();
}

template<typename T>
size_t CCByteBuffer::writeList(const list<T>& v) {
	for(typename list<T>::const_iterator i = v.begin(); i != v.end(); i++) {
		write<T>(*i);
	}
	return v.size();
}

template <typename K, typename V>
size_t CCByteBuffer::readMap(size_t msize, map<K, V>& m) {
	m.clear();
	while(msize--) {
		K k = read<K>();
		V v = read<V>();
		m.insert(make_pair(k, v));
	}
	return m.size();
}

template <typename K, typename V>
size_t CCByteBuffer::writeMap(const map<K, V>& m) {
	for(typename map<K, V>::const_iterator i = m.begin(); i != m.end(); i++) {
		write<K>(i->first);
		write<V>(i->second);
	}
	return m.size();
}

template<typename T>
void CCByteBuffer::write(const T& data) {
	size_t new_size = m_writePos + sizeof(T);
	if(new_size > m_bufferSize) {
		new_size = (new_size / DEFAULT_INCREASE_SIZE + 1) * DEFAULT_INCREASE_SIZE;
		reserve(new_size);
	}
	
	*(T*)&m_buffer[m_writePos] = data;
	m_writePos += sizeof(T);
}

void CCByteBuffer::write(const uint8* data, size_t size) {
	size_t new_size = m_writePos + size;
	if(new_size > m_bufferSize) {
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
	ensureCanWrite(value.length() + 1);
	memcpy(&m_buffer[m_writePos], value.c_str(), value.length() + 1);
	m_writePos += (value.length() + 1);
}

void CCByteBuffer::writePascalString(const string& value) {
	ensureCanWrite(value.length() + sizeof(uint16));
	write<uint16>(value.length());
	memcpy(&m_buffer[m_writePos], value.c_str(), value.length());
	m_writePos += value.length();
}

void CCByteBuffer::skip(size_t len) {
	if(m_readPos + len > m_writePos)
		len = (m_writePos - m_readPos);
	m_readPos += len;
}

void CCByteBuffer::ensureCanWrite(uint32 size) {
	size_t new_size = m_writePos + size;
	if(new_size > m_bufferSize) {
		new_size = (new_size / DEFAULT_INCREASE_SIZE + 1) * DEFAULT_INCREASE_SIZE;
		reserve(new_size);
	}
}

NS_CC_END