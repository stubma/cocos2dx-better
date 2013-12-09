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
#include "CCSPX3Action.h"
#include <stdlib.h>

NS_CC_BEGIN

CCSPX3Action::CCSPX3Action() :
		m_mode(SPX_DELAY),
		m_frameCount(0),
		m_frameIndices(NULL),
		m_delays(NULL) {
	memset(&m_collisionRect, 0, sizeof(CCSPX3Tile));
}

CCSPX3Action::~CCSPX3Action() {
	if(m_frameIndices)
		free(m_frameIndices);
	if(m_delays)
		free(m_delays);
}

CCSPX3Action* CCSPX3Action::create() {
	CCSPX3Action* a = new CCSPX3Action();
	return (CCSPX3Action*)a->autorelease();
}

NS_CC_END