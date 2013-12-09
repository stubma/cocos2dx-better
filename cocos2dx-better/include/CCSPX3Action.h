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
#ifndef __CCSPX3Action_h__
#define __CCSPX3Action_h__

#include "cocos2d.h"
#include "CCSPX3TileSet.h"

NS_CC_BEGIN

/**
 * @class CCSPX3Action
 *
 * animation of SpriteX 2011
 */
class CC_DLL CCSPX3Action : public CCObject {
public:
	/**
	 * data mode of action
	 */
	CCSPX3ActionMode m_mode;

	/**
	 * collision rect of this animation
	 */
	CCSPX3Tile m_collisionRect;

	/**
	 * frame count in this animation
	 */
	int m_frameCount;

	/**
	 * indices of frame
	 */
	short* m_frameIndices;

	/**
	 * duration time of frames. useless if data mode is not time
	 */
	float* m_delays;

protected:
	CCSPX3Action();

public:
    virtual ~CCSPX3Action();
    static CCSPX3Action* create();
};

NS_CC_END

#endif // __CCSPX3Action_h__
