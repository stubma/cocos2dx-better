/****************************************************************************
 Author: Luma (stubma@gmail.com)
 
 https://github.com/stubma/cocos2dx-common
 
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

using namespace std;

NS_CC_BEGIN

/// custom utilities
class CC_DLL CCUtils {
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
	 * @param sub char to be replaced to
	 */
    static void replaceChar(string& s, char c, char sub);
    
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
	
	/**
	 * map path to different platform, for example, a path "/sdcard/a.png" will be:
	 * 1. in android, it is /sdcard/a.png
	 * 2. in iOS and MacOSX, it will be ~/Documents/sdcard/a.png
	 * 3. in Windows, it will be [Executable Dir]/sdcard/a.png
	 *
	 * for iOS and MacOSX, if path is a relative path, then it will be mapped to app folder, for example, 
	 * a path "sdcard/a.png" will be:
	 * 1. in iOS, it will be [app path]/sdcard/a.png
	 * 2. in Mac OS X, it will [app path]/Contents/Resources/sdcard/a.png
	 */
	static string mapLocalPath(string path);
	
	/// get parent path of a path
	static string getParentPath(string path);
	
	/**
	 * create intermediate folders for a path
	 *
	 * @param path must be an absolute path
	 * @return true means successful
	 */
	static bool createIntermediateFolders(string path);
	
	/**
	 * check path existence
	 *
	 * @param path must be an absolute path
	 * @return true means path is existent
	 */
	static bool isPathExistent(string path);
	
	/**
	 * create a folder at specified absolute path, its parent folder should be existent and this
	 * method won't perform checking
	 *
	 * @param path must be an absolute path
	 * @return true means successful
	 */
	static bool createFolder(string path);
	
	/// delete a file
	static bool deleteFile(string path);
	
	/// convert rgb to hsv
	static ccColorHSV ccc32hsv(ccColor3B c);
	
	/// convert hsv to rgb
	static ccColor3B hsv2ccc3(ccColorHSV c);
	
	/// get node origin
	static CCPoint getOrigin(CCNode* node);
	
	/// get node center
	static CCPoint getCenter(CCNode* node);
	
	/// get node position
	static CCPoint getPoint(CCNode* node, float xpercent, float ypercent);
	
	/// get node position
	static CCPoint getPoint(CCNode* node, CCPoint anchor) { return getPoint(node, anchor.x, anchor.y); }
	
	/// is a segment intersected with a box
	static bool testSegmentAABB(CCPoint p0, CCPoint p1, ccAABB b);
	
	/// perform a binary search in a int array
	static int binarySearch(int* a, size_t len, int key);
    
    /// combine two rect
    static CCRect combine(const CCRect& r1, const CCRect& r2);
    
    /// get system language iso two letter code
    static string getLanguage();
    
    /// get system country iso two letter code
    static string getCountry();
    
    /// has external storage, such as sd card
    static bool hasExternalStorage();
    
    /// current time milliseconds from 1970-1-1
    static int64_t currentTimeMillis();
};

NS_CC_END

#endif // __CCUtils_h__
