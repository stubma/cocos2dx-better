#ifndef _AnimTest_H_
#define _AnimTest_H_

#include "../testBasic.h"
#include "cocos2d-better.h"

using namespace std;
USING_NS_CC;

// the class inherit from TestScene
// every Scene each test used must inherit from TestScene,
// make sure the test have the menu item for back to main menu
class AnimTestScene : public TestScene
{
public:
    virtual void runThisTest();
};

class AnimDemo : public CCLayer
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

class AnimMotionWelder : public AnimDemo
{
private:
	CCMWSprite* m_sprite1;
	CCMWSprite* m_sprite2;
	CCMWSprite* m_sprite3;
	CCMWSprite* m_sprite4;
	CCMWSprite* m_sprite5;
    
public:
    virtual void onEnter();
    virtual string subtitle();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void update(float dt);
};

class AnimArctic : public AnimDemo
{
private:
    CCArcticSprite* m_sprite1;
	CCArcticSprite* m_sprite2;
	CCArcticSprite* m_sprite3;
    
public:
    virtual void onEnter();
    virtual string subtitle();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void update(float dt);
};

class AnimAuroraGT : public AnimDemo
{
private:
	CCAuroraSprite* m_sprite1;
	CCAuroraSprite* m_sprite2;
	CCAuroraSprite* m_sprite3;
    
public:
    virtual void onEnter();
    virtual string subtitle();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void update(float dt);
};

class AnimSpriteX : public AnimDemo
{
private:
	CCSPXSprite* m_sprite1;
	CCSPXSprite* m_sprite2;
	CCSPXSprite* m_sprite3;
	CCSPXSprite* m_sprite4;
	CCSPXSprite* m_sprite5;
	CCSPXSprite* m_sprite6;
	CCSPXSprite* m_sprite7;
    
public:
    virtual void onEnter();
    virtual string subtitle();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void update(float dt);
};

class AnimSpriteX2011 : public AnimDemo
{
private:
	CCSPX3Sprite* m_sprite1;
	CCSPX3Sprite* m_sprite2;
	CCSPX3Sprite* m_sprite3;
	CCSPX3Sprite* m_sprite4;
	CCSPX3Sprite* m_sprite5;
	CCSPX3Sprite* m_sprite6;
	CCSPX3Sprite* m_sprite7;
    
public:
    virtual void onEnter();
    virtual string subtitle();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void update(float dt);
};

class AnimClipMappingAuroraGT : public AnimDemo
{
private:
	CCAuroraSprite* m_sprite1;
	int m_nextMapping;
    
public:
    virtual void onEnter();
    virtual string subtitle();
    
    void onSwitchClipMapping(CCObject* sender);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void update(float dt);
};

#endif
