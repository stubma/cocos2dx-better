/****************************************************************************
 Author: Luma (stubma@gmail.com)
 
 https://github.com/stubma/cocos2dx-common
 
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
#ifndef __CCMissile_h__
#define __CCMissile_h__

#include "cocos2d.h"

NS_CC_BEGIN

/**
 * node is moved to another node, called aimed node. The target node
 * and aimed node must belong to same parent node. When moving, target
 * node will head to aimed node so its rotation will be adjusted according 
 * to aimed node position. This action doesn't retain aimed node, you should
 * ensure the aimed node is not released before action completed
 */
class CC_DLL CCMissile : public CCActionInterval {
protected:
	/// aimed node
	CCNode* m_aimed;
	
	/// preset degree
	float m_presetDegree;
	
public:
	/**
	 * static factory method
	 *
	 * @param duration duration time of action in seconds
	 * @param aimed aimed node
	 * @param targetPresetDegree a degree should be reduced when set target node rotation,
	 *		positive means clockwise
	 */
	static CCMissile* create(float duration, CCNode* aimed, float targetPresetDegree = 0);
	
	virtual ~CCMissile();
	
	/** initializes the action */
    bool initWithDuration(float d, CCNode* aimed, float targetPresetDegree);
	
	/// @see CCObject::copyWithZone
	virtual CCObject* copyWithZone(CCZone* pZone);
	
	/// @see CCAction::update
	virtual void update(float t);
};

NS_CC_END

#endif // __CCMissile_h__
