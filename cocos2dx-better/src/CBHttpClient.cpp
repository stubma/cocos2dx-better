/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2012 greathqy
 
 http://www.cocos2d-x.org
 
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

#include "CBHttpClient.h"
#include <queue>
#include <errno.h>
#include "curl/curl.h"
#include <pthread.h>
#include "CBData.h"

using namespace std;

NS_CC_BEGIN

class CURLHandler;

/// context info
typedef struct {
    CBHttpRequest* request;
    CBHttpResponse* response;
    CURLHandler* curl;
    float connectTimeout;
    float readTimeout;
} ccHttpContext;

/// class which really perform http operation
class CURLHandler : public CCObject {
public:
    /// Instance of CURL
    CURL* m_curl;
    
    /// Keeps custom header data
    curl_slist* m_headers;
    
    /// error buffer
    char m_errorBuffer[CURL_ERROR_SIZE];
    
    /// context
    ccHttpContext* m_ctx;
    
    /// response code
    int32_t m_responseCode;
    
    /// mutex
    pthread_mutex_t m_mutex;
    
    /// done flag
    bool m_done;
    
    /// undelivered data
    CBData* m_data;
    
    /// is kCCNotificationHttpDidReceiveResponse delivered?
    bool isHttpDidReceiveResponseDelivered;
    
    /// is header all received?
    bool isHeaderAllReceived;
    
public:
    CURLHandler(ccHttpContext* ctx) :
    m_ctx(NULL),
    m_curl(NULL),
    m_done(false),
    m_responseCode(500),
    isHttpDidReceiveResponseDelivered(false),
    isHeaderAllReceived(false),
    m_headers(NULL) {
        // data
        m_data = new CBData();
        m_ctx = (ccHttpContext*)calloc(1, sizeof(ccHttpContext));
        memcpy(m_ctx, ctx, sizeof(ccHttpContext));
        
        // create mutex
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&m_mutex, &attr);
        pthread_mutexattr_destroy(&attr);
        
