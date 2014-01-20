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
#include "CCLayerClip.h"
#include "CCUtils.h"

NS_CC_BEGIN

CCLayerClip::CCLayerClip() :
m_clipRect(CCRectZero),
m_clipDirty(false),
m_clipEnabled(true) {
}

CCLayerClip::~CCLayerClip() {
}

CCLayerClip* CCLayerClip::create() {
    CCLayerClip* l = new CCLayerClip();
    if(l->initWithColor(cc4TRANSPARENT)) {
        return (CCLayerClip*)l->autorelease();
    }
    
    l->release();
    return NULL;
}

CCLayerClip* CCLayerClip::create(const ccColor4B& color) {
    CCLayerClip* l = new CCLayerClip();
    if(l->initWithColor(color)) {
        return (CCLayerClip*)l->autorelease();
    }
    
    l->release();
    return NULL;
}

void CCLayerClip::visit() {
    // update clip rect if dirty
    if(m_clipDirty) {
        updateClipRect();
        m_clipDirty = false;
    }
    
    // if clip rect is zero, don't clip
    if(!m_clipEnabled || m_clipRect.equals(CCRectZero)) {
        CCLayerColor::visit();
    } else {
        glEnable(GL_SCISSOR_TEST);
		glScissor(m_clipRect.origin.x,
				  m_clipRect.origin.y,
				  m_clipRect.size.width,
				  m_clipRect.size.height);
		
		CCLayerColor::visit();
		
		glDisable(GL_SCISSOR_TEST);
    }
}

void CCLayerClip::setPosition(const CCPoint &position) {
    CCLayerColor::setPosition(position);
    m_clipDirty = true;
}

void CCLayerClip::setContentSize(const CCSize& contentSize) {
    CCLayerColor::setContentSize(contentSize);
    m_clipDirty = true;
}

void CCLayerClip::updateClipRect() {
    const CCRect& viewRect = CCEGLView::sharedOpenGLView()->getViewPortRect();
    CCRect bound = CCUtils::getBoundingBoxInWorldSpace(this);
    float scaleX = CCEGLView::sharedOpenGLView()->getScaleX();
    float scaleY = CCEGLView::sharedOpenGLView()->getScaleY();
    bound.origin.x *= scaleX;
    bound.origin.y *= scaleY;
    bound.origin.x += viewRect.origin.x;
    bound.origin.y += viewRect.origin.y;
    bound.size.width *= scaleX;
    bound.size.height *= scaleY;
    m_clipRect = bound;
}

NS_CC_END
