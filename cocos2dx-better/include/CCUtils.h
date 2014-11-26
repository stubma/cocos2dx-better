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
#ifndef __CCUtils_h__
#define __CCUtils_h__

#include "cocos2d.h"
#include <string>
#include "ccMoreTypes.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	#include <jni.h>
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    #import <UIKit/UIKit.h>
#endif

using namespace std;

NS_CC_BEGIN

/// custom utilities
class CC_DLL CCUtils {
public:
    typedef vector<string> StringList;
    
private:
    /// string list temp
    static StringList s_tmpStringList;
    
    /// tmp CCArray
    static CCArray s_tmpArray;
    
private:
	static unsigned char UnitScalarToByte(float x);
	
public:
    /**
	 * Copy a string
	 *
	 * @param src source string
	 * @return copied string, caller should release it
	 */
	static const char* copy(const char* src);
    
	/**
	 * Copy a string
	 *
	 * @param src source string
	 * @param start start position to copy
	 * @param len length to copy
	 * @return copied string, caller should release it. returns NULL if \c src is NULL
	 */
	static const char* copy(const char* src, int start, size_t len);
    
	/// trim leading and tail space character
	static string trim(const string& s);
	
	/// convert string to lowercase
	static void toLowercase(string& s);
    
    /// start with sub string?
    static bool startsWith(const string& s, const string& sub);
    
    /// end with sub string?
    static bool endsWith(const string& s, const string& sub);
    
    /**
	 * Replace a char with other char
	 *
	 * @param s string reference, its content will be modified
	 * @param c char to be replaced
	 * @param sub char will be replacement to
	 */
    static void replaceChar(string& s, char c, char sub);
    
    /**
	 * Replace a sub string with other string
	 *
	 * @param s string reference, its content will be modified
	 * @param c sub string to be replaced
	 * @param sub string will be replacement to
	 */
    static string replace(string& s, const string& c, const string& sub);
    
    /**
     * remove char from a string
     * 
     * @param s string to be modified
     * @param c char to be removed
     */
    static void removeChar(string& s, char c);
	
	/// decode html entities in a string and return a decoded string
	static string decodeHtmlEntities(const string& src);
    
    /// convert a chinese string into pinyin string, s must be in utf-8 encoding
    /// currently gbk chinese is not supported
    static string getPinyin(const string& s);
    
    /**
     * get digit of a number. For example: 0 returns 1, 12 returns 2,
     * if number is negative, the negate symbol is not counted
     */
    static int getNumDigits(int num);
    
    /// Get index of last slash character, if not found, returns -1
    static ssize_t lastSlashIndex(string path);
    
    /// Get index of last dot character, if not found, returns -1
    static ssize_t lastDotIndex(const string& path);
    
    /// get last segment of path
    static string lastPathComponent(const string& path);
    
    /**
	 * Delete last component in a path, for example:<br>
	 * Input					Output<br>
	 * "/tmp/scratch.tiff"      "/tmp"<br>
	 * "/tmp/scratch"           "/tmp"<br>
	 * "/tmp/"                  "/"<br>
	 * "scratch"                ""<br>
	 * "/" 						"/"<br>
	 *
	 * @param path path string
	 * @return path without last segment, caller should release returned path string
	 */
    static string deleteLastPathComponent(const string& path);
    
    /**
	 * Append a path segment to another path, for example:<br>
	 * Input					Output<br>
	 * "/tmp", "/scratch.tiff"  "/tmp/scratch.tiff"<br>
	 * "/tmp//", "/scratch"     "/tmp/scratch"<br>
	 * "/tmp", "/"              "/tmp"<br>
	 * "/", "tmp/" 				"/tmp"<br>
	 *
	 * @param path path string
	 * @param component segment to be added
	 * @return new path, caller should release returned path string
	 */
	static string appendPathComponent(const string& path, const string& component);
    
    /**
	 * Delete path extension of last path component
	 *
	 * @param path path string
	 * @return path after deleted extension, caller should release it
	 */
	static string deletePathExtension(const string& path);
	
	/// get path extension, with a dot started string. or empty string if no extension
	static string getPathExtension(const string& path);
	
	/// get parent path of a path
	static string getParentPath(const string& path);
    
    /**
     * map a relative path to absolute external path, do nothing if path is absolute
     * in iOS, path will be appended to ~/Documents
     * in Android, path will be appended to internal storage folder
     */
    static string externalize(const string& path);
    
    /**
     * Map path to external path, and then check the path existence. If external path is not
     * existent, it will return the cocos2d-x full path.
     */
    static string getExternalOrFullPath(const string& path);
	
	/// get package name, in iOS, it retuns bundle id
	static string getPackageName();
    
    /// round float for specified precision
    static double pround(double x, int precision);
    
