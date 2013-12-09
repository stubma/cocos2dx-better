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
#include "CCSPX3Sprite.h"
#include "CCSPX3Manager.h"
#include <stdarg.h>

NS_CC_BEGIN

CCSPX3Sprite::CCSPX3Sprite() :
		m_spx(NULL) {
}

CCSPX3Sprite::~CCSPX3Sprite() {
	CC_SAFE_RELEASE(m_spx);
}

CCSPX3Sprite* CCSPX3Sprite::create(const char* spxPath, int actionIndex, CCTexture2D** tex, int count) {
	// create mw sprite
	CCSPX3Sprite* sprite = new CCSPX3Sprite();

	// load anu file data
	sprite->m_spx = CCSPX3Manager::getInstance()->load(spxPath);
	sprite->m_spx->retain();

	// add others
	for(int i = 0; i < count; i++) {
		CCSpriteBatchNode* sheet = CCAFCSprite::createBatchNode(tex[i]);
		sprite->m_sheetList.push_back(sheet);
		sheet->retain();
	}

	// start animation
	sprite->playAnimation(actionIndex);

	// return
	sprite->autorelease();
	return sprite;
}

CCSPX3Sprite* CCSPX3Sprite::create(const char* spxPath, int actionIndex, CCTexture2D* tex, ...) {
	// create mw sprite
	CCSPX3Sprite* sprite = new CCSPX3Sprite();

	// load anu file data
	sprite->m_spx = CCSPX3Manager::getInstance()->load(spxPath);
	sprite->m_spx->retain();

	va_list textures;
	va_start(textures, tex);

	// add first
	CCSpriteBatchNode* sheet = CCAFCSprite::createBatchNode(tex);
	sprite->m_sheetList.push_back(sheet);
	sheet->retain();

	// add others
	for(CCTexture2D* t = va_arg(textures, CCTexture2D*); t != NULL; t = va_arg(textures, CCTexture2D*)) {
		sheet = CCAFCSprite::createBatchNode(t);
		sprite->m_sheetList.push_back(sheet);
		sheet->retain();
	}

	va_end(textures);

	// start animation
	sprite->playAnimation(actionIndex);

	// return
	sprite->autorelease();
	return sprite;
}

CCAFCAnimation* CCSPX3Sprite::getAnimationAt(int index, CCAFCClipMapping* mapping) {
	if(index < 0 || index >= getAnimationCount())
		return NULL;
	else
		return CCSPX3Manager::getInstance()->getAnimationData(m_spx, index, mapping);
}

int CCSPX3Sprite::getAnimationCount() {
	return m_spx->m_actionCount;
}

int CCSPX3Sprite::getImageCount() {
	return m_spx->m_tileSetCount;
}

NS_CC_END