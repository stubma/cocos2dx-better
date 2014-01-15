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
#include "CCClipOut.h"
#include "CCClipIn.h"
#include "ccMoreTypes.h"

NS_CC_BEGIN

CCClipOut::CCClipOut() :
m_direction(ccp(1, 0)) {
}

CCClipOut::~CCClipOut() {
}

CCClipOut* CCClipOut::create(float duration, CCPoint v) {
    CCClipOut* c = new CCClipOut();
    c->initWithDuration(duration, v);
    return (CCClipOut*)c->autorelease();
}

bool CCClipOut::initWithDuration(float d, const CCPoint& v) {
    setDuration(d);
    setDirection(v);
    return true;
}

void CCClipOut::startWithTarget(CCNode *pTarget) {
    CCAssert(dynamic_cast<CCClippingNode*>(pTarget) != NULL, "CCClipOut target must be a CCClippingNode instance");
    
    CCActionInterval::startWithTarget(pTarget);
    
    // ensure the stencil of clipper is CCDrawNode
    CCClippingNode* clipper = (CCClippingNode*)pTarget;
    CCNode* stencil = clipper->getStencil();
    if(dynamic_cast<CCDrawNode*>(stencil) == NULL) {
        clipper->setStencil(CCDrawNode::create());
    }
    
    // direction radian
    // treat is like a clip in
    float r = ccpToAngle(ccp(-m_direction.x, -m_direction.y));
    m_cos = cosf(r);
    m_sin = sinf(r);
    
    // max distance along direction
    const CCSize& size = pTarget->getContentSize();
    m_distance = size.width * fabsf(m_cos) + size.height * fabsf(m_sin);
    
    // calculate fixed edge endpoints
    if(r > M_PI_2) {
        float p1Len = size.width * cosf(r - M_PI_2);
        m_p1.x = size.width - p1Len * cosf(r - M_PI_2);
        m_p1.y = -p1Len * sinf(r - M_PI_2);
        float p2Len = size.height * sinf(r - M_PI_2);
        m_p2.x = size.width + p2Len * cosf(r - M_PI_2);
        m_p2.y = p2Len * sinf(r - M_PI_2);
    } else if(r >= 0) {
        float p1Len = size.height * m_cos;
        m_p1.x = p1Len * cosf(r + M_PI_2);
        m_p1.y = p1Len * sinf(r + M_PI_2);
        float p2Len = size.width * m_sin;
        m_p2.x = p2Len * cosf(r - M_PI_2);
        m_p2.y = p2Len * sinf(r - M_PI_2);
    } else if(r < -M_PI_2) {
        float p1Len = size.height * sinf(-r - M_PI_2);
        m_p1.x = size.width + p1Len * cosf(-r - M_PI_2);
        m_p1.y = size.height - p1Len * sinf(-r - M_PI_2);
        float p2Len = size.width * cosf(-r - M_PI_2);
        m_p2.x = size.width - p2Len * cosf(-r - M_PI_2);
        m_p2.y = size.height + p2Len * sinf(-r - M_PI_2);
    } else {
        float p1Len = size.width * sinf(-r);
        m_p1.x = p1Len * cosf(r + M_PI_2);
        m_p1.y = size.height + p1Len * sinf(r + M_PI_2);
        float p2Len = size.height * cosf(-r);
        m_p2.x = -p2Len * cosf(r + M_PI_2);
        m_p2.y = size.height - p2Len * sinf(r + M_PI_2);
    }
}

void CCClipOut::update(float time) {
    CCClippingNode* clipper = (CCClippingNode*)getTarget();
    CCDrawNode* stencil = (CCDrawNode*)clipper->getStencil();
    
    // clear
    stencil->clear();
    
    // build polygon
    float distance = m_distance * (1 - time);
    if(distance < FLT_EPSILON) {
        CCPoint v[] = {
            CCPointZero,
            CCPointZero,
            CCPointZero,
            CCPointZero
        };
        stencil->drawPolygon(v, 4, cc4fGREEN, 0, cc4fTRANSPARENT);
    } else {
        CCPoint v[] = {
            m_p1,
            m_p2,
            ccp(m_p2.x + distance * m_cos,
                m_p2.y + distance * m_sin),
            ccp(m_p1.x + distance * m_cos,
                m_p1.y + distance * m_sin)
        };
        stencil->drawPolygon(v, 4, cc4fGREEN, 0, cc4fTRANSPARENT);
    }
}

CCActionInterval* CCClipOut::reverse() {
    return CCClipIn::create(getDuration(), ccp(-m_direction.x, -m_direction.y));
}

NS_CC_END