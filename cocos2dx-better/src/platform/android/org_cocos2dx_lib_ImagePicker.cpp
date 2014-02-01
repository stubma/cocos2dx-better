#include <jni.h>
#include "cocos2d.h"
#include "cocos2d-better.h"
#include "platform/android/jni/JniHelper.h"

USING_NS_CC;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_org_cocos2dx_lib_ImagePicker_nativeOnImagePicked
  (JNIEnv * env, jclass clazz, jlong callback, jstring fullPath, jint w, jint h) {
	CCImagePickerCallback* c = (CCImagePickerCallback*)callback;
	if(c) {
		c->onImagePicked(JniHelper::jstring2string(fullPath), w, h);
	}
}

JNIEXPORT void JNICALL Java_org_cocos2dx_lib_ImagePicker_nativeOnImagePickingCancelled
  (JNIEnv * env, jclass clazz, jlong callback) {
	CCImagePickerCallback* c = (CCImagePickerCallback*)callback;
	if(c) {
		c->onImagePickingCancelled();
	}
}

#ifdef __cplusplus
}
#endif
