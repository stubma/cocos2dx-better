#include "CommonTest.h"
#include "../testResource.h"
#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d::ui;

TESTLAYER_CREATE_FUNC(CommonCalendar);
TESTLAYER_CREATE_FUNC(CommonCallFuncT);
TESTLAYER_CREATE_FUNC(CommonCatmullRomSprite);
TESTLAYER_CREATE_FUNC(CommonGradientSprite);
TESTLAYER_CREATE_FUNC(CommonGridView);
TESTLAYER_CREATE_FUNC(CommonImagePicker);
TESTLAYER_CREATE_FUNC(CommonLaserSprite);
TESTLAYER_CREATE_FUNC(CommonLayerClip);
TESTLAYER_CREATE_FUNC(CommonLocale);
TESTLAYER_CREATE_FUNC(CommonLocalization);
TESTLAYER_CREATE_FUNC(CommonMenuItemColor);
TESTLAYER_CREATE_FUNC(CommonProgressHUD);
TESTLAYER_CREATE_FUNC(CommonRichLabel);
TESTLAYER_CREATE_FUNC(CommonRichLabel2);
TESTLAYER_CREATE_FUNC(CommonResourceLoader);
TESTLAYER_CREATE_FUNC(CommonRookieGuide);
TESTLAYER_CREATE_FUNC(CommonScreenshot);
TESTLAYER_CREATE_FUNC(CommonScrollBar);
TESTLAYER_CREATE_FUNC(CommonScrollBar2);
TESTLAYER_CREATE_FUNC(CommonSecureUserDefault);
TESTLAYER_CREATE_FUNC(CommonSlider);
TESTLAYER_CREATE_FUNC(CommonTiledSprite);
TESTLAYER_CREATE_FUNC(CommonToast);

static NEWTESTFUNC createFunctions[] = {
    CF(CommonCalendar),
    CF(CommonCallFuncT),
    CF(CommonCatmullRomSprite),
	CF(CommonGradientSprite),
    CF(CommonGridView),
	CF(CommonImagePicker),
	CF(CommonLaserSprite),
    CF(CommonLayerClip),
	CF(CommonLocale),
    CF(CommonLocalization),
    CF(CommonMenuItemColor),
    CF(CommonProgressHUD),
	CF(CommonRichLabel),
    CF(CommonRichLabel2),
	CF(CommonResourceLoader),
    CF(CommonRookieGuide),
	CF(CommonScreenshot),
	CF(CommonScrollBar),
    CF(CommonScrollBar2),
    CF(CommonSecureUserDefault),
    CF(CommonSlider),
    CF(CommonTiledSprite),
    CF(CommonToast),
};

static int sceneIdx=-1;
#define MAX_LAYER (sizeof(createFunctions) / sizeof(createFunctions[0]))

static CCLayer* nextAction()
{
    sceneIdx++;
    sceneIdx = sceneIdx % MAX_LAYER;
    
    CCLayer* pLayer = (createFunctions[sceneIdx])();
    pLayer->init();
    pLayer->autorelease();
    
    return pLayer;
}

static CCLayer* backAction()
{
    sceneIdx--;
    int total = MAX_LAYER;
    if( sceneIdx < 0 )
        sceneIdx += total;
    
    CCLayer* pLayer = (createFunctions[sceneIdx])();
    pLayer->init();
    pLayer->autorelease();
    
    return pLayer;
}

static CCLayer* restartAction()
{
    CCLayer* pLayer = (createFunctions[sceneIdx])();
    pLayer->init();
    pLayer->autorelease();
    
    return pLayer;
}

void CommonTestScene::runThisTest()
{
    sceneIdx = -1;
    addChild(nextAction());

    CCDirector::sharedDirector()->replaceScene(this);
}


std::string CommonDemo::title()
{
    return "CommonTest";
}

std::string CommonDemo::subtitle()
{
    return "";
}

void CommonDemo::onEnter()
{
    CCLayer::onEnter();

    // add title and subtitle
    std::string str = title();
    const char * pTitle = str.c_str();
    CCLabelTTF* label = CCLabelTTF::create(pTitle, "Arial", 32);
    addChild(label, 1);
    label->setPosition( ccp(VisibleRect::center().x, VisibleRect::top().y - 30) );

    std::string strSubtitle = subtitle();
    if( ! strSubtitle.empty() ) 
    {
        CCLabelTTF* l = CCLabelTTF::create(strSubtitle.c_str(), "Thonburi", 16);
        addChild(l, 1);
        l->setPosition( ccp(VisibleRect::center().x, VisibleRect::top().y - 60) );
    }    

    // add menu
    CCMenuItemImage *item1 = CCMenuItemImage::create(s_pPathB1, s_pPathB2, this, menu_selector(CommonDemo::backCallback) );
    CCMenuItemImage *item2 = CCMenuItemImage::create(s_pPathR1, s_pPathR2, this, menu_selector(CommonDemo::restartCallback) );
    CCMenuItemImage *item3 = CCMenuItemImage::create(s_pPathF1, s_pPathF2, this, menu_selector(CommonDemo::nextCallback) );

    CCMenu *menu = CCMenu::create(item1, item2, item3, NULL);

    menu->setPosition(CCPointZero);
    item1->setPosition(ccp(VisibleRect::center().x - item2->getContentSize().width*2, VisibleRect::bottom().y+item2->getContentSize().height/2));
    item2->setPosition(ccp(VisibleRect::center().x, VisibleRect::bottom().y+item2->getContentSize().height/2));
    item3->setPosition(ccp(VisibleRect::center().x + item2->getContentSize().width*2, VisibleRect::bottom().y+item2->getContentSize().height/2));

    addChild(menu, 1);
}

void CommonDemo::onExit()
{
    CCLayer::onExit();
}

