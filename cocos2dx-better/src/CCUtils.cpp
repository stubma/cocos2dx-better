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
#include "CCUtils.h"
#include "entities.h"
#include "CCLocale.h"
#include "CCMoreMacros.h"
#include "CCMD5.h"
#include "CCImage_richlabel.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    #include <sys/sysctl.h>
    #import <MediaPlayer/MediaPlayer.h>
    #import "CCSystemConfirmDialogDelegate.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    #include <sys/sysctl.h>
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	#include "JniHelper.h"
#endif

NS_CC_BEGIN

// built-in strings
#define S_CANCEL_EN "Cancel"
#define S_CANCEL_ZH "取消"
#define S_OK_EN "OK"
#define S_OK_ZH "确定"

CCUtils::StringList CCUtils::s_tmpStringList;
CCArray CCUtils::s_tmpArray;

unsigned char CCUtils::UnitScalarToByte(float x) {
    if (x < 0) {
        return 0;
    }
    if (x >= 1) {
        return 255;
    }
    return (int)(x * (1 << 16)) >> 8;
}

const char* CCUtils::copy(const char* src) {
	if(src == NULL)
		return NULL;
    
	size_t len = strlen(src);
	char* c = (char*)calloc(len + 1, sizeof(char));
	memcpy(c, src, len);
	return c;
}

const char* CCUtils::copy(const char* src, int start, size_t len) {
	if(src == NULL)
		return NULL;
    
	char* c = (char*)calloc(len + 1, sizeof(char));
	memcpy(c, src + start, len);
	return c;
}

void CCUtils::toLowercase(string& s) {
	if(s.empty())
		return;
	
    size_t len = s.length();
	char* buf = new char[len + 1];
	strcpy(buf, s.c_str());
    for(int i = 0; i < len; i++)
		if(buf[i] >= 0x41 && buf[i] <= 0x5A)
			buf[i] += 0x20;
	
	s.copy(buf, len);
	delete buf;
}

bool CCUtils::startsWith(const string& s, const string& sub) {
    return s.find(sub) == 0;
}

bool CCUtils::endsWith(const string& s, const string& sub) {
    return s.rfind(sub) == s.length() - sub.length();
}

void CCUtils::replaceChar(string& s, char c, char sub) {
    size_t len = s.length();
	char* buf = new char[len + 1];
    strcpy(buf, s.c_str());
    
	for(int i = 0; i < len; i++) {
		if(buf[i] == c) {
            buf[i] = sub;
        }
	}
    
    s.copy(buf, len);
    delete buf;
}

string CCUtils::decodeHtmlEntities(const string& src) {
	char* dest = (char*)calloc(src.length() + 1, sizeof(char));
	decode_html_entities_utf8(dest, src.c_str());
	string decoded = string(dest);
	free(dest);
	return decoded;
}

int CCUtils::getNumDigits(int num) {
    int d = 1;
    num /= 10;
    while(num > 0) {
        d++;
        num /= 10;
    }
    return d;
}

ssize_t CCUtils::lastDotIndex(const string& path) {
	if(path.empty())
		return -1;
    
	size_t len = path.length();
	for(int i = len - 1; i >= 0; i--) {
		if(path[i] == '.')
			return i;
	}
    
	return -1;
}

ssize_t CCUtils::lastSlashIndex(string path) {
	if(path.empty())
		return -1;
    
	// change slash to windows format
    if(CC_PATH_SEPARATOR != '/')
        replaceChar(path, '/', CC_PATH_SEPARATOR);
    
	// find slash index
	size_t len = path.length();
	int end = len;
	int slash = -1;
	for(int i = len - 1; i >= 0; i--) {
		if(path[i] == CC_PATH_SEPARATOR) {
			if(i == end - 1) {
				end--;
				if(i == 0) {
					slash = 0;
					break;
				}
			} else {
				slash = i;
				break;
			}
		}
	}
    
	// skip extra slash
	if(slash != -1) {
		while(slash >= 1 && path[slash - 1] == CC_PATH_SEPARATOR)
			slash--;
	}
    
	// assign to end
	end = slash;
	if(end == 0)
		end = 1;
    
	return end;
}

