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

class CommonCallFuncT : public CommonDemo
{
private:
    CCLabelTTF* m_label;
    
private:
    void randomChangeLabel(int n);
    
public:
    virtual void onEnter();
    virtual string subtitle();
};

class CommonCatmullRomSprite : public CommonDemo
{
private:
    CCCatmullRomSprite* m_curve;
    
public:
    virtual void onEnter();
    virtual string subtitle();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
};

class CommonGradientSprite : public CommonDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
};

class CommonGridView : public CommonDemo, public CCGridViewDataSource, public CCGridViewDelegate
{
public:
    virtual void onEnter();
    virtual string subtitle();
    
    // virtual tabel view
    virtual void scrollViewDidScroll(CCScrollView* view) {}
    virtual void scrollViewDidZoom(CCScrollView* view) {}
    virtual void tableCellTouched(CCGridView* table, CCTableViewCell* cell);
    virtual CCSize tableCellSizeForIndex(CCGridView *table, unsigned int idx);
    virtual CCTableViewCell* tableCellAtIndex(CCGridView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCGridView *table);
    virtual void tableCellHighlight(CCGridView* table, CCTableViewCell* cell);
    virtual void tableCellUnhighlight(CCGridView* table, CCTableViewCell* cell);
};

class CommonImagePicker : public CommonDemo, public CCImagePickerCallback
{
private:
	CCSprite* m_photo;
	
public:
    virtual void onEnter();
    virtual string subtitle();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	
	// image picker callback
	virtual void onImagePicked(const string& fullPath, int w, int h);
	virtual void onImagePickingCancelled();
};

class CommonLaserSprite : public CommonDemo
{
private:
	CCLaserSprite* m_laser1;
	
public:
    virtual void onEnter();
    virtual string subtitle();
	virtual void update(float delta);
};

class CommonLayerClip : public CommonDemo
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

class CommonScreenshot : public CommonDemo
{
private:
	CCSprite* m_shot;
	
public:
    virtual void onEnter();
    virtual string subtitle();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
};

class CommonScrollBar : public CommonDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
};

class CommonScrollBar2 : public CommonDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
    CCLayer* createScrollContent(const CCSize& size);
};

class CommonSecureUserDefault : public CommonDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
    
    // decrypt and encrypt function
    static const char* decrypt(const char* enc, int encLen, int* plainLen);
    static const char* encrypt(const char* plain, int plainLen, int* encLen);
};

class CommonSlider : public CommonDemo
{
private:
    void onValueChanged(CCObject* obj, CCControlEvent event);
    
public:
    virtual void onEnter();
    virtual string subtitle();
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

#endif
