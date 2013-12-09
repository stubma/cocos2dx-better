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
#include "CCMWSprite.h"
#include <stdarg.h>
#include "CCMWManager.h"

NS_CC_BEGIN

CCMWSprite* CCMWSprite::create(const char* path, int animIndex, CCTexture2D** tex, int count) {
	// create mw sprite
	CCMWSprite* sprite = new CCMWSprite();

	// load anu file data
	sprite->m_mw = CCMWManager::getInstance()->load(path);
	sprite->m_mw->retain();

	// add others
	for(int i = 0; i < count; i++) {
		CCSpriteBatchNode* sheet = CCAFCSprite::createBatchNode(tex[i]);
		sprite->m_sheetList.push_back(sheet);
		sheet->retain();
	}

	// start animation
	sprite->playAnimation(animIndex);

	// return
	sprite->autorelease();
	return sprite;
}

CCMWSprite* CCMWSprite::create(const char* path, int animIndex, CCTexture2D* tex, ...) {
	// create mw sprite
	CCMWSprite* sprite = new CCMWSprite();

	// load anu file data
	sprite->m_mw = CCMWManager::getInstance()->load(path);
	sprite->m_mw->retain();

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
	sprite->playAnimation(animIndex);

	// return
	sprite->autorelease();
	return sprite;
}

CCMWSprite::CCMWSprite() :
		m_mw(NULL) {
}

CCMWSprite::~CCMWSprite() {
	CC_SAFE_RELEASE(m_mw);
}

CCAFCAnimation* CCMWSprite::getAnimationAt(int index, CCAFCClipMapping* mapping) {
	if(index < 0 || index >= getAnimationCount())
		return NULL;
	else
		return CCMWManager::getInstance()->getAnimationData(m_mw, index, mapping);
}

int CCMWSprite::getAnimationCount() {
	return m_mw->m_numOfAnimation;
}

int CCMWSprite::getImageCount() {
	return m_mw->m_numOfImage;
}

NS_CC_END