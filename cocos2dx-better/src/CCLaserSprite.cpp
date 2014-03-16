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
#include "CCLaserSprite.h"
#include "CCShaders.h"

static unsigned char cc_2x2_white_image[] = {
    // RGBA8888
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF
};

#define CC_2x2_WHITE_IMAGE_KEY  "cc_2x2_white_image"

NS_CC_BEGIN

CCLaserSprite::CCLaserSprite() {
	
}

CCLaserSprite::~CCLaserSprite() {
}

CCLaserSprite* CCLaserSprite::create(float w, float h) {
	CCLaserSprite* s = new CCLaserSprite();
	s->initWithTexture(NULL);
	s->setContentSize(CCSizeMake(w, h));
	s->setTextureRect(CCRectMake(0, 0, w, h));
	s->setTextureCoords(CCRectMake(0, 0, 2, 2));
	return (CCLaserSprite*)s->autorelease();
}

bool CCLaserSprite::initWithTexture(CCTexture2D *pTexture) {
	// we can't set default texture to laser sprite
	if(!pTexture || CCTextureCache::sharedTextureCache()->textureForKey(CC_2x2_WHITE_IMAGE_KEY) != pTexture) {
		pTexture = CCTextureCache::sharedTextureCache()->textureForKey(CC_2x2_WHITE_IMAGE_KEY);
		if (!pTexture) {
			CCImage* image = new CCImage();
			bool isOK = image->initWithImageData(cc_2x2_white_image, sizeof(cc_2x2_white_image), CCImage::kFmtRawData, 2, 2, 8);
			CCAssert(isOK, "The 2x2 empty texture was created unsuccessfully.");
			pTexture = CCTextureCache::sharedTextureCache()->addUIImage(image, CC_2x2_WHITE_IMAGE_KEY);
			CC_SAFE_RELEASE(image);
		}
	}
	
	// super
	if(!CCSprite::initWithTexture(pTexture)) {
		return false;
	}
	
	// set laser shader
	setShaderProgram(CCShaders::programForKey(kCCShader_laser));
	
	return true;
}

void CCLaserSprite::setTexture(CCTexture2D *texture) {
	if(CCTextureCache::sharedTextureCache()->textureForKey(CC_2x2_WHITE_IMAGE_KEY) != texture) {
		CCLOGWARN("Doesn't support set texture to CCLaserSprite");
	} else {
		CCSprite::setTexture(texture);
	}
}

void CCLaserSprite::setWidth(float w) {
	CCSize size = getContentSize();
	size.width = w;
	setContentSize(size);
	setTextureRect(CCRectMake(0, 0, w, size.height));
	setTextureCoords(CCRectMake(0, 0, 2, 2));
}

void CCLaserSprite::setHeight(float h) {
	CCSize size = getContentSize();
	size.height = h;
	setContentSize(size);
	setTextureRect(CCRectMake(0, 0, size.width, h));
	setTextureCoords(CCRectMake(0, 0, 2, 2));
}

NS_CC_END