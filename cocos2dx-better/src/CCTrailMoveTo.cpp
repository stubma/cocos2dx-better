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
#include "CBSprite.h"
#include "CBArmature.h"
#include "CCUtils.h"

NS_CC_BEGIN

CCTrailMoveTo::CCTrailMoveTo() :
m_distance(0),
m_visibleArmIndex(-1),
m_animationIndex(-1) {
}

CCTrailMoveTo::~CCTrailMoveTo() {
}

CCTrailMoveTo* CCTrailMoveTo::createWithSpriteFrameName(float duration, const CCPoint& position,
                                                        const string& trailFrameName,
                                                        float trailDistance, int trailSegments,
                                                        ccColor3B trailColor, ccColor4B trailColorScale, bool removeWhenDone) {
    CCTrailMoveTo* m = new CCTrailMoveTo();
    if(m->initWithDurationAndSpriteTrail(duration, position, trailFrameName, trailDistance, trailSegments, trailColor, trailColorScale, removeWhenDone)) {
        m->m_mode = SPRITE_FRAME_NAME;
        return (CCTrailMoveTo*)m->autorelease();
    }
    CC_SAFE_RELEASE(m);
    return NULL;
}

CCTrailMoveTo* CCTrailMoveTo::createWithFileName(float duration, const CCPoint& position, const string& fileName,
                                         float trailDistance, int trailSegments,
                                         ccColor3B trailColor, ccColor4B trailColorScale, bool removeWhenDone) {
    CCTrailMoveTo* m = new CCTrailMoveTo();
    if(m->initWithDurationAndSpriteTrail(duration, position, fileName, trailDistance, trailSegments, trailColor, trailColorScale, removeWhenDone)) {
        m->m_mode = FILE_NAME;
        return (CCTrailMoveTo*)m->autorelease();
    }
    CC_SAFE_RELEASE(m);
    return NULL;
}

CCTrailMoveTo* CCTrailMoveTo::createWithArmature(float duration, const CCPoint& position, const string& armatureName,
                                         int animationIndex, float trailDistance, int trailSegments,
                                         ccColor3B trailColor, ccColor4B trailColorScale, bool removeWhenDone) {
    CCTrailMoveTo* m = new CCTrailMoveTo();
    if(m->initWithDurationAndArmatureTrail(duration, position, armatureName, animationIndex, trailDistance, trailSegments, trailColor, trailColorScale, removeWhenDone)) {
        m->m_mode = ARMATURE;
        return (CCTrailMoveTo*)m->autorelease();
    }
    CC_SAFE_RELEASE(m);
    return NULL;
}

CCTrailMoveTo* CCTrailMoveTo::createWithArmature(float duration, const CCPoint& position, const string& armatureName,
                                         const string& animationName, float trailDistance, int trailSegments,
                                         ccColor3B trailColor, ccColor4B trailColorScale, bool removeWhenDone) {
    CCTrailMoveTo* m = new CCTrailMoveTo();
    if(m->initWithDurationAndArmatureTrail(duration, position, armatureName, animationName, trailDistance, trailSegments, trailColor, trailColorScale, removeWhenDone)) {
        m->m_mode = ARMATURE;
        return (CCTrailMoveTo*)m->autorelease();
    }
    CC_SAFE_RELEASE(m);
    return NULL;
}

bool CCTrailMoveTo::initWithDurationAndSpriteTrail(float duration, const CCPoint& position, const string& spriteName,
                                                   float trailDistance, int trailSegments, ccColor3B trailColor,
                                                   ccColor4B trailColorScale, bool removeWhenDone) {
    if(!CCMoveTo::initWithDuration(duration, position))
        return false;
    
    m_spriteName = spriteName;
    m_trailDistance = trailDistance;
    m_trailSegments = trailSegments;
    m_trailColor = trailColor;
    m_trailColorScale = trailColorScale;
    m_alphaStep = 1.0f / (m_trailSegments + 1);
    m_removeWhenDone = removeWhenDone;
    m_speed = ccpLength(ccpSub(m_endPosition, m_startPosition)) / m_fDuration;
    
    return true;
}

