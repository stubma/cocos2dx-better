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
#include "CCMenuItemColor.h"
#include "CCMenuItemColorStateListener.h"

NS_CC_BEGIN

CCMenuItemColor::CCMenuItemColor() :
m_normalColor(ccc4(0, 0, 0, 0)),
m_selectedColor(ccc4(0, 0, 0, 0)),
m_disabledColor(ccc4(0, 0, 0, 0)),
m_focusColor(ccc4(0, 0, 0, 0)),
m_focus(false),
m_stateListener(NULL) {
    // init other
    m_bEnabled = true;
    m_tBlendFunc.src = CC_BLEND_SRC;
    m_tBlendFunc.dst = CC_BLEND_DST;
    memset(m_pSquareVertices, 0, sizeof(m_pSquareVertices));
    memset(m_pSquareColors, 0, sizeof(m_pSquareColors));
    ignoreAnchorPointForPosition(false);
    setAnchorPoint(ccp(0.5f, 0.5f));
}

CCMenuItemColor::~CCMenuItemColor() {
}

CCMenuItemColor* CCMenuItemColor::create(ccColor4B normal) {
    CCMenuItemColor* item = new CCMenuItemColor();
    item->initWithColorAndTarget(normal,
                                 ccc4(0, 0, 0, 0),
                                 ccc4(0, 0, 0, 0),
                                 ccc4(0, 0, 0, 0),
                                 NULL,
                                 NULL);
    return (CCMenuItemColor*)item->autorelease();
}

CCMenuItemColor* CCMenuItemColor::create(ccColor4B normal, ccColor4B focus) {
    CCMenuItemColor* item = new CCMenuItemColor();
    item->initWithColorAndTarget(normal,
                                 focus,
                                 ccc4(0, 0, 0, 0),
                                 ccc4(0, 0, 0, 0),
                                 NULL,
                                 NULL);
    return (CCMenuItemColor*)item->autorelease();
}

CCMenuItemColor* CCMenuItemColor::create(ccColor4B normal, ccColor4B selected, ccColor4B focus) {
    CCMenuItemColor* item = new CCMenuItemColor();
    item->initWithColorAndTarget(normal,
                                 focus,
                                 selected,
                                 ccc4(0, 0, 0, 0),
                                 NULL,
                                 NULL);
    return (CCMenuItemColor*)item->autorelease();
}

CCMenuItemColor* CCMenuItemColor::create(ccColor4B normal, ccColor4B selected, ccColor4B focus, ccColor4B disabled) {
    CCMenuItemColor* item = new CCMenuItemColor();
    item->initWithColorAndTarget(normal,
                                 focus,
                                 selected,
                                 disabled,
                                 NULL,
                                 NULL);
    return (CCMenuItemColor*)item->autorelease();
}

CCMenuItemColor* CCMenuItemColor::create(ccColor4B normal, CCObject* target, SEL_MenuHandler selector) {
    CCMenuItemColor* item = new CCMenuItemColor();
    item->initWithColorAndTarget(normal,
                                 ccc4(0, 0, 0, 0),
                                 ccc4(0, 0, 0, 0),
                                 ccc4(0, 0, 0, 0),
                                 target,
                                 selector);
    return (CCMenuItemColor*)item->autorelease();
}

CCMenuItemColor* CCMenuItemColor::create(ccColor4B normal, ccColor4B focus, CCObject* target, SEL_MenuHandler selector) {
    CCMenuItemColor* item = new CCMenuItemColor();
    item->initWithColorAndTarget(normal,
                                 focus,
                                 ccc4(0, 0, 0, 0),
                                 ccc4(0, 0, 0, 0),
                                 target,
                                 selector);
    return (CCMenuItemColor*)item->autorelease();
}

CCMenuItemColor* CCMenuItemColor::create(ccColor4B normal, ccColor4B selected, ccColor4B focus, CCObject* target, SEL_MenuHandler selector) {
    CCMenuItemColor* item = new CCMenuItemColor();
    item->initWithColorAndTarget(normal,
                                 focus,
                                 selected,
                                 ccc4(0, 0, 0, 0),
                                 target,
                                 selector);
    return (CCMenuItemColor*)item->autorelease();
}

CCMenuItemColor* CCMenuItemColor::create(ccColor4B normal, ccColor4B selected, ccColor4B focus, ccColor4B disabled, CCObject* target, SEL_MenuHandler selector) {
        CCMenuItemColor* item = new CCMenuItemColor();
        item->initWithColorAndTarget(normal,
                                     focus,
                                     selected,
                                     disabled,
                                     target,
                                     selector);
        return (CCMenuItemColor*)item->autorelease();
}

