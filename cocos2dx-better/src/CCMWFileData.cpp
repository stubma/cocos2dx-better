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
#include "CCMWFileData.h"

NS_CC_BEGIN

CCMWFileData::CCMWFileData() :
		m_numOfImage(0),
		m_numOfAnimation(0),
		m_animationTable(NULL),
		m_frameTable(NULL),
		m_framePoolTable(NULL),
		m_imageClipPool(NULL),
		m_ellipseClipPool(NULL),
		m_lineClipPool(NULL),
		m_rectangleClipPool(NULL),
		m_roundedRectangleClipPool(NULL),
		m_positionerRectangleClipPool(NULL),
		m_frameTableIndex(NULL),
		m_imageIndex(NULL) {
}

CCMWFileData::~CCMWFileData() {
	free(m_animationTable);
	free(m_frameTable);
	free(m_framePoolTable);
	free(m_imageClipPool);
	free(m_ellipseClipPool);
	free(m_lineClipPool);
	free(m_rectangleClipPool);
	free(m_roundedRectangleClipPool);
	free(m_positionerRectangleClipPool);
	free(m_frameTableIndex);
	free(m_imageIndex);
}

NS_CC_END