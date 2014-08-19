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
#include "CCToast.h"
#include "CCMoreMacros.h"
#include "CCTreeFadeIn.h"
#include "CCTreeFadeOut.h"

NS_CC_BEGIN

CCToast::CCToast() {
}

CCToast::~CCToast() {
}

CCToast* CCToast::create(CCNode* owner, CCNode* content, int tag, float duration,
                         CCFiniteTimeAction* inAction, CCFiniteTimeAction* outAction) {
    // tag
    if(tag != -1) {
        if(owner->getChildByTag(tag)) {
            return NULL;
        }
    }
    
    CCToast* t = new CCToast();
    t->init();
    t->setTag(tag);
    
    // append to owner
    owner->addChild(t, MAX_INT);
    
    // add content
    t->addChild(content);
    
    // run action for content node
    if(inAction == NULL) {
        CCNodeRGBA* n = dynamic_cast<CCNodeRGBA*>(content);
        if(n)
            n->setOpacity(0);
        inAction = CCTreeFadeIn::create(0.5f);
    }
    if(outAction == NULL) {
        outAction = CCTreeFadeOut::create(0.5f);
    }
    content->runAction(CCSequence::create(inAction,
                                          CCDelayTime::create(duration > 0 ? duration : 3),
                                          outAction,
                                          CCCallFunc::create(t, callfunc_selector(CCNode::removeFromParent)),
                                          NULL));
    
    // return
    t->autorelease();
    return t;
}

void CCToast::forceRemove(CCNode* owner, int tag) {
    CCNode* c = owner->getChildByTag(tag);
    if(c) {
        c->removeFromParent();
    }
}

NS_CC_END