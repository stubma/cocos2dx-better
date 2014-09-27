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
#ifndef __CCScrollBar__
#define __CCScrollBar__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ccMoreTypes.h"
#include "CCMoreMacros.h"

USING_NS_CC_EXT;
using namespace cocos2d::ui;

NS_CC_BEGIN

/**
 * A scroll bar can be used with CCScrollView or UIScrollView(CocoStudio ScrollView)
 */
class CC_DLL CCScrollBar : public CCLayer, public CCScrollViewDelegate {
private:
    /// timer to fade out
    float m_fadeOutTimer;
    
    /// is fading out
    bool m_fadingOut;
    
    /// backup old scrollview delegate
    CCScrollViewDelegate* m_oldCCDelegate;
    
    /// auto fade
    bool m_autoFade;
    
    /// init fade out
    bool m_initFadeOut;
    
protected:
	CCScrollBar();
	
	/// sync thumb position
	void syncThumbPositionSizeForUIScrollView(ScrollView* scrollView);
	
	/// sync thumb position for CCScrollView
	void syncThumbPositionSizeForCCScrollView(CCScrollView* scrollView);
	
	/// UIScrollView event handler
	void onUIScrollViewEvent(CCObject* sender, ScrollviewEventType e);
	
public:
	virtual ~CCScrollBar();
	static CCScrollBar* create(CCScale9Sprite* track, CCScale9Sprite* thumb);
	static CCScrollBar* create(CCScale9Sprite* track, CCSprite* thumb);
	
    // override super
    virtual void update(float delta);
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
    
	/// init
	virtual bool initWithTrackAndThumb(CCScale9Sprite* track, CCScale9Sprite* thumb);
	virtual bool initWithTrackAndFixedThumb(CCScale9Sprite* track, CCSprite* thumb);
	
	/// attach scrollbar to CocoStudio ScrollView
	void attachToUIScrollView(ScrollView* scrollView, ccInsets insets = cciZero, bool horizontal = false);
	
	/// attach scrollbar to CCScrollView
	void attachToCCScrollView(CCScrollView* scrollView, ccInsets insets = cciZero, bool horizontal = false);
    
    /// set auto fade and initial fade state
    void setAutoFade(bool autoFade, bool initFadeOut = false);
	
	/// track bar
	CC_SYNTHESIZE_RETAIN(CCScale9Sprite*, m_track, Track);
	
	/// thumb
	CC_SYNTHESIZE_RETAIN(CCScale9Sprite*, m_thumb, Thumb);
	
	/// unscalable thumb
	CC_SYNTHESIZE_RETAIN(CCSprite*, m_fixedThumb, FixedThumb);
	
	/// is horizontal?
	CC_SYNTHESIZE_BOOL(m_horizontal, Horizontal);
};

NS_CC_END

#endif /* defined(__CCScrollBar__) */
