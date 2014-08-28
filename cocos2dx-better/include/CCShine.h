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
#ifndef __CCShine__
#define __CCShine__

#include "cocos2d.h"

NS_CC_BEGIN

/**
 * display a shining on any node which move from left to right for once
 * the shining is gradient and can be set to max three colors
 */
class CC_DLL CCShine : public CCActionInterval {
private:
    /// old program
    CCGLProgram* m_oldProgram;
    
    /// target size
    CCSize m_size;
    
protected:
    CCShine();
    
public:
    virtual ~CCShine();
    static CCShine* create(float duration, ccColor4B color1, ccColor4B color2, ccColor4B color3, ccVertex3F gradientPositions);
    
    virtual bool initWithColors(float duration, ccColor4B color1, ccColor4B color2, ccColor4B color3, ccVertex3F gradientPositions);
    
    // override super
    virtual void update(float time);
    virtual void startWithTarget(CCNode *pTarget);
    virtual void stop();
    virtual CCObject* copyWithZone(CCZone* pZone);
    
    /// CCFlash reverse action is same as itself
    virtual CCActionInterval* reverse();
    
    // colors
    CC_SYNTHESIZE_PASS_BY_REF(ccColor4B, m_color1, Color1);
    CC_SYNTHESIZE_PASS_BY_REF(ccColor4B, m_color2, Color2);
    CC_SYNTHESIZE_PASS_BY_REF(ccColor4B, m_color3, Color3);
    CC_SYNTHESIZE_PASS_BY_REF(ccVertex3F, m_gradientPositions, GradientPositions);
};

NS_CC_END

#endif /* defined(__CCShine__) */
