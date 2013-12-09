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
#include "CCRichLabelTTFLinkStateSynchronizer.h"

#define TAG_MENU 0x70000

NS_CC_BEGIN

CCRichLabelTTFLinkStateSynchronizer::CCRichLabelTTFLinkStateSynchronizer(CCRichLabelTTF* owner) :
m_owner(owner) {
}

CCRichLabelTTFLinkStateSynchronizer::~CCRichLabelTTFLinkStateSynchronizer() {
}

void CCRichLabelTTFLinkStateSynchronizer::onMenuItemColorSelected(CCMenuItemColor* item) {
	// if no menu, return
	CCMenu* menu = (CCMenu*)m_owner->getChildByTag(TAG_MENU);
	if(!menu)
		return;
	
	// sync state
	int tag = item->getTag();
	CCObject* obj;
	CCARRAY_FOREACH(menu->getChildren(), obj) {
		CCMenuItemColor* sub = (CCMenuItemColor*)obj;
		if(sub != item && sub->getTag() == tag) {
			sub->selectedSilent();
		}
	}
}

void CCRichLabelTTFLinkStateSynchronizer::onMenuItemColorDeselected(CCMenuItemColor* item) {
	// if no menu, return
	CCMenu* menu = (CCMenu*)m_owner->getChildByTag(TAG_MENU);
	if(!menu)
		return;
	
	// sync state
	int tag = item->getTag();
	CCObject* obj;
	CCARRAY_FOREACH(menu->getChildren(), obj) {
		CCMenuItemColor* sub = (CCMenuItemColor*)obj;
		if(sub != item && sub->getTag() == tag) {
			sub->unselectedSilent();
		}
	}
}

void CCRichLabelTTFLinkStateSynchronizer::onMenuItemColorFocused(CCMenuItemColor* item) {
	// if no menu, return
	CCMenu* menu = (CCMenu*)m_owner->getChildByTag(TAG_MENU);
	if(!menu)
		return;
	
	// sync state
	int tag = item->getTag();
	CCObject* obj;
	CCARRAY_FOREACH(menu->getChildren(), obj) {
		CCMenuItemColor* sub = (CCMenuItemColor*)obj;
		if(sub != item && sub->getTag() == tag) {
			sub->setFocusSilent(true);
		}
	}
}

void CCRichLabelTTFLinkStateSynchronizer::onMenuItemColorUnfocused(CCMenuItemColor* item) {
	// if no menu, return
	CCMenu* menu = (CCMenu*)m_owner->getChildByTag(TAG_MENU);
	if(!menu)
		return;
	
	// sync state
	int tag = item->getTag();
	CCObject* obj;
	CCARRAY_FOREACH(menu->getChildren(), obj) {
		CCMenuItemColor* sub = (CCMenuItemColor*)obj;
		if(sub != item && sub->getTag() == tag) {
			sub->setFocusSilent(false);
		}
	}
}

void CCRichLabelTTFLinkStateSynchronizer::onMenuItemColorEnabled(CCMenuItemColor* item) {
	// if no menu, return
	CCMenu* menu = (CCMenu*)m_owner->getChildByTag(TAG_MENU);
	if(!menu)
		return;
	
	// sync state
	int tag = item->getTag();
	CCObject* obj;
	CCARRAY_FOREACH(menu->getChildren(), obj) {
		CCMenuItemColor* sub = (CCMenuItemColor*)obj;
		if(sub != item && sub->getTag() == tag) {
			sub->setEnabledSilent(true);
		}
	}
}

void CCRichLabelTTFLinkStateSynchronizer::onMenuItemColorDisabled(CCMenuItemColor* item) {
	// if no menu, return
	CCMenu* menu = (CCMenu*)m_owner->getChildByTag(TAG_MENU);
	if(!menu)
		return;
	
	// sync state
	int tag = item->getTag();
	CCObject* obj;
	CCARRAY_FOREACH(menu->getChildren(), obj) {
		CCMenuItemColor* sub = (CCMenuItemColor*)obj;
		if(sub != item && sub->getTag() == tag) {
			sub->setEnabledSilent(false);
		}
	}
}

NS_CC_END