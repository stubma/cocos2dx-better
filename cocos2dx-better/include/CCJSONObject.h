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
#ifndef __CCJSONObject_h__
#define __CCJSONObject_h__

#include "cocos2d.h"
#include "CCAssetOutputStream.h"

using namespace std;

NS_CC_BEGIN

class CCJSONArray;

/**
 * CCJSONObject means a map of key value pairs.
 */
class CC_DLL CCJSONObject : public CCObject {
public:
	/**
	 * JSON value type
	 */
	enum Type {
		NIL,
		BOOLEAN,
		STRING,
		OBJECT,
		ARRAY
	};

	/**
	 * Union for a json value reference
	 */
	union Value {
		bool b;
		const char* s;
        CCJSONObject* jo;
        CCJSONArray* ja;
	};

	/**
	 * key value pair of json
	 */
	struct KeyValue {
		const char* k;
		Type t;
		Value v;
	};

private:
	/// json key value map
	typedef map<string, KeyValue> PairMap;
	PairMap m_pairs;

	/// a sequential key value
	typedef vector<KeyValue> KeyValueList;
	KeyValueList m_keyvalues;

protected:
	CCJSONObject();

	/// cast an object to CCJSONObject, if obj is a CCJSONArray, then
	/// this array will be packed in json object with keyword "array"
	static CCJSONObject* safeCast(CCObject* obj);

	/// release key value
	static void releaseKeyValue(const string& key, KeyValue& kv);

public:
	virtual ~CCJSONObject();

	/**
	 * Create an empty json object
	 *
	 * @return json object
	 */
	static CCJSONObject* create();

	/**
	 * Parse a json memory string
	 *
	 * @param json json string in memory, must be in utf-8 encoding
	 * @param length byte length of \c json string
	 * @return json object, if \c json is actually an array then this array will be added
	 * 		with "array" key in returned json object
	 */
	static CCJSONObject* create(const char* json, size_t length);

	/**
	 * Parse a json file saved in assets or file system
	 *
	 * @param path path of json file
	 * @param isFile true means \c path is an absolute path in file system, or false means
	 * 		\c path is a relative path under assets
	 * @return json object, if \c json is actually an array then this array will be added
	 * 		with "array" key in returned json object
	 */
	static CCJSONObject* create(const string& path);

	/**
	 * Print json object key value pairs to an output stream
	 *
	 * @param aos output stream
	 * @param level indentation level
	 */
	void output(CCAssetOutputStream* aos, int level = 1);

	/**
	 * Add a null value for a key
	 *
	 * @param key key string
	 */
	void addNull(const char* key);

	/**
	 * Add a boolean value for a key
	 *
	 * @param key key string
	 * @param b boolean value
	 */
	void addBool(const char* key, bool b);

	/**
	 * Add a integer value for a key
	 *
	 * @param key key string
	 * @param i integer value
	 */
	void addInt(const char* key, int i);

	/**
	 * Add a long value for a key
	 *
	 * @param key key string
	 * @param l long value
	 */
	void addLong(const char* key, long l);

	/**
	 * Add a float value for a key
	 *
	 * @param key key string
	 * @param f float value
	 */
	void addFloat(const char* key, float f);

	/**
	 * Add a double value for a key
	 *
	 * @param key key string
	 * @param d double value
	 */
	void addDouble(const char* key, double d);

	/**
	 * Add a string value for a key
	 *
	 * @param key key string
	 * @param s string
	 */
	void addString(const char* key, const char* s);

	/**
	 * Add a json object value for a key
	 *
	 * @param key key string
	 * @param jo json object
	 */
	void addObject(const char* key, CCJSONObject* jo);

	/**
	 * Add a json array value for a key
	 *
	 * @param key key string
	 * @param ja json array object
	 */
	void addArray(const char* key, CCJSONArray* ja);

	/**
	 * Get boolean value of a key
	 *
	 * @param key key string
	 * @param def default boolean value to be returned if key is not found
	 * @return boolean value
	 */
	bool optBool(const char* key, bool def = false);

	/**
	 * Get integer value of a key
	 *
	 * @param key key string
	 * @param def default integer value to be returned if key is not found
	 * @return integer value
	 */
	int optInt(const char* key, int def = 0);

	/**
	 * Get long value of a key
	 *
	 * @param key key string
	 * @param def default long value to be returned if key is not found
	 * @return long value
	 */
	long optLong(const char* key, long def = 0);

	/**
	 * Get float value of a key
	 *
	 * @param key key string
	 * @param def default float value to be returned if key is not found
	 * @return float value
	 */
	float optFloat(const char* key, float def = 0);

	/**
	 * Get double value of a key
	 *
	 * @param key key string
	 * @param def default double value to be returned if key is not found
	 * @return double value
	 */
	double optDouble(const char* key, double def = 0);

	/**
	 * Get json object of a key
	 *
	 * @param key key string
	 * @return json object, or NULL if key is not found
	 */
	CCJSONObject* optJSONObject(const char* key);

	/**
	 * Get json array of a key
	 *
	 * @param key key string
	 * @return json array, or NULL if key is not found
	 */
	CCJSONArray* optJSONArray(const char* key);

	/**
	 * Get string value of a key
	 *
	 * @param key key string
	 * @param def default string to be returned
	 * @return value string, this string should not released by caller, it is managed by json object and you must
	 * 		copy it if you want to save it
	 */
	string optString(const char* key, const char* def = NULL);

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

	/**
	 * Get count of elements in json object
	 *
	 * @return count of elements in json object
	 */
	int getLength() { return m_keyvalues.size(); }

	/**
	 * get key by index
	 *
	 * @param index index of element
	 * @return key, or NULL if index is invalid, returned key is not a copy, caller should NOT release it
	 */
	const char* keyAt(int index);
};

NS_CC_END

#endif // __CCJSONObject_h__
