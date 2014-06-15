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
#include "CCJSONObject.h"
#include "CCJSONArray.h"
#include "CCJSONParser.h"
#include "CCJSONValue.h"
#include "CCUtils.h"
#include "CCMemoryOutputStream.h"

NS_CC_BEGIN

// tmp buffer
static char s_buf[4096];

CCJSONObject::CCJSONObject() {
}

CCJSONObject::~CCJSONObject() {
	for(PairMap::iterator iter = m_pairs.begin(); iter != m_pairs.end(); iter++) {
		releaseKeyValue(iter->first, iter->second);
	}
}

void CCJSONObject::releaseKeyValue(const string& key, KeyValue& kv) {
	switch(kv.t) {
		case STRING:
			free((void*)kv.v.s);
			break;
		case OBJECT:
			CC_SAFE_RELEASE(kv.v.jo);
			break;
		case ARRAY:
			CC_SAFE_RELEASE(kv.v.ja);
			break;
        default:
            break;
	}
	if(kv.k)
		free((void*)kv.k);
}

CCJSONObject* CCJSONObject::safeCast(CCObject* obj) {
	CCJSONObject* jo = dynamic_cast<CCJSONObject*>(obj);
	if(jo) {
		return jo;
	} else {
		CCJSONArray* ja = dynamic_cast<CCJSONArray*>(obj);
		if(ja) {
			CCJSONObject* jo = CCJSONObject::create();
			jo->addArray("array", ja);
			return jo;
		} else {
			return NULL;
		}
	}
}

CCJSONObject* CCJSONObject::create() {
	CCJSONObject* jo = new CCJSONObject();
	return (CCJSONObject*)jo->autorelease();
}

CCJSONObject* CCJSONObject::create(const string& path) {
	CCObject* obj = CCJSONParser::load(path);
	return safeCast(obj);
}

CCJSONObject* CCJSONObject::create(const char* json, size_t length) {
	CCObject* obj = CCJSONParser::load(json, length);
	return safeCast(obj);
}

void CCJSONObject::addNull(const char* key) {
	// basic validation
	if(!key)
		return;

	const char* k = CCUtils::copy(key);
	KeyValue kv = {
			k,
			NIL,
			{ false }
	};
	m_pairs[k] = kv;
	m_keyvalues.push_back(kv);
}

void CCJSONObject::addBool(const char* key, bool b) {
	// basic validation
	if(!key)
		return;

	Value v;
	v.b = b;
	const char* k = CCUtils::copy(key);
	KeyValue kv = {
			k,
			BOOLEAN,
			v
	};
	m_pairs[k] = kv;
	m_keyvalues.push_back(kv);
}

void CCJSONObject::addInt(const char* key, int i) {
	// basic validation
	if(!key)
		return;

	char buf[32];
	sprintf(buf, "%d", i);
	Value v;
	v.s = CCUtils::copy(buf);
	const char* k = CCUtils::copy(key);
	KeyValue kv = {
			k,
			STRING,
			v
	};
	m_pairs[k] = kv;
	m_keyvalues.push_back(kv);
}

void CCJSONObject::addLong(const char* key, long l) {
	// basic validation
	if(!key)
		return;

	char buf[32];
	sprintf(buf, "%ld", l);
	Value v;
	v.s = CCUtils::copy(buf);
	const char* k = CCUtils::copy(key);
	KeyValue kv = {
			k,
			STRING,
			v
	};
	m_pairs[k] = kv;
	m_keyvalues.push_back(kv);
}

void CCJSONObject::addFloat(const char* key, float f) {
	// basic validation
	if(!key)
		return;

	char buf[32];
	sprintf(buf, "%f", f);
	Value v;
	v.s = CCUtils::copy(buf);
	const char* k = CCUtils::copy(key);
	KeyValue kv = {
			k,
			STRING,
			v
	};
	m_pairs[k] = kv;
	m_keyvalues.push_back(kv);
}

