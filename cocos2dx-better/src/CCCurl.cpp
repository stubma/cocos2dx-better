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
#include "CCCurl.h"

NS_CC_BEGIN

CCCurl::CCCurl() {
}

CCCurl::~CCCurl() {
}

CCCurl* CCCurl::create(float duration, CCPoint center, float toRadius, float angularVelocity) {
    CCCurl* a = new CCCurl();
    if(a->initWithDuration(duration, center, toRadius, angularVelocity)) {
        a->autorelease();
        return a;
    }
    CC_SAFE_RELEASE(a);
    return NULL;
}

bool CCCurl::initWithDuration(float d, CCPoint center, float toRadius, float angularVelocity) {
    setDuration(d);
    m_center = center;
    m_toRadius = toRadius;
    m_angularVelocity = angularVelocity;
    
    return true;
}

void CCCurl::startWithTarget(CCNode* pTarget) {
    CCActionInterval::startWithTarget(pTarget);
    
    // get start radius
    CCPoint v = ccpSub(pTarget->getPosition(), m_center);
    m_fromRadius = ccpLength(v);
    m_initAngle = v.getAngle();
}

void CCCurl::update(float time) {
    // radius and angle
    float radius = m_fromRadius + time * (m_toRadius - m_fromRadius);
    float angle = m_initAngle + time * m_fDuration * m_angularVelocity;
    
    // set target position
    CCPoint v = ccpForAngle(angle) * radius;
    CCPoint p = ccpAdd(m_center, v);
    getTarget()->setPosition(p);
}

CCObject* CCCurl::copyWithZone(CCZone *pZone) {
    CCZone* pNewZone = NULL;
    CCCurl* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) {
        // in case of being called at sub class
        pCopy = (CCCurl*)(pZone->m_pCopyObject);
    } else {
        pCopy = new CCCurl();
        pZone = pNewZone = new CCZone(pCopy);
    }
    
    CCActionInterval::copyWithZone(pZone);
    pCopy->initWithDuration(m_fDuration, m_center, m_toRadius, m_angularVelocity);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

CCActionInterval* CCCurl::reverse() {
    return CCCurl::create(getDuration(), m_center, m_fromRadius, -m_angularVelocity);
}

NS_CC_END