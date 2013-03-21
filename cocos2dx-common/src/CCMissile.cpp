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
#include "CCMissile.h"

NS_CC_BEGIN

#define ANGULAR_VELOCITY 360

CCMissile::~CCMissile() {
}

CCMissile* CCMissile::create(float velocity, CCNode* aimed, float targetPresetDegree) {
	CCMissile* a = new CCMissile();
	a->initWithVelocity(velocity, aimed, targetPresetDegree);
	return (CCMissile*)a->autorelease();
}

bool CCMissile::initWithVelocity(float velocity, CCNode* aimed, float targetPresetDegree) {
    m_dstDegree = 0;
	m_velocity = velocity;
	m_aimed = aimed;
	m_presetDegree = targetPresetDegree;
	return true;
}

CCObject* CCMissile::copyWithZone(CCZone *pZone) {
    CCZone* pNewZone = NULL;
    CCMissile* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) {
        // in case of being called at sub class
        pCopy = (CCMissile*)(pZone->m_pCopyObject);
    } else {
        pCopy = new CCMissile();
        pZone = pNewZone = new CCZone(pCopy);
    }
	
    CCActionInterval::copyWithZone(pZone);
    pCopy->initWithVelocity(m_velocity, m_aimed, m_presetDegree);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCMissile::step(float dt) {
    // position of target and aimed
    CCNode* target = getTarget();
    CCPoint t = target->getPosition();
    CCPoint a = m_aimed->getPosition();
    
    // position vector degree
    CCPoint v = ccpSub(a, t);
    float r = ccpToAngle(v);
    float d = -CC_RADIANS_TO_DEGREES(r);
    d -= m_presetDegree;
    
    // save dest degree
    if(m_dstDegree != d) {
        m_dstDegree = d;
    }
    
    // rotate to dst degree gradually
    if(m_dstDegree > target->getRotation()) {
        float cur = MIN(m_dstDegree, target->getRotation() + dt * ANGULAR_VELOCITY);
        target->setRotation(cur);
    } else if(m_dstDegree < target->getRotation()) {
        float cur = MAX(m_dstDegree, target->getRotation() - dt * ANGULAR_VELOCITY);
        target->setRotation(cur);
    }
    
    // move target by velocity
    float move = m_velocity * dt;
    float distance = ccpLength(v);
    if(move >= distance) {
        target->setPosition(a);
    } else {
        t.x += move * cosf(r);
        t.y += move * sinf(r);
        target->setPosition(t);
    }
}

bool CCMissile::isDone() {
    return getTarget()->getPositionX() == m_aimed->getPositionX() &&
        getTarget()->getPositionY() == m_aimed->getPositionY();
}

NS_CC_END