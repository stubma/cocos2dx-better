#ifndef _CommonTest_H_
#define _CommonTest_H_

#include "../testBasic.h"
#include "cocos2d-better.h"

using namespace std;
USING_NS_CC;

// the class inherit from TestScene
// every Scene each test used must inherit from TestScene,
// make sure the test have the menu item for back to main menu
class CommonTestScene : public TestScene
{
public:
    virtual void runThisTest();
};

class CommonDemo : public CCLayerColor
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

class CommonCalendar : public CommonDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
};

class CommonCatmullRomSprite : public CommonDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
};

class CommonClipInOut : public CommonDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
};

class CommonGradientSprite : public CommonDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
};

class CommonLocale : public CommonDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
};

class CommonLocalization : public CommonDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
};

class CommonMenuItemColor : public CommonDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
};

class CommonMissile : public CommonDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
    
    void onHit();
};

class CommonProgressHUD : public CommonDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
    
    void showHUD(CCObject* sender);
    void changeMessage();
	void changeRichMessage();
	void changePanelColor();
	void changeDimColor();
	void changeIcon();
	void forceSquare();
	void hide();
	void beforeHide();
};

class CommonRichLabel : public CommonDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
};

class CommonRichLabel2 : public CommonDemo
{
private:
	void onLinkClicked();
	
public:
    virtual void onEnter();
    virtual string subtitle();
};

class CommonResourceLoader : public CommonDemo, public CCResourceLoaderListener
{
private:
	CCLabelTTF* m_percent;
	
private:
	void onStartLoadClicked(CCObject* sender);
	
public:
    virtual void onEnter();
    virtual string subtitle();
	
	// CCResourceLoaderListener
	virtual void onResourceLoadingProgress(float progress, float delta);
	virtual void onResourceLoadingDone();
};

class CommonRookieGuide : public CommonDemo
{
private:
    void onGuideClicked();
    
public:
    virtual void onEnter();
    virtual string subtitle();
};

class CommonShake : public CommonDemo
{	
public:
    virtual void onEnter();
    virtual string subtitle();
};

class CommonScrollView : public CommonDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
	CCLayer* createScrollContent(const CCSize& size);
};

class CommonTiledSprite : public CommonDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
};

class CommonToast : public CommonDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
};

class CommonTreeFadeInOut : public CommonDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
};

#endif
