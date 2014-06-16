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
#ifndef __CCFileDownloader__
#define __CCFileDownloader__

#include "cocos2d.h"
#include "CBHttpClient.h"
#include "CCMoreMacros.h"
#include "CCAssetOutputStream.h"

using namespace std;

NS_CC_BEGIN

class CCDownloadEntry;

/**
 * a file downloader which uses http protocol, it manages a list of file infos and download them
 * one by one. All files are saved to local. In iOS, they are saved under ~/Documents. In Android,
 * they are saved to internal storage. You can specify a base folder, for example, "data", then final
 * destination folder in iOS will be "~/Documents/data"
 *
 * CCFileDownloader is a singleton
 */
class CCFileDownloader : public CCObject {
private:
    /// client
    CBHttpClient* m_client;
    
    /// current file stream
    CCAssetOutputStream* m_fos;
    
    /// current entry
    CCDownloadEntry* m_entry;
    
protected:
	CCFileDownloader();
    
    /// init
	virtual bool init();
    
    /// notification
    void onHttpDone(CBHttpResponse* response);
    void onHttpData(CBHttpResponse* response);
    void onHttpHeaders(CBHttpResponse* response);
    
    /// download next entry
    void downloadNext();
    
public:
	virtual ~CCFileDownloader();
	static CCFileDownloader* getInstance();
    
    /// purge file downloader instance
    static void purge();
    
    /**
     * add a download entry, the destination file name will be last segment of url
     *
     * @param url file download url, only http is supported
     * @param append true means downloaded data is appended to existent file, by default it is false
     */
    void addFile(const string& url, bool append = false);
    
    /**
     * add a download entry, you can change the name of destination file
     * 
     * @param url file download url, only http is supported
     * @param dstFilename destination file name
     * @param append true means downloaded data is appended to existent file, by default it is false
     */
    void addFile(const string& url, const string& dstFilename, bool append = false);
    
    /**
     * add a download entry, you can change the name of destination file
     *
     * @param url file download url, only http is supported
     * @param dstFilename destination file name
     * @param sizeHint size of file to be downloaded
     * @param append true means downloaded data is appended to existent file, by default it is false
     */
    void addFile(const string& url, const string& dstFilename, size_t sizeHint, bool append = false);
    
    /// start download
    void start();
    
    /// abort
    void abort();
    
    /// get downloaded size of current downloading file
    size_t getCurrentDownloadedSize();
    
    /// get name of current downloading file
    string getCurrentDownloadingFileName();
    
    /// get full path of current downloading file
    string getCurrentDownloadingFileFullPath();
    
    /// get total size of current downloading file, maybe zero if size is still unknown
    size_t getCurrentDownloadingFileSize();
    
    /// file folder prefix
    CC_SYNTHESIZE_PASS_BY_REF(string, m_folder, Folder);
    
    /// download entry list
    CC_SYNTHESIZE_PASS_BY_REF(CCArray, m_entries, DownloadEntries);
    
    /// entries which are failed
    CC_SYNTHESIZE_PASS_BY_REF(CCArray, m_failedEntries, FailedEntries);
    
    /// is downloading
    CC_SYNTHESIZE_BOOL(m_downloading, Downloading);
    
    // total bytes
    CC_SYNTHESIZE_READONLY(size_t, m_totalSize, TotalSize);
    
    // total bytes downloaded
    CC_SYNTHESIZE_READONLY(size_t, m_totalDownloadedSize, TotalDownloadedSize);
};

NS_CC_END

#endif /* defined(__CCFileDownloader__) */
