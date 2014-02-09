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
#include "CLBitmapDC.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_org_cocos2dx_lib_CCImage_1richlabel_nativeInitBitmapDC(JNIEnv* env, jclass clazz, jint width, jint height, jbyteArray pixels) {
	CLBitmapDC& bitmapDC = CLBitmapDC::sharedCLBitmapDC();
	bitmapDC.m_nWidth = width;
	bitmapDC.m_nHeight = height;

	if(pixels) {
		int size = width * height * 4;
		bitmapDC.m_pData = new unsigned char[size];
		env->GetByteArrayRegion(pixels, 0, size, (jbyte*)bitmapDC.m_pData);

		// swap data
		unsigned int* tempPtr = (unsigned int*)bitmapDC.m_pData;
		unsigned int tempdata = 0;
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				tempdata = *tempPtr;
				*tempPtr++ = bitmapDC.swapAlpha(tempdata);
			}
		}
	}
}

JNIEXPORT void JNICALL Java_org_cocos2dx_lib_CCImage_1richlabel_nativeSaveLinkMeta(JNIEnv* env, jclass clazz, jint normalBgColor, jint selectedBgColor,
		jfloat x, jfloat y, jfloat width, jfloat height, jint tag) {
	CLBitmapDC& bitmapDC = CLBitmapDC::sharedCLBitmapDC();
	LinkMeta meta = {
		normalBgColor,
		selectedBgColor,
		tag,
		x,
		y,
		width,
		height
	};
	bitmapDC.m_linkMetas.push_back(meta);
}

JNIEXPORT void JNICALL Java_org_cocos2dx_lib_CCImage_1richlabel_nativeSaveShadowStrokePadding(JNIEnv* env, jclass clazz, jfloat x, jfloat y) {
	CLBitmapDC& bitmapDC = CLBitmapDC::sharedCLBitmapDC();
	bitmapDC.m_shadowStrokePadding.x = x;
	bitmapDC.m_shadowStrokePadding.y = y;
}

JNIEXPORT void JNICALL Java_org_cocos2dx_lib_CCImage_1richlabel_nativeResetBitmapDC(JNIEnv* env, jclass clazz) {
	CLBitmapDC& bitmapDC = CLBitmapDC::sharedCLBitmapDC();
	bitmapDC.m_linkMetas.clear();
	bitmapDC.m_shadowStrokePadding = CCPointZero;
}

JNIEXPORT jstring JNICALL Java_org_cocos2dx_lib_CCImage_1richlabel_nativeFullPathForFilename(JNIEnv* env, jclass clazz, jstring filename) {
	string fn = JniHelper::jstring2string(filename);
	string path = CCFileUtils::sharedFileUtils()->fullPathForFilename(fn.c_str());
	return env->NewStringUTF(path.c_str());
}

#ifdef __cplusplus
}
#endif
