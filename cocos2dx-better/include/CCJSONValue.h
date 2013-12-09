/****************************************************************************
 Author: Luma (stubma@gmail.com)
 
 https://github.com/stubma/cocos2dx-json
 
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
#ifndef __CCJSONValue_h__
#define __CCJSONValue_h__

#include "CCJSONObject.h"
#include "CCJSONArray.h"

NS_CC_BEGIN

/**
 * @class CCJSONValue
 *
 * help class of json value casting
 */
class CC_DLL CCJSONValue {
public:
	/// cast value to boolean
	static bool castToBool(CCJSONObject::KeyValue& kv);

	/// cast value to integer
	static int castToInt(CCJSONObject::KeyValue& kv);

	/// cast value to long
	static long castToLong(CCJSONObject::KeyValue& kv);

	/// cast value to float
	static float castToFloat(CCJSONObject::KeyValue& kv);

	/// cast value to double
	static double castToDouble(CCJSONObject::KeyValue& kv);

	/// cast value to json object
	static CCJSONObject* castToObject(CCJSONObject::KeyValue& kv);

	/// cast value to json array
	static CCJSONArray* castToArray(CCJSONObject::KeyValue& kv);

	/// cast value to string value
	static const char* castToString(CCJSONObject::KeyValue& kv);
};

NS_CC_END

#endif // __CCJSONValue_h__
