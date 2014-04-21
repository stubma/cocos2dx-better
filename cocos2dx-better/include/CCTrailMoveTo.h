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

using namespace std;

NS_CC_BEGIN

/**
 * display a trail when object is moving
 */
class CC_DLL CCTrailMoveTo : public CCMoveTo {
protected:
    /// trails
    CCArray m_trails;
    
    /// segment distance
    float m_trailDistance;
    
    /// alpha step
    float m_alphaStep;
    
    /// direction of moving
    CCPoint m_direction;
    
    /// speed
    float m_speed;
    
    /// distance
    float m_distance;
    
    /// end flag
    bool m_end;
    
protected:
    CCTrailMoveTo();
    
    /// clean trail when action is done
    void cleanTrails();
    
public:
    virtual ~CCTrailMoveTo();
    
    /**
     * create CCTrailMoveTo instance
     *
     * @param duration duration
     * @param position destination position
     * @param trailFrameName trail sprite frame name, in an atlas
     * @param trailLength length of trail
     * @param trailSegments segments of trail
     * @param trailColor trail color, default is white, means no change
     * @param trailColorScale the color will be multiplied to original color, default is white, means no change
     * @return CCTrailMoveTo instance
     */
    static CCTrailMoveTo* create(float duration, const CCPoint& position, const string& trailFrameName,
                                 float trailLength, int trailSegments, ccColor3B trailColor = ccWHITE, ccColor3B trailColorScale = ccWHITE);
    
    /// initializes the action with duration and trail arguments
    bool initWithDurationAndSpriteTrail(float duration, const CCPoint& position, const string& trailFrameName,
                                        float trailLength, int trailSegments, ccColor3B trailColor,
                                        ccColor3B trailColorScale);

    // override super
    virtual CCObject* copyWithZone(CCZone* pZone);
    virtual void startWithTarget(CCNode* pTarget);
    virtual void update(float time);
    
    CC_SYNTHESIZE(float, m_trailLength, TrailLength);
    CC_SYNTHESIZE(int, m_trailSegments, TrailSegments);
    CC_SYNTHESIZE_PASS_BY_REF(ccColor3B, m_trailColor, TrailColor);
    CC_SYNTHESIZE_PASS_BY_REF(ccColor3B, m_trailColorScale, TrailColorScale);
    CC_SYNTHESIZE_PASS_BY_REF(string, m_trailFrameName, TrailFrameName);
};

NS_CC_END

#endif /* defined(__CCTrailMoveTo__) */
