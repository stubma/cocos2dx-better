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
#include "CCWorldPacket.h"

NS_CC_BEGIN

CCWorldPacket::CCWorldPacket() :
m_opcode(0) {
}

CCWorldPacket::CCWorldPacket(uint16 opcode, size_t res) :
CCByteBuffer(res),
m_opcode(opcode) {
}

CCWorldPacket::CCWorldPacket(size_t res) :
CCByteBuffer(res),
m_opcode(0) {
}

CCWorldPacket::CCWorldPacket(const CCWorldPacket& packet) :
CCByteBuffer(packet),
m_opcode(packet.m_opcode) {
}

CCWorldPacket::~CCWorldPacket() {
}

CCWorldPacket* CCWorldPacket::create() {
	CCWorldPacket* p = new CCWorldPacket();
	return (CCWorldPacket*)p->autorelease();
}

CCWorldPacket* CCWorldPacket::create(uint16 opcode) {
	CCWorldPacket* p = new CCWorldPacket(opcode);
	return (CCWorldPacket*)p->autorelease();
}

CCWorldPacket* CCWorldPacket::create(const CCWorldPacket& packet) {
	CCWorldPacket* p = new CCWorldPacket(packet);
	return (CCWorldPacket*)p->autorelease();
}

void CCWorldPacket::clear(uint16 opcode) {
	CCByteBuffer::clear();
	m_opcode = opcode;
}

void CCWorldPacket::setLength(const uint16& len) {
	uint16* plen = (uint16*)&(getBuffer()[2]);
	*plen = len;
}

template<typename T>
void CCWorldPacket::writeString(const string& str) {
	T ilen = (T)str.size();
	write<T>(ilen);
	if(ilen > 0)
		write((const uint8*)str.c_str(), ilen);
}

template<typename T>
bool CCWorldPacket::readString(string &str) {
	T ilen = read<T>();
	if (ilen == 0)
		return true;
	uint8 buf[ilen + 1];
	uint16 plen = available();
	if (plen < ilen)
		return false;
	read(buf, ilen);
	buf[ilen] = 0;
	str = (char*)buf;
	return true;
}

NS_CC_END