string CCUtils::lastPathComponent(const string& path) {
	// change slash to windows format
	if(CC_PATH_SEPARATOR != '/')
		replaceChar((string&)path, '/', CC_PATH_SEPARATOR);
    
	size_t len = path.length();
	int start = 0;
	int end = len;
	for(int i = len - 1; i >= 0; i--) {
		if(path[i] == CC_PATH_SEPARATOR) {
			if(i == end - 1)
				end--;
			else {
				start = i + 1;
				break;
			}
		}
	}
    
	if(end < start)
		return path;
	else
		return path.substr(start, end);
}

string CCUtils::deleteLastPathComponent(const string& path) {
	ssize_t end = lastSlashIndex(path);
	if(end < 0)
		return path;
	else
		return path.substr(0, end);
}

string CCUtils::appendPathComponent(const string& path, const string& component) {
	// change slash to windows format
	if(CC_PATH_SEPARATOR != '/')
		replaceChar((string&)path, '/', CC_PATH_SEPARATOR);
    
	// validating
	if(path.empty()) {
		if(component.empty())
			return "";
		else
			return component;
	} else if(component.empty()) {
		return path;
    }
    
	// allocate a big enough buffer
	// plus 2 because one for slash, one for null terminator
	size_t len = path.length();
	int cLen = component.length();
    char* buf = new char[len + cLen + 2];
    memset(buf, 0, len + cLen + 2);
    
	// copy path
	memcpy(buf, path.c_str(), len);
    
	// take care of slash
	int start = len;
	if(start > 0) {
		if(buf[start - 1] != CC_PATH_SEPARATOR) {
			buf[start++] = CC_PATH_SEPARATOR;
		} else {
			while(start >= 2 && buf[start - 2] == CC_PATH_SEPARATOR)
				start--;
		}
	}
    
	// copy component
	int cStart = 0;
	while(cStart < cLen && component[cStart] == CC_PATH_SEPARATOR)
		cStart++;
	if(cStart > 0 && start == 0)
		cStart--;
	memcpy(buf + start, component.c_str() + cStart, cLen - cStart);
    
    // remove end slash
    int end = start + cLen - cStart - 1;
    while(buf[end] == CC_PATH_SEPARATOR)
        buf[end--] = 0;
    
    string ret = buf;
    delete buf;
	return ret;
}

string CCUtils::deletePathExtension(const string& path) {
    ssize_t end = lastDotIndex(path);
	ssize_t slash = lastSlashIndex((string&)path);
	if(end >= 0) {
		if(end > slash)
			return path.substr(0, end);
		else
			return path;
	} else {
		return path;
	}
}

string CCUtils::getPathExtension(const string& path) {
	ssize_t end = lastDotIndex(path);
	if(end >= 0) {
		return path.substr(end);
	} else {
		return "";
	}
}

bool CCUtils::deleteFile(string path) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
	string mappedPath = mapLocalPath(path);
	NSString* p = [NSString stringWithFormat:@"%s", mappedPath.c_str()];
	NSFileManager* fm = [NSFileManager defaultManager];
	NSError* error = nil;
	[fm removeItemAtPath:p error:&error];
	return error == nil;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	return unlink(path.c_str()) == 0;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	string mappedPath = mapLocalPath(path);
	return DeleteFile(mappedPath.c_str()) != 0;
#else
	CCLOGERROR("CCUtils::mapLocalPath is not implemented for this platform, please finish it");
	return false;
#endif
}

string CCUtils::mapLocalPath(string path) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
	if(CCFileUtils::sharedFileUtils()->isAbsolutePath(path)) {
		NSString* nsPath = [NSString stringWithFormat:@"~/Documents/%s", path.c_str()];
		nsPath = [nsPath stringByExpandingTildeInPath];
		return [nsPath cStringUsingEncoding:NSUTF8StringEncoding];
	} else {
		NSBundle* bundle = [NSBundle mainBundle];
		NSString* relativePath = [NSString stringWithFormat:@"%s", path.c_str()];
		NSString* ext = [relativePath pathExtension];
		NSString* filename = [relativePath lastPathComponent];
		NSString* filenameWithoutExt = [filename stringByDeletingPathExtension];
		NSString* dir = [relativePath stringByDeletingLastPathComponent];
		NSString* path = [bundle pathForResource:filenameWithoutExt ofType:ext inDirectory:dir];
		return [path cStringUsingEncoding:NSUTF8StringEncoding];
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	return path;
#else
	CCLOGERROR("CCUtils::mapLocalPath is not implemented for this platform, please finish it");
#endif
}

