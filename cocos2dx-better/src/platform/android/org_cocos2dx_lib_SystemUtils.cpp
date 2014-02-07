#include <jni.h>
#include "cocos2d.h"
#include "cocos2d-better.h"
#include "platform/android/jni/JniHelper.h"

USING_NS_CC;
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_org_cocos2dx_lib_SystemUtils_nativeExecuteCallFunc
  (JNIEnv * env, jclass clazz, jlong func) {
	CCCallFunc* c = (CCCallFunc*)func;
	if(c) {
		c->execute();
	}
}

JNIEXPORT void JNICALL Java_org_cocos2dx_lib_SystemUtils_nativeRetain
  (JNIEnv * env, jclass clazz, jlong func) {
	CCObject* obj = (CCObject*)func;
	CC_SAFE_RETAIN(obj);
}

JNIEXPORT void JNICALL Java_org_cocos2dx_lib_SystemUtils_nativeRelease
  (JNIEnv * env, jclass clazz, jlong func) {
	CCObject* obj = (CCObject*)func;
	CC_SAFE_RELEASE(obj);
}

#ifdef __cplusplus
}
#endif
