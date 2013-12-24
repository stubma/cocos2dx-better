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
#include "CCPointList.h"

NS_CC_BEGIN

CCPointList::CCPointList() :
m_capacity(10),
m_buffer(NULL),
m_count(0) {
	m_buffer = (CCPoint*)malloc(m_capacity * sizeof(CCPoint));
}

CCPointList::~CCPointList() {
	CC_SAFE_FREE(m_buffer);
}

CCPointList* CCPointList::create() {
    CCPointList* pl = new CCPointList();
    return (CCPointList*)pl->autorelease();
}

void CCPointList::addPoint(float x, float y) {
	while(m_count >= m_capacity) {
		m_capacity *= 2;
		m_buffer = (CCPoint*)realloc(m_buffer, m_capacity * sizeof(CCPoint));
	}

	m_buffer[m_count].x = x;
	m_buffer[m_count].y = y;
	m_count++;
}

void CCPointList::addPoints(CCPointList& plist) {
	while(m_count + plist.getCount() >= m_capacity) {
		m_capacity *= 2;
		m_buffer = (CCPoint*)realloc(m_buffer, m_capacity * sizeof(CCPoint));
	}

	memcpy(m_buffer + m_count, plist.getBuffer(), plist.getCount() * sizeof(CCPoint));
	m_count += plist.getCount();
}

void CCPointList::clear() {
	m_count = 0;
}

void CCPointList::deletePointAt(int index) {
	if(index < 0 || index >= m_count)
		return;

	if(index == m_count - 1) {
		m_count--;
	} else {
		memmove(m_buffer + index, m_buffer + index + 1, (m_count - index - 1) * sizeof(CCPoint));
		m_count--;
	}
}

CCPoint CCPointList::getPointAt(int index) const {
	if(m_count > 0) {
		index = MAX(0, MIN(m_count - 1, index));
		return m_buffer[index];
	} else {
		return CCPointZero;
	}
}

NS_CC_END