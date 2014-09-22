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
#include "CCStoryLayer.h"
#include "CCUtils.h"
#include "CCResourceLoader.h"
#include "CCStoryCommandSet.h"
#include "CCStoryPlayer.h"
#include "CCByteBuffer.h"

#ifdef __cplusplus
extern "C" {
#endif
    
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "tolua++.h"
    
#ifdef __cplusplus
}
#endif

#include "cb_story_tolua.h"

static lua_State* L = NULL;
static CCDictionary sGlobalParams;

NS_CC_BEGIN

CCStoryLayer::CCStoryLayer() :
m_player(NULL),
m_doneFunc(NULL),
m_decFunc(NULL),
m_playing(false) {
    if(!L) {
		L = lua_open();
		luaL_openlibs(L);
        tolua_story_open(L);
	}
}

CCStoryLayer::~CCStoryLayer() {
    CC_SAFE_RELEASE(m_player);
    CC_SAFE_RELEASE(m_doneFunc);
    unloadImageFiles();
    gStoryCommandSet.removeAllObjects();
    if(L) {
        lua_close(L);
        L = NULL;
    }
}

CCStoryLayer* CCStoryLayer::create() {
	CCStoryLayer* l = new CCStoryLayer();
	if(l->init()) {
		return (CCStoryLayer*)l->autorelease();
	}
	CC_SAFE_RELEASE(l);
	return NULL;
}

string CCStoryLayer::getParameter(const string& key) {
    CCString* v = (CCString*)sGlobalParams.objectForKey(key);
    if(v)
        return v->getCString();
    else
        return "";
}

void CCStoryLayer::setParameter(const string& key, const string& value) {
    sGlobalParams.setObject(CCString::create(value), key);
}

void CCStoryLayer::onExit() {
    CCLayer::onExit();
}

void CCStoryLayer::onEnter() {
    CCLayer::onEnter();
}

bool CCStoryLayer::init() {
	if(!CCLayer::init())
		return false;
	
	return true;
}

void CCStoryLayer::unloadImageFiles() {
    for(vector<string>::iterator iter = m_loadedImageFiles.begin(); iter != m_loadedImageFiles.end(); iter++) {
        CCTextureCache::sharedTextureCache()->removeTextureForKey(iter->c_str());
    }
    m_loadedImageFiles.clear();
}

bool CCStoryLayer::preloadStoryFile(const string& storyScriptFile, CC_DECRYPT_FUNC decFunc) {
    string path = CCUtils::getExternalOrFullPath(storyScriptFile);
    string script = CCResourceLoader::loadString(path, decFunc);
    return preloadStoryString(script);
}

bool CCStoryLayer::preloadStoryString(const string& storyScript) {
    // clear old
    gStoryCommandSet.removeAllObjects();
    unloadImageFiles();
    
    // load resources needed
    CCByteBuffer bb(storyScript.c_str(), storyScript.length(), storyScript.length());
    string line;
    bb.readLine(line);
    while(!line.empty()) {
        if(line.find("--$<") == 0) {
            string resCmd = line.substr(4);
            int comma = (int)resCmd.find(",");
            string type = resCmd.substr(0, comma);
            if(type == "atlas") {
                int secondComma = (int)resCmd.find(",", comma + 1);
                string plist = CCUtils::getExternalOrFullPath(resCmd.substr(comma + 1, secondComma - comma - 1));
                string atlas = CCUtils::getExternalOrFullPath(resCmd.substr(secondComma + 1));
                CCResourceLoader::loadZwoptex(plist, atlas, m_decFunc);
            } else if(type == "arm") {
                string json = CCUtils::getExternalOrFullPath(resCmd.substr(comma + 1));
                CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(json.c_str());
            } else if(type == "image") {
                string filename = resCmd.substr(comma + 1);
                string ext = CCUtils::getPathExtension(filename);
                string path;
                if(ext != ".png" && ext != ".jpg" && ext != ".jpeg") {
                    path = CCUtils::getExternalOrFullPath(filename + ".png");
                    if(!CCUtils::isPathExistent(path)) {
                        path = CCUtils::getExternalOrFullPath(filename + ".jpg");
                        if(!CCUtils::isPathExistent(path)) {
                            path = CCUtils::getExternalOrFullPath(filename + ".jpeg");
                            if(!CCUtils::isPathExistent(path)) {
                                path = filename + ".png";
                            }
                        }
                    }
                } else {
                    path = CCUtils::getExternalOrFullPath(filename);
                    if(!CCUtils::isPathExistent(path)) {
                        path = filename;
                    }
                }
                m_loadedImageFiles.push_back(path);
                CCResourceLoader::loadImage(path, m_decFunc);
            }
        } else {
            break;
        }
        
        bb.readLine(line);
    }
    
    // do script
    int ret = luaL_dostring(L, storyScript.c_str());
    if(ret != 0) {
        CCLOG("failed to load story, return code: %d", ret);
    }
    return ret == 0;
}

void CCStoryLayer::playStory() {
    if(m_playing)
        return;
    m_playing = true;
    
    // lazy create
    if(!m_player) {
        m_player = CCStoryPlayer::create(this);
        CC_SAFE_RETAIN(m_player);
    }
    
    // start
    m_player->start();
}

void CCStoryLayer::stopPlay() {
    if(!m_playing)
        return;
    m_playing = false;
    
    stopAllActions();
    CC_SAFE_RELEASE_NULL(m_player);
    removeAllChildren();
}

void CCStoryLayer::onStoryDone() {
    if(m_doneFunc) {
        m_doneFunc->execute();
    }
}

NS_CC_END