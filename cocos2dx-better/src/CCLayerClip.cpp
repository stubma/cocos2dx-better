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

NS_CC_BEGIN

CCLayerClip::CCLayerClip() :
m_clipRect(CCRectZero) {
}

CCLayerClip::~CCLayerClip() {
}

CCLayerClip* CCLayerClip::create() {
    CCLayerClip* l = new CCLayerClip();
    if(l->init()) {
        return (CCLayerClip*)l->autorelease();
    }
    
    l->release();
    return NULL;
}

void CCLayerClip::visit() {
    if(m_clipRect.equals(CCRectZero)) {
        CCLayer::visit();
    } else {
        glEnable(GL_SCISSOR_TEST);
		glScissor(m_clipRect.origin.x,
				  m_clipRect.origin.y,
				  m_clipRect.size.width,
				  m_clipRect.size.height);
		
		CCLayer::visit();
		
		glDisable(GL_SCISSOR_TEST);
    }
}

void CCLayerClip::setPosition(const CCPoint &position) {
    CCLayer::setPosition(position);
    updateClipRect();
}

void CCLayerClip::setContentSize(const CCSize& contentSize) {
    CCLayer::setContentSize(contentSize);
    updateClipRect();
}

void CCLayerClip::updateClipRect() {
    const CCSize& frameSize = CCEGLView::sharedOpenGLView()->getFrameSize();
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    const CCPoint& pos = getPosition();
    const CCSize& size = getContentSize();
    m_clipRect = CCRectMake((pos.x - origin.x) / visibleSize.width * frameSize.width,
                            (pos.y - origin.y) / visibleSize.height * frameSize.height,
                            size.width / visibleSize.width * frameSize.width,
                            size.height / visibleSize.height * frameSize.height);
}

NS_CC_END
