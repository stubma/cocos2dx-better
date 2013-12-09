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
#ifndef __CCClipOut__
#define __CCClipOut__

#include "cocos2d.h"

NS_CC_BEGIN

/**
 * Hide a node content gradually, this action can only run on a
 * CCClippingNode, and the CCClippingNode must has a CCDrawNode stencil
 */
class CC_DLL CCClipOut : public CCActionInterval {
private:
    /// cos value of direction
    float m_cos;
    
    /// sin value of direction
    float m_sin;
    
    /// full distance
    float m_distance;
    
    /// fix point 1 of bottom edge
    CCPoint m_p1;
    
    /// fix point 2 of bottom edge
    CCPoint m_p2;
    
protected:
    CCClipOut();
    
public:
    virtual ~CCClipOut();
    
    /**
     * create a CCClipOut instance
     *
     * @param duration action duration time
     * @param v the clip in direction vector, by default it is (1, 0), which
     *      means along the x axis
     */
    static CCClipOut* create(float duration, CCPoint v = ccp(1, 0));
    
    /// init self
    bool initWithDuration(float d, const CCPoint& v);
    
    /// @see CCActionInterval::update
    virtual void update(float time);
    
    /// @see CCActionInterval::startWithTarget
    virtual void startWithTarget(CCNode *pTarget);
    
    /// it returns a CCClipIn as reverse action
    virtual CCActionInterval* reverse();
    
    CC_SYNTHESIZE_PASS_BY_REF(CCPoint, m_direction, Direction);
};

NS_CC_END

#endif /* defined(__CCClipOut__) */
