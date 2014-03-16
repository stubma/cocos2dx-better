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
#ifndef __CCLaserSprite__
#define __CCLaserSprite__

#include "cocos2d.h"

NS_CC_BEGIN

/// a laser sprite, just for fun, but you can use it in aircraft game
class CC_DLL CCLaserSprite : public CCSprite {
protected:
	CCLaserSprite();
	
public:
	virtual ~CCLaserSprite();
	static CCLaserSprite* create(float w, float h);
	
	// override super
	virtual bool initWithTexture(CCTexture2D *pTexture);
	virtual void setTexture(CCTexture2D *texture);
	
	// set width and height
	void setWidth(float w);
	void setHeight(float h);
};

NS_CC_END

#endif /* defined(__CCLaserSprite__) */
