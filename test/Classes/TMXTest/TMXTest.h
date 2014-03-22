#ifndef _TMXTest_H_
#define _TMXTest_H_

#include "../testBasic.h"
#include "cocos2d-better.h"

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

class TMXBaseDemo : public TMXDemo {
protected:
	CCSprite* m_sprite;
	CBTMXTileMap* m_map;
	CCPoint m_lastLoc;
	
public:
    virtual void onEnter();
	virtual CBTMXTileMap* createMap() = 0;
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
};

class TMXHexagonalDemo : public TMXBaseDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
	virtual CBTMXTileMap* createMap();
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
};

class TMXIsometricDemo : public TMXBaseDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
	virtual CBTMXTileMap* createMap();
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
};

class TMXOrthogonalColorDemo : public TMXBaseDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
	virtual CBTMXTileMap* createMap();
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
};

class TMXOrthogonalDemo : public TMXBaseDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
	virtual CBTMXTileMap* createMap();
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
};

class TMXOrthogonalFlipDemo : public TMXBaseDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
	virtual CBTMXTileMap* createMap();
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
};

class TMXOrthogonalEditDemo : public TMXBaseDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
	virtual CBTMXTileMap* createMap();
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
};

#endif