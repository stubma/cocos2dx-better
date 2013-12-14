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

NS_CC_BEGIN

/**
 * Byte buffer
 */
class CC_DLL CCByteBuffer : public CCObject {
protected:
    uint8* m_buffer;
    size_t m_readPos;
    size_t m_writePos;
    uint32 m_bufferSize;
	
protected:
	/// Allocates/reallocates buffer with specified size.
    void reserve(size_t res);
	
	/**
	 * Ensures the buffer is big enough to fit the specified number of bytes.
	 *
     * @param size number of bytes to fit
     */
    void ensureCanWrite(uint32 size);
	
public:
	CCByteBuffer();
    CCByteBuffer(size_t res);
	CCByteBuffer(const CCByteBuffer& b);
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
    uint32 available() { return m_writePos; }
	
    /** Reads sizeof(T) bytes from the buffer
     * @return the bytes read
     */
    template<typename T> T read();
	
	/// skip bytes, move read position forward
    void skip(size_t len);
	
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
	
    /** 
	 * Writes sizeof(T) bytes to the buffer, while checking for overflows.
	 *
     * @param T data The data to be written
     */
    template<typename T> void write(const T& data);
	
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
	
    /// get read position
    size_t getReadPos() { return m_readPos; }
	
	/// move read position
    void setReadPos(size_t p) { if(p <= m_writePos) m_readPos = p; }
	
	/// set write position, that will change available size
    void setWritePos(size_t p) { if(p <= m_bufferSize) m_writePos = p; }
	
	/// write a vector
    template<typename T> size_t writeVector(const vector<T>& v);
	
	/// read a vector
    template<typename T> size_t readVector(size_t vsize, vector<T>& v);
	
	/// write a list
    template<typename T> size_t writeList(const list<T>& v);
	
	/// read a list
    template<typename T> size_t readList(size_t vsize, list<T>& v);
	
	/// write a map
    template <typename K, typename V> size_t writeMap(const map<K, V>& m);
	
	/// read a map
    template <typename K, typename V> size_t readMap(size_t msize, map<K, V>& m);
};

NS_CC_END

#endif // __CCByteBuffer_h__