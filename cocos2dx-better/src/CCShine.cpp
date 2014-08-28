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
#include "CCShine.h"
#include "CCShaders.h"

NS_CC_BEGIN

CCShine::CCShine() :
m_oldProgram(NULL) {
}

CCShine::~CCShine() {
}

CCShine* CCShine::create(float duration, ccColor4B color1, ccColor4B color2, ccColor4B color3, ccVertex3F gradientPositions) {
    CCShine* s = new CCShine();
    if(s->initWithColors(duration, color1, color2, color3, gradientPositions)) {
        s->autorelease();
        return s;
    }
    CC_SAFE_RELEASE(s);
    return NULL;
}

bool CCShine::initWithColors(float duration, ccColor4B color1, ccColor4B color2, ccColor4B color3, ccVertex3F gradientPositions) {
    if(!CCActionInterval::initWithDuration(duration)) {
        return false;
    }
    
    // save arguments
    m_color1 = color1;
    m_color2 = color2;
    m_color3 = color3;
    m_gradientPositions = gradientPositions;
    
    return true;
}

void CCShine::update(float time) {
    CCShaders::setShine(m_size.width,
                        ccp(0, m_size.height),
                        ccp(m_size.height, 0),
                        m_color1,
                        m_color2,
                        m_color3,
                        m_gradientPositions,
                        time);
}

void CCShine::startWithTarget(CCNode *pTarget) {
    CCActionInterval::startWithTarget(pTarget);
    
    // save old program
    if(!m_oldProgram) {
        m_oldProgram = pTarget->getShaderProgram();
    }
    
    // calculate speed
    m_size = pTarget->getContentSize();
    
    // set new program
    pTarget->setShaderProgram(CCShaders::programForKey(kCCShader_shine));
    CCShaders::setShine(m_size.width,
                        ccp(0, m_size.height),
                        ccp(m_size.height, 0),
                        m_color1,
                        m_color2,
                        m_color3,
                        m_gradientPositions,
                        0);
}

void CCShine::stop() {
    if(getTarget()) {
        getTarget()->setShaderProgram(m_oldProgram);
    }
    CCActionInterval::stop();
}

CCObject* CCShine::copyWithZone(CCZone* pZone) {
    CCZone* pNewZone = NULL;
    CCShine* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) {
        // in case of being called at sub class
        pCopy = (CCShine*)(pZone->m_pCopyObject);
    } else {
        pCopy = new CCShine();
        pZone = pNewZone = new CCZone(pCopy);
    }
	
    CCActionInterval::copyWithZone(pZone);
    pCopy->initWithColors(m_fDuration, m_color1, m_color2, m_color3, m_gradientPositions);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

CCActionInterval* CCShine::reverse() {
    return (CCActionInterval*)copyWithZone(NULL)->autorelease();
}

NS_CC_END