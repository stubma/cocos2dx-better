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
#ifndef __CCCurl__
#define __CCCurl__

#include "cocos2d.h"

NS_CC_BEGIN

class CC_DLL CCCurl : public CCActionInterval {
private:
    /// center
    CCPoint m_center;
    
    /// from radius
    float m_fromRadius;
    
    /// to radius
    float m_toRadius;
    
    /// angular velocity, in radian
    float m_angularVelocity;
    
    /// init radian
    float m_initAngle;
    
public:
    CCCurl();
    virtual ~CCCurl();
    
    static CCCurl* create(float duration, CCPoint center, float toRadius, float angularVelocity);
    
    /// init self
    bool initWithDuration(float d, CCPoint center, float toRadius, float angularVelocity);
    
    // override super
    virtual void update(float time);
    virtual void startWithTarget(CCNode *pTarget);
    virtual CCObject* copyWithZone(CCZone* pZone);
    virtual CCActionInterval* reverse();
};

NS_CC_END

#endif /* defined(__CCCurl__) */