void CCJSONObject::addDouble(const char* key, double d) {
	// basic validation
	if(!key)
		return;

	char buf[32];
	sprintf(buf, "%lf", d);
	Value v;
	v.s = CCUtils::copy(buf);
	const char* k = CCUtils::copy(key);
	KeyValue kv = {
			k,
			STRING,
			v
	};
	m_pairs[k] = kv;
	m_keyvalues.push_back(kv);
}

void CCJSONObject::addString(const char* key, const char* s) {
	// basic validation
	if(!key)
		return;

	Value v;
	v.s = CCUtils::copy(s);
	const char* k = CCUtils::copy(key);
	KeyValue kv = {
			k,
			STRING,
			v
	};
	m_pairs[k] = kv;
	m_keyvalues.push_back(kv);
}

void CCJSONObject::addObject(const char* key, CCJSONObject* jo) {
	// basic validation
	if(!key)
		return;

	Value v;
	v.jo = jo;
	const char* k = CCUtils::copy(key);
	KeyValue kv = {
			k,
			OBJECT,
			v
	};
	CC_SAFE_RETAIN(jo);
	m_pairs[k] = kv;
	m_keyvalues.push_back(kv);
}

void CCJSONObject::addArray(const char* key, CCJSONArray* ja) {
	// basic validation
	if(!key)
		return;

	Value v;
	v.ja = ja;
	const char* k = CCUtils::copy(key);
	KeyValue kv = {
			k,
			ARRAY,
			v
	};
	CC_SAFE_RETAIN(ja);
	m_pairs[k] = kv;
	m_keyvalues.push_back(kv);
}

bool CCJSONObject::optBool(const char* key, bool def) {
	PairMap::iterator iter = m_pairs.find(key);
	if(iter != m_pairs.end()) {
		return CCJSONValue::castToBool(iter->second);
	} else {
		return def;
	}
}

int CCJSONObject::optInt(const char* key, int def) {
	PairMap::iterator iter = m_pairs.find(key);
	if(iter != m_pairs.end()) {
		return CCJSONValue::castToInt(iter->second);
	} else {
		return def;
	}
}

long CCJSONObject::optLong(const char* key, long def) {
	PairMap::iterator iter = m_pairs.find(key);
	if(iter != m_pairs.end()) {
		return CCJSONValue::castToLong(iter->second);
	} else {
		return def;
	}
}

float CCJSONObject::optFloat(const char* key, float def) {
	PairMap::iterator iter = m_pairs.find(key);
	if(iter != m_pairs.end()) {
		return CCJSONValue::castToFloat(iter->second);
	} else {
		return def;
	}
}

double CCJSONObject::optDouble(const char* key, double def) {
	PairMap::iterator iter = m_pairs.find(key);
	if(iter != m_pairs.end()) {
		return CCJSONValue::castToDouble(iter->second);
	} else {
		return def;
	}
}

CCJSONObject* CCJSONObject::optJSONObject(const char* key) {
	PairMap::iterator iter = m_pairs.find(key);
	if(iter != m_pairs.end()) {
		return CCJSONValue::castToObject(iter->second);
	} else {
		return NULL;
	}
}

CCJSONArray* CCJSONObject::optJSONArray(const char* key) {
	PairMap::iterator iter = m_pairs.find(key);
	if(iter != m_pairs.end()) {
		return CCJSONValue::castToArray(iter->second);
	} else {
		return NULL;
	}
}

string CCJSONObject::optString(const char* key, const char* def) {
	PairMap::iterator iter = m_pairs.find(key);
	if(iter != m_pairs.end()) {
		return CCJSONValue::castToString(iter->second);
	} else {
		return def;
	}
}

bool CCJSONObject::optBool(int index, bool def) {
	if(index < 0 || index >= m_keyvalues.size()) {
		return def;
	}

	return CCJSONValue::castToBool(m_keyvalues.at(index));
}

