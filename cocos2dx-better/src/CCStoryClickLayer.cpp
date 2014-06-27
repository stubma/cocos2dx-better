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
#include "CCStoryClickLayer.h"
#include "CCStoryPlayer.h"

NS_CC_BEGIN

CCStoryClickLayer::CCStoryClickLayer() :
m_player(NULL) {
	
}

CCStoryClickLayer::~CCStoryClickLayer() {
	
}

CCStoryClickLayer* CCStoryClickLayer::create(CCStoryPlayer* player) {
	CCStoryClickLayer* l = new CCStoryClickLayer();
	if(l->initWithPlayer(player)) {
		return (CCStoryClickLayer*)l->autorelease();
	}
	CC_SAFE_RELEASE(l);
	return NULL;
}

bool CCStoryClickLayer::initWithPlayer(CCStoryPlayer* player) {
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

bool CCStoryClickLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    return true;
}

void CCStoryClickLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {
}

void CCStoryClickLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
    m_player->onClickDone();
}

void CCStoryClickLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {
    m_player->onClickDone();
}

void CCStoryClickLayer::keyBackClicked() {
    m_player->onClickDone();
}

NS_CC_END