string CCUtils::getParentPath(string path) {
	if(path.empty())
		return "";
	
	size_t slash = path.rfind('/');
	if(slash == string::npos)
		return "";
	else if(slash == 0)
		return path.substr(0, 1);
	else
		return path.substr(0, slash);
}

bool CCUtils::createIntermediateFolders(string path) {
	string parent = getParentPath(path);
	bool exist = isPathExistent(parent);
	bool success = true;
	if(!exist) {
		createIntermediateFolders(parent);
		success = createFolder(parent);
	}
	
	// return success flag
	return success;
}

bool CCUtils::createFolder(string path) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
	string mappedPath = mapLocalPath(path);
	NSString* nsPath = [NSString stringWithFormat:@"%s", mappedPath.c_str()];
	NSFileManager* fm = [NSFileManager defaultManager];
	return [fm createDirectoryAtPath:nsPath withIntermediateDirectories:YES attributes:NULL error:NULL];
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	return mkdir(path.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) == 0;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	string mappedPath = mapLocalPath(path);
	return CreateDirectory(mappedPath.c_str(), NULL) != 0;
#else
	CCLOGERROR("CCUtils::mapLocalPath is not implemented for this platform, please finish it");
	return false;
#endif
}

bool CCUtils::isPathExistent(string path) {
	// if path is empty, directly return
	if(path.empty())
		return true;
	
	// get mapped path
	string mappedPath = mapLocalPath(path);
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
	NSString* nsPath = [NSString stringWithFormat:@"%s", mappedPath.c_str()];
	return [[NSFileManager defaultManager] fileExistsAtPath:nsPath];
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	return access(mappedPath.c_str(), 0) == 0;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	DWORD dwAttrib = GetFileAttributes((LPCTSTR)mappedPath.c_str());
	return dwAttrib != INVALID_FILE_ATTRIBUTES;
#else
	CCLOGERROR("CCUtils::mapLocalPath is not implemented for this platform, please finish it");
	return false;
#endif
}

string CCUtils::getPackageName() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
	NSBundle* bundle = [NSBundle mainBundle];
    NSString* bundleId = [bundle bundleIdentifier];
    return [bundleId cStringUsingEncoding:NSUTF8StringEncoding];
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
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
#else
	CCLOGERROR("CCUtils::getPackageName is not implemented for this platform, please finish it");
	return false;
#endif
}

ccColorHSV CCUtils::ccc32hsv(ccColor3B c) {
	unsigned char min = MIN(c.r, MIN(c.g, c.b));
    unsigned char max = MAX(c.r, MAX(c.g, c.b));
    unsigned char delta = max - min;
	
    // get v
    float v = max / 255.f;
	
    // if input color a gray color?
    if(delta == 0) {
    	return cchsv(0, 0, v);
    }
	
    // get s
    float s = (float)delta / max;
	
    // get h
    float h;
    if(c.r == max) {
        h = (float)(c.g - c.b) / delta;
    } else if (c.g == max) {
        h = 2 + (float)(c.b - c.r) / delta;
    } else { // b == max
        h = 4 + (float)(c.r - c.g) / delta;
    }
	
    // adjust h
    h *= 60;
    if (h < 0) {
        h += 360;
    }
	
    return cchsv(h, s, v);
}

ccColor3B CCUtils::hsv2ccc3(ccColorHSV c) {
	unsigned char s = UnitScalarToByte(c.s);
    unsigned char v = UnitScalarToByte(c.v);
	
    // if s is zero then rgb color is gray
    if (0 == s) {
    	return ccc3(v, v, v);
    }
	
    int hx = (c.h < 0 || c.h >= 360.f) ? 0 : (int)((c.h / 60) * (1 << 16));
    int f = hx & 0xFFFF;
	
    unsigned char v_scale = v + 1;
    unsigned char p = ((255 - s) * v_scale) >> 8;
    unsigned char q = ((255 - (s * f >> 16)) * v_scale) >> 8;
    unsigned char t = ((255 - (s * ((1 << 16) - f) >> 16)) * v_scale) >> 8;
	
    unsigned char r, g, b;
    switch (hx >> 16) {
        case 0:
        	r = v;
        	g = t;
        	b = p;
        	break;
        case 1:
        	r = q;
        	g = v;
        	b = p;
        	break;
        case 2:
        	r = p;
        	g = v;
        	b = t;
        	break;
        case 3:
        	r = p;
        	g = q;
        	b = v;
        	break;
        case 4:
        	r = t;
        	g = p;
        	b = v;
        	break;
        default:
        	r = v;
        	g = p;
        	b = q;
        	break;
    }
	
    return ccc3(r, g, b);
}

