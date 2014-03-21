#ifndef _TMXTest_H_
#define _TMXTest_H_

#include "../testBasic.h"

using namespace std;
USING_NS_CC;


// the class inherit from TestScene
// every Scene each test used must inherit from TestScene,
// make sure the test have the menu item for back to main menu
class TMXTestScene : public TestScene
{
public:
    virtual void runThisTest();
};

class TMXDemo : public CCLayer
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

class TMXISOParsing : public TMXDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
};

#endif