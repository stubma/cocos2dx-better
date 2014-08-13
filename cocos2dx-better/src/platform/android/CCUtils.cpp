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

#include "CCUtils.h"
#include <jni.h>
#include "JniHelper.h"
#include "CCLocale.h"
#include "CCMD5.h"
#include "CCMoreMacros.h"

bool CCUtils::deleteFile(const string& path) {
	return unlink(path.c_str()) == 0;
}

bool CCUtils::createFolder(const string& path) {
	return mkdir(path.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) == 0;
}

bool CCUtils::isPathExistent(const string& path) {
	// if path is empty, directly return
	if(path.empty())
		return false;

	return access(path.c_str(), 0) == 0;
}

string CCUtils::externalize(const string& path) {
    if(!CCFileUtils::sharedFileUtils()->isAbsolutePath(path)) {
        string internalStorage = getInternalStoragePath();
        return internalStorage + path;
    } else {
        return path;
    }
}

string CCUtils::getPackageName() {
	// get context
    jobject ctx = getContext();

    // get package manager
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Context", "getPackageManager", "()Landroid/content/pm/PackageManager;");
	jobject packageManager = t.env->CallObjectMethod(ctx, t.methodID);

	// get package name
    JniHelper::getMethodInfo(t, "android/content/Context", "getPackageName", "()Ljava/lang/String;");
	jstring packageName = (jstring)t.env->CallObjectMethod(ctx, t.methodID);

	// get c++ string
	const char* cpn = (const char*)t.env->GetStringUTFChars(packageName, NULL);
	string pn = cpn;

	// release
	t.env->ReleaseStringUTFChars(packageName, cpn);

	// return
	return pn;
}

string CCUtils::getInternalStoragePath() {
    // get context
    jobject ctx = getContext();

    // get file dir
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Context", "getFilesDir", "()Ljava/io/File;");
    jobject file = t.env->CallObjectMethod(ctx, t.methodID);

    // get absolute path
    JniHelper::getMethodInfo(t, "java/io/File", "getAbsolutePath", "()Ljava/lang/String;");
    jstring path = (jstring)t.env->CallObjectMethod(file, t.methodID);

    // get c++ string
    string cppPath = path ? JniHelper::jstring2string(path) : "/";

    // release
    t.env->DeleteLocalRef(path);
    t.env->DeleteLocalRef(file);
    t.env->DeleteLocalRef(ctx);

    // return
    return cppPath;
}

bool CCUtils::hasExternalStorage() {
    // get state
    JniMethodInfo t;
    JniHelper::getStaticMethodInfo(t, "android/os/Environment", "getExternalStorageState", "()Ljava/lang/String;");
    jobject jState = t.env->CallStaticObjectMethod(t.classID, t.methodID);

    // get mount state
    jfieldID fid = t.env->GetStaticFieldID(t.classID, "MEDIA_MOUNTED", "Ljava/lang/String;");
    jstring jMounted = (jstring)t.env->GetStaticObjectField(t.classID, fid);

    // is same?
    JniHelper::getMethodInfo(t, "java/lang/Object", "equals", "(Ljava/lang/Object;)Z");
    return t.env->CallBooleanMethod(jMounted, t.methodID, jState);
}

int64_t CCUtils::getAvailableStorageSize() {
    // actually we returned internal storage size
    string path = getInternalStoragePath();
    
    // get statfs
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/os/StatFs", "<init>", "(Ljava/lang/String;)V");
    jstring jPath = t.env->NewStringUTF(path.c_str());
    jobject statfs = t.env->NewObject(t.classID, t.methodID, jPath);
    
    // get block size
    JniHelper::getMethodInfo(t, "android/os/StatFs", "getBlockSize", "()I");
    jint blockSize = t.env->CallIntMethod(statfs, t.methodID);
    
    // get available block count
    JniHelper::getMethodInfo(t, "android/os/StatFs", "getAvailableBlocks", "()I");
    jint blocks = t.env->CallIntMethod(statfs, t.methodID);
    
    // release
    t.env->DeleteLocalRef(jPath);
    
    // return
    return blockSize * blocks;
}

