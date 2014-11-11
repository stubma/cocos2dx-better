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
#ifndef __CBHttpResponse__
#define __CBHttpResponse__

#include "cocos2d.h"
#include "CBHttpRequest.h"
#include "CBData.h"
#include "CCMoreMacros.h"

using namespace std;

NS_CC_BEGIN

/**
 * Http response
 */
class CC_DLL CBHttpResponse : public CCObject {
private:
    CBData m_errorData;
    
public:
    CBHttpResponse(CBHttpRequest* request) :
    m_success(false),
    m_request(NULL),
    m_responseCode(0) {
        m_request = request;
        CC_SAFE_RETAIN(m_request);
    }
    
    bool initWithRequest(CBHttpRequest* request);
    
    virtual ~CBHttpResponse() {
        CC_SAFE_RELEASE(m_request);
    }
    
    /// get error data
    inline const CBData& getErrorData() {
        return m_errorData;
    }
    
    /// set error data
    inline void setErrorData(char* buf) {
        m_errorData.appendBytes((uint8_t*)buf, strlen(buf));
    }
    
    /// add header
    inline void addHeader(const string& name, const string& value) {
        CCString* v = new CCString(CCUtils::trim(value));
        m_headers.setObject(v, name);
        v->release();
    }
    
    /// get header
    inline string getHeader(const string& name) {
        CCString* value = (CCString*)m_headers.objectForKey(name);
        return value ? value->getCString() : "";
    }
    
    /// data segment of last receive, it is just used for notification
    CC_SYNTHESIZE(CBData*, m_data, Data);
    
    /// request
    CC_SYNTHESIZE_READONLY(CBHttpRequest*, m_request, Request);
    
    /// success flag
    CC_SYNTHESIZE_BOOL(m_success, Success);
    
    /// response code
    CC_SYNTHESIZE(int, m_responseCode, ResponseCode);
    
    /// header map
    CC_SYNTHESIZE_PASS_BY_REF(CCDictionary, m_headers, Headers);
};

NS_CC_END

#endif //__CBHttpResponse__
