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
#include "ccMoreTypes.h"
#include "CCMoreMacros.h"

using namespace std;

NS_CC_BEGIN

/**
 * display a trail when object is moving
 * 
 * \note
 * When CCTrailMoveTo is run on a CCArmature, the trail is not added to target so you
 * must reserve enough z order space for trail
 */
class CC_DLL CCTrailMoveTo : public CCMoveTo {
private:
    enum Mode {
        SPRITE_FRAME_NAME,
        FILE_NAME,
        ARMATURE
    };
    
protected:
    /// trails
    CCArray m_trails;
    
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
    
    /// mode
    Mode m_mode;
    
    /// sprite name, may be file name or frame name
    string m_spriteName;
    
    /// current visible armature trail index
    int m_visibleArmIndex;
    
protected:
    CCTrailMoveTo();
    
    /// clean trail when action is done
    void cleanTrails();
    
    /// pre draw
    void onPreDraw(CCObject* sender);
    
    /// remove a trail
    void removeTrail(CCNode* trail);
    
public:
    virtual ~CCTrailMoveTo();
    
    /**
     * create CCTrailMoveTo instance
     *
     * @param duration duration
     * @param position destination position
     * @param trailFrameName trail sprite frame name, in an atlas
     * @param trailDistance distance between trails
     * @param trailSegments segments of trail
     * @param trailColor trail color, default is white, means no change
     * @param trailColorScale the color will be multiplied to original color, default is white, means no change
     * @param removeWhenDone true means trails will be removed when moving is done, by default it is false
     * @return CCTrailMoveTo instance
     */
    static CCTrailMoveTo* createWithSpriteFrameName(float duration, const CCPoint& position, const string& trailFrameName,
                                 float trailDistance, int trailSegments, ccColor3B trailColor = ccWHITE, ccColor4B trailColorScale = cc4BLACK, bool removeWhenDone = false);
    
    /**
     * create CCTrailMoveTo instance
     *
     * @param duration duration
     * @param position destination position
     * @param trailFrameName trail sprite frame name, in an atlas
     * @param trailDistance distance between trails
     * @param trailSegments segments of trail
     * @param trailColor trail color, default is white, means no change
     * @param trailColorScale the color will be multiplied to original color, default is white, means no change
     * @param removeWhenDone true means trails will be removed when moving is done, by default it is false
     * @return CCTrailMoveTo instance
     */
    static CCTrailMoveTo* createWithFileName(float duration, const CCPoint& position, const string& fileName,
                                                    float trailDistance, int trailSegments, ccColor3B trailColor = ccWHITE, ccColor4B trailColorScale = cc4BLACK, bool removeWhenDone = false);
    
    /**
     * create CCTrailMoveTo instance, the target must be a CCArmature
     *
     * @param duration duration
     * @param position destination position
     * @param armatureName armature name
     * @param animationIndex the animation index will be played
     * @param trailDistance distance between trails
     * @param trailSegments segments of trail
     * @param trailColor trail color, default is white, means no change
     * @param trailColorScale the color will be multiplied to original color, default is white, means no change
     * @param removeWhenDone true means trails will be removed when moving is done, by default it is false
     * @return CCTrailMoveTo instance
     */
    static CCTrailMoveTo* createWithArmature(float duration, const CCPoint& position, const string& armatureName,
                                             int animationIndex, float trailDistance, int trailSegments,
                                             ccColor3B trailColor = ccWHITE, ccColor4B trailColorScale = cc4BLACK, bool removeWhenDone = false);
    
    /**
     * create CCTrailMoveTo instance, the target must be a CCArmature
     *
     * @param duration duration
     * @param position destination position
     * @param armatureName armature name
     * @param animationName the animation name will be played
     * @param trailDistance distance between trails
     * @param trailSegments segments of trail
     * @param trailColor trail color, default is white, means no change
     * @param trailColorScale the color will be multiplied to original color, default is white, means no change
     * @param removeWhenDone true means trails will be removed when moving is done, by default it is false
     * @return CCTrailMoveTo instance
     */
    static CCTrailMoveTo* createWithArmature(float duration, const CCPoint& position, const string& armatureName,
                                             const string& animationName, float trailDistance, int trailSegments,
                                             ccColor3B trailColor = ccWHITE, ccColor4B trailColorScale = cc4BLACK, bool removeWhenDone = false);
    
    bool initWithDurationAndSpriteTrail(float duration, const CCPoint& position, const string& spriteName,
                                        float trailDistance, int trailSegments, ccColor3B trailColor,
                                        ccColor4B trailColorScale, bool removeWhenDone);
    
    bool initWithDurationAndArmatureTrail(float duration, const CCPoint& position, const string& armatureName,
                                          const string& animationName, float trailDistance, int trailSegments,
                                          ccColor3B trailColor, ccColor4B trailColorScale, bool removeWhenDone);
    
    bool initWithDurationAndArmatureTrail(float duration, const CCPoint& position, const string& armatureName,
                                          int animationIndex, float trailDistance, int trailSegments,
                                          ccColor3B trailColor, ccColor4B trailColorScale, bool removeWhenDone);
    
    // override super
    virtual CCObject* copyWithZone(CCZone* pZone);
    virtual void startWithTarget(CCNode* pTarget);
    virtual void update(float time);
    
    CC_SYNTHESIZE(float, m_trailDistance, trailDistance);
    CC_SYNTHESIZE(int, m_trailSegments, TrailSegments);
    CC_SYNTHESIZE_PASS_BY_REF(ccColor3B, m_trailColor, TrailColor);
    CC_SYNTHESIZE_PASS_BY_REF(ccColor4B, m_trailColorScale, TrailColorScale);
    CC_SYNTHESIZE(int, m_animationIndex, AnimationIndex);
    CC_SYNTHESIZE_PASS_BY_REF(string, m_animationName, AnimationName);
    CC_SYNTHESIZE_BOOL(m_removeWhenDone, RemoveWhenDone);
};

NS_CC_END

#endif /* defined(__CCTrailMoveTo__) */
