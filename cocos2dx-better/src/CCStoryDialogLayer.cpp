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
#include "CCStoryDialogLayer.h"
#include "CCRichLabelTTF.h"
#include "CCStoryCommandSet.h"
#include "CCStoryPlayer.h"
#include "CCUtils.h"

NS_CC_BEGIN

CCStoryDialogLayer::CCStoryDialogLayer() :
m_label(NULL),
m_looping(false),
m_player(NULL) {
	
}

CCStoryDialogLayer::~CCStoryDialogLayer() {
	
}

CCStoryDialogLayer* CCStoryDialogLayer::create(CCStoryPlayer* player) {
	CCStoryDialogLayer* l = new CCStoryDialogLayer();
	if(l->initWithPlayer(player)) {
		return (CCStoryDialogLayer*)l->autorelease();
	}
	CC_SAFE_RELEASE(l);
	return NULL;
}

void CCStoryDialogLayer::onExit() {
    CCLayer::onExit();
}

void CCStoryDialogLayer::onEnter() {
    CCLayer::onEnter();
}

bool CCStoryDialogLayer::initWithPlayer(CCStoryPlayer* player) {
	if(!CCLayer::init())
		return false;
    
    // init
    m_player = player;
    
    // event
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setKeypadEnabled(true);
	
	return true;
}

void CCStoryDialogLayer::showMessage(CCStoryCommand* cmd) {
    // check and set flag
    if(m_looping)
        return;
    m_looping = true;
    
    // create label
    m_label = CCRichLabelTTF::create(cmd->m_param.msg.s, "Helvetica", 40);
    m_label->setPosition(CCUtils::getLocalCenter(this));
    addChild(m_label);
    
    // start loop
    m_label->startLoopDisplay(0.05f, 0, 0, CCCallFunc::create(this, callfunc_selector(CCStoryDialogLayer::onDialogEndLooping)));
}

void CCStoryDialogLayer::handleUserClick() {
    // if looping, stop
    // if not looping, set command done
    if(m_looping) {
        m_label->stopLoopDisplay();
        m_looping = false;
        schedule(schedule_selector(CCStoryDialogLayer::onDialogDisplayedSomeWhile), 1, 0, 0);
    } else {
        m_player->markCurrentCommandDone();
        if(m_label) {
            unschedule(schedule_selector(CCStoryDialogLayer::onDialogDisplayedSomeWhile));
            m_label->removeFromParent();
            m_label = NULL;
        }
    }
}

bool CCStoryDialogLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    handleUserClick();
    return true;
}

void CCStoryDialogLayer::keyBackClicked() {
    handleUserClick();
}

void CCStoryDialogLayer::onDialogDisplayedSomeWhile(float delta) {
    m_player->markCurrentCommandDone();
    if(m_label) {
        m_label->removeFromParent();
        m_label = NULL;
    }
}

void CCStoryDialogLayer::onDialogEndLooping() {
    m_looping = false;
    schedule(schedule_selector(CCStoryDialogLayer::onDialogDisplayedSomeWhile), 1, 0, 0);
}

NS_CC_END