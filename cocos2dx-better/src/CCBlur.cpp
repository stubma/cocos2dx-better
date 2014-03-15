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
#include "CCBlur.h"
#include "CCShaders.h"

NS_CC_BEGIN

CCBlur::CCBlur() :
m_oldProgram(NULL) {
}

CCBlur::~CCBlur() {
}

CCBlur* CCBlur::create(float duration, CCSize startBlurSize, CCSize endBlurSize) {
    CCBlur* a = new CCBlur();
    a->initWithBlurSize(duration, startBlurSize, endBlurSize);
    return (CCBlur*)a->autorelease();
}

bool CCBlur::initWithBlurSize(float d, CCSize startBlurSize, CCSize endBlurSize) {
    setDuration(d);
    m_startBlurSize = startBlurSize;
	m_endBlurSize = endBlurSize;
	m_deltaBlurSize = m_endBlurSize - m_startBlurSize;
    
    return true;
}

void CCBlur::update(float time) {
	CCShaders::setBlur(getTarget()->getContentSize(), m_startBlurSize + m_deltaBlurSize * time);
}

void CCBlur::startWithTarget(CCNode *pTarget) {
    CCActionInterval::startWithTarget(pTarget);
    
    // save old program
    if(!m_oldProgram) {
        m_oldProgram = pTarget->getShaderProgram();
    }
    
    // set new program
    pTarget->setShaderProgram(CCShaders::programForKey(kCCShader_blur));
	CCShaders::setBlur(pTarget->getContentSize(), m_startBlurSize);
}

void CCBlur::stop() {
    getTarget()->setShaderProgram(m_oldProgram);
    CCActionInterval::stop();
}

CCObject* CCBlur::copyWithZone(CCZone *pZone) {
    CCZone* pNewZone = NULL;
    CCBlur* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) {
        // in case of being called at sub class
        pCopy = (CCBlur*)(pZone->m_pCopyObject);
    } else {
        pCopy = new CCBlur();
        pZone = pNewZone = new CCZone(pCopy);
    }
	
    CCActionInterval::copyWithZone(pZone);
    pCopy->initWithBlurSize(m_fDuration, m_startBlurSize, m_endBlurSize);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

CCActionInterval* CCBlur::reverse() {
    return CCBlur::create(getDuration(), m_endBlurSize, m_startBlurSize);
}

NS_CC_END