void CommonDemo::restartCallback(CCObject* pSender)
{
    CCScene* s = new CommonTestScene();
    s->addChild( restartAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void CommonDemo::nextCallback(CCObject* pSender)
{
    CCScene* s = new CommonTestScene();
    s->addChild( nextAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void CommonDemo::backCallback(CCObject* pSender)
{
    CCScene* s = new CommonTestScene();
    s->addChild( backAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

//------------------------------------------------------------------
//
// Calendar
//
//------------------------------------------------------------------
void CommonCalendar::onEnter()
{
    CommonDemo::onEnter();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    CCCalendar* c = CCCalendar::sharedCalendar();
    char buf[512];
    sprintf(buf, "time=%f\nyear=%d\nmonth=%d\nday=%d\nweekday=%d\nhour=%d\nminute=%d\nsecond=%d",
            c->getTime(),
            c->getYear(),
            c->getMonth(),
            c->getDay(),
            c->getWeekday(),
            c->getHour(),
            c->getMinute(),
            c->getSecond());
    
    CCLabelTTF* label = CCLabelTTF::create(buf, "Helvetica", 14);
    label->setPosition(ccp(origin.x + visibleSize.width / 2,
                           origin.y + visibleSize.height / 2));
    addChild(label);
}

std::string CommonCalendar::subtitle()
{
    return "Calendar";
}

//------------------------------------------------------------------
//
// CallFuncT
//
//------------------------------------------------------------------
void CommonCallFuncT::onEnter()
{
    CommonDemo::onEnter();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    m_label = CCLabelTTF::create("0", "Helvetica", 14);
    m_label->setPosition(ccp(origin.x + visibleSize.width / 2,
                             origin.y + visibleSize.height / 2));
    addChild(m_label);
    
    runAction(CCSequence::create(CCDelayTime::create(1),
                                 CCCallFuncT<int>::create(this, (CCCallFuncT<int>::SEL_CallFuncT)&CommonCallFuncT::randomChangeLabel, CCRANDOM_0_1() * 1000),
                                 NULL));
}

void CommonCallFuncT::randomChangeLabel(int n) {
    char buf[32];
    sprintf(buf, "%d", n);
    m_label->setString(buf);
    
    runAction(CCSequence::create(CCDelayTime::create(1),
                                 CCCallFuncT<int>::create(this, (CCCallFuncT<int>::SEL_CallFuncT)&CommonCallFuncT::randomChangeLabel, CCRANDOM_0_1() * 1000),
                                 NULL));
}

std::string CommonCallFuncT::subtitle()
{
    return "CallFuncT";
}

//------------------------------------------------------------------
//
// CatmullRom Sprite
//
//------------------------------------------------------------------
void CommonCatmullRomSprite::onEnter()
{
    CommonDemo::onEnter();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    m_curve = CCCatmullRomSprite::create("Images/line_pattern.png");
    m_curve->setPosition(CCPointZero);
    m_curve->addControlPoint(ccp(origin.x + visibleSize.width / 20,
                                origin.y + visibleSize.height / 10));
    m_curve->addControlPoint(ccp(origin.x + visibleSize.width / 5,
                                origin.y + visibleSize.height / 2));
    addChild(m_curve);
    
    // you can hide some segment
//    m_curve->setSegmentVisible(2, false);
    
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    
    // hint
    CCToast* t = CCToast::create(this, CCLabelTTF::create("Touch to add point", "Helvetica", 40 / CC_CONTENT_SCALE_FACTOR()));
    t->setPosition(ccp(origin.x + visibleSize.width / 2,
                       origin.y + visibleSize.height / 5));
}

std::string CommonCatmullRomSprite::subtitle()
{
    return "CatmullRom Sprite";
}

bool CommonCatmullRomSprite::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    m_curve->addControlPoint(pTouch->getLocation());
    return false;
}

void CommonCatmullRomSprite::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {
    
}

void CommonCatmullRomSprite::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
    
}

void CommonCatmullRomSprite::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {
    
}

//------------------------------------------------------------------
//
// Gradient Sprite
//
//------------------------------------------------------------------
void CommonGradientSprite::onEnter()
{
    CommonDemo::onEnter();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
	// red bottom to green top
	CCGradientSprite* s = CCGradientSprite::create("Images/usa_flag.jpg");
	s->setColor(ccc4(255, 0, 0, 255), ccc4(0, 255, 0, 255), ccp(0, 1));
	s->setPosition(ccp(origin.x + visibleSize.width / 4,
					   origin.y + visibleSize.height / 2));
	s->setScale(0.6f);
	addChild(s);
	
	// red bottom to green top, and fade out
	s = CCGradientSprite::create("Images/usa_flag.jpg");
	s->setColor(ccc4(255, 0, 0, 255), ccc4(0, 255, 0, 0), ccp(0, 1));
	s->setPosition(ccp(origin.x + visibleSize.width / 2,
					   origin.y + visibleSize.height / 2));
	s->setScale(0.6f);
	addChild(s);
	
	// red left to green right
	s = CCGradientSprite::create("Images/usa_flag.jpg");
	s->setColor(ccc4(255, 0, 0, 255), ccc4(0, 255, 0, 255), ccp(1, 0));
	s->setPosition(ccp(origin.x + visibleSize.width * 3 / 4,
					   origin.y + visibleSize.height / 2));
	s->setScale(0.6f);
	addChild(s);
}

std::string CommonGradientSprite::subtitle()
{
    return "Gradient Sprite";
}

//------------------------------------------------------------------
//
// Grid View
//
//------------------------------------------------------------------
void CommonGridView::onEnter()
{
    CommonDemo::onEnter();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    {
        CCGridView* gridView = CCGridView::create(this, CCSizeMake(visibleSize.width * 0.45f, visibleSize.height * 0.6f));
        gridView->ignoreAnchorPointForPosition(false);
        gridView->setAnchorPoint(ccp(0, 0.5f));
        gridView->setColCount(4);
        gridView->setPosition(ccp(origin.x, origin.y + visibleSize.height / 2));
        gridView->setDelegate(this);
        gridView->setDirection(kCCScrollViewDirectionVertical);
        gridView->setTag(1);
        addChild(gridView);
        gridView->reloadData();
    }
    
    {
        CCGridView* gridView = CCGridView::create(this, CCSizeMake(visibleSize.width * 0.45f, visibleSize.height * 0.6f));
        gridView->ignoreAnchorPointForPosition(false);
        gridView->setAnchorPoint(ccp(1, 0.5f));
        gridView->setColCount(3);
        gridView->setPosition(ccp(origin.x + visibleSize.width, origin.y + visibleSize.height / 2));
        gridView->setDelegate(this);
        gridView->setDirection(kCCScrollViewDirectionHorizontal);
        gridView->setTag(2);
        addChild(gridView);
        gridView->reloadData();
    }
}

std::string CommonGridView::subtitle()
{
    return "Grid View";
}

void CommonGridView::tableCellTouched(CCGridView* table, CCTableViewCell* cell) {
    CCLOG("clicked cell: %d", cell->getTag());
}

CCSize CommonGridView::tableCellSizeForIndex(CCGridView *table, unsigned int idx) {
    CCSize size = table->getViewSize();
    size.width /= table->getColCount();
    switch (table->getTag()) {
        case 1:
        {
            if(idx % 4 == 1) {
                size.width -= 40;
            } else if(idx % 4 == 3) {
                size.width += 40;
            }
            int row = idx / table->getColCount();
            size.height /= ((row % 2) == 1) ? 3 : 4;
            break;
        }
        default:
        {
            // for horizontal scrolling grid view, index start from top-left
            // and increase downward
            size.height /= 4;
            if(idx % 4 == 1) {
                size.height -= 20;
            } else if(idx % 4 == 3) {
                size.height += 20;
            }
            break;
        }
    }

    return size;
}

CCTableViewCell* CommonGridView::tableCellAtIndex(CCGridView *table, unsigned int idx) {
    // create cell
    CCTableViewCell* cell = table->dequeueCell();
    if(!cell) {
        cell = new CCTableViewCell();
        cell->autorelease();
    }
    
    // set content size and tag
    CCSize cellSize = tableCellSizeForIndex(table, idx);
    cell->setContentSize(cellSize);
    cell->setTag(idx);
    
    // add a random color layer
    CCLayerColor* bg = (CCLayerColor*)cell->getChildByTag(1);
    if(!bg) {
        bg = CCLayerColor::create();
        bg->setOpacity(255);
        cell->addChild(bg, 0, 1);
    }
    bg->setContentSize(cellSize);
    bg->setColor(ccc3(CCRANDOM_0_X_INT(255) & 0xff,
                      CCRANDOM_0_X_INT(255) & 0xff,
                      CCRANDOM_0_X_INT(255) & 0xff));
    
    // add a number label
    CCRichLabelTTF* label = (CCRichLabelTTF*)cell->getChildByTag(2);
    char buf[64];
    sprintf(buf, "%u", idx);
    if(!label) {
        label = CCRichLabelTTF::create(buf, "Helvetica", 40);
        cell->addChild(label, 1, 2);
    }
    label->setPosition(CCUtils::getLocalCenter(cell));
    label->setString(buf);
    
    // return cell
    return cell;
}

unsigned int CommonGridView::numberOfCellsInTableView(CCGridView *table) {
    return 100;
}

void CommonGridView::tableCellHighlight(CCGridView* table, CCTableViewCell* cell) {
}

void CommonGridView::tableCellUnhighlight(CCGridView* table, CCTableViewCell* cell) {
}

//------------------------------------------------------------------
//
// Image Picker
//
//------------------------------------------------------------------
void CommonImagePicker::onEnter()
{
    CommonDemo::onEnter();
	m_photo = NULL;
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	// hint
    CCToast* t = CCToast::create(this, CCLabelTTF::create("Touch to bring image picker out", "Helvetica", 40 / CC_CONTENT_SCALE_FACTOR()));
    t->setPosition(ccp(origin.x + visibleSize.width / 2,
                       origin.y + visibleSize.height / 5));
	
	// enable touch
	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
}

std::string CommonImagePicker::subtitle()
{
    return "Image Picker";
}

bool CommonImagePicker::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
	if(CCImagePicker::hasFrontCamera())
		CCImagePicker::pickFromFrontCamera("a.jpg", this, 100, 100);
	else if(CCImagePicker::hasCamera())
		CCImagePicker::pickFromCamera("a.png", this, 100, 100);
	else
		CCImagePicker::pickFromAlbum("a.jpg", this, 100, 100);
	
	return true;
}

void CommonImagePicker::onImagePicked(const string& fullPath, int w, int h) {
	if(m_photo)
		m_photo->removeFromParent();

	CCTextureCache::sharedTextureCache()->removeTextureForKey(fullPath.c_str());
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	m_photo = CCSprite::create(fullPath.c_str());
	m_photo->setPosition(ccp(origin.x + visibleSize.width / 2,
							 origin.y + visibleSize.height / 2));
	addChild(m_photo);
}

void CommonImagePicker::onImagePickingCancelled() {
	CCLOG("onImagePickingCancelled");
}

//------------------------------------------------------------------
//
// Laser Sprite
//
//------------------------------------------------------------------
void CommonLaserSprite::onEnter()
{
    CommonDemo::onEnter();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
	// laser 1
	m_laser1 = CCLaserSprite::create(10, visibleSize.height);
	m_laser1->setAnchorPoint(ccp(0.5f, 0));
	m_laser1->setPosition(ccp(origin.x + visibleSize.width / 2,
							  origin.y));
	addChild(m_laser1);
	
	scheduleUpdate();
}

std::string CommonLaserSprite::subtitle()
{
    return "Laser Sprite";
}

void CommonLaserSprite::update(float delta) {
	static bool add = false;
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCSize size = m_laser1->getContentSize();
	if(add) {
		size.height++;
		if(size.height >= visibleSize.height) {
			add = false;
		}
	} else {
		size.height--;
		if(size.height <= visibleSize.height / 2) {
			add = true;
		}
	}
	m_laser1->setHeight(size.height);
}

//------------------------------------------------------------------
//
// Layer Clip
//
//------------------------------------------------------------------
void CommonLayerClip::onEnter()
{
    CommonDemo::onEnter();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    // clip layer
    CCSprite* flag = CCSprite::create("Images/usa_flag.jpg");
    CCLayerClip* layer = CCLayerClip::create(cc4RED);
    layer->ignoreAnchorPointForPosition(false);
    layer->setContentSize(CCSizeMake(visibleSize.width / 2,
                                     flag->getContentSize().height));
    layer->setPosition(ccp(origin.x + visibleSize.width / 2,
                           origin.y + visibleSize.height / 2));
    addChild(layer);
    
    // add flag to clip layer
    flag->setAnchorPoint(ccp(0, 0.5f));
    flag->setPosition(ccp(layer->getContentSize().width,
                           layer->getContentSize().height / 2));
    layer->addChild(flag);
    
    // let flag move to see clip effect
    CCMoveBy* move = CCMoveBy::create(5, ccp(-flag->getContentSize().width - layer->getContentSize().width, 0));
    CCFiniteTimeAction* back = (CCFiniteTimeAction*)move->reverse();
    CCSequence* seq = CCSequence::createWithTwoActions(move, back);
    flag->runAction(CCRepeatForever::create(seq));
}

std::string CommonLayerClip::subtitle()
{
    return "Layer Clip";
}

//------------------------------------------------------------------
//
// Locale
//
//------------------------------------------------------------------
void CommonLocale::onEnter()
{
    CommonDemo::onEnter();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    CCLocale* locale = CCLocale::sharedLocale();
    char buf[512];
    sprintf(buf, "language=%s\ncountry=%s",
            locale->getLanguage().c_str(),
            locale->getCountry().c_str());
    
    CCLabelTTF* label = CCLabelTTF::create(buf, "Helvetica", 16);
    label->setPosition(ccp(origin.x + visibleSize.width / 2,
                           origin.y + visibleSize.height / 2));
    addChild(label);
}

std::string CommonLocale::subtitle()
{
    return "Locale";
}

//------------------------------------------------------------------
//
// Localization
//
//------------------------------------------------------------------
void CommonLocalization::onEnter()
{
    CommonDemo::onEnter();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    CCLocalization::sharedLocalization()->addAndroidStrings("en", "strings/strings.xml");
    CCLocalization::sharedLocalization()->addAndroidStrings("zh", "strings/strings_zh.xml");
    
    CCLabelTTF* label = CCLabelTTF::create(CCLC("hello"), "Helvetica", 16);
	label->setPosition(ccp(origin.x + visibleSize.width / 2,
                           origin.y + visibleSize.height / 2));
    addChild(label);
    
    CCLabelTTF* hint = CCLabelTTF::create("把系统语言设置成中文再看上面的label显示的文字", "Helvetica", 16);
	hint->setPosition(ccp(origin.x + visibleSize.width / 2,
                          origin.y + visibleSize.height / 4));
    addChild(hint);
}

std::string CommonLocalization::subtitle()
{
    return "Localization";
}

//------------------------------------------------------------------
//
// Localization
//
//------------------------------------------------------------------
void CommonMenuItemColor::onEnter()
{
    CommonDemo::onEnter();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    CCMenuItemColor* item1 = CCMenuItemColor::create(cc4RED, cc4BLUE);
    item1->setContentSize(CCSizeMake(visibleSize.width / 5, visibleSize.height / 8));
    item1->setPosition(ccp(origin.x + visibleSize.width / 2,
                           origin.y + visibleSize.height / 2));
    
    CCMenu* menu = CCMenu::create(item1, NULL);
    menu->setPosition(CCPointZero);
    addChild(menu);
}

std::string CommonMenuItemColor::subtitle()
{
    return "Color Menu Item";
}

//------------------------------------------------------------------
//
// ProgressHUD
//
//------------------------------------------------------------------
void CommonProgressHUD::onEnter()
{
    CommonDemo::onEnter();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
 
    CCMenuItemLabel* item1 = CCMenuItemLabel::create(CCLabelTTF::create("Start HUD", "Helvetica", 80 / CC_CONTENT_SCALE_FACTOR()),
													 this,
													 menu_selector(CommonProgressHUD::showHUD));
    item1->setPosition(ccp(origin.x + visibleSize.width / 2,
                           origin.y + visibleSize.height / 2));
    
    CCMenu* menu = CCMenu::create(item1, NULL);
    menu->setPosition(CCPointZero);
    addChild(menu);
}

std::string CommonProgressHUD::subtitle()
{
    return "Progress HUD";
}

void CommonProgressHUD::showHUD(CCObject* sender) {
    CCProgressHUD* hud = CCProgressHUD::show("Loading...");
    hud->setPanelColor(ccc4(255, 0, 0, 200));
    
    runAction(CCSequence::create(CCDelayTime::create(2),
                                 CCCallFunc::create(this, callfunc_selector(CommonProgressHUD::changeMessage)),
								 CCDelayTime::create(2),
                                 CCCallFunc::create(this, callfunc_selector(CommonProgressHUD::changeRichMessage)),
								 CCDelayTime::create(2),
								 CCCallFunc::create(this, callfunc_selector(CommonProgressHUD::changePanelColor)),
								 CCDelayTime::create(2),
								 CCCallFunc::create(this, callfunc_selector(CommonProgressHUD::changeDimColor)),
								 CCDelayTime::create(2),
								 CCCallFunc::create(this, callfunc_selector(CommonProgressHUD::forceSquare)),
								 CCDelayTime::create(2),
								 CCCallFunc::create(this, callfunc_selector(CommonProgressHUD::changeIcon)),
								 CCDelayTime::create(2),
								 CCCallFunc::create(this, callfunc_selector(CommonProgressHUD::beforeHide)),
								 CCDelayTime::create(2),
								 CCCallFunc::create(this, callfunc_selector(CommonProgressHUD::hide)),
                                 NULL));
}

void CommonProgressHUD::changeMessage() {
    CCProgressHUD* hud = CCProgressHUD::current();
    hud->setMessage("你可以随时改变提示信息");
}

void CommonProgressHUD::changeRichMessage() {
	CCProgressHUD* hud = CCProgressHUD::current();
    hud->setMessage("消息是一个[color=ff0000ff]CCRichLabelTTF[/color]");
}

void CommonProgressHUD::changeDimColor() {
	CCProgressHUD* hud = CCProgressHUD::current();
	hud->setMessage("可以改变整体背景色");
	hud->setDimColor(ccc4(0, 0, 200, 100));
}

void CommonProgressHUD::changePanelColor() {
	CCProgressHUD* hud = CCProgressHUD::current();
	hud->setMessage("可以改变面板背景色");
	hud->setPanelColor(ccc4(127, 183, 90, 200));
}

void CommonProgressHUD::forceSquare() {
	CCProgressHUD* hud = CCProgressHUD::current();
	hud->setMessage("强制为正方形");
	hud->setForceSquare(true);
}

void CommonProgressHUD::changeIcon() {
	CCProgressHUD* hud = CCProgressHUD::current();
	hud->setMessage("可以使用自定义图标");
	hud->setIcon(CCSprite::create("Images/grossini.png"));
}

void CommonProgressHUD::beforeHide() {
	CCProgressHUD* hud = CCProgressHUD::current();
	hud->setIcon(NULL);
	hud->setMessage("2秒后消失");
}

void CommonProgressHUD::hide() {
	CCProgressHUD::hide();
}

//------------------------------------------------------------------
//
// Rich Label
//
//------------------------------------------------------------------
void CommonRichLabel::onEnter()
{
    CommonDemo::onEnter();
	
	setOpacity(255);
	setColor(ccRED);
	
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
    // English, Chinese, color, font, overall test
	CCRichLabelTTF* label = CCRichLabelTTF::create("H[color=ff00ff00][i][b][u]e我[/u][/b][/i][/color]llo [size=20][font=font/Arial Rounded MT Bold.ttf][color=ffffffff]W[/color]orld![/font][/size]",
                                                   "Helvetica",
                                                   30);
	label->enableStroke(ccBLUE, 0.5f);
	label->setPosition(ccp(origin.x + visibleSize.width / 2,
						   origin.y + visibleSize.height / 5));
	label->enableShadow(CCSizeMake(-10, -10), 0xafffff00, 4);
	label->setFontFillColor(ccc3(179, 179, 179));
	addChild(label);
    
    // image tag test
    // text and other tags between image tag will be ignored
    // if image path starts with '/', we treat it like an external image. In iOS, it will be mapped to ~/Documents
    label = CCRichLabelTTF::create("Hello, [image=Images/a.png w=64 h=32 offsety=-32]blo[i]c[/i]ks[/image], [u]Gross[image= w=32 h=48]grossini[/image]ini[/u]\n[color=ff0000ff to=000000ff duration=0.5 transient=true]This[/color] is [color=ff0000ff to=ff00ff00 duration=0.5]second[/color] li[image=usa_flag.jpg plist=Images/z_test.plist atlas=Images/z_test.png scale=1.2]usa[/image]ne",
                                   "Helvetica",
                                   20);
	label->setPosition(ccp(origin.x + visibleSize.width / 2,
						   origin.y + visibleSize.height * 2 / 5));
	addChild(label);
	
	// now create a grossini in placeholder place and move it
	CCRect r = label->getImageBound(1);
	CCSprite* g = CCSprite::create("Images/grossini.png");
	g->setScale(r.size.width / g->getContentSize().width);
	g->setScale(r.size.height / g->getContentSize().height);
	g->setAnchorPoint(CCPointZero);
	g->setPosition(r.origin);
	label->addChild(g);
	g->runAction(CCRepeatForever::create(CCSequence::create(CCMoveBy::create(0.5f, ccp(0, 10)),
															CCMoveBy::create(0.5f, ccp(0, -10)),
															NULL)));
    
    // font tag test
    label = CCRichLabelTTF::create("H[i][b][u]ell[/u][/b][/i]o[size=20][font=font/Arial Rounded MT Bold.ttf]H[i]ell[/i]o[/font][/size]",
                                   "Helvetica",
                                   30);
    label->setColor(ccc4(0, 0, 255, 255), ccc4(0, 255, 0, 255), ccp(1, 0));
	label->setPosition(ccp(origin.x + visibleSize.width / 2,
						   origin.y + visibleSize.height * 3 / 5));
    label->startLoopDisplay(0.1f, kCCRepeatForever);
	addChild(label);
}

std::string CommonRichLabel::subtitle()
{
    return "Rich Label";
}

//------------------------------------------------------------------
//
// Rich Label (Link Tag)
//
//------------------------------------------------------------------
void CommonRichLabel2::onEnter()
{
    CommonDemo::onEnter();
	
	setOpacity(255);
	setColor(ccRED);
	
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
    // label 1
	CCRichLabelTTF* label = CCRichLabelTTF::create("This label has a [color=ff0000ff][link bg=3f7f7f7f bg_click=9fffff00]clickable[/link][/color] link, great!",
                                                   "Helvetica",
                                                   20);
	label->setPosition(ccp(origin.x + visibleSize.width / 2,
						   origin.y + visibleSize.height / 5));
	label->setLinkTarget(0, CCCallFunc::create(this, callfunc_selector(CommonRichLabel2::onLinkClicked)));
	addChild(label);
    
    // measure api demo
    CCLOG("label content: %f, %f", label->getContentSize().width, label->getContentSize().height);
    CCSize s = CCUtils::measureRichString("This label has a [color=ff0000ff][link bg=3f7f7f7f bg_click=9fffff00]clickable[/link][/color] link, great!",
                                          "Helvetica",
                                          20);
    CCLOG("measure: %f, %f", s.width, s.height);
	
    // label 2
	label = CCRichLabelTTF::create("Wow! Image[link bg=3f7f7f7f bg_click=9f00ff00] [image=Images/a.png scale=2]\ufffc[/image] [/link]can also be linked!",
                                   "Helvetica",
                                   24);
	label->setPosition(ccp(origin.x + visibleSize.width / 2,
						   origin.y + visibleSize.height * 2 / 5));
	label->setLinkTarget(0, CCCallFunc::create(this, callfunc_selector(CommonRichLabel2::onLinkClicked)));
	addChild(label);
	
    // label 3
	label = CCRichLabelTTF::create("Link works fine even in [color=ff0000ff][link bg=3f7f7f7f bg_click=9f00ff00]line\nbreak![/link][/color] That's incredible!",
                                                   "Helvetica",
                                                   20);
	label->setPosition(ccp(origin.x + visibleSize.width / 2,
						   origin.y + visibleSize.height * 3 / 5));
	label->setLinkTarget(0, CCCallFunc::create(this, callfunc_selector(CommonRichLabel2::onLinkClicked)));
	label->enableShadow(CCSizeMake(-10, -10), 0xafffff00, 4);
    label->setLineSpacing(20);
	addChild(label);
}

std::string CommonRichLabel2::subtitle()
{
    return "Rich Label (Link Tag)";
}

void CommonRichLabel2::onLinkClicked() {
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCSprite* content = CCSprite::create("Images/grossini.png");
    content->setPosition(ccp(origin.x + visibleSize.width / 2,
                             origin.y - content->getContentSize().height / 2));
    float dy = visibleSize.height / 4 + content->getContentSize().height / 2;
    CCToast::create(this,
                    content,
                    1000, // tag, if not -1, it won't show more than one at the same time
                    3,
                    CCEaseExponentialOut::create(CCMoveBy::create(0.5f, ccp(0, dy))),
                    CCEaseExponentialIn::create(CCMoveBy::create(0.5f, ccp(0, -dy))));
}

//------------------------------------------------------------------
//
// Resource Loader
//
//------------------------------------------------------------------
void CommonResourceLoader::onEnter()
{
    CommonDemo::onEnter();
	
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	CCMenuItemLabel* item1 = CCMenuItemLabel::create(CCLabelTTF::create("Start Load", "Helvetica", 40),
													 this,
													 menu_selector(CommonResourceLoader::onStartLoadClicked));
    item1->setPosition(ccp(origin.x + visibleSize.width / 2,
                           origin.y + visibleSize.height / 2));
    
    CCMenu* menu = CCMenu::create(item1, NULL);
    menu->setPosition(CCPointZero);
    addChild(menu);
	
	m_percent = CCLabelTTF::create("0%", "Helvetica", 40);
	m_percent->setPosition(ccp(origin.x + visibleSize.width / 2,
							   origin.y + visibleSize.height / 5));
	addChild(m_percent);
}

std::string CommonResourceLoader::subtitle()
{
    return "Resource Loading";
}

void CommonResourceLoader::onResourceLoadingProgress(float progress, float delta) {
	// because resource loading is scheduled in OpenGL thread, so it is safe to update UI in callback
	char buf[32];
	sprintf(buf, "%d%%", (int)progress);
	m_percent->setString(buf);
}

void CommonResourceLoader::onResourceLoadingDone() {
	m_percent->setString("Done");
}

void CommonResourceLoader::onStartLoadClicked(CCObject* sender) {
	// we don't have large resource. So we repeatly loading one image and set a big idle time.
	// of course, same resource will only be loaded once. Just for demo purpose.
	CCResourceLoader* rl = new CCResourceLoader(this);
    
	//////////////////////////////////////////////////////////////
	// DEMO SNIPPET
	// if resource is encrypted, provide decrypt function
//  rl->addImageTask("mainbg.jpg", decryptRes);
//  rl->addZwoptexTask("z_res_help.plist", "z_res_help.jpg", decryptRes);
    
    // animation frame name support pattern string
//  rl->addZwoptexAnimTask("fly", 0.1f, "fly_%d.png", 1, 4);
    
    // sound, for music and effect
//	rl->addCDMusicTask("audio/bg.mp3");
//  rl->addCDEffectTask("audio/click.mp3");
	///////////////////////////////////////////////////////////////
	
	// fake loading code
	rl->addImageTask("Images/grossini.png", 0.1f);
	rl->addImageTask("Images/grossini.png", 0.1f);
	rl->addImageTask("Images/grossini.png", 0.1f);
	rl->addImageTask("Images/grossini.png", 0.1f);
	rl->addImageTask("Images/grossini.png", 0.1f);
	rl->addImageTask("Images/grossini.png", 0.1f);
	rl->addImageTask("Images/grossini.png", 0.1f);
	rl->addImageTask("Images/grossini.png", 0.1f);
	rl->addImageTask("Images/grossini.png", 0.1f);
	rl->addImageTask("Images/grossini.png", 0.1f);
	rl->addImageTask("Images/grossini.png", 0.1f);
	rl->addImageTask("Images/grossini.png", 0.1f);
	rl->addImageTask("Images/grossini.png", 0.1f);
    
    // start to load resource
    rl->run();
}

//------------------------------------------------------------------
//
// Rookie Guide Layer
//
//------------------------------------------------------------------
void CommonRookieGuide::onEnter()
{
    CommonDemo::onEnter();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	CCSprite* g1 = CCSprite::create("Images/grossini.png");
	g1->setPosition(ccp(origin.x + visibleSize.width / 4,
                        origin.y + visibleSize.height / 2));
	addChild(g1);
    
    CCSprite* g2 = CCSprite::create("Images/grossini.png");
	g2->setPosition(ccp(origin.x + visibleSize.width * 3 / 4,
                        origin.y + visibleSize.height / 2));
	addChild(g2);
    
    // create rookie guide
    CCSimpleRookieGuide* rg = CCSimpleRookieGuide::create();
    
    // set bg
    rg->setBgColor(ccc4(0xff, 0, 0, 0x4f));
    
    // add region
    rg->addRegion(g2,
                  CCCallFunc::create(this, callfunc_selector(CommonRookieGuide::onGuideClicked)));
    
    // set arrow
    rg->setArrow(CCSprite::create("Images/f1.png"));
    rg->pointToRegionCenter(0, 100 / CC_CONTENT_SCALE_FACTOR(), 200);
    
    // set hint
    CCRichLabelTTF* hint = CCRichLabelTTF::create("[color=0xff0000ff]This is rookie guide, you MUST click it![/color]",
                                                  "Helvetica",
                                                  24 / CC_CONTENT_SCALE_FACTOR(),
                                                  CCSizeMake(150 / CC_CONTENT_SCALE_FACTOR(), 0),
                                                  kCCTextAlignmentLeft);
    rg->setHint(hint);
    rg->shiftHint(-80 / CC_CONTENT_SCALE_FACTOR(), -20 / CC_CONTENT_SCALE_FACTOR());
    
    // add guide layer, must add it after set
    addChild(rg);
}

std::string CommonRookieGuide::subtitle()
{
    return "Rookie Guide";
}

void CommonRookieGuide::onGuideClicked() {
    CCLOG("guide clicked!");
}

//------------------------------------------------------------------
//
// ScrollBar
//
//------------------------------------------------------------------
void CommonScrollBar::onEnter()
{
    CommonDemo::onEnter();
	
	TouchGroup* tg = TouchGroup::create();
	addChild(tg);
	
	// load scrollview layout
	Layout* _layout = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile("cocosgui/UIEditorTest/UIScrollView_Editor/UIScrollView_Vertical_Editor/ui_scrollview_editor_1.json"));
	tg->addWidget(_layout);
	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
	CCSize rootSize = _layout->getSize();
	tg->setPosition(CCPoint((screenSize.width - rootSize.width) / 2,
							(screenSize.height - rootSize.height) / 2));
	
	// hide title
	Label* _sceneTitle = static_cast<Label*>(UIHelper::seekWidgetByName(_layout, "UItest"));
	_sceneTitle->setVisible(false);
	
	// hide back
	Label* back_label = static_cast<Label*>(UIHelper::seekWidgetByName(_layout, "back"));
	back_label->setVisible(false);
	
	// scroll view
	ScrollView* sv = (ScrollView*)UIHelper::seekWidgetByName(_layout, "ScrollView_1137");
	
	// vertical bar
	CCScale9Sprite* track = CCScale9Sprite::create("Images/track.png");
	CCSprite* thumb = CCSprite::create("Images/thumb.png");
	CCScrollBar* vsb = CCScrollBar::create(track, thumb);
	vsb->setAutoFade(true);
	vsb->attachToUIScrollView(sv, cci(5, 5, 5, 5));
}

std::string CommonScrollBar::subtitle()
{
    return "ScrollBar - Works with CocoStudio ScrollView";
}

//------------------------------------------------------------------
//
// ScrollBar2
//
//------------------------------------------------------------------
void CommonScrollBar2::onEnter()
{
    CommonDemo::onEnter();
	
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	CCSize size = CCSizeMake(visibleSize.width * 4 / 5, visibleSize.height * 3 / 5);
	CCScrollView* scroll = CCScrollView::create(size);
    scroll->ignoreAnchorPointForPosition(false);
    scroll->setAnchorPoint(ccp(0.5f, 0.5f));
    scroll->setPosition(ccp(origin.x + visibleSize.width / 2,
							origin.y + visibleSize.height / 2));
    scroll->setDirection(kCCScrollViewDirectionVertical);
    addChild(scroll);
	
	CCLayer* content = createScrollContent(size);
    scroll->addChild(content);
    scroll->setContentSize(content->getContentSize());
    scroll->setContentOffset(scroll->minContainerOffset());
    
    // vertical bar
	CCScale9Sprite* track = CCScale9Sprite::create("Images/track.png");
	CCSprite* thumb = CCSprite::create("Images/thumb.png");
	CCScrollBar* vsb = CCScrollBar::create(track, thumb);
	vsb->setAutoFade(true);
	vsb->attachToCCScrollView(scroll, cci(5, 5, 5, 5));
}

CCLayer* CommonScrollBar2::createScrollContent(const CCSize& size) {
	CCLayerColor* layer = CCLayerColor::create(ccc4(255, 0, 0, 255));
	
	float y = 0;
	char buf[64];
	for(int i = 0; i < 100; i++) {
		sprintf(buf, "test label %d", i);
		CCLabelTTF* label = CCLabelTTF::create(buf, "Helvetica", 24);
		label->setAnchorPoint(ccp(0, 0));
		label->setPosition(ccp(0, y));
		layer->addChild(label);
		
		y += label->getContentSize().height + 10 / CC_CONTENT_SCALE_FACTOR();
	}
	
	layer->setContentSize(CCSizeMake(size.width, y));
	return layer;
}

std::string CommonScrollBar2::subtitle()
{
    return "ScrollBar - Works with CCScrollView";
}

//------------------------------------------------------------------
//
// SecureUserDefault
//
//------------------------------------------------------------------
void CommonSecureUserDefault::onEnter()
{
    CommonDemo::onEnter();

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	// init
    CCSecureUserDefault::init(encrypt, decrypt);
    
    // save a value
    CCSecureUserDefault::getInstance()->setStringForKey("testkey", "testvalue");
    
    // now read it from standard default, it will be unreadable
    string v = CCUserDefault::sharedUserDefault()->getStringForKey("testkey");
    char buf[1024];
    sprintf(buf, "read by CCUserDefault: %s", v.c_str());
    CCRichLabelTTF* label = CCRichLabelTTF::create(buf, "Helvetica", 30);
    label->setPosition(ccp(origin.x + visibleSize.width / 2,
                           origin.y + visibleSize.height * 2 / 3));
    addChild(label);
    
    // read it from secure default, ok
    v = CCSecureUserDefault::getInstance()->getStringForKey("testkey");
    sprintf(buf, "read by CCSecureUserDefault: %s", v.c_str());
    label = CCRichLabelTTF::create(buf, "Helvetica", 30);
    label->setPosition(ccp(origin.x + visibleSize.width / 2,
                           origin.y + visibleSize.height / 3));
    addChild(label);
}

std::string CommonSecureUserDefault::subtitle()
{
    return "Secure User Default";
}

const char* CommonSecureUserDefault::decrypt(const char* enc, int encLen, int* plainLen) {
    // to decrypt, NOT again
    char* dec = (char*)malloc(encLen * sizeof(char));
    for(int i = 0; i < encLen; i++) {
        dec[i] = ~enc[i];
    }
    if(plainLen)
        *plainLen = encLen;
    return dec;
}

const char* CommonSecureUserDefault::encrypt(const char* plain, int plainLen, int* encLen) {
    // very simple encryption, just NOT
    char* enc = (char*)malloc(plainLen * sizeof(char));
    for(int i = 0; i < plainLen; i++) {
        enc[i] = ~plain[i];
    }
    if(encLen)
        *encLen = plainLen;
    return enc;
}

//------------------------------------------------------------------
//
// Screenshot
//
//------------------------------------------------------------------
void CommonScreenshot::onEnter()
{
    CommonDemo::onEnter();
	m_shot = NULL;
	
	setOpacity(255);
	setColor(ccRED);
	
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	// grossini
	CCSprite* s = CCSprite::create("Images/grossini.png");
	s->setPosition(ccp(origin.x + visibleSize.width / 2,
					   origin.y + visibleSize.height / 2));
	addChild(s);
	
	// rotate grossini
	s->runAction(CCRepeatForever::create(CCRotateBy::create(5, 360)));
	
	// hint
    CCToast* t = CCToast::create(this, CCLabelTTF::create("Touch to capture screen", "Helvetica", 40 / CC_CONTENT_SCALE_FACTOR()));
    t->setPosition(ccp(origin.x + visibleSize.width / 2,
                       origin.y + visibleSize.height / 5));
	
	// enable touch
	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
}

std::string CommonScreenshot::subtitle()
{
    return "Screenshot";
}

bool CommonScreenshot::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
	// only shot this layer
	string path = CCUtils::makeScreenshot(this, "a.jpg");
	
	// remove old screenshot texture
	CCTextureCache::sharedTextureCache()->removeTextureForKey(path.c_str());
	
	// create sprite from screenshot, to avoid to capturing it, we add it to scene
	// another way is setting unwanted node to invisible before makeScreeshot
	if(m_shot)
		m_shot->removeFromParent();
	m_shot = CCSprite::create(path.c_str());
	m_shot->setAnchorPoint(CCPointZero);
	m_shot->setPosition(CCDirector::sharedDirector()->getVisibleOrigin());
	m_shot->setScale(0.25f);
	getParent()->addChild(m_shot);
	
	return true;
}

//------------------------------------------------------------------
//
// Slider
//
//------------------------------------------------------------------
void CommonSlider::onEnter()
{
    CommonDemo::onEnter();
    
    setOpacity(127);
	setColor(ccRED);
	
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
    // Add the slider
    CCSlider* slider = CCSlider::create(CCSprite::create("Images/sliderTrack.png"),
                                        CCSprite::create("Images/sliderThumb.png"),
                                        CCSprite::create("Images/sliderProgress.png"));
    slider->setMinimumValue(0.0f);
    slider->setMaximumValue(5.0f);
    slider->setPosition(ccp(origin.x + visibleSize.width / 2,
                            origin.y + visibleSize.height * 3 / 5));
    slider->addTargetWithActionForControlEvents(this, cccontrol_selector(CommonSlider::onValueChanged), CCControlEventValueChanged);
    addChild(slider);
    
    // value label of slider
    CCLabelTTF* value = CCLabelTTF::create("0", "Helvetica", 20 / CC_CONTENT_SCALE_FACTOR());
    value->setPosition(ccp(origin.x + visibleSize.width / 2,
                              CCUtils::getPoint(slider, 0.5f, 1).y + 15 / CC_CONTENT_SCALE_FACTOR()));
    addChild(value);
    slider->setUserData(value);
    
    // discrete slider, without progress
    slider = CCSlider::create(CCSprite::create("Images/sliderTrack.png"),
                              CCSprite::create("Images/sliderThumb.png"));
    slider->setMaximumValue(20);
    slider->setDiscreteMode(true);
    slider->setPosition(ccp(origin.x + visibleSize.width / 2,
                            origin.y + visibleSize.height * 2 / 5));
    slider->addTargetWithActionForControlEvents(this, cccontrol_selector(CommonSlider::onValueChanged), CCControlEventValueChanged);
    addChild(slider);
    
    // value label for discrete slider
    value = CCLabelTTF::create("0", "Helvetica", 20 / CC_CONTENT_SCALE_FACTOR());
    value->setPosition(ccp(origin.x + visibleSize.width / 2,
                           CCUtils::getPoint(slider, 0.5f, 1).y + 15 / CC_CONTENT_SCALE_FACTOR()));
    addChild(value);
    slider->setUserData(value);
}

void CommonSlider::onValueChanged(CCObject* obj, CCControlEvent event) {
    CCSlider* slider = (CCSlider*)obj;
    char buf[32];
    sprintf(buf, "%f", slider->getValue());
    ((CCLabelTTF*)slider->getUserData())->setString(buf);
}

std::string CommonSlider::subtitle()
{
    return "Slider";
}

//------------------------------------------------------------------
//
// Tiled Sprite
//
//------------------------------------------------------------------
void CommonTiledSprite::onEnter()
{
    CommonDemo::onEnter();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	CCTiledSprite* s = CCTiledSprite::create("Images/tiled.png");
    s->setHeight(visibleSize.height / 2);
    s->setAnchorPoint(ccp(0.5f, 0.5f));
	s->setPosition(ccp(origin.x + visibleSize.width / 2,
					   origin.y + visibleSize.height / 2));
	addChild(s);
    
    s = CCTiledSprite::create("Images/tiled.png");
    s->setHeight(visibleSize.height / 2);
    s->setRotation(90);
    s->setAnchorPoint(ccp(0.5f, 0.5f));
	s->setPosition(ccp(origin.x + visibleSize.width / 2,
					   origin.y + visibleSize.height / 2));
	addChild(s);
    
    // run animation on tiled sprite
    CCRect r = CCRectMake(0, 0, 28, 1);
    CCAnimation* anim = CCAnimation::create();
    anim->addSpriteFrame(CCSpriteFrame::create("Images/tiled.png", r));
    anim->addSpriteFrame(CCSpriteFrame::create("Images/tiled_2.png", r));
    anim->addSpriteFrame(CCSpriteFrame::create("Images/tiled_3.png", r));
    anim->setDelayPerUnit(0.05f);
    s->runAction(CCRepeatForever::create(CCAnimate::create(anim)));
}

std::string CommonTiledSprite::subtitle()
{
    return "Tiled Sprite";
}

//------------------------------------------------------------------
//
// Toast
//
//------------------------------------------------------------------
void CommonToast::onEnter()
{
    CommonDemo::onEnter();
	
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    CCLabelTTF* hint = CCLabelTTF::create("touch screen to show toast", "Helvetica", 24);
    hint->setPosition(ccp(origin.x + visibleSize.width / 2,
                          origin.y + visibleSize.height / 3));
    addChild(hint);
    
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
}

bool CommonToast::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCSprite* content = CCSprite::create("Images/grossini.png");
    content->setPosition(ccp(origin.x + visibleSize.width / 2,
                             origin.y - content->getContentSize().height / 2));
    float dy = visibleSize.height / 4 + content->getContentSize().height / 2;
    CCToast::create(this,
                    content,
                    1000, // tag, if not -1, it won't show more than one at the same time
                    3,
                    CCEaseExponentialOut::create(CCMoveBy::create(0.5f, ccp(0, dy))),
                    CCEaseExponentialIn::create(CCMoveBy::create(0.5f, ccp(0, -dy))));
    
    return false;
}

std::string CommonToast::subtitle()
{
    return "Toast";
}