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
#ifndef __CCMWLoader_h__
#define __CCMWLoader_h__

#include "CCMWFileData.h"

NS_CC_BEGIN

/**
 * @class CCMWLoader
 *
 * Loader of Motion Welder file
 */
class CCMWLoader {
private:
	/**
	 * Load a Motion Welder file and return file data object
	 *
	 * @param data Motion Welder file raw data
	 * @param length length of \c data
	 * @param resScale resource scale of Motion Welder file data
	 * @return \link CCMWFileData CCMWFileData\endlink, or NULL if loading failed
	 */
	static CCMWFileData* load(const char* data, size_t length, float resScale = 1.f);

public:
	/**
	 * Load a Motion Welder file and return file data object
	 *
	 * @param path path of Motion Welder file
	 * @return \link CCMWFileData CCMWFileData\endlink
	 */
	static CCMWFileData* load(const char* path);
};

NS_CC_END

#endif // __CCMWLoader_h__
