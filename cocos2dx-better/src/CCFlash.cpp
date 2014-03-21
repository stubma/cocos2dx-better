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
#include "CCFlash.h"
#include "CCShaders.h"

NS_CC_BEGIN

CCFlash::CCFlash() :
m_oldProgram(NULL) {
}

CCFlash::~CCFlash() {
}

CCFlash* CCFlash::create(float duration, ccColor3B c) {
    CCFlash* f = new CCFlash();
    f->initWithDurationAndColor(duration, c);
    return (CCFlash*)f->autorelease();
}

bool CCFlash::initWithDurationAndColor(float d, ccColor3B c) {
    setDuration(d);
    m_color = c;
    
    // float color
    m_r = m_color.r / 255.0f;
    m_g = m_color.g / 255.0f;
    m_b = m_color.b / 255.0f;
    
    return true;
}

void CCFlash::update(float time) {
    // time percentage
    float t;
    if(time > 0.5f) {
        t = 1 - (time - 0.5f) / 0.5f;
    } else {
        t = time / 0.5f;
    }
    
    // shader, set
	CCShaders::setFlash(m_r, m_g, m_b, t);
}

void CCFlash::startWithTarget(CCNode *pTarget) {
    CCActionInterval::startWithTarget(pTarget);
    
    // save old program
    if(!m_oldProgram) {
        m_oldProgram = pTarget->getShaderProgram();
    }
    
    // set new program
    pTarget->setShaderProgram(CCShaders::programForKey(kCCShader_flash));
}

void CCFlash::stop() {
    if(getTarget()) {
        getTarget()->setShaderProgram(m_oldProgram);
    }
    CCActionInterval::stop();
}

CCActionInterval* CCFlash::reverse() {
    return CCFlash::create(getDuration(), m_color);
}

CCObject* CCFlash::copyWithZone(CCZone *pZone) {
    CCZone* pNewZone = NULL;
    CCFlash* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) {
        // in case of being called at sub class
        pCopy = (CCFlash*)(pZone->m_pCopyObject);
    } else {
        pCopy = new CCFlash();
        pZone = pNewZone = new CCZone(pCopy);
    }
	
    CCActionInterval::copyWithZone(pZone);
    pCopy->initWithDurationAndColor(m_fDuration, m_color);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

NS_CC_END