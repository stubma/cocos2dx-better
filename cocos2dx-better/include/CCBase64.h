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
#ifndef __CCBase64__
#define __CCBase64__

#include "cocos2d.h"

using namespace std;

NS_CC_BEGIN

/**
 * Base64 utilities
 */
class CC_DLL CCBase64 {
public:
	/**
	 * encode data into base64 format
	 *
	 * @param data raw data
	 * @param len bytes of raw data
	 * @return base64 string, it returns empty string if data can't be encoded
	 */
	static string encode(const void* data, int len);
	
	/**
	 * decode base64 string
	 *
	 * @param data base64 string
	 * @param outLen if not NULL, it will return decoded data length
	 * @return raw data, caller should release returned data pointer. It returns NULL if data can't be decoded
	 */
	static const char* decode(const string& data, int* outLen);
	
	/// it will append zero to decode content
	static const char* decodeAsCString(const string& data, int* outLen);
};

NS_CC_END

#endif /* defined(__CCBase64__) */
