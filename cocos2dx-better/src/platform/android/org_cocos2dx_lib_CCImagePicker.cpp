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

JNIEXPORT void JNICALL Java_org_cocos2dx_lib_CCImagePicker_nativeOnImagePicked
  (JNIEnv * env, jclass clazz, jlong callback, jstring fullPath, jint w, jint h) {
	CCImagePickerCallback* c = (CCImagePickerCallback*)callback;
	if(c) {
		new DelayCallback(c, JniHelper::jstring2string(fullPath), w, h);
	}
}

JNIEXPORT void JNICALL Java_org_cocos2dx_lib_CCImagePicker_nativeOnImagePickingCancelled
  (JNIEnv * env, jclass clazz, jlong callback) {
	CCImagePickerCallback* c = (CCImagePickerCallback*)callback;
	if(c) {
		new DelayCallback(c);
	}
}

#ifdef __cplusplus
}
#endif

#endif // #if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID