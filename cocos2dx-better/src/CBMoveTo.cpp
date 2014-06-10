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
#include "CBMoveTo.h"

NS_CC_BEGIN

CBMoveTo::CBMoveTo() {
}

CBMoveTo::~CBMoveTo() {
}

CBMoveTo* CBMoveTo::create(float duration, const CCPoint& position, bool autoHeadOn, float initAngle) {
    CBMoveTo* j = new CBMoveTo();
    if(j->initWithDuration(duration, position, autoHeadOn, initAngle)) {
        return (CBMoveTo*)j->autorelease();
    }
    
    j->release();
    return j;
}

bool CBMoveTo::initWithDuration(float duration, const CCPoint& position, bool autoHeadOn, float initAngle) {
    if (CCMoveTo::initWithDuration(duration, position)) {
        m_autoHeadOn = autoHeadOn;
        m_initAngle = initAngle;
        return true;
    }
    
    return false;
}

CCObject* CBMoveTo::copyWithZone(CCZone* pZone) {
    CCZone* pNewZone = NULL;
    CBMoveTo* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) {
        //in case of being called at sub class
        pCopy = (CBMoveTo*)(pZone->m_pCopyObject);
    } else {
        pCopy = new CBMoveTo();
        pZone = pNewZone = new CCZone(pCopy);
    }
    
    CCActionInterval::copyWithZone(pZone);
    
    pCopy->initWithDuration(m_fDuration, m_endPosition, m_autoHeadOn, m_initAngle);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CBMoveTo::update(float time) {
    // old and new position
    CCPoint oldPos = m_previousPosition;
    CCMoveTo::update(time);
    CCPoint newPos = m_pTarget->getPosition();
    
    // auto head on
    if(m_autoHeadOn) {
        CCPoint v = ccpSub(newPos, oldPos);
        float r = ccpToAngle(v);
        float d = -CC_RADIANS_TO_DEGREES(r);
        d += m_initAngle;
        m_pTarget->setRotation(d);
    }
}

CCActionInterval* CBMoveTo::reverse() {
    CCLOGERROR("CBMoveTo doesn't have reverse action");
    return NULL;
}

NS_CC_END