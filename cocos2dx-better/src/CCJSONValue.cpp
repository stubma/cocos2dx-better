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
#include "CCJSONValue.h"
#include <stdio.h>

NS_CC_BEGIN

// hold tmp value need to be returned
static char s_buf[64 * 1024];

bool CCJSONValue::castToBool(CCJSONObject::KeyValue& kv) {
	switch(kv.t) {
		case CCJSONObject::NIL:
			return false;
		case CCJSONObject::BOOLEAN:
			return kv.v.b;
		case CCJSONObject::STRING:
			return !strcmp("true", kv.v.s);
		case CCJSONObject::OBJECT:
			return kv.v.jo != NULL;
		case CCJSONObject::ARRAY:
			return kv.v.ja != NULL;
		default:
			CCLOGERROR("CCJSONValue::castToBool: unexpected json value type: %d", kv.t);
			return false;
	}
}

int CCJSONValue::castToInt(CCJSONObject::KeyValue& kv) {
	switch(kv.t) {
		case CCJSONObject::NIL:
			return 0;
		case CCJSONObject::BOOLEAN:
			return kv.v.b ? 1 : 0;
		case CCJSONObject::STRING:
			return atoi(kv.v.s);
		case CCJSONObject::OBJECT:
			return *((int*)&kv.v.jo);
		case CCJSONObject::ARRAY:
			return *((int*)&kv.v.ja);
		default:
			CCLOGERROR("CCJSONValue::castToInt: unexpected json value type: %d", kv.t);
			return 0;
	}
}

long CCJSONValue::castToLong(CCJSONObject::KeyValue& kv) {
	switch(kv.t) {
		case CCJSONObject::NIL:
			return 0;
		case CCJSONObject::BOOLEAN:
			return kv.v.b ? 1 : 0;
		case CCJSONObject::STRING:
			return atoi(kv.v.s);
		case CCJSONObject::OBJECT:
			return (long)kv.v.jo;
		case CCJSONObject::ARRAY:
			return (long)kv.v.ja;
		default:
			CCLOGERROR("CCJSONValue::castToLong: unexpected json value type: %d", kv.t);
			return 0;
	}
}

float CCJSONValue::castToFloat(CCJSONObject::KeyValue& kv) {
	switch(kv.t) {
		case CCJSONObject::NIL:
			return 0;
		case CCJSONObject::BOOLEAN:
			return kv.v.b ? 1 : 0;
		case CCJSONObject::STRING:
			return atof(kv.v.s);
		case CCJSONObject::OBJECT:
			return (float)*((int*)&kv.v.jo);
		case CCJSONObject::ARRAY:
			return (float)*((int*)&kv.v.ja);
		default:
			CCLOGERROR("CCJSONValue::castToFloat: unexpected json value type: %d", kv.t);
			return 0;
	}
}

double CCJSONValue::castToDouble(CCJSONObject::KeyValue& kv) {
	switch(kv.t) {
		case CCJSONObject::NIL:
			return 0;
		case CCJSONObject::BOOLEAN:
			return kv.v.b ? 1 : 0;
		case CCJSONObject::STRING:
			return atof(kv.v.s);
		case CCJSONObject::OBJECT:
			return (double)*((int*)&kv.v.jo);
		case CCJSONObject::ARRAY:
			return (double)*((int*)&kv.v.ja);
		default:
			CCLOGERROR("CCJSONValue::castToDouble: unexpected json value type: %d", kv.t);
			return 0;
	}
}

CCJSONObject* CCJSONValue::castToObject(CCJSONObject::KeyValue& kv) {
	switch(kv.t) {
		case CCJSONObject::NIL:
		case CCJSONObject::BOOLEAN:
		case CCJSONObject::STRING:
		case CCJSONObject::ARRAY:
			return NULL;
		case CCJSONObject::OBJECT:
			return kv.v.jo;
		default:
			CCLOGERROR("CCJSONValue::castToObject: unexpected json value type: %d", kv.t);
			return 0;
	}
}

CCJSONArray* CCJSONValue::castToArray(CCJSONObject::KeyValue& kv) {
	switch(kv.t) {
		case CCJSONObject::NIL:
		case CCJSONObject::BOOLEAN:
		case CCJSONObject::STRING:
		case CCJSONObject::OBJECT:
			return NULL;
		case CCJSONObject::ARRAY:
			return kv.v.ja;
		default:
			CCLOGERROR("CCJSONValue::castToArray: unexpected json value type: %d", kv.t);
			return 0;
	}
}

const char* CCJSONValue::castToString(CCJSONObject::KeyValue& kv) {
	switch(kv.t) {
		case CCJSONObject::NIL:
			return NULL;
		case CCJSONObject::BOOLEAN:
			if(kv.v.b) {
				sprintf(s_buf, "true");
			} else {
				sprintf(s_buf, "false");
			}
			return s_buf;
		case CCJSONObject::STRING:
			return kv.v.s;
		case CCJSONObject::OBJECT:
			sprintf(s_buf, "CCJSONObject");
			return s_buf;
		case CCJSONObject::ARRAY:
			sprintf(s_buf, "CCJSONArray");
			return s_buf;
		default:
			CCLOGERROR("CCJSONValue::castToString: unexpected json value type: %d", kv.t);
			return NULL;
	}
}

NS_CC_END