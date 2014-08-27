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
#ifndef __CCShake_h__
#define __CCShake_h__

#include "cocos2d.h"

NS_CC_BEGIN

/**
 * node moves randomly in a circle area, looks like shaking
 */
class CC_DLL CCShake : public CCActionInterval {
private:
    typedef enum {
        RING,
        DYNAMIC_RING,
    } Mode;
protected:
	/// min radius of shaking area
	float m_minRadius;
    
    /// max radius of shaking area
    float m_maxRadius;
    
    /// end max delta, for DYNAMIC_RING
    float m_endMaxRadius;
    
    /// end min delta, for DYNAMIC_RING
    float m_endMinRadius;

	/// original x position of node
	float m_originalX;

	/// original y position of node
	float m_originalY;
    
    /// mode
    Mode m_mode;

public:
	/**
	 * create a shake which in ring mode
	 *
	 * @param duration duration time of action in seconds
	 * @param maxRadius max radius of shaking area
     * @param minRadius min radius of shaking area, by default it is zero
	 */
	static CCShake* create(float duration, float maxRadius, float minRadius = 0);
    
	/**
	 * create a shake which in dynamic ring mode
	 *
	 * @param duration duration time of action in seconds
	 * @param startMaxRadius max radius of shaking area
     * @param startMinRadius min radius of shaking area, by default it is zero
     * @param endMaxRadius max radius at the end of action
     * @param endMinRadius min radius at the end of action
	 */
    static CCShake* create(float duration, float startMaxRadius, float endMaxRadius, float startMinRadius, float endMinRadius);

	virtual ~CCShake();

	/** initializes the action */
    bool initWithRing(float d, float maxR, float minR);
    bool initWithDynamicRing(float d, float startMaxR, float endMaxR, float startMinR, float endMinR);
	
	/// @see CCObject::copyWithZone
	virtual CCObject* copyWithZone(CCZone* pZone);

	/// @see CCActionInterval::reverse
	virtual CCActionInterval* reverse();

	/// @see CCAction::update
	virtual void update(float t);

	/// @see CCAction::startWithTarget
	virtual void startWithTarget(CCNode* pTarget);
};

NS_CC_END

#endif // __CCShake_h__
