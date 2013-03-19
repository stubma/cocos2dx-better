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

CCMissile::~CCMissile() {
}

CCMissile* CCMissile::create(float duration, CCNode* aimed, float targetPresetDegree) {
	CCMissile* a = new CCMissile();
	a->initWithDuration(duration, aimed, targetPresetDegree);
	return (CCMissile*)a->autorelease();
}

bool CCMissile::initWithDuration(float d, CCNode* aimed, float targetPresetDegree) {
	m_fDuration = d;
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
    pCopy->initWithDuration(m_fDuration, m_aimed, m_presetDegree);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCMissile::update(float t) {
}

NS_CC_END