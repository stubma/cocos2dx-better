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
#ifndef __CCWorldPacket_h__
#define __CCWorldPacket_h__

#include "cocos2d.h"
#include "CCByteBuffer.h"

NS_CC_BEGIN

class CC_DLL CCWorldPacket : public CCByteBuffer {
protected:
    uint16 m_opcode;
	
public:
	CCWorldPacket();
	CCWorldPacket(uint16 opcode, size_t res);
	CCWorldPacket(size_t res);
    CCWorldPacket(const CCWorldPacket &packet);
	virtual ~CCWorldPacket();
	
	static CCWorldPacket* create();
	static CCWorldPacket* create(uint16 opcode);
	static CCWorldPacket* create(const CCWorldPacket& packet);
	
    /// Clear packet and set opcode all in one mighty blow
    void clear(uint16 opcode);
	
    uint16 GetOpcode() const { return m_opcode; }
    void SetOpcode(const uint16 & opcode) { m_opcode = opcode; }
	
	/// set length of packet
    void setLength(const uint16& len);
	
	/// get buffer as a string
    string getString() { return (const char*)getBuffer(); }
	
    /**
	 * write a string into buffer, the string has a length header, and the length
	 * can be different data type
	 */
    template<typename T> void writeString(const string& str);
	
    /**
	 * read a string from buffer, it will first read length for a custom data type
	 * then read bytes. If failed, it returns false
     */
    template<typename T> bool readString(string& str);
};

NS_CC_END

#endif // __CCWorldPacket_h__