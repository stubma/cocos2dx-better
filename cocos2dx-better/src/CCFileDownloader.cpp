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
#include "CCFileDownloader.h"
#include "CCUtils.h"

NS_CC_BEGIN

/// download entry
class CCDownloadEntry : public CCObject {
public:
    string m_url;
    string m_dstFilename;
    size_t m_size;
    bool m_append;
    
public:
    CCDownloadEntry() :
    m_size(0),
    m_append(false) {
    }
    
    virtual ~CCDownloadEntry() {}
    
    static CCDownloadEntry* create() {
        CCDownloadEntry* e = new CCDownloadEntry();
        return (CCDownloadEntry*)e->autorelease();
    }
};

static CCFileDownloader* sInstance = NULL;

CCFileDownloader::CCFileDownloader() :
m_downloading(false),
m_totalSize(0),
m_totalDownloadedSize(0),
m_fos(NULL) {
    m_client = CBHttpClient::create();
    CC_SAFE_RETAIN(m_client);
    
    // listener
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CCFileDownloader::onHttpDone), kCCNotificationHttpRequestCompleted, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CCFileDownloader::onHttpData), kCCNotificationHttpDataReceived, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CCFileDownloader::onHttpHeaders), kCCNotificationHttpDidReceiveResponse, NULL);
}

CCFileDownloader::~CCFileDownloader() {
	CC_SAFE_RELEASE(m_client);
    sInstance = NULL;
}

CCFileDownloader* CCFileDownloader::getInstance() {
	if(!sInstance) {
        sInstance = new CCFileDownloader();
    }
    return sInstance;
}

void CCFileDownloader::purge() {
    if(sInstance) {
        // remove listener
        CCNotificationCenter::sharedNotificationCenter()->removeObserver(sInstance, kCCNotificationHttpRequestCompleted);
        CCNotificationCenter::sharedNotificationCenter()->removeObserver(sInstance, kCCNotificationHttpDataReceived);
        CCNotificationCenter::sharedNotificationCenter()->removeObserver(sInstance, kCCNotificationHttpDidReceiveResponse);
        
        // release
        CC_SAFE_RELEASE(sInstance);
    }
}

bool CCFileDownloader::init() {
	return true;
}

void CCFileDownloader::addFile(const string& url, bool append) {
    string dst = CCUtils::lastPathComponent(url);
    addFile(url, dst, append);
}

void CCFileDownloader::addFile(const string& url, const string& dstFilename, bool append) {
    addFile(url, dstFilename, 0, append);
}

void CCFileDownloader::addFile(const string& url, const string& dstFilename, size_t sizeHint, bool append) {
    CCDownloadEntry* e = CCDownloadEntry::create();
    e->m_url = url;
    e->m_dstFilename = dstFilename;
    e->m_size = sizeHint;
    e->m_append = append;
    m_totalSize += sizeHint;
    m_entries.addObject(e);
}

void CCFileDownloader::start() {
    if(!m_downloading) {
        // if has entry, go
        if(m_entries.count() > 0) {
            m_failedEntries.removeAllObjects();
            downloadNext();
            m_downloading = true;
        }
    }
}

void CCFileDownloader::downloadNext() {
    // get first entry
    m_entry = (CCDownloadEntry*)m_entries.objectAtIndex(0);
    
    // create stream
    string path = m_folder + m_entry->m_dstFilename;
    m_fos = CCAssetOutputStream::create(CCUtils::externalize(path), m_entry->m_append);
    CC_SAFE_RETAIN(m_fos);
    
    // start downloading
    CBHttpRequest* request = CBHttpRequest::create();
    request->setUrl(m_entry->m_url);
    request->setMethod(CBHttpRequest::kHttpGet);
    m_client->asyncExecute(request);
}

void CCFileDownloader::abort() {
    if(m_downloading) {
        m_client->cancelAll();
        CC_SAFE_RELEASE_NULL(m_fos);
        m_entry = NULL;
        m_entries.removeAllObjects();
        m_downloading = false;
    }
}

size_t CCFileDownloader::getCurrentDownloadedSize() {
    return m_fos ? m_fos->getPosition() : 0;
}

string CCFileDownloader::getCurrentDownloadingFileName() {
    return m_entry ? CCUtils::lastPathComponent(m_entry->m_dstFilename) : "";
}

string CCFileDownloader::getCurrentDownloadingFileFullPath() {
    return m_entry ? CCUtils::externalize(m_entry->m_dstFilename) : "";
}

size_t CCFileDownloader::getCurrentDownloadingFileSize() {
    return m_entry ? m_entry->m_size : 0;
}

void CCFileDownloader::onHttpDone(CBHttpResponse* response) {
    // if entry is not set, this notification is not for me
    if(!m_entry)
        return;
    
    // fail?
    if(!response->isSuccess()) {
        m_failedEntries.addObject(m_entry);
    }
    
    // close stream
    CC_SAFE_RELEASE_NULL(m_fos);
    m_entry = NULL;
    
    // remove first entry
    m_entries.removeObjectAtIndex(0);
    
    // next
    if(m_entries.count() > 0) {
        downloadNext();
    } else {
        m_downloading = false;
    }
}

void CCFileDownloader::onHttpData(CBHttpResponse* response) {
    if(m_fos) {
        CBData* data = response->getData();
        m_fos->write((const char*)data->getBytes(), data->getSize());
        m_totalDownloadedSize += data->getSize();
    }
}

void CCFileDownloader::onHttpHeaders(CBHttpResponse* response) {
    if(m_entry) {
        if(m_entry->m_size == 0) {
            string contentLength = response->getHeader("Content-Length");
            m_entry->m_size = atoi(contentLength.c_str());
        }
    }
}

NS_CC_END