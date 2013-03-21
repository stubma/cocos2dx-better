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
#ifndef __ccMoreTypes_h__
#define __ccMoreTypes_h__

#include "cocos2d.h"

USING_NS_CC;

NS_CC_BEGIN

/// hsv color
typedef struct ccColorHSV { float h, s, v; } ccColorHSV;
static inline ccColorHSV cchsv(float h, float s, float v) {
	ccColorHSV c = { h, s, v };
	return c;
}

// aabb
typedef struct ccAABB { CCPoint min, max; } ccAABB;
static inline ccAABB ccaabb(CCPoint min, CCPoint max) {
	ccAABB b = {
		min,
		max
	};
	return b;
}

// position
typedef struct ccPosition { int x, y; } ccPosition;
static inline ccPosition ccpos(int x, int y) {
    ccPosition p = {
        x, y
    };
    return p;
}

NS_CC_END

#endif // __ccMoreTypes_h__