static jbyteArray getFirstSignatureBytes() {
    // get context
    jobject ctx = CCUtils::getContext();

    // get package manager
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Context", "getPackageManager", "()Landroid/content/pm/PackageManager;");
	jobject packageManager = t.env->CallObjectMethod(ctx, t.methodID);

    // get package name
    JniHelper::getMethodInfo(t, "android/content/Context", "getPackageName", "()Ljava/lang/String;");
	jstring packageName = (jstring)t.env->CallObjectMethod(ctx, t.methodID);

    // get package info
    JniHelper::getMethodInfo(t, "android/content/pm/PackageManager", "getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
	jint flags = t.env->GetStaticIntField(t.classID, t.env->GetStaticFieldID(t.classID, "GET_SIGNATURES", "I"));
	jobject packageInfo = t.env->CallObjectMethod(packageManager, t.methodID, packageName, flags);

    // get first signature java object
	jclass klazz = t.env->GetObjectClass(packageInfo);
	jobjectArray signatures = (jobjectArray)t.env->GetObjectField(packageInfo,
                                                                  t.env->GetFieldID(klazz, "signatures", "[Landroid/content/pm/Signature;"));
	jobject signature = t.env->GetObjectArrayElement(signatures, 0);

    // get byte array of signature
	klazz = t.env->GetObjectClass(signature);
    t.methodID = t.env->GetMethodID(klazz, "toByteArray", "()[B");
	jbyteArray certificate = (jbyteArray)t.env->CallObjectMethod(signature, t.methodID);

    // release
    t.env->DeleteLocalRef(ctx);
    t.env->DeleteLocalRef(packageManager);
    t.env->DeleteLocalRef(packageName);
    t.env->DeleteLocalRef(packageInfo);
    t.env->DeleteLocalRef(signatures);
    t.env->DeleteLocalRef(signature);

    // return
    return certificate;
}

bool CCUtils::isDebugSignature() {
	// get sig data
    jbyteArray certificate = getFirstSignatureBytes();

    // create input stream
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "java/io/ByteArrayInputStream", "<init>", "([B)V");
    jobject bais = t.env->NewObject(t.classID, t.methodID, certificate);

    // cert factory
    JniHelper::getStaticMethodInfo(t, "java/security/cert/CertificateFactory", "getInstance", "(Ljava/lang/String;)Ljava/security/cert/CertificateFactory;");
    jstring protocol = t.env->NewStringUTF("X.509");
    jobject cf = t.env->CallStaticObjectMethod(t.classID, t.methodID, protocol);

    // cert
    JniHelper::getMethodInfo(t, "java/security/cert/CertificateFactory", "generateCertificate", "(Ljava/io/InputStream;)Ljava/security/cert/Certificate;");
    jobject cert = t.env->CallObjectMethod(cf, t.methodID, bais);

    // issuer dn
    JniHelper::getMethodInfo(t, "java/security/cert/X509Certificate", "getIssuerDN", "()Ljava/security/Principal;");
    jobject ip = t.env->CallObjectMethod(cert, t.methodID);

    // issuer dn name
    JniHelper::getMethodInfo(t, "java/security/Principal", "getName", "()Ljava/lang/String;");
    jstring ipn = (jstring)t.env->CallObjectMethod(ip, t.methodID);

    // check issuer dn name
    bool debug = false;
    string sub = "Android Debug";
    string cppipn = JniHelper::jstring2string(ipn);
    if(cppipn.find(sub) != string::npos) {
        debug = true;
    }

    // check more?
    if(!debug) {
        // subject dn
        JniHelper::getMethodInfo(t, "java/security/cert/X509Certificate", "getSubjectDN", "()Ljava/security/Principal;");
        jobject sp = t.env->CallObjectMethod(cert, t.methodID);

        // subject dn name
        JniHelper::getMethodInfo(t, "java/security/Principal", "getName", "()Ljava/lang/String;");
        jstring spn = (jstring)t.env->CallObjectMethod(sp, t.methodID);

        // check
        string cppspn = JniHelper::jstring2string(spn);
        if(cppspn.find(sub) != string::npos) {
            debug = true;
        }

        // release
        t.env->DeleteLocalRef(sp);
        t.env->DeleteLocalRef(spn);
    }

    // release
    t.env->DeleteLocalRef(bais);
    t.env->DeleteLocalRef(certificate);
    t.env->DeleteLocalRef(cf);
    t.env->DeleteLocalRef(protocol);
    t.env->DeleteLocalRef(cert);
    t.env->DeleteLocalRef(ip);
    t.env->DeleteLocalRef(ipn);

    // return
    return debug;
}

