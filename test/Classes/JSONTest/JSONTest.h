#ifndef _JSONTest_H_
#define _JSONTest_H_

#include "../testBasic.h"

using namespace std;
USING_NS_CC;


enum
{
    JSON_PARSING = 0,
    JSON_LAYER_COUNT,
};


// the class inherit from TestScene
// every Scene each test used must inherit from TestScene,
// make sure the test have the menu item for back to main menu
class JSONTestScene : public TestScene
{
public:
    virtual void runThisTest();
};

class JSONDemo : public CCLayer
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

class JSONParsing : public JSONDemo
{
private:
	CCLabelTTF* m_hintLabel;
	
public:
    virtual void onEnter();
    virtual string subtitle();
};

#endif