bool CCTrailMoveTo::initWithDurationAndArmatureTrail(float duration, const CCPoint& position, const string& armatureName,
                                      const string& animationName, float trailDistance, int trailSegments,
                                      ccColor3B trailColor, ccColor4B trailColorScale, bool removeWhenDone) {
    if(!CCMoveTo::initWithDuration(duration, position))
        return false;
    
    m_spriteName = armatureName;
    m_animationName = animationName;
    m_trailDistance = trailDistance;
    m_trailSegments = trailSegments;
    m_trailColor = trailColor;
    m_trailColorScale = trailColorScale;
    m_alphaStep = 1.0f / (m_trailSegments + 1);
    m_removeWhenDone = removeWhenDone;
    m_speed = ccpLength(ccpSub(m_endPosition, m_startPosition)) / m_fDuration;
    
    return true;
}

bool CCTrailMoveTo::initWithDurationAndArmatureTrail(float duration, const CCPoint& position, const string& armatureName,
                                      int animationIndex, float trailDistance, int trailSegments,
                                      ccColor3B trailColor, ccColor4B trailColorScale, bool removeWhenDone) {
    if(!CCMoveTo::initWithDuration(duration, position))
        return false;
    
    m_spriteName = armatureName;
    m_animationIndex = animationIndex;
    m_trailDistance = trailDistance;
    m_trailSegments = trailSegments;
    m_trailColor = trailColor;
    m_trailColorScale = trailColorScale;
    m_alphaStep = 1.0f / (m_trailSegments + 1);
    m_removeWhenDone = removeWhenDone;
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
    
    if(m_mode == ARMATURE) {
        if(m_animationIndex >= 0)
            pCopy->initWithDurationAndArmatureTrail(m_fDuration, m_endPosition, m_spriteName, m_animationIndex, m_trailDistance, m_trailSegments, m_trailColor, m_trailColorScale, m_removeWhenDone);
        else
            pCopy->initWithDurationAndArmatureTrail(m_fDuration, m_endPosition, m_spriteName, m_animationName, m_trailDistance, m_trailSegments, m_trailColor, m_trailColorScale, m_removeWhenDone);
    } else {
         pCopy->initWithDurationAndSpriteTrail(m_fDuration, m_endPosition, m_spriteName, m_trailDistance, m_trailSegments, m_trailColor, m_trailColorScale, m_removeWhenDone);
    }
    pCopy->m_mode = m_mode;
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCTrailMoveTo::startWithTarget(CCNode* pTarget) {
    // super
    CCMoveTo::startWithTarget(pTarget);
    
    // set other
    m_direction = ccpSub(m_endPosition, m_startPosition);
    m_direction = ccpNormalize(m_direction);
    m_distance = 0;
    m_visibleArmIndex = -1;
    m_end = false;
    
    // create armature tail if armature mode
    if(m_mode == ARMATURE) {
        CCNode* parent = m_pTarget->getParent();
        int z = m_pTarget->getZOrder();
        while(m_trails.count() < m_trailSegments) {
            CBArmature* trail = CBArmature::create(m_spriteName.c_str());
            if(m_animationIndex >= 0)
                trail->getAnimation()->playWithIndex(m_animationIndex);
            else
                trail->getAnimation()->play(m_animationName.c_str());
            trail->setPreDrawFunction(CCCallFuncO::create(this, callfuncO_selector(CCTrailMoveTo::onPreDraw), trail));
            trail->setShaderProgram(CCShaders::programForKey(kCCShader_lighting));
            trail->setAnchorPoint(m_pTarget->getAnchorPoint());
            trail->setPosition(m_pTarget->getPosition());
            trail->setScaleX(m_pTarget->getScaleX());
            trail->setScaleY(m_pTarget->getScaleY());
            trail->setVisible(false);
            
            // add to target
            m_trails.addObject(trail);
            parent->addChild(trail, z - m_trails.count());
        }
    }
}

void CCTrailMoveTo::update(float time) {
    CCPoint previousPos = m_pTarget->getPosition();
    CCMoveTo::update(time);
    CCPoint currentPos = m_pTarget->getPosition();
    
    // create tail
    if(m_mode == ARMATURE) {
        if(m_visibleArmIndex < (int)m_trails.count() - 1) {
            float d = ccpLength(ccpSub(currentPos, previousPos));
            m_distance += d;
            
            while(m_visibleArmIndex < (int)m_trails.count() - 1 && m_distance >= m_trailDistance) {
                m_visibleArmIndex++;
                CBArmature* trail = (CBArmature*)m_trails.objectAtIndex(m_visibleArmIndex);
                trail->setVisible(true);
                
                // subtract distance
                m_distance -= m_trailDistance;
            }
        }
    } else {
        if(m_trails.count() < m_trailSegments) {
            float d = ccpLength(ccpSub(currentPos, previousPos));
            m_distance += d;
            while(m_trails.count() < m_trailSegments && m_distance >= m_trailDistance) {
                CBSprite* trail = NULL;
                if(m_mode == SPRITE_FRAME_NAME)
                    trail = CBSprite::createWithSpriteFrameName(m_spriteName.c_str());
                else
                    trail = CBSprite::create(m_spriteName.c_str());
                trail->setPreDrawFunction(CCCallFuncO::create(this, callfuncO_selector(CCTrailMoveTo::onPreDraw), trail));
                trail->setShaderProgram(CCShaders::programForKey(kCCShader_lighting));
                trail->setAnchorPoint(m_pTarget->getAnchorPoint());
                trail->setPosition(m_pTarget->getAnchorPointInPoints());
                
                // add to target
                m_trails.addObject(trail);
                m_pTarget->addChild(trail, -m_trails.count());
                
                // subtract distance
                m_distance -= m_trailDistance;
            }
        }
    }
    
    // update trail position and alpha
    CCObject* obj;
    int index = 0;
    int alpha = 255;
    CCPoint previousTrailPos = m_pTarget->getAnchorPointInPoints();
    if(m_mode == ARMATURE)
        previousTrailPos = m_pTarget->getPosition();
    CCARRAY_FOREACH(&m_trails, obj) {
        CCNodeRGBA* trail = dynamic_cast<CCNodeRGBA*>(obj);
        if(!trail || !trail->isVisible())
            continue;
        
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
        CCNode* trail = (CCNode*)obj;
        
        if(m_removeWhenDone) {
            removeTrail(trail);
        } else {
            // current position
            CCPoint pos = trail->getPosition();
            
            // duration of moving trail
            float distance = 0;
            if(m_mode == ARMATURE)
                distance = ccpLength(ccpSub(finalPos, pos));
            else
                distance = ccpLength(ccpSub(finalLocalPos, pos));
            float duration = distance / m_speed;
            
            // if not armature, move to parent
            if(m_mode != ARMATURE) {
                // final position in parent space
                pos = ccpAdd(origin, pos);
                
                // move trail to target's parent
                trail->retain();
                trail->removeFromParent();
                trail->setPosition(pos);
                trail->setScaleX(m_pTarget->getScaleX());
                trail->setScaleY(m_pTarget->getScaleY());
                parent->addChild(trail);
                trail->release();
            }
            
            // run a action to make trail to target current position
            trail->runAction(CCSequence::createWithTwoActions(CCMoveTo::create(duration, finalPos),
                                                              CCCallFuncO::create(this, callfuncO_selector(CCTrailMoveTo::removeTrail), trail)));
        }
    }
    
    // clear array
    m_trails.removeAllObjects();
}

void CCTrailMoveTo::removeTrail(CCNode* trail) {
    if(m_mode == ARMATURE) {
        ((CBArmature*)trail)->setPreDrawFunction(NULL);
    }
    trail->removeFromParent();
}

void CCTrailMoveTo::onPreDraw(CCObject* sender) {
    // set color
    CCShaders::setLighting(m_trailColorScale, m_trailColor);
}

NS_CC_END