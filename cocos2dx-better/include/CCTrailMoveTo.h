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
#ifndef __CCTrailMoveTo__
#define __CCTrailMoveTo__

#include "cocos2d.h"

NS_CC_BEGIN

/**
 * display a trail when object is moving
 */
class CC_DLL CCTrailMoveTo : public CCMoveTo {
protected:
    CCTrailMoveTo();
    
public:
    virtual ~CCTrailMoveTo();
    
    /**
     * create CCTrailMoveTo instance
     *
     * @param duration duration
     * @param position destination position
     * @param trailLength length of trail
     * @param trailSegments segments of trail
     * @param trailColor trail color, default is white, means no change
     * @param trailColorScale the color will be multiplied to original color, default is white, means no change
     * @return CCTrailMoveTo instance
     */
    static CCTrailMoveTo* create(float duration, const CCPoint& position, float trailLength, int trailSegments,
                            ccColor3B trailColor = ccWHITE, ccColor3B trailColorScale = ccWHITE);
    
    /// initializes the action with duration and trail arguments
    bool initWithDurationAndTrail(float duration, const CCPoint& position, float trailLength,
                                  int trailSegments, ccColor3B trailColor, ccColor3B trailColorScale);

    // override super
    virtual CCObject* copyWithZone(CCZone* pZone);
    virtual void startWithTarget(CCNode* pTarget);
    
    CC_SYNTHESIZE(float, m_trailLength, TrailLength);
    CC_SYNTHESIZE(int, m_trailSegments, TrailSegments);
    CC_SYNTHESIZE_PASS_BY_REF(ccColor3B, m_trailColor, TrailColor);
    CC_SYNTHESIZE_PASS_BY_REF(ccColor3B, m_trailColorScale, TrailColorScale);
};

NS_CC_END

#endif /* defined(__CCTrailMoveTo__) */
