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
#ifndef __CCArcticLoader_h__
#define __CCArcticLoader_h__

#include "CCArcticFileData.h"

NS_CC_BEGIN

/**
 * @class CCArcticLoader
 *
 * Loader of ASprite file
 */
class CC_DLL CCArcticLoader {
private:
	/**
	 * Load a ASprite file and return file data object
	 *
	 * @param data ASprite file raw data
	 * @param length length of \c data
	 * @param resScale resource scale of ASprite file data
	 * @return \link CCArcticFileData CCArcticFileData\endlink, or NULL if loading failed
	 */
	static CCArcticFileData* load(const char* data, size_t length, float resScale = 1.f);

public:
	/**
	 * Load a ASprite file and return file data object
	 *
	 * @param asPath path of ASprite file
	 * @return \link CCArcticFileData CCArcticFileData\endlink
	 */
	static CCArcticFileData* load(const char* asPath);
};

NS_CC_END

#endif // __CCArcticLoader_h__
