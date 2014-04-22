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
#include "CCShaders.h"

NS_CC_BEGIN

CCTrailMoveTo::CCTrailMoveTo() :
m_distance(0) {
}

CCTrailMoveTo::~CCTrailMoveTo() {
    
}

CCTrailMoveTo* CCTrailMoveTo::createWithSpriteFrameName(float duration, const CCPoint& position,
                                                        const string& trailFrameName,
                                                        float trailDistance, int trailSegments,
                                                        ccColor3B trailColor, ccColor3B trailColorScale) {
    CCTrailMoveTo* m = new CCTrailMoveTo();
    if(m->initWithDurationAndSpriteTrail(duration, position, trailFrameName, trailDistance, trailSegments, trailColor, trailColorScale)) {
        m->m_mode = SPRITE_FRAME_NAME;
        return (CCTrailMoveTo*)m->autorelease();
    }
    CC_SAFE_RELEASE(m);
    return NULL;
}

CCTrailMoveTo* CCTrailMoveTo::createWithFileName(float duration, const CCPoint& position, const string& fileName,
                                         float trailDistance, int trailSegments,
                                         ccColor3B trailColor, ccColor3B trailColorScale) {
    CCTrailMoveTo* m = new CCTrailMoveTo();
    if(m->initWithDurationAndSpriteTrail(duration, position, fileName, trailDistance, trailSegments, trailColor, trailColorScale)) {
        m->m_mode = FILE_NAME;
        return (CCTrailMoveTo*)m->autorelease();
    }
    CC_SAFE_RELEASE(m);
    return NULL;
}

bool CCTrailMoveTo::initWithDurationAndSpriteTrail(float duration, const CCPoint& position, const string& spriteName,
                                                   float trailDistance, int trailSegments, ccColor3B trailColor,
                                                   ccColor3B trailColorScale) {
    if(!CCMoveTo::initWithDuration(duration, position))
        return false;
    
    m_spriteName = spriteName;
    m_trailDistance = trailDistance;
    m_trailSegments = trailSegments;
    m_trailColor = trailColor;
    m_trailColorScale = trailColorScale;
    m_alphaStep = 1.0f / (m_trailSegments + 1);
    m_speed = ccpLength(ccpSub(m_endPosition, m_startPosition)) / m_fDuration;
    
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
    
    pCopy->initWithDurationAndSpriteTrail(m_fDuration, m_endPosition, m_spriteName, m_trailDistance, m_trailSegments, m_trailColor, m_trailColorScale);
    pCopy->m_mode = m_mode;
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCTrailMoveTo::startWithTarget(CCNode* pTarget) {
    CCMoveTo::startWithTarget(pTarget);
    
    // set other
    m_direction = ccpSub(m_endPosition, m_startPosition);
    m_direction = ccpNormalize(m_direction);
    m_distance = 0;
    m_end = false;
    
    // set color
    ccColor4B scale = ccc4(m_trailColorScale.r, m_trailColorScale.g, m_trailColorScale.b, 255);
    CCShaders::setLighting(scale, m_trailColor);
}

void CCTrailMoveTo::update(float time) {
    CCPoint previousPos = m_pTarget->getPosition();
    CCMoveTo::update(time);
    CCPoint currentPos = m_pTarget->getPosition();
    
    // create tail
    if(m_trails.count() < m_trailSegments) {
        float d = ccpLength(ccpSub(currentPos, previousPos));
        m_distance += d;
        while(m_trails.count() < m_trailSegments && m_distance >= m_trailDistance) {
            CCSprite* trail = NULL;
            if(m_mode == SPRITE_FRAME_NAME)
                trail = CCSprite::createWithSpriteFrameName(m_spriteName.c_str());
            else
                trail = CCSprite::create(m_spriteName.c_str());
            trail->setShaderProgram(CCShaders::programForKey(kCCShader_lighting));
            trail->setAnchorPoint(m_pTarget->getAnchorPoint());
            trail->setPosition(m_pTarget->getAnchorPointInPoints());
            m_trails.addObject(trail);
            m_pTarget->addChild(trail, -m_trails.count());
            m_distance -= m_trailDistance;
        }
    }
    
    // update trail position and alpha
    CCObject* obj;
    int index = 0;
    int alpha = 255;
    CCPoint previousTrailPos = m_pTarget->getAnchorPointInPoints();
    CCARRAY_FOREACH(&m_trails, obj) {
        CCSprite* trail = (CCSprite*)obj;
        
        // set pos
        CCPoint pos = m_direction * -m_trailDistance;
        pos = ccpAdd(pos, previousTrailPos);
        trail->setPosition(pos);
        previousTrailPos = pos;
        
        // alpha
        alpha -= m_alphaStep * 255.0f;
        trail->setOpacity(MAX(0, alpha));
        
        index++;
    }
    
    // when this move is done, must clean trail
    // however, trail must be moved out in case the target is removed
    if(time >= 1 && !m_end) {
        m_end = true;
        cleanTrails();
    }
}

void CCTrailMoveTo::cleanTrails() {
    // position of target in parent space
    CCPoint finalPos = m_pTarget->getPosition();
    
    // origin of target in parent space
    CCPoint origin = CCUtils::getOrigin(m_pTarget);
    
    // target anchor position
    CCPoint finalLocalPos = m_pTarget->getAnchorPointInPoints();
    
    // parent
    CCNode* parent = m_pTarget->getParent();
    
    // process every trail
    CCObject* obj;
    CCARRAY_FOREACH(&m_trails, obj) {
        // trail and current position in target space
        CCSprite* trail = (CCSprite*)obj;
        CCPoint pos = trail->getPosition();
        
        // duration of moving trail
        float distance = ccpLength(ccpSub(finalLocalPos, pos));
        float duration = distance / m_speed;
        
        // final position in parent space
        pos = ccpAdd(origin, pos);
        
        // move trail to target's parent
        trail->retain();
        trail->removeFromParent();
        trail->setPosition(pos);
        parent->addChild(trail);
        trail->release();
        
        // run a action to make trail to target current position
        trail->runAction(CCSequence::createWithTwoActions(CCMoveTo::create(duration, finalPos),
                                                          CCCallFunc::create(trail, callfunc_selector(CCNode::removeFromParent))));
    }
}

NS_CC_END