CCPoint CCUtils::getOrigin(CCNode* node) {
	if(node->isIgnoreAnchorPointForPosition()) {
		return node->getPosition();
	} else {
		return ccp(node->getPositionX() - node->getAnchorPointInPoints().x,
				   node->getPositionY() - node->getAnchorPointInPoints().y);
	}
}

CCPoint CCUtils::getCenter(CCNode* node) {
	if(node->isIgnoreAnchorPointForPosition()) {
		return ccpAdd(node->getPosition(), ccp(node->getContentSize().width / 2, node->getContentSize().height / 2));
	} else {
		return ccpAdd(ccp(node->getPositionX() - node->getAnchorPointInPoints().x,
						  node->getPositionY() - node->getAnchorPointInPoints().y),
					  ccp(node->getContentSize().width / 2, node->getContentSize().height / 2));
	}
}

CCPoint CCUtils::getLocalCenter(CCNode* node) {
    return ccp(node->getContentSize().width / 2,
               node->getContentSize().height / 2);
}

CCPoint CCUtils::getLocalPoint(CCNode* node, float xpercent, float ypercent) {
	CCSize s = node->getContentSize();
	return ccp(s.width * xpercent, s.height * ypercent);
}

CCPoint CCUtils::getLocalPoint(CCNode* node, CCPoint anchor) {
	CCSize s = node->getContentSize();
	return ccp(s.width * anchor.x, s.height * anchor.y);
}

CCPoint CCUtils::getPoint(CCNode* node, float xpercent, float ypercent) {
	CCPoint origin = getOrigin(node);
	CCSize size = node->getContentSize();
	return ccp(origin.x + size.width * xpercent, origin.y + size.height * ypercent);
}

CCRect CCUtils::getBoundingBoxInWorldSpace(CCNode* node) {
	CCRect r;
	r.origin = CCPointZero;
	r.size = node->getContentSize();
	CCAffineTransform t = node->nodeToWorldTransform();
	r = CCRectApplyAffineTransform(r, t);
	return r;
}

void CCUtils::setTreeOpacity(CCNode* n, int o) {
	// self
	CCRGBAProtocol* p = dynamic_cast<CCRGBAProtocol*>(n);
	if(p) {
		p->setOpacity((GLubyte)o);
	}
	
	// children
	CCArray* children = n->getChildren();
	CCObject* pObj = NULL;
	CCARRAY_FOREACH(children, pObj) {
		setTreeOpacity((CCNode*)pObj, o);
	}
}

CCScene* CCUtils::getScene(CCNode* n) {
    while(n) {
        CCScene* s = dynamic_cast<CCScene*>(n);
        if(s) {
            return s;
        } else {
            n = n->getParent();
        }
    }
    
    return NULL;
}

bool CCUtils::testSegmentAABB(CCPoint p0, CCPoint p1, ccAABB b) {
	CCPoint c = ccpMult(ccpAdd(b.min, b.max), 0.5f);
	CCPoint e = ccpSub(b.max, c);
	CCPoint m = ccpMult(ccpAdd(p0, p1), 0.5f);
	CCPoint d = ccpSub(p1, m);
	m = ccpSub(m, c);
	
	// Box center-point
	// Box halflength extents
	// Segment midpoint
	// Segment halflength vector
	// Translate box and segment to origin
	// Try world coordinate axes as separating axes
	float adx = fabs(d.x);
	if (fabs(m.x) > e.x + adx)
		return false;
	float ady = fabs(d.y);
	if (fabs(m.y) > e.y + ady)
		return false;
	
	// Add in an epsilon term to counteract arithmetic errors when segment is
	// (near) parallel to a coordinate axis (see text for detail)
	adx += FLT_EPSILON;
	ady += FLT_EPSILON;
	
	// Try cross products of segment direction vector with coordinate axes
	if (fabs(m.x * d.y - m.y * d.x) > e.x * ady + e.y * adx)
		return false;
	
	// No separating axis found; segment must be overlapping AABB
	return true;
}

