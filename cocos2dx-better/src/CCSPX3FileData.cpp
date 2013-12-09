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
#include "CCSPX3FileData.h"

NS_CC_BEGIN

CCSPX3FileData::CCSPX3FileData() :
		m_version(0),
		m_tileSetCount(0),
		m_frameCount(0),
		m_actionCount(0) {
}

CCSPX3FileData::~CCSPX3FileData() {
	for(SPX3TileSetPtrList::iterator iter = m_tileSets.begin(); iter != m_tileSets.end(); iter++) {
		(*iter)->release();
	}

	for(SPX3FramePtrList::iterator iter = m_frames.begin(); iter != m_frames.end(); iter++) {
		(*iter)->release();
	}

	for(SPX3ActionPtrList::iterator iter = m_actions.begin(); iter != m_actions.end(); iter++) {
		(*iter)->release();
	}
}

CCSPX3FileData* CCSPX3FileData::create() {
	CCSPX3FileData* fd = new CCSPX3FileData();
	return (CCSPX3FileData*)fd->autorelease();
}

NS_CC_END