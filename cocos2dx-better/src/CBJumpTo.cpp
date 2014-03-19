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
#include "CBJumpTo.h"

NS_CC_BEGIN

CBJumpTo::CBJumpTo() {
}

CBJumpTo::~CBJumpTo() {
}

CBJumpTo* CBJumpTo::create(float duration, const CCPoint& position, float height, unsigned int jumps, bool autoHeadOn, float initAngle) {
    CBJumpTo* j = new CBJumpTo();
    if(j->initWithDuration(duration, position, height, jumps, autoHeadOn, initAngle)) {
        j->m_destPosition = position;
        return (CBJumpTo*)j->autorelease();
    }
    
    j->release();
    return j;
}

CCObject* CBJumpTo::copyWithZone(CCZone* pZone) {
    CCZone* pNewZone = NULL;
    CBJumpTo* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) {
        //in case of being called at sub class
        pCopy = (CBJumpTo*)(pZone->m_pCopyObject);
    } else {
        pCopy = new CBJumpTo();
        pZone = pNewZone = new CCZone(pCopy);
    }
    
    CCActionInterval::copyWithZone(pZone);
    
    pCopy->initWithDuration(m_fDuration, m_destPosition, m_height, m_nJumps, m_autoHeadOn, m_initAngle);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CBJumpTo::startWithTarget(CCNode *pTarget) {
    CBJumpBy::startWithTarget(pTarget);
    m_delta = ccp(m_destPosition.x - m_startPosition.x, m_destPosition.y - m_startPosition.y);
}

CCActionInterval* CBJumpTo::reverse() {
    CCLOGERROR("CBJumpTo doesn't have reverse action");
    return NULL;
}

NS_CC_END