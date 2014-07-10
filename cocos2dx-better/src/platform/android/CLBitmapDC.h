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
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#include "CCImage_richlabel.h"
#include <jni.h>
#include "JniHelper.h"
#include "CCResourceLoader.h"

using namespace std;
USING_NS_CC;

class CLBitmapDC {
private:
	static CLBitmapDC s_BmpDC;

public:
	// link meta list
	LinkMetaList m_linkMetas;

	// image rect list
	vector<CCRect> m_imageRects;

	// shadow stroke padding
	CCPoint m_shadowStrokePadding;

	// bitmap width
	int m_nWidth;

	// bitmap height
	int m_nHeight;

	// jni env
	JNIEnv* env;

	// this data will pass to CCImage, so it should NOT be released in this class
	unsigned char* m_pData;

	// decrypt function
	CC_DECRYPT_FUNC m_decryptFunc;

	// length of unstyled string
	int m_realLength;

    /// true means this label has continuous effect, so we need pass elapsed time to update it
    bool m_needTime;

protected:
	CLBitmapDC();

public:
	~CLBitmapDC(void);

	static CLBitmapDC& sharedCLBitmapDC();

	bool getBitmapFromJavaShadowStroke(const char *text, int nWidth, int nHeight, CCImage::ETextAlign eAlignMask, const char * pFontName, float fontSize,
			float textTintR = 1.0, float textTintG = 1.0, float textTintB = 1.0, bool shadow = false, float shadowDeltaX = 0.0, float shadowDeltaY = 0.0, int shadowColor = 0,
			float shadowBlur = 0.0, bool stroke = false, float strokeColorR = 0.0, float strokeColorG = 0.0, float strokeColorB =
					0.0, float strokeSize = 0.0, float lineSpacing = 0, float globalImageScaleFactor = 1, int toCharIndex = -1, float elapsedTime = 0, CC_DECRYPT_FUNC decryptFunc = NULL, bool sizeOnly = false);

	bool getBitmapFromJava(const char *text, int nWidth, int nHeight, CCImage::ETextAlign eAlignMask, const char * pFontName, float fontSize);

	// ARGB -> RGBA
	inline unsigned int swapAlpha(unsigned int value) {
		return ((value << 8 & 0xffffff00) | (value >> 24 & 0x000000ff));
	}
};

#endif // #if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
