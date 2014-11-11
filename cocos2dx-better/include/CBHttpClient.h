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
#ifndef __CBHttpClient__
#define __CBHttpClient__

#include "cocos2d.h"
#include "CBHttpRequest.h"
#include "CBHttpResponse.h"
#include <pthread.h>

NS_CC_BEGIN

/// object is a CBHttpResponse, you should check success flag of it
#define kCCNotificationHttpRequestCompleted "kCCNotificationHttpRequestCompleted"

/// object is a CBHttpResponse, you can get data segment from it
#define kCCNotificationHttpDataReceived "kCCNotificationHttpDataReceived"

/// when header is all received but before data transimitting. object is a CBHttpResponse and you can get header from it
#define kCCNotificationHttpDidReceiveResponse "kCCNotificationHttpDidReceiveResponse"

/**
 * A customized version of cocos2d-x extension CBHttpClient class. 
 * What I did:
 * 1. can get callback during operation, not only when done
 * 2. request can be cancelled
 * 3. not singleton any more
 * 4. changed to cocos2d namespace
 *
 * \par
 * You don't need hold a http client, the http request will be executed in a thread so retaining a client
 * instance or not doesn't matter.
 *
 * \note
 * Using CB prefix to avoid name conflict, CB stands for cocos2dx-better. When you see a class starts with CB,
 * you should know it is a rewriten class which is better than the original.
 */
class CC_DLL CBHttpClient : public CCObject {
private:
    /// thread entry
    static void* httpThreadEntry(void* arg);
    
    /// handler map, key is tag
    vector<void*> m_activeContexts;
    
protected:
    CBHttpClient();
    
public:
    virtual ~CBHttpClient();
    static CBHttpClient* create();
    
    /// init
    virtual bool init();
    
    /**
     * execute a request in a sub thread
     *
     * @param request a CBHttpRequest object, which includes url, response callback etc.
     *      please make sure request->_requestData is clear before calling "execute" here.
     */
    void asyncExecute(CBHttpRequest* request);
    
    /// cancel one request which has specified tag
    void cancel(int tag);
    
    /// stop all ongoing http operation
    void cancelAll();
  
    /// connect timeout
    CC_SYNTHESIZE(float, m_connectTimeout, ConnectTimeout);
    
    /// read timeout
    CC_SYNTHESIZE(float, m_readTimeout, ReadTimeout);
};

NS_CC_END

#endif //__CBHttpClient__
