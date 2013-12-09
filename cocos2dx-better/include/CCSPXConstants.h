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
#ifndef __CCSPXConstants_h__
#define __CCSPXConstants_h__

NS_CC_BEGIN

/**
 * @typedef CCSPXTransform
 *
 * SpriteX transform constants
 */
typedef enum {
	SPX_TRANS_NONE,
	SPX_TRANS_MIRROR_ROT180,
	SPX_TRANS_MIRROR,
	SPX_TRANS_ROT180,
	SPX_TRANS_MIRROR_ROT270,
	SPX_TRANS_ROT90,
	SPX_TRANS_ROT270,
	SPX_TRANS_MIRROR_ROT90
} CCSPXTransform;

NS_CC_END

#endif // __CCSPXConstants_h__
