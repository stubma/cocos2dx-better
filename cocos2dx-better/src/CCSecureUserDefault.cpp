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
#include "CCSecureUserDefault.h"
#include "CCBase64.h"

NS_CC_BEGIN

static CCSecureUserDefault* s_instance = NULL;

CCSecureUserDefault::CCSecureUserDefault() {
}

CCSecureUserDefault::~CCSecureUserDefault() {
	s_instance = NULL;
}

CCSecureUserDefault* CCSecureUserDefault::getInstance() {
	if(!s_instance) {
		s_instance = new CCSecureUserDefault();
	}
	return s_instance;
}

void CCSecureUserDefault::init(CC_ENCRYPT_FUNC eFunc, CC_DECRYPT_FUNC dFunc) {
	CCSecureUserDefault* d = CCSecureUserDefault::getInstance();
	d->m_encryptFunc = eFunc;
	d->m_decryptFunc = dFunc;
}

void CCSecureUserDefault::purge() {
	delete s_instance;
}

const char* CCSecureUserDefault::getSecureValue(const char* pKey, int* outLen) {
	string v = CCUserDefault::sharedUserDefault()->getStringForKey(pKey);
	int len;
	const char* dec = CCBase64::decodeAsCString(v, &len);
	const char* plain = dec ? (*m_decryptFunc)(dec, len, outLen) : NULL;
	
	// free dec
	if(dec && plain != dec)
		free((void*)dec);
	
	return plain;
}

bool CCSecureUserDefault::getBoolForKey(const char* pKey) {
	return getBoolForKey(pKey, false);
}

bool CCSecureUserDefault::getBoolForKey(const char* pKey, bool defaultValue) {
	if(m_decryptFunc) {
		// for bool, we save "true" string
        int len;
		const char* plain = getSecureValue(pKey, &len);
		bool result = defaultValue;
		if(plain) {
            result = !strcmp(plain, "true");
        }
		
		// free
		if(plain)
			free((void*)plain);
		
		// return
		return result;
	} else {
		return CCUserDefault::sharedUserDefault()->getBoolForKey(pKey, defaultValue);
	}
}

int CCSecureUserDefault::getIntegerForKey(const char* pKey) {
	return getIntegerForKey(pKey, 0);
}

int CCSecureUserDefault::getIntegerForKey(const char* pKey, int defaultValue) {
	if(m_decryptFunc) {
		// for integer, we save number string
        int len;
		const char* plain = getSecureValue(pKey, &len);
		int result = defaultValue;
		if(plain) {
			sscanf(plain, "%d", &result);
		}
		
		// free
		if(plain)
			free((void*)plain);
		
		// return
		return result;
	} else {
		return CCUserDefault::sharedUserDefault()->getIntegerForKey(pKey, defaultValue);
	}
}

float CCSecureUserDefault::getFloatForKey(const char* pKey) {
	return getFloatForKey(pKey, 0);
}

float CCSecureUserDefault::getFloatForKey(const char* pKey, float defaultValue) {
	if(m_decryptFunc) {
		// for integer, we save number string
        int len;
		const char* plain = getSecureValue(pKey, &len);
		float result = defaultValue;
		if(plain) {
			sscanf(plain, "%f", &result);
		}
		
		// free
		if(plain)
			free((void*)plain);
		
		// return
		return result;
	} else {
		return CCUserDefault::sharedUserDefault()->getFloatForKey(pKey, defaultValue);
	}
}

double CCSecureUserDefault::getDoubleForKey(const char* pKey) {
	return getDoubleForKey(pKey, 0);
}

double CCSecureUserDefault::getDoubleForKey(const char* pKey, double defaultValue) {
	if(m_decryptFunc) {
		// for integer, we save number string
        int len;
		const char* plain = getSecureValue(pKey, &len);
		double result = defaultValue;
		if(plain) {
			sscanf(plain, "%lf", &result);
		}
		
		// free
		if(plain)
			free((void*)plain);
		
		// return
		return result;
	} else {
		return CCUserDefault::sharedUserDefault()->getDoubleForKey(pKey, defaultValue);
	}
}

