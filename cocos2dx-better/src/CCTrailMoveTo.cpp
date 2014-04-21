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
#include "CCTrailMoveTo.h"

NS_CC_BEGIN

CCTrailMoveTo::CCTrailMoveTo() {
    
}

CCTrailMoveTo::~CCTrailMoveTo() {
    
}

CCTrailMoveTo* CCTrailMoveTo::create(float duration, const CCPoint& position, float trailLength, int trailSegments,
                                     ccColor3B trailColor, ccColor3B trailColorScale) {
    CCTrailMoveTo* m = new CCTrailMoveTo();
    if(m->initWithDurationAndTrail(duration, position, trailLength, trailSegments, trailColor, trailColorScale)) {
        return (CCTrailMoveTo*)m->autorelease();
    }
    CC_SAFE_RELEASE(m);
    return NULL;
}

bool CCTrailMoveTo::initWithDurationAndTrail(float duration, const CCPoint& position, float trailLength,
                                             int trailSegments, ccColor3B trailColor, ccColor3B trailColorScale) {
    if(!CCMoveTo::initWithDuration(duration, position))
        return false;
    
    m_trailLength = trailLength;
    m_trailSegments = trailSegments;
    m_trailColor = trailColor;
    m_trailColorScale = trailColorScale;
    
    return true;
}

CCObject* CCTrailMoveTo::copyWithZone(CCZone* pZone) {
    CCZone* pNewZone = NULL;
    CCTrailMoveTo* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) {
        pCopy = (CCTrailMoveTo*)(pZone->m_pCopyObject);
    } else {
        pCopy = new CCTrailMoveTo();
        pZone = pNewZone = new CCZone(pCopy);
    }
    
    CCMoveBy::copyWithZone(pZone);
    
    pCopy->initWithDurationAndTrail(m_fDuration, m_endPosition, m_trailLength, m_trailSegments, m_trailColor, m_trailColorScale);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCTrailMoveTo::startWithTarget(CCNode* pTarget) {
    CCMoveTo::startWithTarget(pTarget);
}

NS_CC_END