        // schedule event dispatch
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(CURLHandler::dispatchNotification), this, 0, kCCRepeatForever, 0, false);
    }
    
    virtual ~CURLHandler() {
        if(m_curl)
            curl_easy_cleanup(m_curl);
        if(m_headers)
            curl_slist_free_all(m_headers);
        CC_SAFE_RELEASE(m_ctx->request);
        CC_SAFE_RELEASE(m_ctx->response);
        CC_SAFE_FREE(m_ctx);
        CC_SAFE_RELEASE(m_data);
        pthread_mutex_destroy(&m_mutex);
    }
    
    /// Callback function used by libcurl for collect response data
    static size_t writeData(void* ptr, size_t size, size_t nmemb, void* userdata) {
        CURLHandler* handler = (CURLHandler*)userdata;
        size_t sizes = size * nmemb;
        
        // add data to the end of recvBuffer
        pthread_mutex_lock(&handler->m_mutex);
        handler->m_data->appendBytes((uint8_t*)ptr, sizes);
        handler->isHeaderAllReceived = true;
        pthread_mutex_unlock(&handler->m_mutex);
        
        // return a value which is different with sizes will abort it
        if(handler->m_ctx->request->isCancel())
            return sizes + 1;
        else
            return sizes;
    }
    
    /// Callback function used by libcurl for collect header data
    static size_t writeHeaderData(void* ptr, size_t size, size_t nmemb, void* userdata) {
        CURLHandler* handler = (CURLHandler*)userdata;
        size_t sizes = size * nmemb;
        
        // lock
        pthread_mutex_lock(&handler->m_mutex);
        
        // parse pair
        string header((const char*)ptr, sizes);
        CCArray* pair = new CCArray();
        if(!header.empty()) {
            // remove head and tailing brace, bracket, parentheses
            size_t start = 0;
            size_t end = header.length() - 1;
            char c = header[start];
            while(c == '{' || c == '[' || c == '(') {
                start++;
                c = header[start];
            }
            c = header[end];
            while(c == '}' || c == ']' || c == ')') {
                end--;
                c = header[end];
            }
            
            // iterate string
            size_t compStart = start;
            for(size_t i = start; i <= end; i++) {
                c = header[i];
                if(c == ':') {
                    CCString* s = new CCString(header.substr(compStart, i - compStart));
                    pair->addObject(s);
                    s->release();
                    compStart = i + 1;
                } else if(c == ' ' || c == '\t' || c == '\r' || c == '\n') {
                    if(compStart == i) {
                        compStart++;
                    }
                }
            }
            
            // last comp
            // or, if last char is separator, append an empty string
            if(compStart <= end) {
                CCString* s = new CCString(header.substr(compStart, end - compStart + 1));
                pair->addObject(s);
                s->release();
            } else if(header[end] == ':') {
                CCString* s = new CCString("");
                pair->addObject(s);
                s->release();
            }
        }
        
        // if pair count is two, means ok
        if(pair->count() == 2) {
            handler->m_ctx->response->addHeader(((CCString*)pair->objectAtIndex(0))->getCString(),
                                                ((CCString*)pair->objectAtIndex(1))->getCString());
        }
        
        // release array
        pair->release();
        
        // unlock
        pthread_mutex_unlock(&handler->m_mutex);
        
        // return a value which is different with sizes will abort it
        if(handler->m_ctx->request->isCancel())
            return sizes + 1;
        else
            return sizes;
    }
    
    bool init() {
        // check
        m_curl = curl_easy_init();
        if (!m_curl)
            return false;
        if (!configureCURL())
            return false;
        
        /* get custom header data (if set) */
       	const vector<string>& headers = m_ctx->request->getHeaders();
        if(!headers.empty()) {
            // append custom headers one by one
            for (vector<string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
                m_headers = curl_slist_append(m_headers, it->c_str());
            
            // set custom headers for curl
            if (curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_headers) != CURLE_OK)
                return false;
        }
        
        return curl_easy_setopt(m_curl, CURLOPT_URL, m_ctx->request->getUrl().c_str()) == CURLE_OK &&
            curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writeData) == CURLE_OK &&
            curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this) == CURLE_OK &&
            curl_easy_setopt(m_curl, CURLOPT_HEADERFUNCTION, writeHeaderData) == CURLE_OK &&
            curl_easy_setopt(m_curl, CURLOPT_HEADERDATA, this) == CURLE_OK;
    }
    
    bool configureCURL() {
        if(curl_easy_setopt(m_curl, CURLOPT_ERRORBUFFER, m_errorBuffer) != CURLE_OK) {
            return false;
        }
        if(curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, m_ctx->readTimeout) != CURLE_OK) {
            return false;
        }
        if(curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, m_ctx->connectTimeout) != CURLE_OK) {
            return false;
        }
        curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, 0L);
        
        // FIXED #3224: The subthread of CBHttpClient interrupts main thread if timeout comes.
        // Document is here: http://curl.haxx.se/libcurl/c/curl_easy_setopt.html#CURLOPTNOSIGNAL
        curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1L);
        
        return true;
    }
    
    bool processGetTask() {
        curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, true);
        return perform();
    }
    
    bool processPostTask() {
        curl_easy_setopt(m_curl, CURLOPT_POST, 1);
        CBData* data = m_ctx->request->getRequestData();
        if(data) {
            curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, data->getBytes());
            curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, data->getSize());
        }
        return perform();
    }
    
    bool processPutTask() {
        curl_easy_setopt(m_curl, CURLOPT_CUSTOMREQUEST, "PUT");
        CBData* data = m_ctx->request->getRequestData();
        if(data) {
            curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, data->getBytes());
            curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, data->getSize());
        }

        return perform();
    }
    
    bool processDeleteTask() {
        curl_easy_setopt(m_curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, true);
        return perform();
    }
    
    bool perform() {
        // if easy perform not ok, return false
        CURLcode code = curl_easy_perform(m_curl);
        if (code != CURLE_OK) {
            CCLOG("curl_easy_perform error: %d, request: %s", code, m_ctx->request->getUrl().c_str());
            return false;
        }
        
        // get return code
        code = curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE, &m_responseCode);
        if (code != CURLE_OK || m_responseCode != 200)
            return false;
        
        return true;
    }
    
    /// called when request is done
    void onRequestDone() {
        pthread_mutex_lock(&m_mutex);
        m_done = true;
        pthread_mutex_unlock(&m_mutex);
    }
    
    /// dispatch end notification
    void dispatchNotification(float delta) {
        // notification center
        CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
        
        // check
        pthread_mutex_lock(&m_mutex);
        
        // header notification
        if(!isHttpDidReceiveResponseDelivered && isHeaderAllReceived) {
            isHttpDidReceiveResponseDelivered = true;
            nc->postNotification(kCCNotificationHttpDidReceiveResponse, m_ctx->response);
        }
        
        // notification
        if(m_data->getSize() > 0) {
            m_ctx->response->setData(m_data);
            nc->postNotification(kCCNotificationHttpDataReceived, m_ctx->response);
            
            // recreate a new data
            CC_SAFE_RELEASE(m_data);
            m_data = new CBData();
        }
        
        // is done?
        if(m_done) {
            // notification
            nc->postNotification(kCCNotificationHttpRequestCompleted, m_ctx->response);
            
            // balance retain in httpThreadEntry
            autorelease();
            
            // unschedule dispatcher
            CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(CURLHandler::dispatchNotification), this);
        }
        
        pthread_mutex_unlock(&m_mutex);
    }
};

