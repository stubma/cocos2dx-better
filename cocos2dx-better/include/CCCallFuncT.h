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
#ifndef __CCCallFuncT__
#define __CCCallFuncT__

#include "cocos2d.h"

NS_CC_BEGIN

template <typename T>
class CC_DLL CCCallFuncT : public CCCallFunc {
public:
    /// function pointer of target
    typedef void (CCObject::*SEL_CallFuncT)(T);
    
private:
    /// argument
    T m_arg;
    
    /// selector
    SEL_CallFuncT m_selector;
    
protected:
    CCCallFuncT() {}
    
public:
    virtual ~CCCallFuncT() {}
    
    /// create action instance
    static CCCallFuncT* create(CCObject* pSelectorTarget, SEL_CallFuncT selector, T arg);
    
    // override
    virtual bool initWithTarget(CCObject* pSelectorTarget, SEL_CallFuncT selector, T arg);
    virtual CCObject* copyWithZone(CCZone* pZone);
    virtual void execute();
};

template <typename T>
CCCallFuncT<T>* CCCallFuncT<T>::create(CCObject* pSelectorTarget, SEL_CallFuncT selector, T arg) {
    CCCallFuncT* c = new CCCallFuncT();
    if(c->initWithTarget(pSelectorTarget, selector, arg)) {
        return (CCCallFuncT*)c->autorelease();
    }
    
    c->release();
    return NULL;
}

template <typename T>
bool CCCallFuncT<T>::initWithTarget(CCObject* pSelectorTarget, SEL_CallFuncT selector, T arg) {
    if (CCCallFunc::initWithTarget(pSelectorTarget)) {
        m_arg = arg;
        m_selector = selector;
        return true;
    }
    
    return false;
}

template <typename T>
CCObject* CCCallFuncT<T>::copyWithZone(CCZone* pZone) {
    CCZone* pNewZone = NULL;
    CCCallFuncT* pRet = NULL;
    
    if (pZone && pZone->m_pCopyObject) {
        //in case of being called at sub class
        pRet = (CCCallFuncT*)pZone->m_pCopyObject;
    } else {
        pRet = new CCCallFuncT();
        pZone = pNewZone = new CCZone(pRet);
    }
    
    CCCallFunc::copyWithZone(pZone);
    pRet->initWithTarget(m_pSelectorTarget, m_selector, m_arg);
    CC_SAFE_DELETE(pNewZone);
    return pRet;
}

template <typename T>
void CCCallFuncT<T>::execute() {
    if(m_selector) {
        (m_pSelectorTarget->*m_selector)(m_arg);
    }
}

NS_CC_END

#endif /* defined(__CCCallFuncT__) */
