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
#include "CCResourceLoader.h"
#include "SimpleAudioEngine.h"
#include "CCArmatureDataManager.h"

using namespace CocosDenshion;
USING_NS_CC_EXT;

NS_CC_BEGIN

void CCResourceLoader::CDMusicTask::load() {
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(name.c_str());
}

void CCResourceLoader::CDEffectTask::load() {
	SimpleAudioEngine::sharedEngine()->preloadEffect(name.c_str());
}

void CCResourceLoader::ArmatureTask::load() {
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(configFilePath.c_str());
}

CCResourceLoader::CCResourceLoader(CCResourceLoaderListener* listener) :
		m_listener(listener),
		m_delay(0),
		m_remainingIdle(0),
        m_nextLoad(0) {
}

CCResourceLoader::~CCResourceLoader() {
    for(LoadTaskPtrList::iterator iter = m_loadTaskList.begin(); iter != m_loadTaskList.end(); iter++) {
        delete *iter;
    }
}

unsigned char* CCResourceLoader::loadRaw(const string& name, unsigned long* size, DECRYPT_FUNC decFunc) {
    // load encryptd data
	unsigned long len;
	char* data = (char*)CCFileUtils::sharedFileUtils()->getFileData(name.c_str(), "rb", &len);
    
    // create texture
	int decLen;
    const char* dec = NULL;
	if(decFunc) {
        dec = (*decFunc)(data, len, &decLen);
    } else {
        dec = data;
        decLen = (int)len;
    }
    
    // free
    if(dec != data)
        free(data);
    
    // save size
    if(size)
        *size = decLen;
    
    // return
    return (unsigned char*)dec;
}

char* CCResourceLoader::loadCString(const string& name, DECRYPT_FUNC decFunc) {
    // load encryptd data
	unsigned long len;
	char* data = (char*)CCFileUtils::sharedFileUtils()->getFileData(name.c_str(), "rb", &len);

	// create texture
	int decLen;
    const char* dec = NULL;
	if(decFunc) {
        dec = (*decFunc)(data, len, &decLen);
    } else {
        dec = data;
        decLen = (int)len;
    }
    
    // copy as c string
    char* ret = (char*)malloc((decLen + 1) * sizeof(char));
    memcpy(ret, dec, decLen);
    ret[decLen] = 0;
    
    // free
    if(dec != data)
        free((void*)dec);
    free(data);
    
    // return
    return ret;
}

void CCResourceLoader::loadImage(const string& name, DECRYPT_FUNC decFunc) {
	// load encryptd data
	unsigned long len;
	char* data = (char*)CCFileUtils::sharedFileUtils()->getFileData(name.c_str(), "rb", &len);
	
	// create texture
	int decLen;
    const char* dec = NULL;
	if(decFunc) {
        dec = (*decFunc)(data, len, &decLen);
    } else {
        dec = data;
        decLen = (int)len;
    }
	CCImage* image = new CCImage();
	image->initWithImageData((void*)dec, decLen);
	image->autorelease();
	CCTextureCache::sharedTextureCache()->addUIImage(image, name.c_str());
	
	// free
    if(dec != data)
        free((void*)dec);
	free(data);
}

void CCResourceLoader::loadZwoptex(const string& plistName, const string& texName, DECRYPT_FUNC decFunc) {
	// load encryptd data
	unsigned long len;
	char* data = (char*)CCFileUtils::sharedFileUtils()->getFileData(texName.c_str(), "rb", &len);
	
	// create texture
	int decLen;
	const char* dec = NULL;
	if(decFunc) {
        dec = (*decFunc)(data, len, &decLen);
    } else {
        dec = data;
        decLen = (int)len;
    }
    CCImage* image = new CCImage();
	image->initWithImageData((void*)dec, decLen);
	image->autorelease();
	CCTexture2D* tex = CCTextureCache::sharedTextureCache()->addUIImage(image, texName.c_str());
	
	// free
    if(dec != data)
        free((void*)dec);
	free(data);
	
	// add zwoptex
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plistName.c_str(), tex);
}

void CCResourceLoader::run() {
	CCScheduler* scheduler = CCDirector::sharedDirector()->getScheduler();
	scheduler->scheduleSelector(schedule_selector(CCResourceLoader::doLoad), this, 0, kCCRepeatForever, m_delay, false);
}

void CCResourceLoader::addAndroidStringTask(const string& lan, const string& path, bool merge) {
    AndroidStringLoadTask* t = new AndroidStringLoadTask();
    t->lan = lan;
    t->path = path;
    t->merge = merge;
    addLoadTask(t);
}

void CCResourceLoader::addImageTask(const string& name, float idle) {
	ImageLoadTask* t = new ImageLoadTask();
    t->idle = idle;
    t->name = name;
    addLoadTask(t);
}

void CCResourceLoader::addImageTask(const string& name, DECRYPT_FUNC decFunc, float idle) {
	EncryptedImageLoadTask* t = new EncryptedImageLoadTask();
	t->idle = idle;
	t->name = name;
	t->func = decFunc;
	addLoadTask(t);
}

void CCResourceLoader::addBMFontTask(const string& fntFile, float idle) {
    BMFontLoadTask* t = new BMFontLoadTask();
    t->idle = idle;
    t->name = fntFile;
    addLoadTask(t);
}

void CCResourceLoader::addBMFontTask(const string& fntFile, DECRYPT_FUNC decFunc, float idle) {
    EncryptedBMFontLoadTask* t = new EncryptedBMFontLoadTask();
    t->idle = idle;
    t->name = fntFile;
    t->func = decFunc;
    addLoadTask(t);
}