string CCSecureUserDefault::getStringForKey(const char* pKey) {
	return getStringForKey(pKey, "");
}

string CCSecureUserDefault::getStringForKey(const char* pKey, const string& defaultValue) {
	if(m_decryptFunc) {
		// for integer, we save number string
        int len;
		const char* plain = getSecureValue(pKey, &len);
		if(plain) {
            string ret = plain;
			free((void*)plain);
			return ret;
		} else {
			return defaultValue;
		}
	} else {
		return CCUserDefault::sharedUserDefault()->getStringForKey(pKey, defaultValue);
	}
}

void CCSecureUserDefault::setBoolForKey(const char* pKey, bool value) {
	if(m_encryptFunc) {
		int encLen;
		const char* enc = (*m_encryptFunc)(value ? "true" : "false", value ? 4 : 5, &encLen);
		string b64 = CCBase64::encode(enc, encLen);
		CCUserDefault::sharedUserDefault()->setStringForKey(pKey, b64);
        
        // free
        if(enc)
            free((void*)enc);
	} else {
		CCUserDefault::sharedUserDefault()->setBoolForKey(pKey, value);
	}
}

void CCSecureUserDefault::setIntegerForKey(const char* pKey, int value) {
	if(m_encryptFunc) {
		char buf[32];
		sprintf(buf, "%d", value);
		int encLen;
		const char* enc = (*m_encryptFunc)(buf, (int)strlen(buf), &encLen);
		string b64 = CCBase64::encode(enc, encLen);
		CCUserDefault::sharedUserDefault()->setStringForKey(pKey, b64);
        
        // free
        if(enc && buf != enc)
            free((void*)enc);
	} else {
		CCUserDefault::sharedUserDefault()->setIntegerForKey(pKey, value);
	}
}

void CCSecureUserDefault::setFloatForKey(const char* pKey, float value) {
	if(m_encryptFunc) {
		char buf[64];
		sprintf(buf, "%f", value);
		int encLen;
		const char* enc = (*m_encryptFunc)(buf, (int)strlen(buf), &encLen);
		string b64 = CCBase64::encode(enc, encLen);
		CCUserDefault::sharedUserDefault()->setStringForKey(pKey, b64);
        
        // free
        if(enc && buf != enc)
            free((void*)enc);
	} else {
		CCUserDefault::sharedUserDefault()->setFloatForKey(pKey, value);
	}
}

void CCSecureUserDefault::setDoubleForKey(const char* pKey, double value) {
	if(m_encryptFunc) {
		char buf[128];
		sprintf(buf, "%lf", value);
		int encLen;
		const char* enc = (*m_encryptFunc)(buf, (int)strlen(buf), &encLen);
		string b64 = CCBase64::encode(enc, encLen);
		CCUserDefault::sharedUserDefault()->setStringForKey(pKey, b64);
        
        // free
        if(enc && buf != enc)
            free((void*)enc);
	} else {
		CCUserDefault::sharedUserDefault()->setDoubleForKey(pKey, value);
	}
}

void CCSecureUserDefault::setStringForKey(const char* pKey, const string& value) {
	if(m_encryptFunc) {
		int encLen;
		const char* enc = (*m_encryptFunc)(value.c_str(), (int)value.length(), &encLen);
		string b64 = CCBase64::encode(enc, encLen);
		CCUserDefault::sharedUserDefault()->setStringForKey(pKey, b64);
        
        // free
        if(enc && value.c_str() != enc)
            free((void*)enc);   
	} else {
		CCUserDefault::sharedUserDefault()->setStringForKey(pKey, value);
	}
}

void CCSecureUserDefault::flush() {
	CCUserDefault::sharedUserDefault()->flush();
}

NS_CC_END