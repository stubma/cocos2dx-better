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
#include "CCStoryMessageLayer.h"
#include "CCRichLabelTTF.h"
#include "CCStoryCommandSet.h"
#include "CCStoryPlayer.h"
#include "CCUtils.h"

NS_CC_BEGIN

CCStoryMessageLayer::CCStoryMessageLayer() :
m_msgLabel(NULL),
m_nameLabel(NULL),
m_looping(false),
m_player(NULL),
m_wait(0) {
	
}

CCStoryMessageLayer::~CCStoryMessageLayer() {
	
}

CCStoryMessageLayer* CCStoryMessageLayer::create(CCStoryPlayer* player) {
	CCStoryMessageLayer* l = new CCStoryMessageLayer();
	if(l->initWithPlayer(player)) {
		return (CCStoryMessageLayer*)l->autorelease();
	}
	CC_SAFE_RELEASE(l);
	return NULL;
}

void CCStoryMessageLayer::onExit() {
    CCLayer::onExit();
}

void CCStoryMessageLayer::onEnter() {
    CCLayer::onEnter();
}

bool CCStoryMessageLayer::initWithPlayer(CCStoryPlayer* player) {
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

void CCStoryMessageLayer::showMessage(CCStoryCommand* cmd) {
    // check and set flag
    if(m_looping)
        return;
    m_looping = true;
    
    // save
    m_wait = cmd->m_param.msg.wait;
    
    // create name label
    m_nameLabel = CCRichLabelTTF::create(cmd->m_param.msg.name, m_player->getNameFont().c_str(), m_player->getNameSize());
    m_nameLabel->setAnchorPoint(m_player->getNameAnchor());
    m_nameLabel->setPosition(m_player->getNamePos());
    m_nameLabel->setFontFillColor(ccc3FromInt(m_player->getNameColor()));
    m_nameLabel->setOpacity((m_player->getNameColor() >> 24) & 0xff);
    addChild(m_nameLabel);
    
    // create message label
    m_msgLabel = CCRichLabelTTF::create(cmd->m_param.msg.s,
                                        m_player->getMessageFont().c_str(),
                                        m_player->getMessageSize(),
                                        m_player->getMessageConstraints(),
                                        m_player->getMessageAlignment());
    m_msgLabel->setAnchorPoint(m_player->getMessageAnchor());
    m_msgLabel->setPosition(m_player->getMessagePos());
    m_msgLabel->setFontFillColor(ccc3FromInt(m_player->getMessageColor()));
    m_msgLabel->setOpacity((m_player->getMessageColor() >> 24) & 0xff);
    addChild(m_msgLabel);
    
    // start loop
    m_msgLabel->startLoopDisplay(0.05f, 0, 0, CCCallFunc::create(this, callfunc_selector(CCStoryMessageLayer::onDialogEndLooping)));
}

void CCStoryMessageLayer::handleUserClick() {
    // if looping, stop
    // if not looping, set command done
    if(m_looping) {
        m_msgLabel->stopLoopDisplay();
        m_looping = false;
        
        if(m_wait >= 0) {
            schedule(schedule_selector(CCStoryMessageLayer::onDialogDisplayedSomeWhile), 0, 0, m_wait);
        }
    } else {
        if(m_msgLabel) {
            unschedule(schedule_selector(CCStoryMessageLayer::onDialogDisplayedSomeWhile));
            m_msgLabel->removeFromParent();
            m_msgLabel = NULL;
        }
        if(m_nameLabel) {
            m_nameLabel->removeFromParent();
            m_nameLabel = NULL;
        }
        m_player->onMessageDone();
    }
}

bool CCStoryMessageLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    handleUserClick();
    return true;
}

void CCStoryMessageLayer::keyBackClicked() {
    handleUserClick();
}

void CCStoryMessageLayer::onDialogDisplayedSomeWhile(float delta) {
    if(m_msgLabel) {
        m_msgLabel->removeFromParent();
        m_msgLabel = NULL;
    }
    if(m_nameLabel) {
        m_nameLabel->removeFromParent();
        m_nameLabel = NULL;
    }
    m_player->onMessageDone();
}

void CCStoryMessageLayer::onDialogEndLooping() {
    m_looping = false;
    
    if(m_wait >= 0) {
        schedule(schedule_selector(CCStoryMessageLayer::onDialogDisplayedSomeWhile), 0, 0, m_wait);
    }
}

NS_CC_END