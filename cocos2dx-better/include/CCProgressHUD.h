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
#ifndef __CCProgressHUD__
#define __CCProgressHUD__

#include "cocos2d.h"
#include "CCRichLabelTTF.h"
#include "CCMoreMacros.h"

using namespace std;

NS_CC_BEGIN

/**
 * CCProgressHUD display a general indeterminate progress indicator and
 * a message. The progress indicator can be replaced by other image, and
 * CCProgressHUD can be set to be cancelable or not.
 */
class CC_DLL CCProgressHUD : public CCLayerColor {
protected:
    /// indicator sprite
    CCSprite* m_indicator;
    
    /// message label
    CCRichLabelTTF* m_msgLabel;
    
    /// panel size
    CCSize m_panelSize;
    
    /// time to update animation
    float m_time;
    
protected:
    CCProgressHUD();
    
    /// create instance
    static CCProgressHUD* create(const string& message);
    
    /// relayout
    void relayout();
    
public:
    virtual ~CCProgressHUD();
    
    // override super
    virtual bool initWithMessage(const string& message);
    virtual void draw();
    virtual void update(float delta);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void keyBackClicked();
    
    /**
     * create a CCProgressHUD. If currently one instance exists, no
     * new CCProgressHUD will be created.
     *
     * \note the CCProgressHUD will be added to running scene, with max
     * z order value
     */
    static CCProgressHUD* show(const string& message);
    
    /**
     * create a CCProgressHUD. If currently one instance exists, no
     * new CCProgressHUD will be created.
     *
     * \note the CCProgressHUD will be added to specified parent node, with max
     * z order value
     */
    static CCProgressHUD* show(const string& message, CCNode* parent);
    
    /// hide current instance of CCProgressHUD, or do nothing if no instance
    static void hide();
    
    /// return current instance or NULL if no
    static CCProgressHUD* current();
    
    /// cancelable or not
    CC_SYNTHESIZE_BOOL(m_cancellable, Cancellable);
    
    /// dim color
    CC_SYNTHESIZE_PASS_BY_REF_SETTER(ccColor4B, m_dimColor, DimColor);
    
    /// background color of progress indicator panel
    CC_SYNTHESIZE_PASS_BY_REF_SETTER(ccColor4B, m_panelColor, PanelColor);
    
    /// icon sprite, it will be retained by HUD
	/// if NULL, default progress indicator will be used
    CC_SYNTHESIZE_SETTER(CCSprite*, m_icon, Icon);
    
    /// message
    CC_SYNTHESIZE_PASS_BY_REF_SETTER(string, m_message, Message);
	
	/// true means the panel bg must be in square shape
	CC_SYNTHESIZE_BOOL_SETTER(m_forceSquare, ForceSquare);
};

NS_CC_END

#endif /* defined(__CCProgressHUD__) */