bool CCUtils::verifySignature(void* validSign, size_t len) {
	// basic check
	if(!validSign)
		return true;

	// get sig data
    jbyteArray certificate = getFirstSignatureBytes();

	// md5
    JNIEnv* env = getJNIEnv();
	bool valid = true;
	jsize cLen = env->GetArrayLength(certificate);
	jbyte* cData = env->GetByteArrayElements(certificate, JNI_FALSE);
	if (cLen > 0) {
		const char* md5 = CCMD5::md5(cData, cLen);
		size_t md5Len = strlen(md5);
		if(md5Len != len) {
			valid = false;
		} else {
			char* p = (char*)validSign;
			for(size_t i = 0; i < md5Len; i++) {
				if(md5[i] != p[i]) {
					valid = false;
					break;
				}
			}
		}
	}

	// release
	env->ReleaseByteArrayElements(certificate, cData, 0);
    env->DeleteLocalRef(certificate);

	// return
	return valid;
}

int CCUtils::getCpuHz() {
    // get package manager
    JniMethodInfo t;
    JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/CCUtils", "getCPUFrequencyMax", "()I");
	return t.env->CallStaticIntMethod(t.classID, t.methodID);
}

void CCUtils::purgeDefaultForKey(const string& key) {
    // context
    jobject ctx = getContext();

    // preference
	// cocos2d-x doesn't use default preference name, so we have to get pref name
	JniMethodInfo t;
    JniHelper::getMethodInfo(t,
							 "android/content/Context",
							 "getSharedPreferences",
							 "(Ljava/lang/String;I)Landroid/content/SharedPreferences;");
	jclass clazz = t.env->FindClass("org/cocos2dx/lib/Cocos2dxHelper");
	jfieldID fid = t.env->GetStaticFieldID(clazz, "PREFS_NAME", "Ljava/lang/String;");
	jstring pn = (jstring)t.env->GetStaticObjectField(clazz, fid);
	jobject pref = t.env->CallObjectMethod(ctx, t.methodID, pn, 0);

    // editor
    JniHelper::getMethodInfo(t,
                             "android/content/SharedPreferences",
                             "edit",
                             "()Landroid/content/SharedPreferences$Editor;");
    jobject edit = t.env->CallObjectMethod(pref, t.methodID);

    // remove
    jstring jKey = t.env->NewStringUTF(key.c_str());
    JniHelper::getMethodInfo(t,
                             "android/content/SharedPreferences$Editor",
                             "remove",
                             "(Ljava/lang/String;)Landroid/content/SharedPreferences$Editor;");
    t.env->CallObjectMethod(edit, t.methodID, jKey);

    // commit
    JniHelper::getMethodInfo(t,
                             "android/content/SharedPreferences$Editor",
                             "commit",
                             "()Z");
    t.env->CallBooleanMethod(edit, t.methodID);

    // release
	t.env->DeleteLocalRef(clazz);
	t.env->DeleteLocalRef(pn);
    t.env->DeleteLocalRef(jKey);
    t.env->DeleteLocalRef(ctx);
    t.env->DeleteLocalRef(pref);
    t.env->DeleteLocalRef(edit);
}

bool CCUtils::isInternalMusicPlaying() {
    return false;
}

void CCUtils::playInternalMusic() {
    // TODO not easy on Android
}

void CCUtils::stopInternalMusic() {
    // TODO not easy on Android
}

void CCUtils::openUrl(const string& url) {
    JniMethodInfo t;
    JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/CCUtils", "openUrl", "(Ljava/lang/String;)V");
    jstring jUrl = t.env->NewStringUTF(url.c_str());
	t.env->CallStaticVoidMethod(t.classID, t.methodID, jUrl);
}

void CCUtils::openAppInStore(const string& appId) {
    JniMethodInfo t;
    JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/CCUtils", "openAppInStore", "()V");
	t.env->CallStaticVoidMethod(t.classID, t.methodID);
}

void CCUtils::showSystemConfirmDialog(const char* title, const char* msg, const char* positiveButton, const char* negativeButton, CCCallFunc* onOK, CCCallFunc* onCancel) {
    // find method
    JniMethodInfo t;
    JniHelper::getStaticMethodInfo(t,
                                   "org/cocos2dx/lib/CCUtils",
                                   "showConfirmDialog",
                                   "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;JJ)V");

	// create jstring
	jstring jTitle = title ? t.env->NewStringUTF(title) : NULL;
	jstring jMsg = msg ? t.env->NewStringUTF(msg) : NULL;
	jstring jPositiveButton = positiveButton ? t.env->NewStringUTF(positiveButton) : NULL;
	jstring jNegativeButton = negativeButton ? t.env->NewStringUTF(negativeButton) : NULL;

	// call java side
    t.env->CallStaticVoidMethod(t.classID, t.methodID, jTitle, jMsg, jPositiveButton, jNegativeButton, (jlong)onOK, (jlong)onCancel);

	// delete reference
	if(jTitle)
		t.env->DeleteLocalRef(jTitle);
	if(jMsg)
		t.env->DeleteLocalRef(jMsg);
    if(jPositiveButton)
        t.env->DeleteLocalRef(jPositiveButton);
    if(jNegativeButton)
        t.env->DeleteLocalRef(jNegativeButton);
}

