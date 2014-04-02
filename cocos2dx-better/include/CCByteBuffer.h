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
#ifndef __CCByteBuffer_h__
#define __CCByteBuffer_h__

#include "cocos2d.h"
#include "CCNetworkCommon.h"

using namespace std;

NS_CC_BEGIN

/**
 * Byte buffer
 */
class CC_DLL CCByteBuffer : public CCObject {
private:
    static const int DEFAULT_INCREASE_SIZE = 200;
    
protected:
	/// buffer pointer
    uint8* m_buffer;
	
	/// read position
    size_t m_readPos;
	
	/// write position
    size_t m_writePos;
	
	/// buffer memory size
    size_t m_bufferSize;
    
    /// external mode
    bool m_external;
	
protected:
	/// Allocates/reallocates buffer with specified size.
    void reserve(size_t res);
	
	/**
	 * Ensures the buffer is big enough to fit the specified number of bytes.
	 *
     * @param size number of bytes to fit
     */
    void ensureCanWrite(size_t size);
	
public:
	CCByteBuffer();
    CCByteBuffer(size_t res);
	CCByteBuffer(const CCByteBuffer& b);
    
    /**
     * it wraps an external buffer insteal of allocating memory
     *
     * @param buf buffer
     * @param bufSize total size of buffer
     * @param dataLen available data in buffer
     */
    CCByteBuffer(const char* buf, size_t bufSize, size_t dataLen);
    
    virtual ~CCByteBuffer();
	
	/// Creates a CCByteBuffer with the default size
	static CCByteBuffer* create();
	
	/// Creates a CCByteBuffer with the specified size
	static CCByteBuffer* create(size_t res);
	
    /// Resets read/write indexes
    void clear() { m_readPos = m_writePos = 0; }
	
    /// Returns the buffer pointer
    const uint8* getBuffer() { return m_buffer; }
	
    /// Gets the readable content size.
    size_t available() { return m_writePos - m_readPos; }
	
    /** 
	 * Reads sizeof(T) bytes from the buffer
	 *
     * @return the bytes read
     */
    template<typename T> T read() {
        if(m_readPos + sizeof(T) > m_writePos)
            return (T)0;
        T ret = *(T*)&m_buffer[m_readPos];
        m_readPos += sizeof(T);
        return ret;
    }
	
	/// skip bytes, move read position forward
    void skip(size_t len);
    
    /// move read pos back
    void revoke(size_t len);
    
    /// move available data to start of buffer
    void compact();
	
    /**
	 * Reads x bytes from the buffer, if len exceeds max available bytes, only available bytes
	 * is read
	 * 
	 * @param buffer read buffer
	 * @param len bytes want to be read, read buffer must be large enough to hold those bytes
	 * @return actual read bytes
	 */
    size_t read(uint8* buffer, size_t len);
	
	/// read a string
	void read(string& dest);
	
	/// read a c string
	void readCString(string& dest);
	
	/// read a pascal string from buffer, i.e., the first two bytes must be string length
	void readPascalString(string& dest);
	
	/// read a string, until encounter new line or end
	void readLine(string& dest);
	
    /** 
	 * Writes sizeof(T) bytes to the buffer, while checking for overflows.
	 *
     * @param T data The data to be written
     */
    template<typename T> void write(const T& data) {
        size_t new_size = m_writePos + sizeof(T);
        if(new_size > m_bufferSize) {
            new_size = (new_size / DEFAULT_INCREASE_SIZE + 1) * DEFAULT_INCREASE_SIZE;
            reserve(new_size);
        }
        
        *(T*)&m_buffer[m_writePos] = data;
        m_writePos += sizeof(T);
    }
    
    /** writes x bytes to the buffer, while checking for overflows
     * @param ptr the data to be written
     * @param size byte count
	 */
    void write(const uint8* data, size_t size);
	
    /// specialize write for string
	void write(const string& value);
	
	/// write a c string
	void writeCString(const string& value);
	
	/**
	 * write a string into buffer in pascal format, i.e., the first two bytes is string length and
	 * string won't be appended a zero byte
	 */
	void writePascalString(const string& value);
	
	/**
	 * write a string and new line to buffer
	 * 
	 * @param value string to be written
	 */
	void writeLine(const string& value);
	
    /// get read position
    size_t getReadPos() { return m_readPos; }
	
	/// move read position
    void setReadPos(size_t p) { if(p <= m_writePos) m_readPos = p; }
	
	/// set write position, that will change available size
    void setWritePos(size_t p) { if(p <= m_bufferSize) m_writePos = p; }
	
	/// write a vector
    template<typename T> size_t writeVector(const vector<T>& v) {
        for(typename vector<T>::const_iterator i = v.begin(); i != v.end(); i++) {
            write<T>(*i);
        }
        return v.size();
    }
	
	/// read a vector
    template<typename T> size_t readVector(size_t vsize, vector<T>& v) {
        v.clear();
        while(vsize--) {
            T t = read<T>();
            v.push_back(t);
        }
        return v.size();
    }
	
	/// write a list
    template<typename T> size_t writeList(const list<T>& v) {
        for(typename list<T>::const_iterator i = v.begin(); i != v.end(); i++) {
            write<T>(*i);
        }
        return v.size();
    }
	
	/// read a list
    template<typename T> size_t readList(size_t vsize, list<T>& v) {
        v.clear();
        while(vsize--) {
            T t = read<T>();
            v.push_back(t);
        }
        return v.size();
    }
	
	/// write a map
    template <typename K, typename V> size_t writeMap(const map<K, V>& m) {
        for(typename map<K, V>::const_iterator i = m.begin(); i != m.end(); i++) {
            write<K>(i->first);
            write<V>(i->second);
        }
        return m.size();
    }
	
	/// read a map
    template <typename K, typename V> size_t readMap(size_t msize, map<K, V>& m) {
        m.clear();
        while(msize--) {
            K k = read<K>();
            V v = read<V>();
            m.insert(make_pair(k, v));
        }
        return m.size();
    }
};

NS_CC_END

#endif // __CCByteBuffer_h__