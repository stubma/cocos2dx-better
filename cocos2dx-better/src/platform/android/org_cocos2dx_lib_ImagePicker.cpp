#include <jni.h>
#include "cocos2d.h"
#include "cocos2d-better.h"
#include "platform/android/jni/JniHelper.h"

USING_NS_CC;
using namespace std;

/**
 * XXX: If we directly callback when image picker is done, the texture won't be created.
 * I can't figure out why, maybe the surface is not running so texture loading is failed.
 * This is a workaround for this issue, I delay the callback until game re-gains the
 * control. However, I have to define a stub CCObject to provide a schedule selector.
 */
class DelayCallback : public CCObject {
private:
	bool m_cancelled;
	CCImagePickerCallback* m_callback;
	string m_fullPath;
	int m_w;
	int m_h;

public:
	DelayCallback(CCImagePickerCallback* c) :
		m_cancelled(true),
		m_callback(c) {
		// set a very small delay so that it will be called next frame
		CCScheduler* s = CCDirector::sharedDirector()->getScheduler();
		s->scheduleSelector(schedule_selector(DelayCallback::delayCallback), this, 0, 0, 0.001f, false);
	}

	DelayCallback(CCImagePickerCallback* c, string fullPath, int w, int h) :
		m_cancelled(false),
		m_callback(c),
		m_fullPath(fullPath),
		m_w(w),
		m_h(h) {
		// set a very small delay so that it will be called next frame
		CCScheduler* s = CCDirector::sharedDirector()->getScheduler();
		s->scheduleSelector(schedule_selector(DelayCallback::delayCallback), this, 0, 0, 0.001f, false);
	}

	virtual ~DelayCallback() {}

	void delayCallback(float delta) {
		if(m_cancelled) {
			m_callback->onImagePickingCancelled();
		} else {
			m_callback->onImagePicked(m_fullPath, m_w, m_h);
		}

		// release self
		autorelease();
	}
};

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_org_cocos2dx_lib_ImagePicker_nativeOnImagePicked
  (JNIEnv * env, jclass clazz, jlong callback, jstring fullPath, jint w, jint h) {
	CCImagePickerCallback* c = (CCImagePickerCallback*)callback;
	if(c) {
		new DelayCallback(c, JniHelper::jstring2string(fullPath), w, h);
	}
}

JNIEXPORT void JNICALL Java_org_cocos2dx_lib_ImagePicker_nativeOnImagePickingCancelled
  (JNIEnv * env, jclass clazz, jlong callback) {
	CCImagePickerCallback* c = (CCImagePickerCallback*)callback;
	if(c) {
		new DelayCallback(c);
	}
}

#ifdef __cplusplus
}
#endif
