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
#include "CCAuroraFileData.h"
#include <stdlib.h>

NS_CC_BEGIN

CCAuroraFileData::CCAuroraFileData() :
		m_flags(0),
		m_imageCount(0),
		m_moduleCount(0),
		m_frameModuleCount(0),
		m_frameCount(0),
		m_collisionRectCount(0),
		m_animationFrameCount(0),
		m_animationCount(0),
		m_modules(NULL),
		m_frameModules(NULL),
		m_collisionRects(NULL),
		m_frames(NULL),
		m_animationFrames(NULL),
		m_animations(NULL) {
}

CCAuroraFileData::~CCAuroraFileData() {
	if(m_modules)
		free(m_modules);
	if(m_frameModules)
		free(m_frameModules);
	if(m_collisionRects)
		free(m_collisionRects);
	if(m_frames)
		free(m_frames);
	if(m_animationFrames)
		free(m_animationFrames);
	if(m_animations)
		free(m_animations);
}

CCAuroraFileData* CCAuroraFileData::create() {
	CCAuroraFileData* d = new CCAuroraFileData();
	return (CCAuroraFileData*)d->autorelease();
}

NS_CC_END