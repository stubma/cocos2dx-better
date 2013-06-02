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

#include "CCColorLabelTTF.h"

NS_CC_BEGIN

CCColorLabelTTF::CCColorLabelTTF() {
	
}

CCColorLabelTTF::~CCColorLabelTTF() {
	
}

CCColorLabelTTF* CCColorLabelTTF::create(const char *string, const char *fontName, float fontSize) {
	CCColorLabelTTF* l = new CCColorLabelTTF();
	l->initWithString(string, fontName, fontSize);
	return (CCColorLabelTTF*)l->autorelease();
}

CCColorLabelTTF* CCColorLabelTTF::create(const char *string, const char *fontName, float fontSize,
										 const CCSize& dimensions, CCTextAlignment hAlignment) {
	CCColorLabelTTF* l = new CCColorLabelTTF();
	l->initWithString(string, fontName, fontSize, dimensions, hAlignment);
	return (CCColorLabelTTF*)l->autorelease();
}

CCColorLabelTTF* CCColorLabelTTF::create(const char *string, const char *fontName, float fontSize,
										 const CCSize& dimensions, CCTextAlignment hAlignment,
										 CCVerticalTextAlignment vAlignment) {
	CCColorLabelTTF* l = new CCColorLabelTTF();
	l->initWithString(string, fontName, fontSize, dimensions, hAlignment, vAlignment);
	return (CCColorLabelTTF*)l->autorelease();
}

NS_CC_END