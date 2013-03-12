/****************************************************************************
 Author: Luma (stubma@gmail.com)
 
 https://github.com/stubma/cocos2dx-db
 
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

using namespace std;

NS_CC_BEGIN

/// custom utilities
class CC_DLL CCUtils {
public:
	/// convert string to lowercase
	static void toLowercase(string& s);
	
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
};

NS_CC_END

#endif // __CCUtils_h__
