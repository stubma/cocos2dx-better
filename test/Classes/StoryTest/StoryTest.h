#ifndef _StoryTest_H_
#define _StoryTest_H_

#include "../testBasic.h"

using namespace std;
USING_NS_CC;


// the class inherit from TestScene
// every Scene each test used must inherit from TestScene,
// make sure the test have the menu item for back to main menu
class StoryTestScene : public TestScene
{
public:
    virtual void runThisTest();
};

class StoryDemo : public CCLayer
{
public:
    virtual void onEnter();
    virtual void onExit();
	
    virtual string title();
    virtual string subtitle();

    void restartCallback(CCObject* pSender);
    void nextCallback(CCObject* pSender);
    void backCallback(CCObject* pSender);
};

class Story1 : public StoryDemo
{
private:
	CCLabelTTF* m_hintLabel;
	
public:
    virtual void onEnter();
    virtual void onExit();
    virtual string subtitle();
};

#endif