#ifndef _ActionTest_H_
#define _ActionTest_H_

#include "../testBasic.h"
#include "cocos2d-better.h"

using namespace std;
USING_NS_CC;

// the class inherit from TestScene
// every Scene each test used must inherit from TestScene,
// make sure the test have the menu item for back to main menu
class ActionTestScene : public TestScene
{
public:
    virtual void runThisTest();
};

class ActionDemo : public CCLayer
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

class ActionBlur : public ActionDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
};

class ActionCount : public ActionDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
};

class ActionCurl : public ActionDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
};

class ActionFlash : public ActionDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
};

class ActionClipInOut : public ActionDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
};

class ActionJumpEx : public ActionDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
};

class ActionMissile : public ActionDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
    
    void onHit();
};

class ActionShake : public ActionDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
};

class ActionShine : public ActionDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
};

class ActionTrailMove : public ActionDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
};

class ActionTreeFadeInOut : public ActionDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
};

#endif