int CCJSONObject::optInt(int index, int def) {
	if(index < 0 || index >= m_keyvalues.size()) {
		return def;
	}

	return CCJSONValue::castToInt(m_keyvalues.at(index));
}

long CCJSONObject::optLong(int index, long def) {
	if(index < 0 || index >= m_keyvalues.size()) {
		return def;
	}

	return CCJSONValue::castToLong(m_keyvalues.at(index));
}

float CCJSONObject::optFloat(int index, float def) {
	if(index < 0 || index >= m_keyvalues.size()) {
		return def;
	}

	return CCJSONValue::castToFloat(m_keyvalues.at(index));
}

double CCJSONObject::optDouble(int index, double def) {
	if(index < 0 || index >= m_keyvalues.size()) {
		return def;
	}

	return CCJSONValue::castToDouble(m_keyvalues.at(index));
}

CCJSONObject* CCJSONObject::optJSONObject(int index) {
	if(index < 0 || index >= m_keyvalues.size()) {
		return NULL;
	}

	return CCJSONValue::castToObject(m_keyvalues.at(index));
}

CCJSONArray* CCJSONObject::optJSONArray(int index) {
	if(index < 0 || index >= m_keyvalues.size()) {
		return NULL;
	}

	return CCJSONValue::castToArray(m_keyvalues.at(index));
}

string CCJSONObject::optString(int index, const char* def) {
	if(index < 0 || index >= m_keyvalues.size()) {
		return def;
	}

	return CCJSONValue::castToString(m_keyvalues.at(index));
}

const char* CCJSONObject::keyAt(int index) {
	if(index < 0 || index >= m_keyvalues.size()) {
		return NULL;
	}

	return m_keyvalues.at(index).k;
}

string CCJSONObject::toString() {
    CCMemoryOutputStream mos;
    output(&mos);
    return string(mos.getBuffer(), mos.getLength());
}

void CCJSONObject::output(CCAssetOutputStream* aos, int level) {
	// bracket of object
	s_buf[0] = '{';
	s_buf[1] = '\n';
	aos->write(s_buf, 2);

	int count = 0;
	for(PairMap::iterator iter = m_pairs.begin(); iter != m_pairs.end(); iter++) {
		// output key indentation
		for(int i = 0; i < level; i++) {
			s_buf[i] = '\t';
		}
		aos->write(s_buf, level);

		// output key
		sprintf(s_buf, "\"%s\" : ", iter->second.k);
		aos->write(s_buf, strlen(s_buf));

		switch(iter->second.t) {
			case OBJECT:
				// output object
				iter->second.v.jo->output(aos, level + 1);

				// comma
				if(count != m_pairs.size() - 1)
					aos->write(",", 1);

				break;
			case ARRAY:
				// output array
				iter->second.v.ja->output(aos, level + 1);

				// comma
				if(count != m_pairs.size() - 1)
					aos->write(",", 1);

				break;
			case BOOLEAN:
				// output value
				if(count == m_pairs.size() - 1)
					sprintf(s_buf, "%s\n", iter->second.v.b ? "true" : "false");
				else
					sprintf(s_buf, "%s,\n", iter->second.v.b ? "true" : "false");
				aos->write(s_buf, strlen(s_buf));
				break;
			default:
				// output value
				if(count == m_pairs.size() - 1)
					sprintf(s_buf, "\"%s\"\n", CCJSONValue::castToString(iter->second));
				else
					sprintf(s_buf, "\"%s\",\n", CCJSONValue::castToString(iter->second));
				aos->write(s_buf, strlen(s_buf));
				break;
		}

		count++;
	}

	// end bracket indentation
	for(int i = 0; i < level - 1; i++) {
		s_buf[i] = '\t';
	}
	aos->write(s_buf, level - 1);

	// end bracket
	s_buf[0] = '}';
	s_buf[1] = '\n';
	aos->write(s_buf, 2);
}

NS_CC_END