    /// floor float for specified precision
    static double pfloor(double x, int precision);
    
    /// ceil float for specified precision
    static double pceil(double x, int precision);
    
    /// linear interpolation for float
    static float lerp(float a, float b, float p) {
        return a * (1 - p) + b * p;
    }
    
    /// rect contains another rect?
    static bool containsRect(const CCRect& r1, const CCRect& r2);
	
	/**
	 * create intermediate folders for a path
	 *
	 * @param path must be an absolute path
	 * @return true means successful
	 */
	static bool createIntermediateFolders(const string& path);
	
	/**
	 * check path existence
	 *
	 * @param path must be an absolute path
	 * @return true means path is existent
	 */
	static bool isPathExistent(const string& path);
	
	/**
	 * create a folder at specified absolute path, its parent folder should be existent and this
	 * method won't perform checking
	 *
	 * @param path must be an absolute path
	 * @return true means successful
	 */
	static bool createFolder(const string& path);
	
	/// delete a file
	static bool deleteFile(const string& path);
	
	/// convert rgb to hsv
	static ccColorHSV ccc32hsv(ccColor3B c);
	
	/// convert hsv to rgb
	static ccColor3B hsv2ccc3(ccColorHSV c);
	
	/// get node origin, relative to parent space
	static CCPoint getOrigin(CCNode* node);
	
	/// get node center, relative to parent space
	static CCPoint getCenter(CCNode* node);
    
    /// get local center
    static CCPoint getLocalCenter(CCNode* node);
	
	/// get node position, relative to parent space
	static CCPoint getPoint(CCNode* node, float xpercent, float ypercent);
	
	/// get node position, relative to parent space
	static CCPoint getPoint(CCNode* node, CCPoint anchor) { return getPoint(node, anchor.x, anchor.y); }
	
	// get node point, in node space
	static CCPoint getLocalPoint(CCNode* node, float xpercent, float ypercent);
	
	// get node point, in node space
	static CCPoint getLocalPoint(CCNode* node, CCPoint anchor);
	
	/// get bounding box in world space
	static CCRect getBoundingBoxInWorldSpace(CCNode* node);
    
    /**
     * most of time, we just need the center part of a scalable image,
     * so this method auto return center rect for you
     */
    static CCRect getCenterRect(const string& frameName);
    
    /**
     * most of time, we just need the center part of a scalable image,
     * so this method auto return center rect for you
     */
    static CCRect getCenterRect(CCSpriteFrame* f);
    
    /// get scene to which a node belongs
    static CCScene* getScene(CCNode* n);
	
	/// is a segment intersected with a box
	static bool testSegmentAABB(CCPoint p0, CCPoint p1, ccAABB b);
	
	/// perform a binary search in a int array
	static int binarySearch(int* a, size_t len, int key);
    
    /// combine two rect
    static CCRect combine(const CCRect& r1, const CCRect& r2);
    
    /// has external storage, such as sd card
    static bool hasExternalStorage();
    
    /// get a good place to save app internal files
    static string getInternalStoragePath();
    
    /// get available size of mobile phone storage
    static int64_t getAvailableStorageSize();
    
    /// current time milliseconds from 1970-1-1
    static int64_t currentTimeMillis();
    
    /// verify app signature, if has, basically it is only used for android
    static bool verifySignature(void* validSign, size_t len);
    
    /// check whether app is signed by a debug cert, it always return false for iOS
    static bool isDebugSignature();
    
    /**
     * split string into components by a separator
     * returned string list is const and no need to release
     * the list is shared, you must copy its content if you want to keep content for later use
     *
     * @param s string
     * @param sep separator character
     * @return a const vector, must copy it if you want to keep its content
     */
    static CCArray& componentsOfString(const string& s, const char sep);
    
    /**
     * split string into components by a separator
     * returned CCArray contains CCInteger object
     * the array is shared, you must copy its content if you want to keep content for later use
     *
     * @param s string
     * @param sep separator character
     * @return a const CCArray, must copy it if you want to keep its content
     */
    static CCArray& intComponentsOfString(const string& s, const char sep);
    
    /**
     * split string into components by a separator
     * returned CCArray contains CCFloat object
     * the array is shared, you must copy its content if you want to keep content for later use
     *
     * @param s string
     * @param sep separator character
     * @return a const CCArray, must copy it if you want to keep its content
     */
    static CCArray& floatComponentsOfString(const string& s, const char sep);
    
    /**
     * split string into components by a separator
     * returned CCArray contains CCBool object
     * the array is shared, you must copy its content if you want to keep content for later use
     *
     * @param s string
     * @param sep separator character
     * @return a const CCArray, must copy it if you want to keep its content
     */
    static CCArray& boolComponentsOfString(const string& s, const char sep);
    
