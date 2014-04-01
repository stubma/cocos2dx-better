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
#ifndef __CCSecureUserDefault__
#define __CCSecureUserDefault__

#include "cocos2d.h"
#include "ccMoreTypes.h"

using namespace std;

NS_CC_BEGIN

/**
 * You can set a decrypt and encrypt method for it. If not set, it is just same as
 * CCUserDefault
 */
class CC_DLL CCSecureUserDefault {	
private:
	CC_DECRYPT_FUNC m_decryptFunc;
	CC_ENCRYPT_FUNC m_encryptFunc;
	
protected:
	CCSecureUserDefault();
	
	/// get decrypted value from defaults
	const char* getSecureValue(const char* pKey, int* outLen);
	
public:
	virtual ~CCSecureUserDefault();
	
	/// get singleton
	static CCSecureUserDefault* getInstance();
	
	/// init
	static void init(CC_ENCRYPT_FUNC eFunc, CC_DECRYPT_FUNC dFunc);
	
	/// default all defaults
    static void purge();
	
	// get methods
    bool getBoolForKey(const char* pKey);
    bool getBoolForKey(const char* pKey, bool defaultValue);
    int getIntegerForKey(const char* pKey);
    int getIntegerForKey(const char* pKey, int defaultValue);
    float getFloatForKey(const char* pKey);
    float getFloatForKey(const char* pKey, float defaultValue);
    double getDoubleForKey(const char* pKey);
    double getDoubleForKey(const char* pKey, double defaultValue);
    string getStringForKey(const char* pKey);
    string getStringForKey(const char* pKey, const string& defaultValue);
	
	// set methods
    void setBoolForKey(const char* pKey, bool value);
    void setIntegerForKey(const char* pKey, int value);
    void setFloatForKey(const char* pKey, float value);
    void setDoubleForKey(const char* pKey, double value);
    void setStringForKey(const char* pKey, const string& value);
	
	// flush it
    void flush();
};

NS_CC_END

#endif /* defined(__CCSecureUserDefault__) */
