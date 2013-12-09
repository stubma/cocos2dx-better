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
#ifndef __CCJSONArray_h__
#define __CCJSONArray_h__

#include "CCJSONObject.h"
#include "CCAssetOutputStream.h"

NS_CC_BEGIN

/**
 * JSON object array
 */
class CC_DLL CCJSONArray : public CCObject {
private:
	/// value array
    typedef vector<CCJSONObject::KeyValue> KeyValueList;
	KeyValueList m_array;

protected:
	CCJSONArray();

	/// cast an object to CCJSONArray, if obj is a CCJSONObject, then the
	/// object will be saved as first element in returned array
	static CCJSONArray* safeCast(CCObject* obj);

	/// release key value
	static void releaseKeyValue(CCJSONObject::KeyValue& kv);

public:
	virtual ~CCJSONArray();

	/**
	 * Create an empty json array object
	 *
	 * @return json array object
	 */
	static CCJSONArray* create();

	/**
	 * Parse a json memory string
	 *
	 * @param json json string in memory, must be in utf-8 encoding
	 * @param length byte length of \c json string
	 * @return json array object, if \c json is actually a json object then this json object is saved
	 *  	in the first element of array.
	 */
	static CCJSONArray* create(const char* json, size_t length);

	/**
	 * Parse a json file saved in assets or file system
	 *
	 * @param path path of json file
	 * @return json array object, if \c json is actually a json object then this json object is saved
	 *  	in the first element of array.
	 */
	static CCJSONArray* create(const string& path);

	/**
	 * Get count of elements in json array
	 *
	 * @return count of elements in json array
	 */
	int getLength() { return m_array.size(); }

	/**
	 * Add a null value to an array
	 */
	void addNull();

	/**
	 * Add a boolean value to array
	 *
	 * @param b boolean value
	 */
	void addBool(bool b);

	/**
	 * Add an integer value to array
	 *
	 * @param i integer value
	 */
	void addInt(int i);

	/**
	 * Add a long value to array
	 *
	 * @param l long value
	 */
	void addLong(long l);

	/**
	 * Add a float value to array
	 *
	 * @param f float value
	 */
	void addFloat(float f);

	/**
	 * Add a double value to array
	 *
	 * @param d double value
	 */
	void addDouble(double d);

	/**
	 * Add a string value to array
	 *
	 * @param s string value
	 */
	void addString(const char* s);

	/**
	 * Add json object to array
	 *
	 * @param jo json object
	 */
	void addObject(CCJSONObject* jo);

	/**
	 * Add json array to array
	 *
	 * @param ja json数组
	 */
	void addArray(CCJSONArray* ja);

	/**
	 * Print json array values to an output stream
	 *
	 * @param aos output stream
	 * @param level indentation level
	 */
	void output(CCAssetOutputStream* aos, int level = 1);

	/**
	 * Get boolean value at given index
	 *
	 * @param index index of element
	 * @param def default boolean to be returned
	 * @return boolean value
	 */
	bool optBool(int index, bool def = false);

	/**
	 * Get integer value at given index
	 *
	 * @param index index of element
	 * @param def default integer to be returned
	 * @return integer value
	 */
	int optInt(int index, int def = 0);

	/**
	 * Get long value at given index
	 *
	 * @param index index of element
	 * @param def default long to be returned
	 * @return long value
	 */
	long optLong(int index, long def = 0);

	/**
	 * Get float value at given index
	 *
	 * @param index index of element
	 * @param def default float to be returned
	 * @return float value
	 */
	float optFloat(int index, float def = 0);

	/**
	 * Get double value at given index
	 *
	 * @param index index of element
	 * @param def default double to be returned
	 * @return double value
	 */
	double optDouble(int index, double def = 0);

	/**
	 * Get json object at given index
	 *
	 * @param index index of element
	 * @return json object value
	 */
	CCJSONObject* optJSONObject(int index);

	/**
	 * Get json array at given index
	 *
	 * @param index index of element
	 * @return json array
	 */
	CCJSONArray* optJSONArray(int index);

	/**
	 * Get string value at given index
	 *
	 * @param index index of element
	 * @param def default string to be returned
	 * @return value string, this string should not released by caller, it is managed by json object and you must
	 * 		copy it if you want to save it
	 */
	string optString(int index, const char* def = NULL);
};

NS_CC_END

#endif // __CCJSONArray_h__
