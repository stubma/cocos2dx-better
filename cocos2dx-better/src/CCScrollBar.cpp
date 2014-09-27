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
#include "CCScrollBar.h"
#include "CCUtils.h"
#include "CCTreeFadeOut.h"

NS_CC_BEGIN

#define FADE_OUT_TIME 1
#define TAG_FADE_OUT 1000

CCScrollBar::CCScrollBar() :
m_thumb(NULL),
m_track(NULL),
m_fixedThumb(NULL),
m_fadingOut(false),
m_fadeOutTimer(0),
m_oldCCDelegate(NULL),
m_autoFade(false),
m_initFadeOut(false) {
	
}

CCScrollBar::~CCScrollBar() {
	CC_SAFE_RELEASE(m_track);
	CC_SAFE_RELEASE(m_thumb);
	CC_SAFE_RELEASE(m_fixedThumb);
}

CCScrollBar* CCScrollBar::create(CCScale9Sprite* track, CCScale9Sprite* thumb) {
	CCScrollBar* sb = new CCScrollBar();
	sb->initWithTrackAndThumb(track, thumb);
	return (CCScrollBar*)sb->autorelease();
}

CCScrollBar* CCScrollBar::create(CCScale9Sprite* track, CCSprite* thumb) {
	CCScrollBar* sb = new CCScrollBar();
	sb->initWithTrackAndFixedThumb(track, thumb);
	return (CCScrollBar*)sb->autorelease();
}

bool CCScrollBar::initWithTrackAndFixedThumb(CCScale9Sprite* track, CCSprite* thumb) {
	if(!CCLayer::init()) {
		return false;
	}
	
	// anchor
	ignoreAnchorPointForPosition(false);
	
	// save track and thumb
	setTrack(track);
	setFixedThumb(thumb);
	
    // update
    scheduleUpdate();
    
	return true;
}

bool CCScrollBar::initWithTrackAndThumb(CCScale9Sprite* track, CCScale9Sprite* thumb) {
	if(!CCLayer::init()) {
		return false;
	}
	
	// anchor
	ignoreAnchorPointForPosition(false);
	
	// save track and thumb
	setTrack(track);
	setThumb(thumb);
    
    // update
    scheduleUpdate();
	
	return true;
}

void CCScrollBar::attachToUIScrollView(ScrollView* scrollView, ccInsets insets, bool horizontal) {
	// save flag
	m_horizontal = horizontal;
	
	// add to scroll view
	float thumbLength = 0;
	Widget* svParent = dynamic_cast<Widget*>(scrollView->getParent());
	CCSize svSize = scrollView->getSize();
	CCPoint svOrigin = CCUtils::getOrigin(scrollView);
	CCSize innerSize = scrollView->getInnerContainerSize();
	CCSize sbSize;
	if(horizontal) {
		sbSize = CCSizeMake(m_track->getContentSize().width,
							svSize.width - insets.left - insets.right);
		setContentSize(sbSize);
		setAnchorPoint(ccp(0, 0.5f));
		setPosition(ccp(svOrigin.x + svSize.width / 2,
						svOrigin.y + insets.bottom));
		setRotation(-90);
		svParent->addNode(this, MAX_INT);
		
		// thumb length
		if(m_fixedThumb)
			thumbLength = m_fixedThumb->getContentSize().height;
		else
			thumbLength = MIN(1, svSize.width / innerSize.width) * sbSize.height;
	} else {
		sbSize = CCSizeMake(m_track->getContentSize().width,
							svSize.height - insets.top - insets.bottom);
		setContentSize(sbSize);
		setAnchorPoint(ccp(1, 0.5f));
		setPosition(ccp(svOrigin.x + svSize.width - insets.right,
						svOrigin.y + svSize.height / 2));
		svParent->addNode(this, MAX_INT);
		
		// thumb length
		if(m_fixedThumb)
			thumbLength = m_fixedThumb->getContentSize().height;
		else
			thumbLength = MIN(1, svSize.height / innerSize.height) * sbSize.height;
	}
	
	// add track
	m_track->setPreferredSize(sbSize);
	m_track->setPosition(CCUtils::getLocalCenter(this));
	addChild(m_track);
	
	// clipping node to hold thumb
	CCClippingNode* thumbClipping = CCClippingNode::create(m_track);
	thumbClipping->ignoreAnchorPointForPosition(false);
	thumbClipping->setAnchorPoint(ccp(0.5f, 0.5f));
	thumbClipping->setContentSize(sbSize);
	thumbClipping->setPosition(CCUtils::getLocalCenter(this));
	thumbClipping->setAlphaThreshold(0.5f);
	thumbClipping->setScaleX((sbSize.width - 4) / sbSize.width);
	thumbClipping->setScaleY((sbSize.height - 4) / sbSize.height);
	addChild(thumbClipping);
	
	// thumb or fixed thumb
	if(m_thumb) {
		m_thumb->setPreferredSize(CCSizeMake(sbSize.width, thumbLength));
		m_thumb->setPosition(ccp(sbSize.width / 2,
								 sbSize.height - thumbLength / 2));
		thumbClipping->addChild(m_thumb);
	} else {
		m_fixedThumb->setPosition(ccp(sbSize.width / 2,
									  sbSize.height - thumbLength / 2));
		thumbClipping->addChild(m_fixedThumb);
	}
	
	// sync thumb position
	syncThumbPositionSizeForUIScrollView(scrollView);
	
	// listen to scrollview scrolling event
	scrollView->addEventListenerScrollView(this, scrollvieweventselector(CCScrollBar::onUIScrollViewEvent));
    
    // init fade out
    if(m_initFadeOut) {
        m_fadingOut = true;
        CCUtils::setOpacityRecursively(this, 0);
    }
}