JNIEnv* CCUtils::getJNIEnv() {
	JavaVM* vm = JniHelper::getJavaVM();
	JNIEnv* env = NULL;
    if(vm) {
        jint ret = vm->GetEnv((void**) &env, JNI_VERSION_1_4);
        if (ret != JNI_OK) {
        	jint status = vm->AttachCurrentThread(&env, NULL);
        	if(status < 0) {
        	    CCLOGERROR("getJNIEnv: failed to attach current thread");
        	    env = NULL;
            }
        }
    }

	return env;
}

jobject CCUtils::getContext() {
    JniMethodInfo t;
    JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxActivity", "getContext", "()Landroid/content/Context;");
    jobject ctx = t.env->CallStaticObjectMethod(t.classID, t.methodID);
    return ctx;
}

jobject CCUtils::newIntent(const char* activityName) {
    // get context
    jobject context = getContext();

    // find constructor
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Intent", "<init>", "(Landroid/content/Context;Ljava/lang/Class;)V");

    // create activity name
    size_t len = strlen(activityName);
    char* jniName = (char*)calloc(len + 1, sizeof(char));
    for(int i = 0; i < len; i++) {
        if(activityName[i] == '.')
            jniName[i] = '/';
        else
            jniName[i] = activityName[i];
    }
    jclass actClass = t.env->FindClass(jniName);

    // new intent
    jobject intent = t.env->NewObject(t.classID, t.methodID, context, actClass);

    // clear
    t.env->DeleteLocalRef(actClass);
    t.env->DeleteLocalRef(context);
    CC_SAFE_FREE(jniName);

    return intent;
}

jobject CCUtils::newIntentByAction(const char* action) {
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Intent", "<init>", "(Ljava/lang/String;)V");
    jstring a = t.env->NewStringUTF(action);
    jobject intent = t.env->NewObject(t.classID, t.methodID, a);
    t.env->DeleteLocalRef(a);
    return intent;
}

void CCUtils::setIntentUri(jobject intent, const char* uri) {
    JniMethodInfo t;
    JniHelper::getStaticMethodInfo(t, "android/net/Uri", "parse", "(Ljava/lang/String;)Landroid/net/Uri;");
    jstring jUriStr = t.env->NewStringUTF(uri);
    jobject jUri = t.env->CallStaticObjectMethod(t.classID, t.methodID, jUriStr);
    JniHelper::getMethodInfo(t, "android/content/Intent", "setData", "(Landroid/net/Uri;)Landroid/content/Intent;");
    t.env->CallObjectMethod(intent, t.methodID, jUri);
    t.env->DeleteLocalRef(jUri);
    t.env->DeleteLocalRef(jUriStr);
}

void CCUtils::putBooleanExtra(jobject intent, const char* name, bool value) {
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Intent", "putExtra", "(Ljava/lang/String;Z)Landroid/content/Intent;");
    jstring s = t.env->NewStringUTF(name);
    t.env->CallObjectMethod(intent, t.methodID, s, value);
    t.env->DeleteLocalRef(s);
}

void CCUtils::putByteExtra(jobject intent, const char* name, unsigned char value) {
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Intent", "putExtra", "(Ljava/lang/String;B)Landroid/content/Intent;");
    jstring s = t.env->NewStringUTF(name);
    t.env->CallObjectMethod(intent, t.methodID, s, value);
    t.env->DeleteLocalRef(s);
}

void CCUtils::putCharExtra(jobject intent, const char* name, unsigned short value) {
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Intent", "putExtra", "(Ljava/lang/String;C)Landroid/content/Intent;");
    jstring s = t.env->NewStringUTF(name);
    t.env->CallObjectMethod(intent, t.methodID, s, value);
    t.env->DeleteLocalRef(s);
}

void CCUtils::putDoubleExtra(jobject intent, const char* name, double value) {
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Intent", "putExtra", "(Ljava/lang/String;D)Landroid/content/Intent;");
    jstring s = t.env->NewStringUTF(name);
    t.env->CallObjectMethod(intent, t.methodID, s, value);
    t.env->DeleteLocalRef(s);
}

void CCUtils::putFloatExtra(jobject intent, const char* name, float value) {
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Intent", "putExtra", "(Ljava/lang/String;F)Landroid/content/Intent;");
    jstring s = t.env->NewStringUTF(name);
    t.env->CallObjectMethod(intent, t.methodID, s, value);
    t.env->DeleteLocalRef(s);
}

