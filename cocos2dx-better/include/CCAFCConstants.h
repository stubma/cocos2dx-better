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
#ifndef __CCAFCConstants_h__
#define __CCAFCConstants_h__

NS_CC_BEGIN

/**
 * @typedef CCAFCClipType
 *
 * Type of clip
 */
typedef enum {
	/**
	 * clip is a image
	 */
	AFC_CLIP_IMAGE,

	/**
	 * clip is a ellipse
	 */
	AFC_CLIP_ELLIPSE,

	/**
	 * clip is a line
	 */
	AFC_CLIP_LINE,

	/**
	 * clip is a rectangle
	 */
	AFC_CLIP_RECT,

	/**
	 * clip is a round corner rectangle
	 */
	AFC_CLIP_ROUNDRECT,

	/**
	 * clip is a collision rect. Collision rectangle only has size property, it
	 * can't have a fill color, etc.
	 */
	AFC_CLIP_COLLISION_RECT,

	/**
	 * clip is a reference point
	 */
	AFC_CLIP_POINT,

	/**
	 * clip is a triangle
	 */
	AFC_CLIP_TRIANGLE
} CCAFCClipType;

NS_CC_END

#endif // __CCAFCConstants_h__