void CCScrollBar::attachToCCScrollView(CCScrollView* scrollView, ccInsets insets, bool horizontal) {
    // it must have parent node
    CCNode* svParent = scrollView->getParent();
    if(!svParent) {
        CCLOGWARN("CCScrollView must be added to one node before calling attachToCCScrollView");
        return;
    }
    
	// save flag
	m_horizontal = horizontal;
	
	// add to scroll view
	float thumbLength = 0;
    CCPoint svOrigin = CCUtils::getOrigin(scrollView);
	CCSize svSize = scrollView->getViewSize();
	CCSize innerSize = scrollView->getContainer()->getContentSize();
	CCSize sbSize;
	if(horizontal) {
		sbSize = CCSizeMake(m_track->getContentSize().width,
							svSize.width - insets.left - insets.right);
		setContentSize(sbSize);
		setAnchorPoint(ccp(0, 0.5f));
		setPosition(ccp(svOrigin.x + svSize.width / 2, svOrigin.y + insets.bottom));
		setRotation(-90);
        UIWidget* svpWidght = dynamic_cast<UIWidget*>(svParent);
        if(svpWidght)
            svpWidght->addNode(this, MAX_INT);
        else
            svParent->addChild(this, MAX_INT);
		
		// thumb length
		if(m_fixedThumb)
			thumbLength = m_fixedThumb->getContentSize().height;
		else
			thumbLength = MIN(1, svSize.width / innerSize.width) * sbSize.height;
	} else {
		sbSize = CCSizeMake(m_track->getContentSize().width,
							svSize.height - insets.top - insets.bottom);
		setContentSize(sbSize);
		setAnchorPoint(ccp(1, 0.5f));
		setPosition(ccp(svOrigin.x + svSize.width - insets.right, svOrigin.y + svSize.height / 2));
        UIWidget* svpWidght = dynamic_cast<UIWidget*>(svParent);
        if(svpWidght)
            svpWidght->addNode(this, MAX_INT);
        else
            svParent->addChild(this, MAX_INT);
		
		// thumb length
		if(m_fixedThumb)
			thumbLength = m_fixedThumb->getContentSize().height;
		else
			thumbLength = MIN(1, svSize.height / innerSize.height) * sbSize.height;
	}
	
	// add track
	m_track->setPreferredSize(sbSize);
	m_track->setPosition(CCUtils::getLocalCenter(this));
	addChild(m_track);
	
	// clipping node to hold thumb
	CCClippingNode* thumbClipping = CCClippingNode::create(m_track);
	thumbClipping->ignoreAnchorPointForPosition(false);
	thumbClipping->setAnchorPoint(ccp(0.5f, 0.5f));
	thumbClipping->setContentSize(sbSize);
	thumbClipping->setPosition(CCUtils::getLocalCenter(this));
	thumbClipping->setAlphaThreshold(0.5f);
	thumbClipping->setScaleX((sbSize.width - 4) / sbSize.width);
	thumbClipping->setScaleY((sbSize.height - 4) / sbSize.height);
	addChild(thumbClipping);
	
	// thumb or fixed thumb
	if(m_thumb) {
		m_thumb->setPreferredSize(CCSizeMake(sbSize.width, thumbLength));
		m_thumb->setPosition(ccp(sbSize.width / 2,
								 sbSize.height - thumbLength / 2));
		thumbClipping->addChild(m_thumb);
	} else {
		m_fixedThumb->setPosition(ccp(sbSize.width / 2,
									  sbSize.height - thumbLength / 2));
		thumbClipping->addChild(m_fixedThumb);
	}
	
	// sync thumb position
	syncThumbPositionSizeForCCScrollView(scrollView);
    
    // delegate
    m_oldCCDelegate = scrollView->getDelegate();
    scrollView->setDelegate(this);
    
    // init fade out
    if(m_initFadeOut) {
        m_fadingOut = true;
        CCUtils::setOpacityRecursively(this, 0);
    }
}

