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
#include "CCJSONArray.h"
#include "CCJSONValue.h"
#include "CCJSONParser.h"
#include "CCUtils.h"
#include "CCMemoryOutputStream.h"

NS_CC_BEGIN

// tmp buffer
static char s_buf[4096];

CCJSONArray::CCJSONArray() {
}

CCJSONArray::~CCJSONArray() {
	for(vector<CCJSONObject::KeyValue>::iterator iter = m_array.begin(); iter != m_array.end(); iter++) {
		releaseKeyValue(*iter);
	}
}

CCJSONArray* CCJSONArray::create() {
	CCJSONArray* a = new CCJSONArray();
	return (CCJSONArray*)a->autorelease();
}

CCJSONArray* CCJSONArray::create(const string& path) {
	CCObject* obj = CCJSONParser::load(path);
	return safeCast(obj);
}

CCJSONArray* CCJSONArray::create(const char* json, size_t length) {
	CCObject* obj = CCJSONParser::load(json, length);
	return safeCast(obj);
}

CCJSONArray* CCJSONArray::safeCast(CCObject* obj) {
	CCJSONArray* ja = dynamic_cast<CCJSONArray*>(obj);
	if(ja) {
		return ja;
	} else {
		CCJSONObject* jo = dynamic_cast<CCJSONObject*>(obj);
		if(jo) {
			CCJSONArray* ja = CCJSONArray::create();
			ja->addObject(jo);
			return ja;
		} else {
			return NULL;
		}
	}
}

void CCJSONArray::releaseKeyValue(CCJSONObject::KeyValue& kv) {
	switch(kv.t) {
		case CCJSONObject::STRING:
			free((void*)kv.v.s);
			break;
		case CCJSONObject::OBJECT:
			CC_SAFE_RELEASE(kv.v.jo);
			break;
		case CCJSONObject::ARRAY:
			CC_SAFE_RELEASE(kv.v.ja);
			break;
        default:
            break;
	}
	if(kv.k)
		free((void*)kv.k);
}

void CCJSONArray::addNull() {
	CCJSONObject::Value v;
	memset(&v, 0, sizeof(CCJSONObject::Value));
	CCJSONObject::KeyValue kv = {
			NULL,
			CCJSONObject::NIL,
			v
	};
	m_array.push_back(kv);
}

void CCJSONArray::addBool(bool b) {
	CCJSONObject::Value v;
	v.b = b;
	CCJSONObject::KeyValue kv = {
			NULL,
			CCJSONObject::BOOLEAN,
			v
	};
	m_array.push_back(kv);
}

void CCJSONArray::addInt(int i) {
	char buf[32];
	sprintf(buf, "%d", i);
	CCJSONObject::Value v;
	v.s = CCUtils::copy(buf);
	CCJSONObject::KeyValue kv = {
			NULL,
			CCJSONObject::STRING,
			v
	};
	m_array.push_back(kv);
}

void CCJSONArray::addLong(long l) {
	char buf[32];
	sprintf(buf, "%ld", l);
	CCJSONObject::Value v;
	v.s = CCUtils::copy(buf);
	CCJSONObject::KeyValue kv = {
			NULL,
			CCJSONObject::STRING,
			v
	};
	m_array.push_back(kv);
}

void CCJSONArray::addFloat(float f) {
	char buf[32];
	sprintf(buf, "%f", f);
	CCJSONObject::Value v;
	v.s = CCUtils::copy(buf);
	CCJSONObject::KeyValue kv = {
			NULL,
			CCJSONObject::STRING,
			v
	};
	m_array.push_back(kv);
}

void CCJSONArray::addDouble(double d) {
	char buf[32];
	sprintf(buf, "%lf", d);
	CCJSONObject::Value v;
	v.s = CCUtils::copy(buf);
	CCJSONObject::KeyValue kv = {
			NULL,
			CCJSONObject::STRING,
			v
	};
	m_array.push_back(kv);
}

void CCJSONArray::addString(const char* s) {
	CCJSONObject::Value v;
	v.s = CCUtils::copy(s);
	CCJSONObject::KeyValue kv = {
			NULL,
			CCJSONObject::STRING,
			v
	};
	m_array.push_back(kv);
}

