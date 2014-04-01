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

#include <jni.h>
#include "cocos2d.h"
#include "cocos2d-better.h"
#include "platform/android/jni/JniHelper.h"

USING_NS_CC;
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_org_cocos2dx_lib_CCUtils_nativeExecuteCallFunc
  (JNIEnv * env, jclass clazz, jlong func) {
	CCCallFunc* c = (CCCallFunc*)func;
	if(c) {
		c->execute();
	}
}

JNIEXPORT void JNICALL Java_org_cocos2dx_lib_CCUtils_nativeRetain
  (JNIEnv * env, jclass clazz, jlong func) {
	CCObject* obj = (CCObject*)func;
	CC_SAFE_RETAIN(obj);
}

JNIEXPORT void JNICALL Java_org_cocos2dx_lib_CCUtils_nativeRelease
  (JNIEnv * env, jclass clazz, jlong func) {
	CCObject* obj = (CCObject*)func;
	CC_SAFE_RELEASE(obj);
}

#ifdef __cplusplus
}
#endif

#endif // #if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID