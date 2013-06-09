/****************************************************************************
 Author: Luma (stubma@gmail.com)
 
 https://github.com/stubma/cocos2dx-common
 
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
#include "CCAutoRenderMenuItemSprite.h"
#include "CCUtils.h"

CCAutoRenderMenuItemSprite::CCAutoRenderMenuItemSprite() :
		m_focus(false),
		m_focusImage(NULL) {
}

CCAutoRenderMenuItemSprite::~CCAutoRenderMenuItemSprite() {
}

CCAutoRenderMenuItemSprite* CCAutoRenderMenuItemSprite::create(CCSprite* sprite, CCObject* target, SEL_MenuHandler selector) {
	CCAutoRenderMenuItemSprite* pRet = new CCAutoRenderMenuItemSprite();
    pRet->initWithNormalSprite(sprite, NULL, NULL, target, selector);
    pRet->autorelease();
    return pRet;
}

CCAutoRenderMenuItemSprite* CCAutoRenderMenuItemSprite::create(CCSprite* normalImage, CCSprite* focusImage) {
    CCAutoRenderMenuItemSprite* pRet = new CCAutoRenderMenuItemSprite();
    pRet->initWithNormalSprite(normalImage, NULL, NULL, NULL, NULL);
    pRet->setFocusImage(focusImage);
    pRet->autorelease();
    return pRet;
}

CCAutoRenderMenuItemSprite* CCAutoRenderMenuItemSprite::create(CCSprite* normalImage, CCSprite* focusImage, CCObject* target, SEL_MenuHandler selector) {
	CCAutoRenderMenuItemSprite* pRet = new CCAutoRenderMenuItemSprite();
    pRet->initWithNormalSprite(normalImage, NULL, NULL, target, selector);
	pRet->setFocusImage(focusImage);
    pRet->autorelease();
    return pRet;
}

CCAutoRenderMenuItemSprite* CCAutoRenderMenuItemSprite::create(CCSprite* normalImage, CCSprite* selectedImage,
															   CCSprite* focusImage, CCObject* target, SEL_MenuHandler selector) {
	CCAutoRenderMenuItemSprite* pRet = new CCAutoRenderMenuItemSprite();
    pRet->initWithNormalSprite(normalImage, selectedImage, NULL, target, selector);
	pRet->setFocusImage(focusImage);
    pRet->autorelease();
    return pRet;
}

CCAutoRenderMenuItemSprite* CCAutoRenderMenuItemSprite::create(CCSprite* normalImage, CCSprite* selectedImage, CCSprite* disabledImage,
															   CCSprite* focusImage, CCObject* target, SEL_MenuHandler selector) {
	CCAutoRenderMenuItemSprite* pRet = new CCAutoRenderMenuItemSprite();
    pRet->initWithNormalSprite(normalImage, selectedImage, disabledImage, target, selector);
	pRet->setFocusImage(focusImage);
    pRet->autorelease();
    return pRet;
}

void CCAutoRenderMenuItemSprite::selected() {
	CCMenuItemSprite::selected();
	
	// set a darker color
	if(!getSelectedImage()) {
		m_oldColor = getColor();
		ccColorHSV hsv = CCUtils::ccc32hsv(m_oldColor);
		hsv.v = MAX(0, hsv.v - 0.2f);
		ccColor3B c = CCUtils::hsv2ccc3(hsv);
		setColor(c);
	}
}

void CCAutoRenderMenuItemSprite::unselected() {
	CCMenuItemSprite::unselected();
	
	// restore old color
	if(!getSelectedImage()) {
		setColor(m_oldColor);
	}
}

void CCAutoRenderMenuItemSprite::updateImagesVisibility() {
	if (m_pNormalImage)
		m_pNormalImage->setVisible(false);
	if (m_pSelectedImage)
		m_pSelectedImage->setVisible(false);
	if (m_pDisabledImage)
		m_pDisabledImage->setVisible(false);
	if (m_focusImage)
		m_focusImage->setVisible(false);
	
	if(m_focusImage) {
		m_focusImage->setVisible(m_focus);
		if(!m_focus)
			CCMenuItemSprite::updateImagesVisibility();
	} else {
		CCMenuItemSprite::updateImagesVisibility();
	}
}

void CCAutoRenderMenuItemSprite::setFocus(bool flag) {
	m_focus = flag;
	updateImagesVisibility();
}

void CCAutoRenderMenuItemSprite::setFocusImage(CCSprite* focusImage) {
	if (focusImage != m_focusImage) {
        if (focusImage) {
            addChild(focusImage, 0);
            focusImage->setAnchorPoint(ccp(0, 0));
        }
		
        if (m_focusImage) {
            removeChild(m_focusImage, true);
        }
		
        m_focusImage = focusImage;
		updateImagesVisibility();
    }
}