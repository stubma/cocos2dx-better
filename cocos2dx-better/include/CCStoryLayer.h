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

using namespace std;

NS_CC_BEGIN

/**
 * It is a container of story script, it will manage the playing of story
 */
class CC_DLL CCStoryLayer : public CCLayer {
protected:
	CCStoryLayer();
	
public:
	virtual ~CCStoryLayer();
	static CCStoryLayer* create();
	
	virtual bool init();
    virtual void onExit();
    virtual void onEnter();
    
    /// preload story script from a file
    virtual bool preloadStoryFile(const string& storyScriptFile, CC_DECRYPT_FUNC decFunc = NULL);
    
    /// preload story script string
    virtual bool preloadStoryString(const string& storyScript);
    
    /// start story playing
    virtual void playStory();
};

NS_CC_END

#endif /* defined(__CCStoryLayer__) */
