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
#ifndef __CCMenuItemColor__
#define __CCMenuItemColor__

#include "cocos2d.h"
#include "CCMoreMacros.h"

NS_CC_BEGIN

class CCMenuItemColorStateListener;

/**
 * A menu item which only show a pure color. Because it is not based
 * on a texture, you must specify content size for it.
 */
class CC_DLL CCMenuItemColor : public CCMenuItem, public CCBlendProtocol {
private:
    /// normal color
    ccColor4B m_normalColor;
    
    /// selected color
    ccColor4B m_selectedColor;
    
    /// disable color
    ccColor4B m_disabledColor;
    
    /// focus color
    ccColor4B m_focusColor;
    
    /// vertices
    ccVertex2F m_pSquareVertices[4];
    
    /// colors
    ccColor4F m_pSquareColors[4];
    
protected:
    CCMenuItemColor();
    
    /// update color
    virtual void updateColor();
    
public:
	virtual ~CCMenuItemColor();
    
    static CCMenuItemColor* create(ccColor4B normal);
    static CCMenuItemColor* create(ccColor4B normal, ccColor4B selected);
    static CCMenuItemColor* create(ccColor4B normal, ccColor4B selected, ccColor4B focus);
    static CCMenuItemColor* create(ccColor4B normal, ccColor4B selected, ccColor4B focus, ccColor4B disabled);
    static CCMenuItemColor* create(ccColor4B normal, CCObject* target, SEL_MenuHandler selector);
    static CCMenuItemColor* create(ccColor4B normal, ccColor4B selected, CCObject* target, SEL_MenuHandler selector);
    static CCMenuItemColor* create(ccColor4B normal, ccColor4B selected, ccColor4B focus, CCObject* target, SEL_MenuHandler selector);
    static CCMenuItemColor* create(ccColor4B normal, ccColor4B selected, ccColor4B focus, ccColor4B disabled, CCObject* target, SEL_MenuHandler selector);
    
    /// init
    void initWithColorAndTarget(ccColor4B normal, ccColor4B focus, ccColor4B selected, ccColor4B disabled, CCObject* target, SEL_MenuHandler selector);
    
    /// override super
    virtual void draw();
    virtual void setContentSize(const CCSize& size);
    virtual void selected();
    virtual void unselected();
    virtual void setEnabled(bool value);
	
	// those method doesn't trigger state event
	virtual void selectedSilent();
	virtual void unselectedSilent();
	virtual void setEnabledSilent(bool value);
	virtual void setFocusSilent(bool flag);
    
    /** BlendFunction. Conforms to CCBlendProtocol protocol */
    CC_PROPERTY(ccBlendFunc, m_tBlendFunc, BlendFunc);
    
    /// focused state
    CC_SYNTHESIZE_BOOL_SETTER(m_focus, Focus);
	
	/// state listener
	CC_SYNTHESIZE(CCMenuItemColorStateListener*, m_stateListener, StateListener);
};

NS_CC_END

#endif /* defined(__CCMenuItemColor__) */