void CCMenuItemColor::initWithColorAndTarget(ccColor4B normal, ccColor4B selected, ccColor4B focus, ccColor4B disabled, CCObject* target, SEL_MenuHandler selector) {
    m_normalColor = normal;
    m_selectedColor = selected;
    m_focusColor = focus;
    m_disabledColor = disabled;
    setTarget(target, selector);
    
    updateColor();
    setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionColor));
}

void CCMenuItemColor::setContentSize(const CCSize& size) {
    m_pSquareVertices[1].x = size.width;
    m_pSquareVertices[2].y = size.height;
    m_pSquareVertices[3].x = size.width;
    m_pSquareVertices[3].y = size.height;
    
    CCMenuItem::setContentSize(size);
}

void CCMenuItemColor::updateColor() {
    if(isSelected()) {
        _displayedColor.r = _realColor.r = m_selectedColor.r;
        _displayedColor.g = _realColor.g = m_selectedColor.g;
        _displayedColor.b = _realColor.b = m_selectedColor.b;
        _displayedOpacity = _realOpacity = m_selectedColor.a;
    } else if(isFocus()) {
        _displayedColor.r = _realColor.r = m_focusColor.r;
        _displayedColor.g = _realColor.g = m_focusColor.g;
        _displayedColor.b = _realColor.b = m_focusColor.b;
        _displayedOpacity = _realOpacity = m_focusColor.a;
    } else if(isEnabled()) {
        _displayedColor.r = _realColor.r = m_normalColor.r;
        _displayedColor.g = _realColor.g = m_normalColor.g;
        _displayedColor.b = _realColor.b = m_normalColor.b;
        _displayedOpacity = _realOpacity = m_normalColor.a;
    } else {
        _displayedColor.r = _realColor.r = m_disabledColor.r;
        _displayedColor.g = _realColor.g = m_disabledColor.g;
        _displayedColor.b = _realColor.b = m_disabledColor.b;
        _displayedOpacity = _realOpacity = m_disabledColor.a;
    }
    
    for(unsigned int i = 0; i < 4; i++) {
        m_pSquareColors[i].r = _displayedColor.r / 255.0f;
        m_pSquareColors[i].g = _displayedColor.g / 255.0f;
        m_pSquareColors[i].b = _displayedColor.b / 255.0f;
        m_pSquareColors[i].a = _displayedOpacity / 255.0f;
    }
}

void CCMenuItemColor::setBlendFunc(ccBlendFunc blendFunc) {
    m_tBlendFunc = blendFunc;
}

ccBlendFunc CCMenuItemColor::getBlendFunc() {
    return m_tBlendFunc;
}

void CCMenuItemColor::selected() {
    selectedSilent();
	
	if(m_stateListener) {
		m_stateListener->onMenuItemColorSelected(this);
	}
}

void CCMenuItemColor::unselected() {
    unselectedSilent();
	
	if(m_stateListener) {
		m_stateListener->onMenuItemColorDeselected(this);
	}
}

void CCMenuItemColor::setEnabled(bool value) {
    setEnabledSilent(value);
	
	if(m_stateListener) {
		if(value)
			m_stateListener->onMenuItemColorEnabled(this);
		else
			m_stateListener->onMenuItemColorDisabled(this);
	}
}

void CCMenuItemColor::setFocus(bool flag) {
    setFocusSilent(flag);
	
	if(m_stateListener) {
		if(flag)
			m_stateListener->onMenuItemColorFocused(this);
		else
			m_stateListener->onMenuItemColorUnfocused(this);
	}
}

void CCMenuItemColor::selectedSilent() {
	CCMenuItem::selected();
    updateColor();
}

void CCMenuItemColor::unselectedSilent() {
	CCMenuItem::unselected();
    updateColor();
}

void CCMenuItemColor::setEnabledSilent(bool value) {
	CCMenuItem::setEnabled(value);
    updateColor();
}

void CCMenuItemColor::setFocusSilent(bool flag) {
	m_focus = flag;
    updateColor();
}

void CCMenuItemColor::draw() {
    CC_NODE_DRAW_SETUP();
    
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position | kCCVertexAttribFlag_Color );
    
    //
    // Attributes
    //
#ifdef EMSCRIPTEN
    setGLBufferData(m_pSquareVertices, 4 * sizeof(ccVertex2F), 0);
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    setGLBufferData(m_pSquareColors, 4 * sizeof(ccColor4F), 1);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_FALSE, 0, 0);
#else
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, m_pSquareVertices);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_FALSE, 0, m_pSquareColors);
#endif // EMSCRIPTEN
    
    ccGLBlendFunc(m_tBlendFunc.src, m_tBlendFunc.dst);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    CC_INCREMENT_GL_DRAWS(1);
}

NS_CC_END