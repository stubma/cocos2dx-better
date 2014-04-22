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
#include "CBSprite.h"

static unsigned char cc_2x2_white_image[] = {
    // RGBA8888
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF
};

#define CC_2x2_WHITE_IMAGE_KEY  "cc_2x2_white_image"

NS_CC_BEGIN

CBSprite::CBSprite() :
m_shouldUpdateBlendFunc(true),
m_preDrawFunction(NULL) {
}

CBSprite::~CBSprite() {
    CC_SAFE_RELEASE(m_preDrawFunction);
}

CBSprite* CBSprite::createWithTexture(CCTexture2D *pTexture)
{
    CBSprite *pobSprite = new CBSprite();
    if (pobSprite && pobSprite->initWithTexture(pTexture))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

CBSprite* CBSprite::createWithTexture(CCTexture2D *pTexture, const CCRect& rect)
{
    CBSprite *pobSprite = new CBSprite();
    if (pobSprite && pobSprite->initWithTexture(pTexture, rect))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

CBSprite* CBSprite::create(const char *pszFileName)
{
    CBSprite *pobSprite = new CBSprite();
    if (pobSprite && pobSprite->initWithFile(pszFileName))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

CBSprite* CBSprite::create(const char *pszFileName, const CCRect& rect)
{
    CBSprite *pobSprite = new CBSprite();
    if (pobSprite && pobSprite->initWithFile(pszFileName, rect))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

CBSprite* CBSprite::createWithSpriteFrame(CCSpriteFrame *pSpriteFrame)
{
    CBSprite *pobSprite = new CBSprite();
    if (pSpriteFrame && pobSprite && pobSprite->initWithSpriteFrame(pSpriteFrame))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

CBSprite* CBSprite::createWithSpriteFrameName(const char *pszSpriteFrameName)
{
    CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pszSpriteFrameName);
    
#if COCOS2D_DEBUG > 0
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", pszSpriteFrameName);
    CCAssert(pFrame != NULL, msg);
#endif
    
    return createWithSpriteFrame(pFrame);
}

CBSprite* CBSprite::create()
{
    CBSprite *pSprite = new CBSprite();
    if (pSprite && pSprite->init())
    {
        pSprite->autorelease();
        return pSprite;
    }
    CC_SAFE_DELETE(pSprite);
    return NULL;
}

void CBSprite::setTexture(CCTexture2D *texture) {
	// If batchnode, then texture id should be the same
    CCAssert(! m_pobBatchNode || texture->getName() == m_pobBatchNode->getTexture()->getName(), "CCSprite: Batched sprites should use the same texture as the batchnode");
    // accept texture==nil as argument
    CCAssert( !texture || dynamic_cast<CCTexture2D*>(texture), "setTexture expects a CCTexture2D. Invalid argument");
	
    if (NULL == texture)
    {
        // Gets the texture by key firstly.
        texture = CCTextureCache::sharedTextureCache()->textureForKey(CC_2x2_WHITE_IMAGE_KEY);
		
        // If texture wasn't in cache, create it from RAW data.
        if (NULL == texture)
        {
            CCImage* image = new CCImage();
            bool isOK = image->initWithImageData(cc_2x2_white_image, sizeof(cc_2x2_white_image), CCImage::kFmtRawData, 2, 2, 8);
            CCAssert(isOK, "The 2x2 empty texture was created unsuccessfully.");
			
            texture = CCTextureCache::sharedTextureCache()->addUIImage(image, CC_2x2_WHITE_IMAGE_KEY);
            CC_SAFE_RELEASE(image);
        }
    }
	
    if (!m_pobBatchNode && m_pobTexture != texture)
    {
        CC_SAFE_RETAIN(texture);
        CC_SAFE_RELEASE(m_pobTexture);
        m_pobTexture = texture;
		if(m_shouldUpdateBlendFunc)
			updateBlendFunc();
    }
}

void CBSprite::draw() {
    if(m_preDrawFunction)
        m_preDrawFunction->execute();
    CCSprite::draw();
}

NS_CC_END