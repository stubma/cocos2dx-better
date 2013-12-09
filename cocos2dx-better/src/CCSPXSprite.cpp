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
#include "CCSPXSprite.h"
#include "CCSPXManager.h"

NS_CC_BEGIN

CCSPXSprite::CCSPXSprite() :
		m_spx(NULL) {
}

CCSPXSprite::~CCSPXSprite() {
	CC_SAFE_RELEASE(m_spx);
}

CCSPXSprite* CCSPXSprite::create(const char* spxPath, CCTexture2D* tex, int actionIndex) {
	// create sprite
	CCSPXSprite* s = new CCSPXSprite();

	// get spx file data
	s->m_spx = CCSPXManager::getInstance()->load(spxPath);
	s->m_spx->retain();

	// create batch node for texture
	CCSpriteBatchNode* sheet = CCAFCSprite::createBatchNode(tex);
	s->m_sheetList.push_back(sheet);
	sheet->retain();

	// start action
	s->playAnimation(actionIndex);

	// return
	return (CCSPXSprite*)s->autorelease();
}

CCAFCAnimation* CCSPXSprite::getAnimationAt(int index, CCAFCClipMapping* mapping) {
	if(index < 0 || index >= getAnimationCount())
		return NULL;
	else
		return CCSPXManager::getInstance()->getAnimationData(m_spx, index, mapping);
}

int CCSPXSprite::getAnimationCount() {
	return m_spx->m_actionCount;
}

int CCSPXSprite::getImageCount() {
	return 1;
}

NS_CC_END