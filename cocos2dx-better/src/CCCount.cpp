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
#include "CCCount.h"
#include "cocos-ext.h"

USING_NS_CC_EXT;
using namespace cocos2d::ui;

NS_CC_BEGIN

CCCount::CCCount() :
m_from(0),
m_to(0),
m_buf(NULL) {
}

CCCount::~CCCount() {
    CC_SAFE_FREE(m_buf);
}

CCCount* CCCount::create(float duration, int from, int to, const string& pattern) {
    CCCount* a = new CCCount();
    if(a->initWithDuration(duration, from, to, pattern)) {
        a->autorelease();
        return a;
    }
    CC_SAFE_RELEASE(a);
    return NULL;
}

bool CCCount::initWithDuration(float d, int from, int to, const string& pattern) {
    setDuration(d);
    m_from = from;
    m_to = to;
    m_pattern = pattern;
    m_buf = (char*)malloc(sizeof(char) * (m_pattern.length() + 32));
    
    return true;
}

void CCCount::startWithTarget(CCNode *pTarget) {
    CCActionInterval::startWithTarget(pTarget);
    
    if(dynamic_cast<CCLabelProtocol*>(pTarget)) {
        m_lt = CC_LABEL_PROTOCOL;
    } else if(dynamic_cast<Label*>(pTarget)) {
        m_lt = CCS_LABEL;
    } else if(dynamic_cast<LabelBMFont*>(pTarget)) {
        m_lt = CCS_BITMAP_LABEL;
    } else {
        CCAssert(false, "CCCount only support CCLabelProtocol or CocoStudio Label/LabelBMFont");
    }
}

void CCCount::update(float time) {
    int v = m_from + (m_to - m_from) * time;
    sprintf(m_buf, m_pattern.c_str(), v);
    switch (m_lt) {
        case CCS_LABEL:
            dynamic_cast<Label*>(m_pTarget)->setText(m_buf);
            break;
        case CCS_BITMAP_LABEL:
            dynamic_cast<LabelBMFont*>(m_pTarget)->setText(m_buf);
            break;
        default:
            dynamic_cast<CCLabelProtocol*>(m_pTarget)->setString(m_buf);
            break;
    }
}

CCObject* CCCount::copyWithZone(CCZone *pZone) {
    CCZone* pNewZone = NULL;
    CCCount* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) {
        // in case of being called at sub class
        pCopy = (CCCount*)(pZone->m_pCopyObject);
    } else {
        pCopy = new CCCount();
        pZone = pNewZone = new CCZone(pCopy);
    }
	
    CCActionInterval::copyWithZone(pZone);
    pCopy->initWithDuration(m_fDuration, m_from, m_to, m_pattern);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

CCActionInterval* CCCount::reverse() {
    return CCCount::create(getDuration(), m_to, m_from, m_pattern);
}

NS_CC_END