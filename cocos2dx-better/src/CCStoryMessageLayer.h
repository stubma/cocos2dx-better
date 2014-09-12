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
#ifndef __CCStoryMessageLayer__
#define __CCStoryMessageLayer__

#include "cocos2d.h"

NS_CC_BEGIN

class CCStoryCommand;
class CCRichLabelTTF;
class CCStoryPlayer;

/// sub layer in story layer used to display dialog
class CCStoryMessageLayer : public CCLayer {
private:
    /// message label
    CCRichLabelTTF* m_msgLabel;
    
    /// name label
    CCRichLabelTTF* m_nameLabel;
    
    /// is loop display done
    bool m_looping;
    
    /// player
    CCStoryPlayer* m_player;
    
    /// wait time
    float m_wait;

protected:
	CCStoryMessageLayer();
    
    // callbacks
    void onDialogEndLooping();
    void onDialogDisplayedSomeWhile(float delta);
    
    // when user click
    void handleUserClick();
	
public:
	virtual ~CCStoryMessageLayer();
	static CCStoryMessageLayer* create(CCStoryPlayer* player);
	
	virtual bool initWithPlayer(CCStoryPlayer* player);
    virtual void onExit();
    virtual void onEnter();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void keyBackClicked();
    
    /// start to show a dialog
    void showMessage(CCStoryCommand* cmd);
};

NS_CC_END

#endif /* defined(__CCStoryMessageLayer__) */
