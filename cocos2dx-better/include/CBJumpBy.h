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
#ifndef __CBJumpBy_h__
#define __CBJumpBy_h__

#include "cocos2d.h"
#include "CCMoreMacros.h"

NS_CC_BEGIN

/**
 * To enhance or fix original CCJumpBy, features added:
 * 1. the sprite can be auto rotated along the moving path
 *
 * \note
 * Sometimes I find defect in cocos2d-x and want to create a new class to workaround it,
 * so I will choose CB prefix instead of CC. I use Ex suffix before but I don't like it anymore.
 * CB stands cocos2dx-better.
 */
class CC_DLL CBJumpBy : public CCJumpBy {
protected:
    CBJumpBy();
    
public:
    virtual ~CBJumpBy();
    
    /**
     * create a CBJumpBy
     *
     * @param duration duration time
     * @param position delta
     * @param height jump height
     * @param jumps jump count
     * @param autoHeadOn optional, true means sprite will auto rotated along
     *      the path
     * @param initAngle optional, sprite initial angle in degree, x axis will be 0 and
     *      positive value means counter-clockwise
     * @return CBJumpBy instance
     */
    static CBJumpBy* create(float duration, const CCPoint& position, float height, unsigned int jumps, bool autoHeadOn = false, float initAngle = 0);
    
    /// initialization
    bool initWithDuration(float duration, const CCPoint& position, float height, unsigned int jumps, bool autoHeadOn, float initAngle);
    
    // override super
    virtual CCObject* copyWithZone(CCZone* pZone);
    virtual void update(float time);
    virtual CCActionInterval* reverse();
    
    CC_SYNTHESIZE_BOOL(m_autoHeadOn, AutoHeadOn);
    CC_SYNTHESIZE(float, m_initAngle, InitAngle);
};

NS_CC_END

#endif /* defined(__CBJumpBy_h__) */
