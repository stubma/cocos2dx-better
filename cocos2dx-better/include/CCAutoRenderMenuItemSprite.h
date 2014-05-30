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
#ifndef __CCAutoRenderMenuItemSprite_h__
#define __CCAutoRenderMenuItemSprite_h__

#include "cocos2d.h"
#include "CCMoreMacros.h"

NS_CC_BEGIN

/// a menu item sprite which can automatically render selected state
class CC_DLL CCAutoRenderMenuItemSprite : public CCMenuItemSprite {
private:
	/// old color
	ccColor3B m_oldColor;
	
	/// is focus?
	bool m_focus;
	
	/// focus image
	CCNode* m_focusImage;
    
    /// selector of selected event
    CCObject* m_selectedEventTarget;
    SEL_MenuHandler m_selectedEventSelector;
    
    /// selector of unselected event
    CCObject* m_unselectedEventTarget;
    SEL_MenuHandler m_unselectedEventSelector;
	
protected:
	/// @see CCMenuItemSprite::updateImagesVisibility
	virtual void updateImagesVisibility();
	
public:
	CCAutoRenderMenuItemSprite();
	virtual ~CCAutoRenderMenuItemSprite();
	
	/**
	 * create a menu item
	 *
	 * @param sprite normal state
	 * @param target target
	 * @param selector selector
	 */
	static CCAutoRenderMenuItemSprite* create(CCNode* sprite, CCObject* target, SEL_MenuHandler selector);
	
    /**
	 * create a menu item without selector bounded
	 *
	 * @param normalImage normal state
	 * @param focusImage focus state
	 */
    static CCAutoRenderMenuItemSprite* create(CCNode* normalImage, CCNode* focusImage);
                                              
	/**
	 * create a menu item
	 *
	 * @param normalImage normal state
	 * @param focusImage focus state
	 * @param target target
	 * @param selector selector
	 */
	static CCAutoRenderMenuItemSprite* create(CCNode* normalImage, CCNode* focusImage, CCObject* target, SEL_MenuHandler selector);
	
	/**
	 * create a menu item
	 *
	 * @param normalImage normal state
	 * @param selectedImage selected state
	 * @param focusImage focus state
	 * @param target target
	 * @param selector selector
	 */
	static CCAutoRenderMenuItemSprite* create(CCNode* normalImage, CCNode* selectedImage,
											  CCNode* focusImage, CCObject* target, SEL_MenuHandler selector);
	
	/**
	 * create a menu item
	 *
	 * @param normalImage normal state
	 * @param selectedImage selected state
	 * @param disabledImage disabled state
	 * @param focusImage focus state
	 * @param target target
	 * @param selector selector
	 */
	static CCAutoRenderMenuItemSprite* create(CCNode* normalImage, CCNode* selectedImage, CCNode* disabledImage,
											  CCNode* focusImage, CCObject* target, SEL_MenuHandler selector);
    
    /// set target selector of selected event
    void setSelectedEvent(CCObject* target, SEL_MenuHandler selector);
    
    /// set target selector of unselected event
    void setUnselectedEvent(CCObject* target, SEL_MenuHandler selector);
	
	/// @see CCMenuItemSprite::selected
	virtual void selected();
	
	/// @see CCMenuItemSprite::unselected
    virtual void unselected();
	
	/// set focus
	void setFocus(bool flag);
	
	/// is focus
	bool isFocus() { return m_focus; }
	
	/// set focus image
	void setFocusImage(CCNode* focusImage);
	
	/// get focus image
	CCNode* getFocusImage() { return m_focusImage; }
    
    /// make every state image center aligned
    void centerAlignImages();
    
    /// true means focus image can be displayed with normal, selected and disable image
    CC_SYNTHESIZE_BOOL(m_focusIsAttachment, FocusIsAttachment);
};

NS_CC_END

#endif // __CCAutoRenderMenuItemSprite_h__