void* CBHttpClient::httpThreadEntry(void* arg) {
    // handler, it is released if init failed or released in dispatchNotification
    ccHttpContext* ctx = (ccHttpContext*)arg;
    CURLHandler* curl = ctx->curl;
    if(!curl->init()) {
        CC_SAFE_RELEASE_NULL(curl);
    }
    
    if(curl) {
        // create response
        CBHttpResponse* response = new CBHttpResponse(curl->m_ctx->request);
        curl->m_ctx->response = response;
        
        // process request
        bool retValue = false;
        switch (curl->m_ctx->request->getMethod()) {
            case CBHttpRequest::kHttpGet:
                retValue = curl->processGetTask();
                break;
            case CBHttpRequest::kHttpPost:
                retValue = curl->processPostTask();
                break;
            case CBHttpRequest::kHttpPut:
                retValue = curl->processPutTask();
                break;
            case CBHttpRequest::kHttpDelete:
                retValue = curl->processDeleteTask();
                break;
            default:
                CCAssert(true, "CBHttpClient: unkown request type, only GET and POST are supported");
                break;
        }
        
        // save response
        response->setResponseCode(curl->m_responseCode);
        if (retValue) {
            response->setSuccess(true);
        } else {
            response->setSuccess(false);
            response->setErrorData(curl->m_errorBuffer);
        }
        
        // done
        curl->onRequestDone();
    }

    // exit
    pthread_exit(NULL);
    
    return NULL;
}

CBHttpClient::CBHttpClient() :
m_connectTimeout(30),
m_readTimeout(60) {
}

CBHttpClient::~CBHttpClient() {
    for(vector<void*>::iterator iter = m_activeContexts.begin(); iter != m_activeContexts.end(); iter++) {
        ccHttpContext* ctx = (ccHttpContext*)*iter;
        CC_SAFE_RELEASE(ctx->request);
        CC_SAFE_FREE(ctx);
    }
    m_activeContexts.clear();
}

CBHttpClient* CBHttpClient::create() {
    CBHttpClient* c = new CBHttpClient();
    if(c->init()) {
        return (CBHttpClient*)c->autorelease();
    }
    CC_SAFE_RELEASE(c);
    return NULL;
}

bool CBHttpClient::init() {
    return true;
}

void CBHttpClient::asyncExecute(CBHttpRequest* request) {
    if (!request) {
        return;
    }
    
    // create context
    ccHttpContext* ctx = (ccHttpContext*)calloc(1, sizeof(ccHttpContext));
    ctx->request = request;
    ctx->connectTimeout = m_connectTimeout;
    ctx->readTimeout = m_readTimeout;
    ctx->curl = new CURLHandler(ctx);
    CC_SAFE_RETAIN(request); // release in CURLHandler
    CC_SAFE_RETAIN(request); // release in this
    
    // add to cache and clear useless context
    m_activeContexts.push_back(ctx);

    // start network thread
    pthread_t thread;
    pthread_create(&thread, NULL, httpThreadEntry, ctx);
    pthread_detach(thread);
}

void CBHttpClient::cancel(int tag) {
    for(vector<void*>::iterator iter = m_activeContexts.begin(); iter != m_activeContexts.end(); iter++) {
        ccHttpContext* ctx = (ccHttpContext*)*iter;
        if(ctx->request->getTag() == tag) {
            ctx->request->setCancel(true);
            break;
        }
    }
}

void CBHttpClient::cancelAll() {
    for(vector<void*>::iterator iter = m_activeContexts.begin(); iter != m_activeContexts.end(); iter++) {
        ccHttpContext* ctx = (ccHttpContext*)*iter;
        ctx->request->setCancel(true);
    }
}

NS_CC_END