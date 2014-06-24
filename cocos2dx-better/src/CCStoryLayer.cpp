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
#include "CCStoryLayer.h"

NS_CC_BEGIN

CCStoryLayer::CCStoryLayer() {
	
}

CCStoryLayer::~CCStoryLayer() {
	
}

CCStoryLayer* CCStoryLayer::create() {
	CCStoryLayer* l = new CCStoryLayer();
	if(l->init()) {
		return (CCStoryLayer*)l->autorelease();
	}
	CC_SAFE_RELEASE(l);
	return NULL;
}

void CCStoryLayer::onExit() {
    CCLayer::onExit();
}

void CCStoryLayer::onEnter() {
    CCLayer::onEnter();
}

bool CCStoryLayer::init() {
	if(!CCLayer::init())
		return false;
    
    // surface
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	return true;
}

NS_CC_END