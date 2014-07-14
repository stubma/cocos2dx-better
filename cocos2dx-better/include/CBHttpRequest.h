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
#ifndef __CBHttpRequest__
#define __CBHttpRequest__

#include "cocos2d.h"
#include "CCMoreMacros.h"
#include "CBData.h"
#include "CCUtils.h"

using namespace std;

NS_CC_BEGIN

/**
 * Http request
 */
class CC_DLL CBHttpRequest : public CCObject {
public:
    /// http method definition
    typedef enum {
        kHttpGet,
        kHttpPost,
        kHttpPut,
        kHttpDelete,
        kHttpUnknown,
    } HttpMethod;
    
    CBHttpRequest() {
        m_method = kHttpUnknown;
        m_requestData = NULL;
        m_userData = NULL;
        m_tag = -1;
        m_cancel = false;
    }
    
    /** Destructor */
    virtual ~CBHttpRequest() {
        CC_SAFE_RELEASE(m_requestData);
    }
    
    static CBHttpRequest* create() {
        CBHttpRequest* req = new CBHttpRequest();
        return (CBHttpRequest*)req->autorelease();
    }
    
    /// http method
    CC_SYNTHESIZE(HttpMethod, m_method, Method);
    
    /// url string
    CC_SYNTHESIZE_PASS_BY_REF(string, m_url, Url);
    
    /// tag if you want to identify request
    CC_SYNTHESIZE_PASS_BY_REF(int, m_tag, Tag);
    
    /// custom data, request doesn't retain it
    CC_SYNTHESIZE(void*, m_userData, UserData);
    
    /// request data for post and put
    CC_SYNTHESIZE_RETAIN(CBData*, m_requestData, RequestData);
    
    /// header list
    CC_SYNTHESIZE_PASS_BY_REF(vector<string>, m_headers, Headers);
    
    /// cancel flag, if set, http operation will abort if possible
    CC_SYNTHESIZE_BOOL(m_cancel, Cancel);
};

NS_CC_END

#endif //__CBHttpRequest__