int CCUtils::binarySearch(int* a, size_t len, int key) {
	int low = 0;
	int high = len - 1;
	
	while(low <= high) {
		int mid = (low + high) >> 1;
		int midVal = a[mid];
		
		if(midVal < key)
			low = mid + 1;
		else if(midVal > key)
			high = mid - 1;
		else
			return mid; // key found
	}
	return -(low + 1); // key not found.
}

CCRect CCUtils::combine(const CCRect& r1, const CCRect& r2) {
    float left = MIN(r1.origin.x, r2.origin.x);
	float right = MAX(r1.origin.x + r1.size.width, r2.origin.x + r2.size.width);
	float bottom = MIN(r1.origin.y, r2.origin.y);
	float top = MAX(r1.origin.y + r1.size.height, r2.origin.y + r2.size.height);
	return CCRectMake(left, bottom, right - left, top - bottom);
}

string CCUtils::getInternalStoragePath() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    NSString* docDir = @"~/Documents";
    docDir = [docDir stringByExpandingTildeInPath];
    return [docDir cStringUsingEncoding:NSUTF8StringEncoding];
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
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
#else
    CCLOGERROR("CCUtils::getInternalStoragePath is not implemented for this platform, please finish it");
#endif
}

bool CCUtils::hasExternalStorage() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    return true;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
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
#else
    CCLOGERROR("CCUtils::hasExternalStorage is not implemented for this platform, please finish it");
#endif
}

int64_t CCUtils::currentTimeMillis() {
	struct timeval tv;
	gettimeofday(&tv, (struct timezone *) NULL);
	int64_t when = tv.tv_sec * 1000LL + tv.tv_usec / 1000;
	return when;
}

bool CCUtils::isDebugSignature() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
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
#else
    return false;
#endif
}

bool CCUtils::verifySignature(void* validSign, size_t len) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
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
#else
    return true;
#endif
}

CCUtils::StringList& CCUtils::componentsOfString(const string& s, const char sep) {
    // remove head and tailing brace, bracket, parentheses
    int start = 0;
    int end = s.length() - 1;
    char c = s[start];
    while(c == '{' || c == '[' || c == '(') {
        start++;
        c = s[start];
    }
    c = s[end];
    while(c == '}' || c == ']' || c == ')') {
        end--;
        c = s[end];
    }
    
    // returned string list
    s_tmpStringList.clear();
    
    // iterate string
    int compStart = start;
    for(int i = start; i <= end; i++) {
        c = s[i];
        if(c == sep) {
            s_tmpStringList.push_back(s.substr(compStart, i - compStart));
            compStart = i + 1;
        } else if(c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            if(compStart == i) {
                compStart++;
            }
        }
    }
    
    // last comp
    if(compStart <= end) {
        s_tmpStringList.push_back(s.substr(compStart, end - compStart + 1));
    }
    
    // return
    return s_tmpStringList;
}

CCPoint CCUtils::ccpFromString(const string& s) {
    StringList comp = componentsOfString(s, ',');
    float x = 0, y = 0;
    if(comp.size() > 0) {
        x = atof(comp.at(0).c_str());
    }
    if(comp.size() > 1) {
        y = atof(comp.at(1).c_str());
    }
    return ccp(x, y);
}

CCSize CCUtils::ccsFromString(const string& s) {
    StringList comp = componentsOfString(s, ',');
    float x = 0, y = 0;
    if(comp.size() > 0) {
        x = atof(comp.at(0).c_str());
    }
    if(comp.size() > 1) {
        y = atof(comp.at(1).c_str());
    }
    return CCSizeMake(x, y);
}

