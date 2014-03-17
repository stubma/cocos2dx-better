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

NS_CC_BEGIN

CCScrollBar::CCScrollBar() :
m_thumb(NULL),
m_track(NULL),
m_fixedThumb(NULL),
m_autoFade(false) {
	
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
	syncThumbPositionForUIScrollView(scrollView);
	
	// listen to scrollview scrolling event
	scrollView->addEventListenerScrollView(this, scrollvieweventselector(CCScrollBar::onUIScrollViewEvent));
}

void CCScrollBar::attachToCCScrollView(CCScrollView* scrollView, ccInsets insets, bool horizontal) {
	// save flag
	m_horizontal = horizontal;
	
	// add to scroll view
	float thumbLength = 0;
	CCSize svSize = scrollView->getContentSize();
	CCSize innerSize = scrollView->getContainer()->getContentSize();
	CCSize sbSize;
	if(horizontal) {
		sbSize = CCSizeMake(m_track->getContentSize().width,
							svSize.width - insets.left - insets.right);
		setContentSize(sbSize);
		setAnchorPoint(ccp(0, 0.5f));
		setPosition(ccp(svSize.width / 2, insets.bottom));
		setRotation(-90);
		scrollView->addChild(this, MAX_INT);
		
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
		setPosition(ccp(svSize.width - insets.right, svSize.height / 2));
		scrollView->addChild(this, MAX_INT);
		
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
	syncThumbPositionForCCScrollView(scrollView);
}

void CCScrollBar::syncThumbPositionForCCScrollView(CCScrollView* scrollView) {
	CCSize svSize = scrollView->getContentSize();
	CCSize innerSize = scrollView->getContainer()->getContentSize();
	CCSize sbSize = getContentSize();
	CCSize thumbSize = m_thumb ? m_thumb->getContentSize() : m_fixedThumb->getContentSize();
	float percentage = 0;
	if(m_horizontal) {
		float minX = svSize.width - innerSize.width;
		percentage = (scrollView->getContainer()->getPosition().x - minX) / -minX;
	} else {
		float minY = svSize.height - innerSize.height;
		percentage = (scrollView->getContainer()->getPosition().y - minY) / -minY;
	}
	percentage = clampf(percentage, 0, 1);
	if(m_thumb) {
		m_thumb->setPosition(ccp(sbSize.width / 2,
								 sbSize.height - percentage * (sbSize.height - thumbSize.height) - thumbSize.height / 2));
	} else {
		m_fixedThumb->setPosition(ccp(sbSize.width / 2,
									  sbSize.height - percentage * (sbSize.height - thumbSize.height) - thumbSize.height / 2));
	}
}

void CCScrollBar::syncThumbPositionForUIScrollView(ScrollView* scrollView) {
	CCSize svSize = scrollView->getSize();
	CCSize innerSize = scrollView->getInnerContainerSize();
	CCSize sbSize = getContentSize();
	CCSize thumbSize = m_thumb ? m_thumb->getContentSize() : m_fixedThumb->getContentSize();
	float percentage = 0;
	if(m_horizontal) {
		float minX = svSize.width - innerSize.width;
		percentage = (scrollView->getInnerContainer()->getPosition().x - minX) / -minX;
	} else {
		float minY = svSize.height - innerSize.height;
		percentage = (scrollView->getInnerContainer()->getPosition().y - minY) / -minY;
	}
	percentage = clampf(percentage, 0, 1);
	if(m_thumb) {
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
		syncThumbPositionForUIScrollView(scrollView);
	}
}

NS_CC_END