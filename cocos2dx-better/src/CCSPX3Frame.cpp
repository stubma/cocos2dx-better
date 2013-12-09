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
#include "CCSPX3Frame.h"
#include <stdlib.h>

NS_CC_BEGIN

CCSPX3Frame::CCSPX3Frame() :
		m_flags(0),
		m_patchCount(0),
		m_collisionCount(0),
		m_patches(NULL),
		m_collisionRects(NULL) {
	memset(&m_rect, 0, sizeof(CCSPX3Tile));
}

CCSPX3Frame::~CCSPX3Frame() {
	if(m_patches) {
		CCSPX3Patch* p = m_patches;
		for(int i = 0; i < m_patchCount; i++, p++) {
			if(p->type == SPX_PATCH_SHAPE) {
				if(p->shape.params)
					free(p->shape.params);
			}
		}

		free(m_patches);
	}

	if(m_collisionRects)
		free(m_collisionRects);
}

CCSPX3Frame* CCSPX3Frame::create() {
	CCSPX3Frame* ts = new CCSPX3Frame();
	return (CCSPX3Frame*)ts->autorelease();
}

NS_CC_END