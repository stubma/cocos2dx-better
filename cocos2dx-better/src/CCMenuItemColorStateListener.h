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
#ifndef __CCMenuItemColorStateListener_h__
#define __CCMenuItemColorStateListener_h__

#include "CCMenuItemColor.h"

NS_CC_BEGIN

/**
 * A interface to listen state change event of CCMenuItemColor. Actually this
 * class is created for CCRichLabelTTF link implementation. In very rare situation
 * you want to use it, so it is internal.
 */
class CCMenuItemColorStateListener {
public:
	virtual void onMenuItemColorSelected(CCMenuItemColor* item) = 0;
	virtual void onMenuItemColorDeselected(CCMenuItemColor* item) = 0;
	virtual void onMenuItemColorFocused(CCMenuItemColor* item) = 0;
	virtual void onMenuItemColorUnfocused(CCMenuItemColor* item) = 0;
	virtual void onMenuItemColorEnabled(CCMenuItemColor* item) = 0;
	virtual void onMenuItemColorDisabled(CCMenuItemColor* item) = 0;
};

NS_CC_END

#endif // __CCMenuItemColorStateListener_h__
