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
#include "PreviewScene.h"

Preview::Preview() :
m_storyLayer(NULL) {
}

Preview::~Preview() {
}

CCScene* Preview::scene() {
    // 'scene' is an autorelease object
    CCScene* scene = CCScene::create();
    
    // 'layer' is an autorelease object
    Preview* layer = Preview::create();
	
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

bool Preview::init() {
    // super init first
    if (!CCLayer::init()) {
        return false;
    }
    
    // story layer
    m_storyLayer = CCStoryLayer::create();
    addChild(m_storyLayer);
    
    // put this in init in order to receive first selection event
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Preview::onFrameSizeChanged), kCCNotificationFrameSizeChanged, NULL);
    
    return true;
}

void Preview::onEnter() {
    CCLayer::onEnter();
}

void Preview::onExit() {
	CCLayer::onExit();
    
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, kCCNotificationFrameSizeChanged);
}

void Preview::onFrameSizeChanged(CCObject* sender) {
    setContentSize(CCEGLView::sharedOpenGLView()->getDesignResolutionSize());
    m_storyLayer->setContentSize(getContentSize());
}