CCRect CCUtils::ccrFromString(const string& s) {
    StringList comp = componentsOfString(s, ',');
    float x = 0, y = 0, w = 0, h = 0;
    if(comp.size() > 0) {
        x = atof(comp.at(0).c_str());
    }
    if(comp.size() > 1) {
        y = atof(comp.at(1).c_str());
    }
    if(comp.size() > 2) {
        w = atof(comp.at(2).c_str());
    }
    if(comp.size() > 3) {
        h = atof(comp.at(3).c_str());
    }
    return CCRectMake(x, y, w, h);
}

CCArray& CCUtils::arrayFromString(const string& s) {
    StringList comp = componentsOfString(s, ',');
    
    // clear
    s_tmpArray.removeAllObjects();
    
    // iterator components
    for(StringList::iterator iter = comp.begin(); iter != comp.end(); iter++) {
        string& cs = *iter;
        if(cs.length() > 0) {
            if(cs[0] == '\'' || cs[0] == '"') {
                int start = 1;
                int end = cs.length() - 1;
                if(cs[end] == '\'' || cs[end] == '"') {
                    end--;
                }
                if(end >= start) {
                    s_tmpArray.addObject(CCString::create(cs.substr(start, end - start + 1)));
                } else {
                    s_tmpArray.addObject(CCString::create(""));
                }
            } else {                
                float f = atof(cs.c_str());
                s_tmpArray.addObject(CCFloat::create(f));
            }
        } else {
            s_tmpArray.addObject(CCFloat::create(0));
        }
    }
    
    // return
    return s_tmpArray;
}

string CCUtils::arrayToString(const CCArray& array) {
    string ret = "[";
    CCObject* obj;
    char buf[128]; 
    CCARRAY_FOREACH(&array, obj) {
        CCString* s = dynamic_cast<CCString*>(obj);
        if(s) {
            if(ret.length() > 1)
                ret.append(",");
            ret.append("\"");
            ret.append(s->getCString());
            ret.append("\"");
            continue;
        }
        
        CCInteger* i = dynamic_cast<CCInteger*>(obj);
        if(i) {
            if(ret.length() > 1)
                ret.append(",");
            sprintf(buf, "%d", i->getValue());
            ret.append(buf);
            continue;
        }
        
        CCFloat* f = dynamic_cast<CCFloat*>(obj);
        if(f) {
            if(ret.length() > 1)
                ret.append(",");
            sprintf(buf, "%f", f->getValue());
            ret.append(buf);
            continue;
        }
        
        CCDouble* d = dynamic_cast<CCDouble*>(obj);
        if(d) {
            if(ret.length() > 1)
                ret.append(",");
            sprintf(buf, "%lf", d->getValue());
            ret.append(buf);
            continue;
        }
    }
    
    ret.append("]");
    return ret;
}

void CCUtils::setOpacityRecursively(CCNode* node, int o) {
	CCRGBAProtocol* p = dynamic_cast<CCRGBAProtocol*>(node);
	if(p) {
		p->setOpacity(o);
	}
	
	CCArray* children = node->getChildren();
    int cc = node->getChildrenCount();
    for(int i = 0; i < cc; i++) {
        CCNode* child = (CCNode*)children->objectAtIndex(i);        
        setOpacityRecursively(child, o);
    }
}

CCArray* CCUtils::getChildrenByTag(CCNode* parent, int tag) {
	CCArray* ret = CCArray::create();
	CCObject* obj;
	CCARRAY_FOREACH(parent->getChildren(), obj) {
		CCNode* child = (CCNode*)obj;
		if(child->getTag() == tag) {
			ret->addObject(child);
		}
	}
	
	return ret;
}

void CCUtils::removeChildrenByTag(CCNode* parent, int tag) {
	CCObject* obj;
	CCARRAY_FOREACH_REVERSE(parent->getChildren(), obj) {
		CCNode* child = (CCNode*)obj;
		if(child->getTag() == tag) {
			child->removeFromParent();
		}
	}
}

double CCUtils::pround(double x, int precision) {
    double div = pow(10, -precision);
    return floor(x / div + 0.5) * div;
}

double CCUtils::pfloor(double x, int precision) {
    double div = pow(10, -precision);
    return (int)floor(x / div) * div;
}

double CCUtils::pceil(double x, int precision) {
    double div = pow(10, -precision);
    return (int)ceil(x / div) * div;
}

