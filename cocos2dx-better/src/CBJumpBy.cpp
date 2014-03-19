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
#include "CBJumpBy.h"

NS_CC_BEGIN

CBJumpBy::CBJumpBy() :
m_autoHeadOn(false),
m_initAngle(0) {
}

CBJumpBy::~CBJumpBy() {
}

bool CBJumpBy::initWithDuration(float duration, const CCPoint& position, float height, unsigned int jumps, bool autoHeadOn, float initAngle) {
    if (CCJumpBy::initWithDuration(duration, position, height, jumps)) {
        m_autoHeadOn = autoHeadOn;
        m_initAngle = initAngle;
        return true;
    }
    
    return false;
}

CBJumpBy* CBJumpBy::create(float duration, const CCPoint& position, float height, unsigned int jumps, bool autoHeadOn, float initAngle) {
    CBJumpBy* j = new CBJumpBy();
    if(j->initWithDuration(duration, position, height, jumps, autoHeadOn, initAngle)) {
        return (CBJumpBy*)j->autorelease();
    }
    
    j->release();
    return j;
}

CCObject* CBJumpBy::copyWithZone(CCZone* pZone) {
    CCZone* pNewZone = NULL;
    CBJumpBy* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) {
        //in case of being called at sub class
        pCopy = (CBJumpBy*)(pZone->m_pCopyObject);
    } else {
        pCopy = new CBJumpBy();
        pZone = pNewZone = new CCZone(pCopy);
    }
    
    CCActionInterval::copyWithZone(pZone);
    
    pCopy->initWithDuration(m_fDuration, m_delta, m_height, m_nJumps, m_autoHeadOn, m_initAngle);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CBJumpBy::update(float time) {
    // old and new position
    CCPoint oldPos = m_previousPos;
    CCJumpBy::update(time);
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

CCActionInterval* CBJumpBy::reverse() {
    return CBJumpBy::create(getDuration(), -m_delta, m_height, m_nJumps, m_autoHeadOn, 180 - m_initAngle);
}

NS_CC_END