void CCScrollBar::syncThumbPositionSizeForCCScrollView(CCScrollView* scrollView) {
	CCSize svSize = scrollView->getViewSize();
	CCSize innerSize = scrollView->getContainer()->getContentSize();
	CCSize sbSize = getContentSize();
	CCSize thumbSize = m_thumb ? m_thumb->getContentSize() : m_fixedThumb->getContentSize();
	float percentage = 0;
    float thumbLength = 0;
	if(m_horizontal) {
		float minX = svSize.width - innerSize.width;
		percentage = (scrollView->getContainer()->getPosition().x - minX) / -minX;
        thumbLength = MIN(1, svSize.width / innerSize.width) * sbSize.height;
	} else {
		float minY = svSize.height - innerSize.height;
		percentage = (scrollView->getContainer()->getPosition().y - minY) / -minY;
        thumbLength = MIN(1, svSize.height / innerSize.height) * sbSize.height;
	}
	percentage = clampf(percentage, 0, 1);
	if(m_thumb) {
        m_thumb->setPreferredSize(CCSizeMake(sbSize.width, thumbLength));
		m_thumb->setPosition(ccp(sbSize.width / 2,
								 sbSize.height - percentage * (sbSize.height - thumbSize.height) - thumbSize.height / 2));
	} else {
		m_fixedThumb->setPosition(ccp(sbSize.width / 2,
									  sbSize.height - percentage * (sbSize.height - thumbSize.height) - thumbSize.height / 2));
	}
}

void CCScrollBar::syncThumbPositionSizeForUIScrollView(ScrollView* scrollView) {
	CCSize svSize = scrollView->getSize();
	CCSize innerSize = scrollView->getInnerContainerSize();
	CCSize sbSize = getContentSize();
	CCSize thumbSize = m_thumb ? m_thumb->getContentSize() : m_fixedThumb->getContentSize();
	float percentage = 0;
    float thumbLength = 0;
	if(m_horizontal) {
		float minX = svSize.width - innerSize.width;
		percentage = (scrollView->getInnerContainer()->getPosition().x - minX) / -minX;
        thumbLength = MIN(1, svSize.width / innerSize.width) * sbSize.height;
	} else {
		float minY = svSize.height - innerSize.height;
		percentage = (scrollView->getInnerContainer()->getPosition().y - minY) / -minY;
        thumbLength = MIN(1, svSize.height / innerSize.height) * sbSize.height;
	}
	percentage = clampf(percentage, 0, 1);
	if(m_thumb) {
        m_thumb->setPreferredSize(CCSizeMake(sbSize.width, thumbLength));
		m_thumb->setPosition(ccp(sbSize.width / 2,
								 sbSize.height - percentage * (sbSize.height - thumbSize.height) - thumbSize.height / 2));
	} else {
		m_fixedThumb->setPosition(ccp(sbSize.width / 2,
									  sbSize.height - percentage * (sbSize.height - thumbSize.height) - thumbSize.height / 2));
	}
}

void CCScrollBar::onUIScrollViewEvent(CCObject* sender, ScrollviewEventType e) {
	if(e == SCROLLVIEW_EVENT_SCROLLING) {
		ScrollView* scrollView = (ScrollView*)sender;
		syncThumbPositionSizeForUIScrollView(scrollView);
        
        // reset timer to fade out
        stopActionByTag(TAG_FADE_OUT);
        CCUtils::setOpacityRecursively(this, 255);
        m_fadingOut = false;
        m_fadeOutTimer = 0;
	}
}

void CCScrollBar::setAutoFade(bool autoFade, bool initFadeOut) {
    m_autoFade = autoFade;
    m_initFadeOut = initFadeOut;
}

void CCScrollBar::update(float delta) {
    if(m_autoFade) {
        if(!m_fadingOut) {
            m_fadeOutTimer += delta;
            if(m_fadeOutTimer >= FADE_OUT_TIME) {
                runAction(CCTreeFadeOut::create(0.5f));
                m_fadingOut = true;
            }
        }
    }
}

void CCScrollBar::scrollViewDidScroll(CCScrollView* view) {
    syncThumbPositionSizeForCCScrollView(view);
    
    // reset timer to fade out
    stopActionByTag(TAG_FADE_OUT);
    CCUtils::setOpacityRecursively(this, 255);
    m_fadingOut = false;
    m_fadeOutTimer = 0;
    
    // to old
    if(m_oldCCDelegate)
        m_oldCCDelegate->scrollViewDidScroll(view);
}

void CCScrollBar::scrollViewDidZoom(CCScrollView* view) {
    if(m_oldCCDelegate)
        m_oldCCDelegate->scrollViewDidZoom(view);
}

NS_CC_END