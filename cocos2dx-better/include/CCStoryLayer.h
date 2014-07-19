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
#ifndef __CCStoryLayer__
#define __CCStoryLayer__

#include "cocos2d.h"
#include "ccMoreTypes.h"
#include "CCMoreMacros.h"

using namespace std;

NS_CC_BEGIN

class CCStoryPlayer;

/**
 * It is a container of story script, it will manage the playing of story
 */
class CC_DLL CCStoryLayer : public CCLayer {
    friend class CCStoryPlayer;
    
private:
    /// story player
    CCStoryPlayer* m_player;
    
    /// update is schedule
    bool m_playing;
    
    /// single image file loaded
    vector<string> m_loadedImageFiles;
    
protected:
	CCStoryLayer();
    
    /// when story is done
    void onStoryDone();
    
    /// unload single image files
    void unloadImageFiles();
	
public:
	virtual ~CCStoryLayer();
	static CCStoryLayer* create();
    
    /// access global parameters
    static string getParameter(const string& key);
    static void setParameter(const string& key, const string& value);
	
	virtual bool init();
    virtual void onExit();
    virtual void onEnter();
    
    /// preload story script from a file
    bool preloadStoryFile(const string& storyScriptFile, CC_DECRYPT_FUNC decFunc = NULL);
    
    /// preload story script string
    bool preloadStoryString(const string& storyScript);
    
    /// start story playing
    void playStory();
    
    /// stop playing
    void stopPlay();
    
    /// function to be invoked when story playing is done
    CC_SYNTHESIZE_RETAIN(CCCallFunc*, m_doneFunc, DoneFunction);
    
    /// c function used to decrypt resources
    CC_SYNTHESIZE(CC_DECRYPT_FUNC, m_decFunc, DecryptFunction);
};

NS_CC_END

#endif /* defined(__CCStoryLayer__) */