    /// generate a string which is a join of all strings in an CCArray
    static string joinString(const CCArray& a, const char sep);
    
    /// generate a string which is a join of all integer in an CCArray
    static string joinInt(const CCArray& a, const char sep);
    
    /// generate a string which is a join of all float in an CCArray
    static string joinFloat(const CCArray& a, const char sep);
    
    /// generate a string which is a join of all boolean in an CCArray
    static string joinBool(const CCArray& a, const char sep);
    
    /**
     * parse a CCPoint from a string in format {x,y}, such as {3.2,3.4}.
     * The brace can be replaced with bracket or parentheses, i.e., [3.2,3.4] and
     * (3.2,3.4) is also valid. Furthurmore, white space is allowed so [3.2,   3.4] is valid
     *
     * \note
     * for partial valid string, the result will be
     * {3.2} ==> ccp(3.2, 0)
     * {,3.4} ==> ccp(0, 3.4)
     * {3.2 ==> ccp(3.2, 0)
     * {,3.4 ==> ccp(0, 3.4)
     * {2,3,4} ==> ccp(2, 3)
     * {[2,3}]) ==> ccp(2,3)
     * {,} ==> ccp(0, 0)
     * 
     * @param s string
     * @return CCPoint, or a zero point if string format is invalid
     */
    static CCPoint ccpFromString(const string& s);
    
    /**
     * parse a CCSize from a string in format {x,y}, such as {3.2,3.4}.
     * The brace can be replaced with bracket or parentheses, i.e., [3.2,3.4] and
     * (3.2,3.4) is also valid. Furthurmore, white space is allowed so [3.2,   3.4] is valid
     *
     * \note
     * for partial valid string, the result will be
     * {3.2} ==> ccp(3.2, 0)
     * {,3.4} ==> ccp(0, 3.4)
     * {3.2 ==> ccp(3.2, 0)
     * {,3.4 ==> ccp(0, 3.4)
     * {2,3,4} ==> ccp(2, 3)
     * {[2,3}]) ==> ccp(2,3)
     * {,} ==> ccp(0, 0)
     *
     * @param s string
     * @return CCSize, or a zero point if string format is invalid
     */
    static CCSize ccsFromString(const string& s);
    
    /**
     * parse a CCRect from a string in format {x,y,w,h}, such as {1,2,3,4}
     * The brace can be replaced with bracket or parentheses, and whitespace
     * is allowed
     *
     * \note
     * This method has same tolerance as ccpFromString and ccsFromString
     *
     * @param s string
     * @return CCRect, or a zero point of string format is invalid
     */
    static CCRect ccrFromString(const string& s);
    
    /**
     * create a CCArray from a string in format {a1,a2,a3,...}. If the element is
     * embraced by quote or double quote, it will be a CCString object.
     * If no quote, it will be a CCFloat
     *
     * @param s string
     * @return CCArray, you should copy it if you want to keep its content for later use
     */
    static CCArray& arrayFromString(const string& s);
    
    /**
     * convert CCArray to string format, rule is same as arrayFromString
     */
    static string arrayToString(const CCArray& array);
	
	/// set opacity from a node, to all its descentants
	static void setOpacityRecursively(CCNode* node, int o);
	
	/**
	 * get child node whth specified tag, it can return all children nodes if they has
	 * same tag
	 *
	 * @param parent parent node
	 * @param tag tag
	 * @return array pointer contains all children node whose tag matched, array is auto released
	 */
	static CCArray* getChildrenByTag(CCNode* parent, int tag);
	
	/**
	 * remove child node with specified tag, it can remove more than one child as long as the tag matched
	 * 
	 * @param parent parent node
	 * @param tag tag
	 */
	static void removeChildrenByTag(CCNode* parent, int tag);
    
    /**
     * get CPU freqency, in Hz
     *
     * @return freqency. However, for iOS, it returns a fake and approximate value
     *      It will returns 0 if it can't be queried
     */
    static int getCpuHz();
    
    /// remove a default setting, CCUserDefault doesn't provide this feature
    static void purgeDefaultForKey(const string& key);
    
    /**
	 * Calculate how many bytes of a utf-8 encoded character
	 *
	 * @param c first byte of character in utf-8 encoding
	 * @return byte length of this character
	 */
	static int getUTF8Bytes(unsigned char c);
    
    /**
	 * Get count of utf-8 character in a string
	 *
	 * @param s utf-8 string
	 * @return count of utf-8 character
	 */
	static int strlen8(const char* s);
    