void CCUtils::putIntExtra(jobject intent, const char* name, int value) {
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Intent", "putExtra", "(Ljava/lang/String;I)Landroid/content/Intent;");
    jstring s = t.env->NewStringUTF(name);
    t.env->CallObjectMethod(intent, t.methodID, s, value);
    t.env->DeleteLocalRef(s);
}

void CCUtils::putStringExtra(jobject intent, const char* name, const char* value) {
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Intent", "putExtra", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;");
    jstring s = t.env->NewStringUTF(name);
    t.env->CallObjectMethod(intent, t.methodID, s, value);
    t.env->DeleteLocalRef(s);
}

void CCUtils::putLongExtra(jobject intent, const char* name, long value) {
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Intent", "putExtra", "(Ljava/lang/String;J)Landroid/content/Intent;");
    jstring s = t.env->NewStringUTF(name);
    t.env->CallObjectMethod(intent, t.methodID, s, value);
    t.env->DeleteLocalRef(s);
}

void CCUtils::putShortExtra(jobject intent, const char* name, short value) {
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Intent", "putExtra", "(Ljava/lang/String;S)Landroid/content/Intent;");
    jstring s = t.env->NewStringUTF(name);
    t.env->CallObjectMethod(intent, t.methodID, s, value);
    t.env->DeleteLocalRef(s);
}

void CCUtils::putParcelableExtra(jobject intent, const char* name, jobject value) {
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Intent", "putExtra", "(Ljava/lang/String;Landroid/os/Parcelable;)Landroid/content/Intent;");
    jstring s = t.env->NewStringUTF(name);
    t.env->CallObjectMethod(intent, t.methodID, s, value);
    t.env->DeleteLocalRef(s);
}

void CCUtils::startActivity(jobject intent) {
    jobject ctx = getContext();
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Context", "startActivity", "(Landroid/content/Intent;)V");
    t.env->CallVoidMethod(ctx, t.methodID, intent);
    t.env->DeleteLocalRef(ctx);
}

void CCUtils::sendBroadcast(jobject intent) {
    jobject ctx = getContext();
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Context", "sendBroadcast", "(Landroid/content/Intent;)V");
    t.env->CallVoidMethod(ctx, t.methodID, intent);
    t.env->DeleteLocalRef(ctx);
}

string CCUtils::getAppVersion() {
    // get context
    jobject ctx = getContext();
    
    // get package manager
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Context", "getPackageManager", "()Landroid/content/pm/PackageManager;");
	jobject packageManager = t.env->CallObjectMethod(ctx, t.methodID);
    
    // get package info
    JniHelper::getMethodInfo(t, "android/content/pm/PackageManager", "getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    string pn = getPackageName();
    jstring jPN = t.env->NewStringUTF(pn.c_str());
	jobject packageInfo = t.env->CallObjectMethod(packageManager, t.methodID, jPN, 0);
    
    // get version
    jclass piClass = t.env->GetObjectClass(packageInfo);
    jstring jVer = (jstring)t.env->GetObjectField(packageInfo, t.env->GetFieldID(piClass, "versionName", "Ljava/lang/String;"));
    string ver = JniHelper::jstring2string(jVer);
                                           
    // release
    t.env->DeleteLocalRef(ctx);
    t.env->DeleteLocalRef(packageManager);
    t.env->DeleteLocalRef(packageInfo);
    t.env->DeleteLocalRef(piClass);
    t.env->DeleteLocalRef(jVer);
    t.env->DeleteLocalRef(jPN);
    
    return ver;
}

string CCUtils::getDeviceType() {
	JniMethodInfo t;
    JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/CCUtils", "getDeviceType", "()Ljava/lang/String;");
	jstring jDevice = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
	string device = JniHelper::jstring2string(jDevice);
	t.env->DeleteLocalRef(jDevice);
	return device;
}

string CCUtils::getMacAddress() {
	JniMethodInfo t;
    JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/CCUtils", "getMacAddress", "()Ljava/lang/String;");
	jstring jMac = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
	string mac = JniHelper::jstring2string(jMac);
	t.env->DeleteLocalRef(jMac);
	return mac;
}

int CCUtils::getSystemVersionInt() {
	JNIEnv* env = getJNIEnv();
	jclass clazz = env->FindClass("android/os/Build$VERSION");
	jfieldID fid = env->GetStaticFieldID(clazz, "SDK_INT", "I");
	int v = env->GetStaticIntField(clazz, fid);
	env->DeleteLocalRef(clazz);
	return v;
}

#endif // #if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
