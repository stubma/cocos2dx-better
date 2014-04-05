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

/// cancel all request
#define kCCNotificationHttpCancelAll "kCCNotificationHttpCancelAll"

/// cancel one request by tag, object is a CCInteger tag
#define kCCNotificationHttpCancelOne "kCCNotificationHttpCancelOne"

/// context info
typedef struct {
    CBHttpRequest* request;
    CBHttpResponse* response;
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
    CURLHandler() :
    m_ctx(NULL),
    m_curl(NULL),
    m_done(false),
    isHttpDidReceiveResponseDelivered(false),
    isHeaderAllReceived(false),
    m_headers(NULL) {
        m_data = new CBData();
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
        
        // listener to some internal notification
        CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
        nc->addObserver(this, callfuncO_selector(CURLHandler::onCancelAll), kCCNotificationHttpCancelAll, nil);
        nc->addObserver(this, callfuncO_selector(CURLHandler::onCancelOne), kCCNotificationHttpCancelOne, nil);
    }
    
    static CURLHandler* create(ccHttpContext* ctx) {
        CURLHandler* curl = new CURLHandler();
        if(curl->initWithContext(ctx)) {
            return (CURLHandler*)curl->autorelease();
        }
        CC_SAFE_RELEASE(curl);
        return NULL;
    }
    
    /// Callback function used by libcurl for collect response data
    static size_t writeData(void* ptr, size_t size, size_t nmemb, void* userdata) {
        CURLHandler* ii = (CURLHandler*)userdata;
        size_t sizes = size * nmemb;
        
        // add data to the end of recvBuffer
        pthread_mutex_lock(&ii->m_mutex);
        ii->m_data->appendBytes((uint8_t*)ptr, sizes);
        ii->isHeaderAllReceived = true;
        pthread_mutex_unlock(&ii->m_mutex);
        
        // return a value which is different with sizes will abort it
        if(ii->m_ctx->request->isCancel())
            return sizes + 1;
        else
            return sizes;
    }
    
    /// Callback function used by libcurl for collect header data
    static size_t writeHeaderData(void* ptr, size_t size, size_t nmemb, void* userdata) {
        CURLHandler* ii = (CURLHandler*)userdata;
        size_t sizes = size * nmemb;
        
        // add header data
        pthread_mutex_lock(&ii->m_mutex);
        string header((const char*)ptr, sizes);
        CCArray& pair = CCUtils::componentsOfString(header, ':');
        if(pair.count() == 2) {
            ii->m_ctx->response->addHeader(((CCString*)pair.objectAtIndex(0))->getCString(),
                                           ((CCString*)pair.objectAtIndex(1))->getCString());
        }
        pthread_mutex_unlock(&ii->m_mutex);
        
        // return a value which is different with sizes will abort it
        if(ii->m_ctx->request->isCancel())
            return sizes + 1;
        else
            return sizes;
    }
    
    bool initWithContext(ccHttpContext* ctx) {
        // save context
        m_ctx = ctx;
        
        // check
        m_curl = curl_easy_init();
        if (!m_curl)
            return false;
        if (!configureCURL())
            return false;
        
        // init mutex
        pthread_mutex_init(&m_mutex, NULL);
        
        /* get custom header data (if set) */
       	const vector<string>& headers = ctx->request->getHeaders();
        if(!headers.empty()) {
            // append custom headers one by one
            for (vector<string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
                m_headers = curl_slist_append(m_headers, it->c_str());
            
            // set custom headers for curl
            if (curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_headers) != CURLE_OK)
                return false;
        }
        
        return curl_easy_setopt(m_curl, CURLOPT_URL, ctx->request->getUrl().c_str()) == CURLE_OK &&
            curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writeData) == CURLE_OK &&
            curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this) == CURLE_OK &&
            curl_easy_setopt(m_curl, CURLOPT_HEADERFUNCTION, writeHeaderData) == CURLE_OK &&
            curl_easy_setopt(m_curl, CURLOPT_HEADERDATA, this) == CURLE_OK;
    }
    
    bool configureCURL() {
        int32_t code;
        code = curl_easy_setopt(m_curl, CURLOPT_ERRORBUFFER, m_errorBuffer);
        if (code != CURLE_OK) {
            return false;
        }
        code = curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, m_ctx->readTimeout);
        if (code != CURLE_OK) {
            return false;
        }
        code = curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, m_ctx->connectTimeout);
        if (code != CURLE_OK) {
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
        if (CURLE_OK != curl_easy_perform(m_curl))
            return false;
        
        // get return code
        CURLcode code = curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE, &m_responseCode);
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
            
            // unlistener internal notification
            nc->removeObserver(this, kCCNotificationHttpCancelAll);
            nc->removeObserver(this, kCCNotificationHttpCancelOne);
            
            // balance retain in httpThreadEntry
            autorelease();
            
            // unschedule dispatcher
            CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(CURLHandler::dispatchNotification), this);
        }
        
        pthread_mutex_unlock(&m_mutex);
    }
    
    void onCancelAll(CCObject* unused) {
        m_ctx->request->setCancel(true);
    }
    
    void onCancelOne(CCInteger* tag) {
        if(m_ctx->request->getTag() == tag->getValue()) {
            m_ctx->request->setCancel(true);
        }
    }
};

void* CBHttpClient::httpThreadEntry(void* arg) {
    // handle class
    ccHttpContext* ctx = (ccHttpContext*)arg;
    CURLHandler* curl = CURLHandler::create(ctx);
    CC_SAFE_RETAIN(curl);
    
    if(curl) {
        // create response
        CBHttpResponse* response = new CBHttpResponse(ctx->request);
        ctx->response = response;
        
        // process request
        bool retValue = false;
        switch (ctx->request->getMethod()) {
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
                CCAssert(true, "CBHttpClient: unkown request type, only GET and POSt are supported");
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
    CC_SAFE_RETAIN(request);

    // start network thread
    pthread_t thread;
    pthread_create(&thread, NULL, httpThreadEntry, ctx);
    pthread_detach(thread);
}

void CBHttpClient::cancel(int tag) {
    CCNotificationCenter::sharedNotificationCenter()->postNotification(kCCNotificationHttpCancelOne, CCInteger::create(tag));
}

void CBHttpClient::cancelAll() {
    CCNotificationCenter::sharedNotificationCenter()->postNotification(kCCNotificationHttpCancelAll);
}

NS_CC_END