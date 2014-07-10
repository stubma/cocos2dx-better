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

#include "CLBitmapDC.h"

CLBitmapDC CLBitmapDC::s_BmpDC;

CLBitmapDC::CLBitmapDC() :
m_pData(NULL),
m_decryptFunc(NULL),
m_nWidth(0),
m_nHeight(0),
m_realLength(0),
m_needTime(false),
m_shadowStrokePadding(CCPointZero) {
}

CLBitmapDC::~CLBitmapDC(void) {
}

CLBitmapDC& CLBitmapDC::sharedCLBitmapDC() {
	return s_BmpDC;
}

bool CLBitmapDC::getBitmapFromJava(const char *text, int nWidth, int nHeight, CCImage::ETextAlign eAlignMask, const char * pFontName, float fontSize) {
	return getBitmapFromJavaShadowStroke(text, nWidth, nHeight, eAlignMask, pFontName, fontSize);
}

bool CLBitmapDC::getBitmapFromJavaShadowStroke(const char *text, int nWidth, int nHeight, CCImage::ETextAlign eAlignMask, const char * pFontName, float fontSize,
		float textTintR, float textTintG, float textTintB, bool shadow, float shadowDeltaX, float shadowDeltaY, int shadowColor,
		float shadowBlur, bool stroke, float strokeColorR, float strokeColorG, float strokeColorB, float strokeSize, float lineSpacing, float globalImageScaleFactor, int toCharIndex,
		float elapsedTime, CC_DECRYPT_FUNC decryptFunc, bool sizeOnly) {
	// save func
	m_decryptFunc = decryptFunc;
	
	// check method
	JniMethodInfo methodInfo;
	if(!JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/lib/CCImage_richlabel", "createRichLabelBitmap",
			"(Ljava/lang/String;Ljava/lang/String;IFFFIIIZFFIFZFFFFFFFIFZZ)V")) {
		CCLOG("%s %d: error to get methodInfo", __FILE__, __LINE__);
		return false;
	}

	// Do a full lookup for the font path using CCFileUtils in case the given font name is a relative path to a font file asset,
	// or the path has been mapped to a different location in the app package:
	string fullPathOrFontName = CCFileUtils::sharedFileUtils()->fullPathForFilename(pFontName);

	// If the path name returned includes the 'assets' dir then that needs to be removed, because the android.content.Context
	// requires this portion of the path to be omitted for assets inside the app package.
	if(fullPathOrFontName.find("assets/") == 0) {
		fullPathOrFontName = fullPathOrFontName.substr(strlen("assets/")); // Chop out the 'assets/' portion of the path.
	}

	/**create bitmap
	 * this method call Cococs2dx.createBitmap()(java code) to create the bitmap, the java code
	 * will call Java_org_cocos2dx_lib_Cocos2dxBitmap_nativeInitBitmapDC() to init the width, height
	 * and data.
	 * use this approach to decrease the jni call number
	 */
	jstring jstrText = methodInfo.env->NewStringUTF(text);
	jstring jstrFont = methodInfo.env->NewStringUTF(fullPathOrFontName.c_str());

	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jstrText, jstrFont, (int) fontSize, textTintR, textTintG, textTintB,
			eAlignMask, nWidth, nHeight, shadow, shadowDeltaX, -shadowDeltaY, shadowColor, shadowBlur, stroke, strokeColorR, strokeColorG, strokeColorB, strokeSize,
			lineSpacing, CC_CONTENT_SCALE_FACTOR(), globalImageScaleFactor, toCharIndex, elapsedTime, decryptFunc != NULL, sizeOnly);

	methodInfo.env->DeleteLocalRef(jstrText);
	methodInfo.env->DeleteLocalRef(jstrFont);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);

	return true;
}

#endif // #if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
