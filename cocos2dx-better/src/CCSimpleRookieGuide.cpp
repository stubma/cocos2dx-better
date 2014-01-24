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
#include "CCSimpleRookieGuide.h"
#include "ccMoreTypes.h"
#include "CCTreeFadeIn.h"

NS_CC_BEGIN

CCSimpleRookieGuide::CCSimpleRookieGuide() :
m_arrow(NULL),
m_hint(NULL) {
}

CCSimpleRookieGuide::~CCSimpleRookieGuide() {
}

CCSimpleRookieGuide* CCSimpleRookieGuide::create() {
    CCSimpleRookieGuide* g = new CCSimpleRookieGuide();
    g->init();
    return (CCSimpleRookieGuide*)g->autorelease();
}

void CCSimpleRookieGuide::fadeIn(float duration, float delay) {
    CCRookieGuide::fadeIn(duration, delay);
    
    // other runs fade in
    CCTreeFadeIn* fadeIn = CCTreeFadeIn::create(duration);
    fadeIn->excludeNode(m_content, false);
    runAction(CCSequence::createWithTwoActions(CCDelayTime::create(delay),
                                               fadeIn));
}

void CCSimpleRookieGuide::pointToRegionCenter(int regionIndex, float distance, float degree, float arrowPresetRotation) {
    // basic check
    if(regionIndex < 0 || regionIndex >= m_regions.size() || !m_arrow)
        return;
    
    // get region
    Region& reg = m_regions.at(regionIndex);
    CCRect& r = reg.r;
    CCPoint center = ccpAdd(r.origin, ccp(r.size.width / 2, r.size.height / 2));
    
    // get center of arrow sprite
    float radian = CC_DEGREES_TO_RADIANS(degree);
    center.x += distance * cosf(radian);
    center.y += distance * sinf(radian);
    
    // set arrow
    m_arrow->setPosition(center);
    m_arrow->setRotation(-(degree - 180 - arrowPresetRotation));
}

void CCSimpleRookieGuide::shiftHint(float dx, float dy) {
    if(m_arrow)
        m_hint->setPosition(ccpAdd(m_arrow->getPosition(), ccp(dx, dy)));
}

void CCSimpleRookieGuide::setArrow(CCSprite* arrow) {
    if(m_arrow) {
        m_arrow->removeFromParent();
        m_arrow = NULL;
    }
    m_arrow = arrow;
    m_arrow->setAnchorPoint(ccp(0.5f, 0.5f));
    m_arrow->ignoreAnchorPointForPosition(false);
    addChild(m_arrow, 1);
}

void CCSimpleRookieGuide::setHint(CCNode* hint) {
    if(m_hint) {
        m_hint->removeFromParent();
        m_hint = NULL;
    }
    m_hint = hint;
    m_hint->setAnchorPoint(ccp(0.5f, 0.5f));
    m_hint->ignoreAnchorPointForPosition(false);
    addChild(m_hint, 2);
}

NS_CC_END