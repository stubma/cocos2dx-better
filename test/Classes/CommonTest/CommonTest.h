#ifndef _CommonTest_H_
#define _CommonTest_H_

#include "../testBasic.h"
#include "cocos2d-common.h"

using namespace std;
USING_NS_CC;

enum
{
    COMMON_SHAKE_LAYER = 0
};


// the class inherit from TestScene
// every Scene each test used must inherit from TestScene,
// make sure the test have the menu item for back to main menu
class CommonTestScene : public TestScene
{
public:
    virtual void runThisTest();
};

class CommonDemo : public CCLayer
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

class CommonShake : public CommonDemo
{
private:
	CCLabelTTF* m_hintLabel;
	
public:
    virtual void onEnter();
    virtual string subtitle();
};

#endif
