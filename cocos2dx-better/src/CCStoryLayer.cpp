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

#ifdef __cplusplus
extern "C" {
#endif
    
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
    
#ifdef __cplusplus
}
#endif

static lua_State* L = NULL;

NS_CC_BEGIN

CCStoryLayer::CCStoryLayer() {
    if(!L) {
		L = lua_open();
		luaL_openlibs(L);
	}
}

CCStoryLayer::~CCStoryLayer() {
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

void CCStoryLayer::onExit() {
    CCLayer::onExit();
}

void CCStoryLayer::onEnter() {
    CCLayer::onEnter();
}

bool CCStoryLayer::init() {
	if(!CCLayer::init())
		return false;
    
    // surface
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	return true;
}

bool CCStoryLayer::preloadStoryFile(const string& storyScriptFile, CC_DECRYPT_FUNC decFunc) {
    string path = CCUtils::getExternalOrFullPath(storyScriptFile);
    string script = CCResourceLoader::loadString(path, decFunc);
    return preloadStoryString(script);
}

bool CCStoryLayer::preloadStoryString(const string& storyScript) {
    // clear old
    gStoryCommandSet.removeAllObjects();
    
    // do script
    int ret = luaL_dostring(L, storyScript.c_str());
    if(ret != 0) {
        CCLOG("failed to load story, return code: %d", ret);
    }
    return ret == 0;
}

void CCStoryLayer::playStory() {
    
}

NS_CC_END