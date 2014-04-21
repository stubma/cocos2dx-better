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
#include "CCRookieGuide.h"
#include "ccMoreTypes.h"
#include "CCUtils.h"

#define TAG_ENABLE_REGION_CHECK 1000

NS_CC_BEGIN

CCRookieGuide::CCRookieGuide() :
m_clickedRegion(NULL),
m_bgColor(ccc4(0, 0, 0, 0x7f)),
m_anyTouchMode(false),
m_shouldCheckRegion(true),
m_stencil(NULL),
m_content(NULL),
m_clipper(NULL),
m_onExitFunc(NULL),
m_onEnterFunc(NULL) {
}

CCRookieGuide::~CCRookieGuide() {
    for(RegionList::iterator iter = m_regions.begin(); iter != m_regions.end(); iter++) {
        Region& r = *iter;
        CC_SAFE_RELEASE(r.func);
    }
    CC_SAFE_RELEASE(m_onExitFunc);
    CC_SAFE_RELEASE(m_onEnterFunc);
}

CCRookieGuide* CCRookieGuide::create() {
    CCRookieGuide* l = new CCRookieGuide();
    l->init();
    return (CCRookieGuide*)l->autorelease();
}

void CCRookieGuide::onEnter() {
    CCLayer::onEnter();
    
    if(m_onEnterFunc)
        m_onEnterFunc->execute();
}

void CCRookieGuide::onExit() {
    if(m_onExitFunc)
        m_onExitFunc->execute();
    
    CCLayer::onExit();
}

bool CCRookieGuide::init() {
    if(!CCLayer::init()) {
        return false;
    }
	
	// stencil
	m_stencil = CBDrawNode::create();
	m_stencil->setPosition(CCPointZero);
	
	// clipper
	m_clipper = CCClippingNode::create();
	m_clipper->setPosition(CCPointZero);
	m_clipper->setStencil(m_stencil);
	m_clipper->setInverted(true);
	addChild(m_clipper);
	
	// add bg
	m_content = CCLayerColor::create(m_bgColor);
	m_clipper->addChild(m_content);
    
    // enable event, and set top priority
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(-MAX_INT);
    setKeypadEnabled(true);
    
    return true;
}

void CCRookieGuide::setBgColor(const ccColor4B &var) {
	m_bgColor = var;
	m_content->setColor(ccc3(m_bgColor.r, m_bgColor.g, m_bgColor.b));
	m_content->setOpacity(m_bgColor.a);
}

void CCRookieGuide::addRegion(const CCRect& r, CCCallFunc* func, bool removeOnTouch) {
	// add region
    Region region = {
        r,
        func,
        removeOnTouch
    };
    CC_SAFE_RETAIN(func);
    m_regions.push_back(region);
	
	// update stencil
	CCPoint v[] = {
		r.origin,
		ccpAdd(r.origin, ccp(r.size.width, 0)),
		ccpAdd(r.origin, ccp(r.size.width, r.size.height)),
		ccpAdd(r.origin, ccp(0, r.size.height))
	};
	m_stencil->drawPolygon(v, 4, cc4fGREEN, 0, cc4fTRANSPARENT);
}

void CCRookieGuide::addRegion(CCNode* n, CCCallFunc* func, bool removeOnTouch) {
    CCRect b = CCRectMake(0, 0, n->getContentSize().width, n->getContentSize().height);
    CCAffineTransform t = n->nodeToWorldTransform();
    b = CCRectApplyAffineTransform(b, t);
    addRegion(b, func, removeOnTouch);
}

void CCRookieGuide::enableRegionCheckAfter(float seconds) {
    // ensure previous action is stopped
    stopActionByTag(TAG_ENABLE_REGION_CHECK);
    
    // now run a new action
    setShouldCheckRegion(false);
    CCAction* a = CCSequence::createWithTwoActions(CCDelayTime::create(seconds),
                                                   CCCallFunc::create(this, callfunc_selector(CCRookieGuide::markShouldCheckRegion)));
    a->setTag(TAG_ENABLE_REGION_CHECK);
    runAction(a);
}

void CCRookieGuide::fadeIn(float duration, float delay) {
    CCUtils::setOpacityRecursively(this, 0);
    m_content->runAction(CCSequence::create(CCDelayTime::create(delay),
                                            CCFadeTo::create(duration, m_bgColor.a),
                                            CCCallFunc::create(this, callfunc_selector(CCRookieGuide::onFadeInDone)),
                                            NULL));
}

void CCRookieGuide::onFadeInDone() {
    // it is up to subclass
}

void CCRookieGuide::setRegionRemoveOnTouch(int index) {
    if(index < 0 || index >= m_regions.size())
		return;
    
    Region& reg = m_regions.at(index);
    reg.removeOnTouch = true;
    CC_SAFE_RELEASE(reg.func);
}

const CCRect& CCRookieGuide::getRegionRect(int index) {
	if(index < 0 || index >= m_regions.size())
		return CCRectZero;
	
	Region& reg = m_regions.at(index);
	return reg.r;
}

bool CCRookieGuide::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    CCPoint loc = pTouch->getLocation();

    // if clicked any region, release this event
    if(!m_anyTouchMode && m_shouldCheckRegion) {
        m_clickedRegion = NULL;
        for(RegionList::iterator iter = m_regions.begin(); iter != m_regions.end(); iter++) {
            Region& r = *iter;
            if(r.r.containsPoint(loc)) {
                if(r.func) {
                    m_clickedRegion = &r;
                    break;
                } else if(r.removeOnTouch) {
                    removeFromParent();
                    return false;
                }
            }
        }
    }
    
    return true;
}

void CCRookieGuide::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {
}

void CCRookieGuide::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
    if(m_anyTouchMode && m_shouldCheckRegion) {
        removeFromParent();
    } else if(m_clickedRegion) {
        m_clickedRegion->func->execute();
        removeFromParent();
    }
}

void CCRookieGuide::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {
}

NS_CC_END