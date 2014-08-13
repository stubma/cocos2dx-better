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
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC

#import "CCUtils.h"
#import "sys/utsname.h"
#include <net/if.h>
#include <net/if_dl.h>
#import <StoreKit/StoreKit.h>
#import "EAGLView.h"
#include "CCLocale.h"
#include <sys/sysctl.h>

// built-in strings
#define S_CANCEL_EN "Cancel"
#define S_CANCEL_ZH "取消"
#define S_OK_EN "OK"
#define S_OK_ZH "确定"

#pragma mark -
#pragma mark CCUtils implementation on iOS

NS_CC_BEGIN

void CCUtils::openUrl(const string& url) {
    // TODO not implemented
}

void CCUtils::openAppInStore(const string& appId) {
    // TODO not implemented
}

void CCUtils::showSystemConfirmDialog(const char* title, const char* msg, const char* positiveButton, const char* negativeButton, CCCallFunc* onOK, CCCallFunc* onCancel) {
	// TODO not implemented
}

void CCUtils::stopInternalMusic() {
    // TODO not implemented
}

void CCUtils::playInternalMusic() {
    // TODO not implemented
}

bool CCUtils::isInternalMusicPlaying() {
    // TODO not implemented
    return false;
}

void CCUtils::purgeDefaultForKey(const string& key) {
    NSString* nsKey = [NSString stringWithCString:key.c_str() encoding:NSUTF8StringEncoding];
    [[NSUserDefaults standardUserDefaults] removeObjectForKey:nsKey];
}

int CCUtils::getCpuHz() {
    // TODO not implemented
    return 0;
}

bool CCUtils::verifySignature(void* validSign, size_t len) {
    return true;
}

bool CCUtils::isDebugSignature() {
    return false;
}

bool CCUtils::hasExternalStorage() {
    return true;
}

string CCUtils::getInternalStoragePath() {
    NSString* docDir = @"~/Documents";
    docDir = [docDir stringByExpandingTildeInPath];
    return [docDir cStringUsingEncoding:NSUTF8StringEncoding];
}

string CCUtils::getPackageName() {
	NSBundle* bundle = [NSBundle mainBundle];
    NSString* bundleId = [bundle bundleIdentifier];
    return [bundleId cStringUsingEncoding:NSUTF8StringEncoding];
}

bool CCUtils::isPathExistent(const string& path) {
	// if path is empty, directly return
	if(path.empty())
		return false;
	
	NSString* nsPath = [NSString stringWithFormat:@"%s", path.c_str()];
	return [[NSFileManager defaultManager] fileExistsAtPath:nsPath];
}

bool CCUtils::createFolder(const string& path) {
	NSString* nsPath = [NSString stringWithFormat:@"%s", path.c_str()];
	NSFileManager* fm = [NSFileManager defaultManager];
	return [fm createDirectoryAtPath:nsPath withIntermediateDirectories:YES attributes:NULL error:NULL];
}

string CCUtils::externalize(const string& path) {
	if(!CCFileUtils::sharedFileUtils()->isAbsolutePath(path)) {
		NSString* nsPath = [NSString stringWithFormat:@"~/Documents/%s", path.c_str()];
		nsPath = [nsPath stringByExpandingTildeInPath];
		return [nsPath cStringUsingEncoding:NSUTF8StringEncoding];
	} else {
        return path;
	}
}

bool CCUtils::deleteFile(const string& path) {
	NSString* p = [NSString stringWithFormat:@"%s", path.c_str()];
	NSFileManager* fm = [NSFileManager defaultManager];
	NSError* error = nil;
	[fm removeItemAtPath:p error:&error];
	return error == nil;
}

string CCUtils::getAppVersion() {
    NSBundle* bundle = [NSBundle mainBundle];
    NSString* ver = [bundle objectForInfoDictionaryKey:@"CFBundleVersion"];
    return [ver cStringUsingEncoding:NSUTF8StringEncoding];
}

string CCUtils::getDeviceType() {
    struct utsname systemInfo;
    uname(&systemInfo);
    char buf[1024];
    sprintf(buf, "%s", systemInfo.machine);
    return buf;
}

string CCUtils::getMacAddress() {
    string mac = "00:00:00:00:00:00";
    char* buf = NULL;
    do {
        // query en0
        int mib[] = {
            CTL_NET,
            AF_ROUTE,
            0,
            AF_LINK,
            NET_RT_IFLIST,
            0
        };
        if ((mib[5] = if_nametoindex("en0")) == 0) {
            break;
        }
        
        // get length of interface struct
        size_t len;
        if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0) {
            break;
        }
        
        // allocate buffer
        if ((buf = (char*)malloc(len * sizeof(char))) == NULL) {
            break;
        }
        
        // get struct
        if (sysctl(mib, 6, buf, &len, NULL, 0) < 0) {
            break;
        }
        struct if_msghdr* ifm = (struct if_msghdr*)buf;
        struct sockaddr_dl* sdl = (struct sockaddr_dl*)(ifm + 1);
        unsigned char* ptr = (unsigned char*)LLADDR(sdl);
        
        // construct mac string
        char buf2[64];
        sprintf(buf2, "%02x:%02x:%02x:%02x:%02x:%02x",
                *ptr & 0xff,
                *(ptr + 1) & 0xff,
                *(ptr + 2) & 0xff,
                *(ptr + 3) & 0xff,
                *(ptr + 4) & 0xff,
                *(ptr + 5) & 0xff);
        mac = buf2;
    } while(0);
    
    // free buf
    CC_SAFE_FREE(buf);
    
    // return
    return mac;
}

int CCUtils::getSystemVersionInt() {
    return 0;
}

NS_CC_END

#endif // #if CC_TARGET_PLATFORM == CC_PLATFORM_MAC