int CCUtils::getCpuHz() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    // get hardward string
    size_t size = 100;
    char* hw_machine = (char*)malloc(size);
    int name[] = {CTL_HW, HW_MACHINE};
    sysctl(name, 2, hw_machine, &size, NULL, 0);
    string hw = hw_machine;
    free(hw_machine);
    
    // check
    if(startsWith(hw, "iPhone")) {
        string majorMinor = hw.substr(6);
        StringList& parts = componentsOfString(majorMinor, ',');
        int major = atoi(parts.at(0).c_str());
        if(major < 4)
            return 500000000;
        else if(major == 4)
            return 800000000;
        else
            return 1500000000;
    } else if(startsWith(hw, "iPod")) {
        string majorMinor = hw.substr(4);
        StringList& parts = componentsOfString(majorMinor, ',');
        int major = atoi(parts.at(0).c_str());
        if(major < 4)
            return 500000000;
        else if(major == 4)
            return 800000000;
        else
            return 1500000000;
    } else if(startsWith(hw, "iPad")) {
        string majorMinor = hw.substr(4);
        StringList& parts = componentsOfString(majorMinor, ',');
        int major = atoi(parts.at(0).c_str());
        if(major < 2)
            return 500000000;
        else if(major == 2)
            return 800000000;
        else
            return 1500000000;
    } else {
        return 1500000000;
    }
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    // get package manager
    JniMethodInfo t;
    JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/SystemUtils", "getCPUFrequencyMax", "()I");
	return t.env->CallStaticIntMethod(t.classID, t.methodID);
#else
    return 0;
#endif
}

void CCUtils::purgeDefaultForKey(const string& key) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    NSString* nsKey = [NSString stringWithCString:key.c_str() encoding:NSUTF8StringEncoding];
    [[NSUserDefaults standardUserDefaults] removeObjectForKey:nsKey];
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
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
#else
    CCLOGERROR("CCUtils::purgeDefaultForKey is not implemented for this platform, please finish it");
#endif
}

CCSize CCUtils::measureRichString(const char* pText,
                                  const char* pFontName,
                                  int nSize,
                                  int maxWidth,
                                  float shadowOffsetX,
                                  float shadowOffsetY,
                                  float strokeSize) {
    return CCImage_richlabel::measureRichString(pText,
                                                pFontName,
                                                nSize,
                                                maxWidth,
                                                shadowOffsetX,
                                                shadowOffsetY,
                                                strokeSize);
}

bool CCUtils::isInternalMusicPlaying() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    MPMusicPlayerController* mp = [MPMusicPlayerController applicationMusicPlayer];
	return [mp playbackState] == MPMusicPlaybackStatePlaying;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return false;
#else
    CCLOGERROR("CCUtils::playInternalMusic is not implemented for this platform, please finish it");
#endif
}

void CCUtils::playInternalMusic() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    MPMusicPlayerController* mp = [MPMusicPlayerController applicationMusicPlayer];
    [mp setQueueWithQuery:[MPMediaQuery songsQuery]];
    [mp play];
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    // TODO not easy on Android
#else
    CCLOGERROR("CCUtils::playInternalMusic is not implemented for this platform, please finish it");
#endif
}

void CCUtils::stopInternalMusic() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    MPMusicPlayerController* mp = [MPMusicPlayerController applicationMusicPlayer];
    [mp stop];
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    // TODO not easy on Android
#else
    CCLOGERROR("CCUtils::playInternalMusic is not implemented for this platform, please finish it");
#endif
}

string CCUtils::makeScreenshot(CCNode* root, const string& path, bool needStencil) {
	// check extension
	string ext = getPathExtension(path);
	bool png = ext == ".png";
	bool jpg = ext == ".jpg" || ext == ".jpeg";
	if(!png && !jpg)
		jpg = true;
	
	// get render root
	CCNode* renderNode = root;
	CCRect bound;
	if(renderNode)
		bound = getBoundingBoxInWorldSpace(renderNode);
	else
		renderNode = CCDirector::sharedDirector()->getRunningScene();
	
	// render in texture
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCRenderTexture* screen = CCRenderTexture::create(winSize.width,
													  winSize.height,
													  kCCTexture2DPixelFormat_RGBA8888,
													  needStencil ? GL_DEPTH24_STENCIL8 : 0);
	screen->begin();
	renderNode->visit();
	screen->end();
	
	// save buffer
	screen->saveToFile(path.c_str(), png ? kCCImageFormatPNG : kCCImageFormatJPEG);
	
	// full path
	return CCFileUtils::sharedFileUtils()->getWritablePath() + path;
}

