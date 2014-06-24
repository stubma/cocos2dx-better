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
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC

#include "CCImage_richlabel.h"
#import <Foundation/Foundation.h>

NS_CC_BEGIN

CCImage_richlabel::CCImage_richlabel() :
m_shadowStrokePadding(CCPointZero) {
}

CCImage_richlabel::~CCImage_richlabel() {
}

bool CCImage_richlabel::initWithRichStringShadowStroke(const char * pText,
														int         nWidth,
														int         nHeight,
														ETextAlign eAlignMask,
														const char * pFontName,
														int         nSize,
														float       textTintR,
														float       textTintG,
														float       textTintB,
														bool shadow,
														float shadowOffsetX,
														float shadowOffsetY,
                                                        int   shadowColor,
														float shadowBlur,
														bool  stroke,
														float strokeR,
														float strokeG,
														float strokeB,
														float strokeSize,
                                                        float globalImageScaleFactor,
                                                        int toCharIndex,
                                                        float elapsedTime,
														CC_DECRYPT_FUNC decryptFunc) {
    return true;
}

CCSize CCImage_richlabel::measureRichString(const char* pText,
                                            const char* pFontName,
                                            int nSize,
                                            int maxWidth,
                                            float shadowOffsetX,
                                            float shadowOffsetY,
                                            float strokeSize,
                                            float globalImageScaleFactor,
											CC_DECRYPT_FUNC decryptFunc) {
    return CCSizeZero;
}

NS_CC_END

#endif // #if CC_TARGET_PLATFORM == CC_PLATFORM_IOS