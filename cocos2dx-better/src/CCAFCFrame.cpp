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
#include "CCAFCFrame.h"

NS_CC_BEGIN

CCAFCFrame::CCAFCFrame() :
		m_delay(0),
        m_flipX(false),
        m_flipY(false),
		m_useTickDelay(false),
		m_inc(ccpZero) {
}

CCAFCFrame::~CCAFCFrame() {
	for(AFCClipPtrList::iterator iter = m_clipList.begin(); iter != m_clipList.end(); iter++) {
		(*iter)->release();
	}
}

CCAFCFrame* CCAFCFrame::create() {
	CCAFCFrame* f = new CCAFCFrame();
	return (CCAFCFrame*)f->autorelease();
}

void CCAFCFrame::addClip(CCAFCClip* clip) {
	if(clip) {
		m_clipList.push_back(clip);
		clip->retain();
	}
}

int CCAFCFrame::getClipCount(CCAFCClipType type) {
	int count = 0;
	for(int i = 0; i < m_clipList.size(); i++) {
		CCAFCClip* clip = getClipAt(i);
		if(clip->getType() == type)
			count++;
	}
	return count;
}

CCAFCClip* CCAFCFrame::getClipAt(CCAFCClipType type, int index) {
	for(int i = 0; i < m_clipList.size(); i++) {
		CCAFCClip* clip = getClipAt(i);
		if(clip->getType() == type) {
			index--;
			if(index < 0)
				return clip;
		}
	}

	return NULL;
}

NS_CC_END