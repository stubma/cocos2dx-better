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
#ifndef __CCFlash__
#define __CCFlash__

#include "cocos2d.h"

NS_CC_BEGIN

/**
 * flash a color on a node opaque area
 */
class CC_DLL CCFlash : public CCActionInterval {
private:
    /// old program
    CCGLProgram* m_oldProgram;
    
    /// color
    ccColor3B m_color;
    
    /// color in float
    float m_r;
    float m_g;
    float m_b;
    
protected:
    CCFlash();
    
public:
    virtual ~CCFlash();
    
    /**
     * create a CCFlash instance
     *
     * @param duration action duration time
     * @param c the flash color
     */
    static CCFlash* create(float duration, ccColor3B c);
    
    /// init self
    bool initWithDurationAndColor(float d, ccColor3B c);
    
    // override super
    virtual void update(float time);
    virtual void startWithTarget(CCNode *pTarget);
    virtual void stop();
    virtual CCObject* copyWithZone(CCZone* pZone);
    
    /// CCFlash reverse action is same as itself
    virtual CCActionInterval* reverse();
};

NS_CC_END

#endif /* defined(__CCFlash__) */