void CCUtils::showSystemConfirmDialog(const char* title, const char* msg, const char* positiveButton, const char* negativeButton, CCCallFunc* onOK, CCCallFunc* onCancel) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	NSString* cancelButtonTitle = negativeButton ? [NSString stringWithUTF8String:negativeButton] : nil;
	NSString* okButtonTitle = positiveButton ? [NSString stringWithUTF8String:positiveButton] : nil;
    string lan = CCLocale::sharedLocale()->getISOLanguage();
	if(cancelButtonTitle == nil) {
		if(lan == "zh")
			cancelButtonTitle = @S_CANCEL_ZH;
		else
			cancelButtonTitle = @S_CANCEL_EN;
	}
	if(okButtonTitle == nil) {
		if(lan == "zh")
			okButtonTitle = @S_OK_ZH;
		else
			okButtonTitle = @S_OK_EN;
	}
    
	// create alert view
	UIAlertView* alert = [[UIAlertView alloc] initWithTitle:[NSString stringWithUTF8String:title]
													message:[NSString stringWithUTF8String:msg]
												   delegate:[[CCSystemConfirmDialogDelegate alloc] initWithOK:onOK cancel:onCancel]
										  cancelButtonTitle:cancelButtonTitle
										  otherButtonTitles:okButtonTitle, nil];
	[alert performSelectorOnMainThread:@selector(show)
							withObject:nil
						 waitUntilDone:NO];
    
    // release
#if !__has_feature(objc_arc)
    [alert release];
#endif
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    // find method
    JniMethodInfo t;
    JniHelper::getStaticMethodInfo(t,
                                   "org/cocos2dx/lib/SystemUtils",
                                   "showConfirmDialog",
                                   "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;JJ)V");
    
	// ensure positive button and negative button not null
	const char* posBtn = positiveButton;
	const char* negBtn = negativeButton;
	string lan = CCLocale::sharedLocale()->getISOLanguage();
	if(!posBtn) {
		if(lan == "zh")
			posBtn = S_OK_ZH;
		else
			posBtn = S_OK_EN;
	}
	if(!negBtn) {
		if(lan == "zh")
			negBtn = S_CANCEL_ZH;
		else
			negBtn = S_CANCEL_EN;
	}
    
	// create jstring
	jstring jTitle = title ? t.env->NewStringUTF(title) : NULL;
	jstring jMsg = msg ? t.env->NewStringUTF(msg) : NULL;
	jstring jPositiveButton = t.env->NewStringUTF(posBtn);
	jstring jNegativeButton = t.env->NewStringUTF(negBtn);
    
	// call java side
    t.env->CallStaticVoidMethod(t.classID, t.methodID, jTitle, jMsg, jPositiveButton, jNegativeButton, (long)onOK, (long)onCancel);
    
	// delete reference
	if(jTitle)
		t.env->DeleteLocalRef(jTitle);
	if(jMsg)
		t.env->DeleteLocalRef(jMsg);
    t.env->DeleteLocalRef(jPositiveButton);
    t.env->DeleteLocalRef(jNegativeButton);
#else
    CCLOGERROR("CCUtils::showSystemConfirmDialog is not implemented for this platform, please finish it");
#endif
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

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

jbyteArray CCUtils::getFirstSignatureBytes() {
    // get context
    jobject ctx = getContext();
    
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

#endif // #if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

UIViewController* CCUtils::findViewController(UIView* view) {
	for (UIView* next = [view superview]; next; next = next.superview) {
		UIResponder* nextResponder = [next nextResponder];
		if ([nextResponder isKindOfClass:[UIViewController class]]) {
			return (UIViewController*)nextResponder;
		}
	}
	
	// try window root view controller
	for (UIView* next = [view superview]; next; next = next.superview) {
		if([next isKindOfClass:[UIWindow class]]) {
			return ((UIWindow*)next).rootViewController;
		}
	}
	
	return nil;
}

#endif // #if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

NS_CC_END