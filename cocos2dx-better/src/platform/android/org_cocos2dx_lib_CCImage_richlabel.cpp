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
#include "CCUtils.h"

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

JNIEXPORT void JNICALL Java_org_cocos2dx_lib_CCImage_1richlabel_nativeSaveImageRect(JNIEnv* env, jclass clazz, jfloat x, jfloat y, jfloat w, jfloat h) {
	CLBitmapDC& bitmapDC = CLBitmapDC::sharedCLBitmapDC();
	bitmapDC.m_imageRects.push_back(CCRectMake(x, y, w, h));
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
	bitmapDC.m_imageRects.clear();
	bitmapDC.m_shadowStrokePadding = CCPointZero;
	bitmapDC.m_needTime = false;
}

JNIEXPORT void JNICALL Java_org_cocos2dx_lib_CCImage_1richlabel_nativeSaveRealLength(JNIEnv* env, jclass clazz, jint length) {
	CLBitmapDC& bitmapDC = CLBitmapDC::sharedCLBitmapDC();
	bitmapDC.m_realLength = length;
}

JNIEXPORT void JNICALL Java_org_cocos2dx_lib_CCImage_1richlabel_nativeSaveNeedTime(JNIEnv* env, jclass clazz, jboolean needTime) {
	CLBitmapDC& bitmapDC = CLBitmapDC::sharedCLBitmapDC();
	bitmapDC.m_needTime = needTime;
}

JNIEXPORT jstring JNICALL Java_org_cocos2dx_lib_CCImage_1richlabel_nativeFullPathForFilename(JNIEnv* env, jclass clazz, jstring filename) {
	string fn = JniHelper::jstring2string(filename);
	string path = CCUtils::getExternalOrFullPath(fn);
	return env->NewStringUTF(path.c_str());
}

JNIEXPORT void JNICALL Java_org_cocos2dx_lib_CCImage_1richlabel_nativeGetSpriteFrameInfo
  (JNIEnv * env, jclass clazz, jstring jPlist, jstring jAtlas, jstring jImageName, jobject jFrame) {
	// get c string
	const char* plist = (const char*)env->GetStringUTFChars(jPlist, NULL);
	const char* atlas = (const char*)env->GetStringUTFChars(jAtlas, NULL);
	const char* imageName = (const char*)env->GetStringUTFChars(jImageName, NULL);

	// get sprite frame
	CLBitmapDC& bitmapDC = CLBitmapDC::sharedCLBitmapDC();
	CCSpriteFrameCache* fc = CCSpriteFrameCache::sharedSpriteFrameCache();
	if(!fc->spriteFrameByName(imageName)) {
		// get atlas/plist full path, external path first
		string atlasPath = CCUtils::getExternalOrFullPath(atlas);
		string plistPath = CCUtils::getExternalOrFullPath(plist);

		// need to be decrypted or not
		if(bitmapDC.m_decryptFunc) {
			// load encryptd data
			unsigned long len;
			char* data = (char*)CCFileUtils::sharedFileUtils()->getFileData(atlasPath.c_str(), "rb", &len);

			// create texture
			int decLen;
			const char* dec = (*bitmapDC.m_decryptFunc)(data, (int)len, &decLen);
			CCImage* image = new CCImage();
			image->initWithImageData((void*)dec, decLen);
			image->autorelease();
			CCTexture2D* tex = CCTextureCache::sharedTextureCache()->addUIImage(image, atlasPath.c_str());

			// add
			fc->addSpriteFramesWithFile(plistPath.c_str(), tex);

			// free
			if(data != dec)
				free((void*)dec);
			free(data);
		} else {
			fc->addSpriteFramesWithFile(plistPath.c_str(), atlasPath.c_str());
		}
	}
	CCSpriteFrame* frame = fc->spriteFrameByName(imageName);

	// get java frame info
	jclass frameClass = env->FindClass("org/cocos2dx/lib/CCImage_richlabel$AtlasFrame");
	jfieldID fid_x = env->GetFieldID(frameClass, "x", "I");
	jfieldID fid_y = env->GetFieldID(frameClass, "y", "I");
	jfieldID fid_w = env->GetFieldID(frameClass, "w", "I");
	jfieldID fid_h = env->GetFieldID(frameClass, "h", "I");
	jfieldID fid_offsetX = env->GetFieldID(frameClass, "offsetX", "I");
	jfieldID fid_offsetY = env->GetFieldID(frameClass, "offsetY", "I");
	jfieldID fid_sourceWidth = env->GetFieldID(frameClass, "sourceWidth", "I");
	jfieldID fid_sourceHeight = env->GetFieldID(frameClass, "sourceHeight", "I");
	jfieldID fid_rotated = env->GetFieldID(frameClass, "rotated", "Z");

	// copy frame info to java object
	const CCSize& sourceSize = frame->getOriginalSizeInPixels();
	bool rotated = frame->isRotated();
	const CCRect& frameRect = frame->getRectInPixels();
	const CCPoint& offset = frame->getOffsetInPixels();
	env->SetIntField(jFrame, fid_x, (int)frameRect.origin.x);
	env->SetIntField(jFrame, fid_y, (int)frameRect.origin.y);
	env->SetIntField(jFrame, fid_w, (int)frameRect.size.width);
	env->SetIntField(jFrame, fid_h, (int)frameRect.size.height);
	env->SetIntField(jFrame, fid_offsetX, (int)offset.x);
	env->SetIntField(jFrame, fid_offsetY, (int)offset.y);
	env->SetIntField(jFrame, fid_sourceWidth, (int)sourceSize.width);
	env->SetIntField(jFrame, fid_sourceHeight, (int)sourceSize.height);
	env->SetBooleanField(jFrame, fid_rotated, rotated);

	// release
	env->DeleteLocalRef(frameClass);
	env->ReleaseStringUTFChars(jPlist, plist);
	env->ReleaseStringUTFChars(jAtlas, atlas);
	env->ReleaseStringUTFChars(jImageName, imageName);
}

JNIEXPORT jbyteArray JNICALL Java_org_cocos2dx_lib_CCImage_1richlabel_nativeDecryptData(JNIEnv* env, jclass clazz, jbyteArray jInput) {
	CLBitmapDC& bitmapDC = CLBitmapDC::sharedCLBitmapDC();
	if(bitmapDC.m_decryptFunc) {
		// get c array
		const char* input = (const char*)env->GetByteArrayElements(jInput, NULL);

		// decrypt
		int decLen;
		const char* dec = (*bitmapDC.m_decryptFunc)(input, (int)env->GetArrayLength(jInput), &decLen);

		// create new array
		jbyteArray ret = env->NewByteArray(decLen);
		env->SetByteArrayRegion(ret, 0, decLen, (const jbyte*)dec);

		// free
		if(input != dec)
			free((void*)dec);
		env->ReleaseByteArrayElements(jInput, (jbyte*)input, JNI_ABORT);

		// return
		return ret;
	}

	// not encrypted? just return input
	return jInput;
}

#ifdef __cplusplus
}
#endif

#endif // #if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