    /**
     * Measure a rich string size without creating a OpenGL texture. Measured size
     * will be same as content size of rich label node. However, it may not exactly same 
     * as content size if content scale factor is not 1, but the deviation will be less than 1
     * pixel
     *
     * @param pText rich text string supported by CCRichLabelTTF
     * @param pFontName font name, optional. If NULL, use default font
     * @param nSize font size, optional. If zero, use default size
     * @param maxWidth max line width, or zero if single line text. optional
     * @param shadowOffsetX shadow x offset, optional
     * @param shadowOffsetY shadow y offset, optional
     * @param strokeSize border line width, optional
     * @param lineSpacing line spacing, optional
     * @param globalImageScaleFactor global image scale factor which applies to all images in this rich label, default is 1
	 * @param decryptFunc if label has embedded images and they are encrypted, you must provide a decrypt function
     */
    static CCSize measureRichString(const char* pText,
                                    const char* pFontName = NULL,
                                    int nSize = 0,
                                    int maxWidth = 0,
                                    float shadowOffsetX = 0,
                                    float shadowOffsetY = 0,
                                    float strokeSize = 0,
                                    float lineSpacing = 0,
                                    float globalImageScaleFactor = 1,
									CC_DECRYPT_FUNC decryptFunc = NULL);
    
    /// start to play internal music randomly
    static void playInternalMusic();
	
	/// stop internal music playing
	static void stopInternalMusic();
	
	/// is internal music playing
	static bool isInternalMusicPlaying();
	
	/**
	 * capture screen and save it to a image file. The file type will auto determined by extension, and 
	 * only jpg, jpeg, png is supported. Other extensions will be ignored and save as jpg. The image file will be
	 * same size as window size (also known as design size).
	 *
	 * @param root the start node to be captured, so that you can only capture part of screen. However, final image
	 *		file is always window size. If root is NULL, whole screen will be captured.
	 * @param path the relative path of image file, it will be mapped to platform writable path. In iOS, it is ~/Documents,
	 *		in Android, it is cache dir
	 * @param needStencil true if you want a stencil attachment, by default it is false
	 * @return full path of saved image file
	 */
	static string makeScreenshot(CCNode* root, const string& path, bool needStencil = false);
    
    /**
	 * Show a confirm dialog and the dialog is backed by system, so its UI is platform-dependent
	 *
	 * @param title title text
	 * @param msg content text
	 * @param positiveButton text of positive button, default is NULL which means it is "OK"
	 * @param negativeButton text of negative button, default is NULL which means it is "Cancel"
	 * @param onOK callback when ok button is clicked
	 * @param onCancel callback when cancel button is clicked
	 */
	static void showSystemConfirmDialog(const char* title, const char* msg, const char* positiveButton = NULL, const char* negativeButton = NULL, CCCallFunc* onOK = NULL, CCCallFunc* onCancel = NULL);
	
	/**
	 * open app in platform-specific store
	 *
	 * \note 
	 * for now, in android, it just open google play web page. For other third-party android market, it is
	 * quite a bit challenge to support them.
	 *
	 * @param appId the app id, in iOS, it is a number identifier you should get it from itunes connect.
	 *		but for android, this parameter is ignored.
	 */
	static void openAppInStore(const string& appId);
    
    /// open an url
    static void openUrl(const string& url);
    
    /// get app version name
    static string getAppVersion();
    
    /// get device type
    static string getDeviceType();
    
    /**
     * get system version nubmer
     *
     * \note
     * for iOS, it convert system major version int
     * for Android, it returns Build.VERSION.SDK_INT
     * for Mac, it returns 0
     */
    static int getSystemVersionInt();
    
    /**
     * get mac address, if failed, return 00:00:00:00:00:00. 
     * 
     * \note
     * for iOS 7.0+, you can't get mac address
     */
    static string getMacAddress();
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	/// get JNIEnv
	static JNIEnv* getJNIEnv();
    
    /// get context
    static jobject getContext();
    
    // for activity starting
    static jobject newIntent(const char* activityName);
    static jobject newIntentByAction(const char* action);
    static void setIntentUri(jobject intent, const char* uri);
    static void putBooleanExtra(jobject intent, const char* name, bool value);
    static void putByteExtra(jobject intent, const char* name, unsigned char value);
    static void putCharExtra(jobject intent, const char* name, unsigned short value);
    static void putDoubleExtra(jobject intent, const char* name, double value);
    static void putFloatExtra(jobject intent, const char* name, float value);
    static void putIntExtra(jobject intent, const char* name, int value);
    static void putStringExtra(jobject intent, const char* name, const char* value);
    static void putLongExtra(jobject intent, const char* name, long value);
    static void putShortExtra(jobject intent, const char* name, short value);
    static void putParcelableExtra(jobject intent, const char* name, jobject value);
    static void startActivity(jobject intent);
    static void sendBroadcast(jobject intent);
#endif // #if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	/// find view controller which contains given view
	static UIViewController* findViewController(UIView* view);
#endif // #if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
};

NS_CC_END

#endif // __CCUtils_h__
