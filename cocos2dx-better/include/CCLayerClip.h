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
#ifndef __CCLayerClip_h__
#define __CCLayerClip_h__

#include "cocos2d.h"
#include "CCMoreMacros.h"
#include "ccMoreTypes.h"

NS_CC_BEGIN

/**
 * A layer which is clip children to its content area. Of course you
 * can use CCClippingNode to do that but it overkills me. This layer
 * uses scissor test to achieve clipping effect. I, on principle, don't 
 * like to modify cocos2d-x code. So, writing a new class is better than
 * changing CCNode code.
 */
class CC_DLL CCLayerClip : public CCLayerColor {
private:
    /// clip rect dirty flag
    bool m_clipDirty;
    
protected:
    CCLayerClip();
    
    /// update clip rectangle
    void updateClipRect();
    
public:
    virtual ~CCLayerClip();
    static CCLayerClip* create();
    static CCLayerClip* create(const ccColor4B& color);
    
    // override super
    virtual void visit();
    virtual void setPosition(const CCPoint &position);
    virtual void setContentSize(const CCSize& contentSize);
    
    /// clip rectangle
    CC_SYNTHESIZE_PASS_BY_REF(CCRect, m_clipRect, ClipRect);
    
    /// a flag can set clipping enablement
    CC_SYNTHESIZE_BOOL(m_clipEnabled, ClipEnabled);
};

NS_CC_END

#endif /* defined(__CCLayerClip_h__) */