void CCResourceLoader::addZwoptexTask(const string& name, float idle) {
    ZwoptexLoadTask* t = new ZwoptexLoadTask();
    t->idle = idle;
    t->name = name;
    addLoadTask(t);
}

void CCResourceLoader::addZwoptexTask(const string& pattern, int start, int end, float idle) {
	char buf[512];
	for(int i = start; i <= end; i++) {
		sprintf(buf, pattern.c_str(), i);
		addZwoptexTask(buf, idle);
	}
}

void CCResourceLoader::addZwoptexTask(const string& plistName, const string& texName, DECRYPT_FUNC decFunc, float idle) {
	EncryptedZwoptexLoadTask* t = new EncryptedZwoptexLoadTask();
	t->idle = idle;
	t->name = plistName;
	t->texName = texName;
	t->func = decFunc;
	addLoadTask(t);
}

void CCResourceLoader::addZwoptexTask(const string& plistPattern, const string& texPattern, int start, int end, DECRYPT_FUNC decFunc, float idle) {
	char buf1[512], buf2[512];
	for(int i = start; i <= end; i++) {
		sprintf(buf1, plistPattern.c_str(), i);
		sprintf(buf2, texPattern.c_str(), i);
		addZwoptexTask(buf1, buf2, decFunc, idle);
	}
}

void CCResourceLoader::addZwoptexAnimTask(const string& name,
										  float unitDelay,
										  const string& pattern,
										  int startIndex,
										  int endIndex,
										  bool restoreOriginalFrame,
										  float idle) {
	ZwoptexAnimLoadTask* t = new ZwoptexAnimLoadTask();
	t->name = name;
	t->unitDelay = unitDelay;
	t->restoreOriginalFrame = restoreOriginalFrame;
	t->idle = idle;
	char buf[256];
	for(int i = startIndex; i <= endIndex; i++) {
		sprintf(buf, pattern.c_str(), i);
		t->frames.push_back(buf);
	}
	addLoadTask(t);
}

void CCResourceLoader::addZwoptexAnimTask(const string& name,
                                          const string& pattern,
                                          int startIndex,
                                          int endIndex,
                                          const CCArray& delays,
                                          bool restoreOriginalFrame,
                                          float idle) {
    ZwoptexAnimLoadTask2* t = new ZwoptexAnimLoadTask2();
	t->name = name;
    t->restoreOriginalFrame = restoreOriginalFrame;
    t->idle = idle;
    
    char buf[256];
	for(int i = startIndex; i <= endIndex; i++) {
		sprintf(buf, pattern.c_str(), i);
		t->frames.push_back(buf);
	}
    
    CCObject* obj;
    CCARRAY_FOREACH(&delays, obj) {
        CCFloat* f = (CCFloat*)obj;
        t->durations.push_back(f->getValue());
    }
    
    addLoadTask(t);
}

void CCResourceLoader::addCDEffectTask(const string& name, float idle) {
	CDEffectTask* t = new CDEffectTask();
	t->idle = idle;
	t->name = name;
	addLoadTask(t);
}

void CCResourceLoader::addCDMusicTask(const string& name, float idle) {
	CDMusicTask* t = new CDMusicTask();
	t->idle = idle;
	t->name = name;
	addLoadTask(t);
}

void CCResourceLoader::addZwoptexAnimTask(const string& name,
										  float unitDelay,
										  const string& pattern,
										  int startIndex,
										  int endIndex,
										  int startIndex2,
										  int endIndex2,
										  bool restoreOriginalFrame,
										  float idle) {
	ZwoptexAnimLoadTask* t = new ZwoptexAnimLoadTask();
	t->name = name;
	t->unitDelay = unitDelay;
	t->restoreOriginalFrame = restoreOriginalFrame;
	t->idle = idle;
	char buf[256];
	for(int i = startIndex; i <= endIndex; i++) {
		sprintf(buf, pattern.c_str(), i);
		t->frames.push_back(buf);
	}
	for(int i = startIndex2; i <= endIndex2; i++) {
		sprintf(buf, pattern.c_str(), i);
		t->frames.push_back(buf);
	}
	addLoadTask(t);
}

void CCResourceLoader::addArmatureTask(string config, float idle) {
    ArmatureTask* t = new ArmatureTask();
    t->idle = idle;
    t->configFilePath = config;
    addLoadTask(t);
}

void CCResourceLoader::addArmatureTask(string plist, string tex, string config, DECRYPT_FUNC func, float idle) {
    if(!plist.empty() && !tex.empty()) {
        addZwoptexTask(plist, tex, func);
    }
    
    if(!config.empty())
        addArmatureTask(config);
}

void CCResourceLoader::addLoadTask(LoadTask* t) {
    m_loadTaskList.push_back(t);
}

void CCResourceLoader::doLoad(float delta) {
    if(m_remainingIdle > 0) {
        m_remainingIdle -= delta;
    } else if(m_loadTaskList.size() <= m_nextLoad) {
        if(m_listener)
            m_listener->onResourceLoadingDone();
        
        CCScheduler* scheduler = CCDirector::sharedDirector()->getScheduler();
        scheduler->unscheduleSelector(schedule_selector(CCResourceLoader::doLoad), this);
        autorelease();
    } else {
        LoadTask* lp = m_loadTaskList.at(m_nextLoad++);
        m_remainingIdle = lp->idle;
        
        lp->load();
        if(m_listener)
            m_listener->onResourceLoadingProgress(m_nextLoad * 100 / m_loadTaskList.size(), delta);
    }
}

NS_CC_END