void CCJSONArray::addObject(CCJSONObject* jo) {
	CCJSONObject::Value v;
	v.jo = jo;
	CCJSONObject::KeyValue kv = {
			NULL,
			CCJSONObject::OBJECT,
			v
	};
	CC_SAFE_RETAIN(jo);
	m_array.push_back(kv);
}

void CCJSONArray::addArray(CCJSONArray* ja) {
	CCJSONObject::Value v;
	v.ja = ja;
	CCJSONObject::KeyValue kv = {
			NULL,
			CCJSONObject::ARRAY,
			v
	};
	CC_SAFE_RETAIN(ja);
	m_array.push_back(kv);
}

bool CCJSONArray::optBool(int index, bool def) {
	if(index < 0 || index >= m_array.size()) {
		return def;
	}

	return CCJSONValue::castToBool(m_array.at(index));
}

int CCJSONArray::optInt(int index, int def) {
	if(index < 0 || index >= m_array.size()) {
		return def;
	}

	return CCJSONValue::castToInt(m_array.at(index));
}

long CCJSONArray::optLong(int index, long def) {
	if(index < 0 || index >= m_array.size()) {
		return def;
	}

	return CCJSONValue::castToLong(m_array.at(index));
}

float CCJSONArray::optFloat(int index, float def) {
	if(index < 0 || index >= m_array.size()) {
		return def;
	}

	return CCJSONValue::castToFloat(m_array.at(index));
}

double CCJSONArray::optDouble(int index, double def) {
	if(index < 0 || index >= m_array.size()) {
		return def;
	}

	return CCJSONValue::castToDouble(m_array.at(index));
}

CCJSONObject* CCJSONArray::optJSONObject(int index) {
	if(index < 0 || index >= m_array.size()) {
		return NULL;
	}

	return CCJSONValue::castToObject(m_array.at(index));
}

CCJSONArray* CCJSONArray::optJSONArray(int index) {
	if(index < 0 || index >= m_array.size()) {
		return NULL;
	}

	return CCJSONValue::castToArray(m_array.at(index));
}

string CCJSONArray::optString(int index, const char* def) {
	if(index < 0 || index >= m_array.size()) {
		return def;
	}

	return CCJSONValue::castToString(m_array.at(index));
}

string CCJSONArray::toString() {
    CCMemoryOutputStream mos;
    output(&mos);
    return string(mos.getBuffer(), mos.getLength());
}

void CCJSONArray::output(CCAssetOutputStream* aos, int level) {
	// bracket of array
	s_buf[0] = '[';
	s_buf[1] = '\n';
	aos->write(s_buf, 2);

	for(vector<CCJSONObject::KeyValue>::iterator iter = m_array.begin(); iter != m_array.end(); iter++) {
		// output key indentation
		for(int i = 0; i < level; i++) {
			s_buf[i] = '\t';
		}
		aos->write(s_buf, level);

		switch(iter->t) {
			case CCJSONObject::OBJECT:
				iter->v.jo->output(aos, level + 1);

				// comma
				if(m_array.end() - iter != 1)
					aos->write(",", 1);

				break;
			case CCJSONObject::ARRAY:
				iter->v.ja->output(aos, level + 1);

				// comma
				if(m_array.end() - iter != 1)
					aos->write(",", 1);

				break;
			case CCJSONObject::BOOLEAN:
				// output value
				if(m_array.end() - iter == 1)
					sprintf(s_buf, "%s\n", iter->v.b ? "true" : "false");
				else
					sprintf(s_buf, "%s,\n", iter->v.b ? "true" : "false");
				aos->write(s_buf, strlen(s_buf));
				break;
			default:
				// output value
				if(m_array.end() - iter == 1)
					sprintf(s_buf, "\"%s\"\n", CCJSONValue::castToString(*iter));
				else
					sprintf(s_buf, "\"%s\",\n", CCJSONValue::castToString(*iter));
				aos->write(s_buf, strlen(s_buf));
				break;
		}
	}

	// end bracket indentation
	for(int i = 0; i < level - 1; i++) {
		s_buf[i] = '\t';
	}
	aos->write(s_buf, level - 1);

	// end bracket
	s_buf[0] = ']';
	s_buf[1] = '\n';
	aos->write(s_buf, 2);
}

NS_CC_END