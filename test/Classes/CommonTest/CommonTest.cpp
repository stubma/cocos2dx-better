#include "CommonTest.h"
#include "../testResource.h"
#include "cocos2d.h"

TESTLAYER_CREATE_FUNC(CommonCalendar);
TESTLAYER_CREATE_FUNC(CommonClipInOut);
TESTLAYER_CREATE_FUNC(CommonGradientSprite);
TESTLAYER_CREATE_FUNC(CommonLocale);
TESTLAYER_CREATE_FUNC(CommonLocalization);
TESTLAYER_CREATE_FUNC(CommonMenuItemColor);
TESTLAYER_CREATE_FUNC(CommonMissile);
TESTLAYER_CREATE_FUNC(CommonRichLabel);
TESTLAYER_CREATE_FUNC(CommonRichLabel2);
TESTLAYER_CREATE_FUNC(CommonResourceLoader);
TESTLAYER_CREATE_FUNC(CommonRookieGuide);
TESTLAYER_CREATE_FUNC(CommonShake);
TESTLAYER_CREATE_FUNC(CommonScrollView);
TESTLAYER_CREATE_FUNC(CommonTiledSprite);
TESTLAYER_CREATE_FUNC(CommonToast);
TESTLAYER_CREATE_FUNC(CommonTreeFadeInOut);

static NEWTESTFUNC createFunctions[] = {
    CF(CommonCalendar),
    CF(CommonClipInOut),
	CF(CommonGradientSprite),
	CF(CommonLocale),
    CF(CommonLocalization),
    CF(CommonMenuItemColor),
    CF(CommonMissile),
	CF(CommonRichLabel),
    CF(CommonRichLabel2),
	CF(CommonResourceLoader),
    CF(CommonRookieGuide),
    CF(CommonShake),
	CF(CommonScrollView),
    CF(CommonTiledSprite),
    CF(CommonToast),
	CF(CommonTreeFadeInOut),
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
// Clip In & Out
//
//------------------------------------------------------------------
void CommonClipInOut::onEnter()
{
    CommonDemo::onEnter();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    CCClippingNode* n = CCClippingNode::create();
    CCSprite* flag = CCSprite::create("Images/usa_flag.jpg");
    CCDrawNode* stencil = CCDrawNode::create();
    n->setStencil(stencil);
    flag->setAnchorPoint(CCPointZero);
    flag->setPosition(CCPointZero);
    n->addChild(flag);
    n->ignoreAnchorPointForPosition(false);
    n->setAnchorPoint(ccp(0.5f, 0.5f));
    n->setContentSize(flag->getContentSize());
    n->setPosition(ccp(origin.x + visibleSize.width / 2,
                       origin.y + visibleSize.height / 2));
    addChild(n, 1);
    
    CCClipIn* in1 = CCClipIn::create(2);
    CCClipOut* out1 = (CCClipOut*)in1->reverse()->autorelease();
    CCClipIn* in2 = CCClipIn::create(2, ccp(1, 1));
    CCClipOut* out2 = (CCClipOut*)in2->reverse()->autorelease();
    CCClipIn* in3 = CCClipIn::create(2, ccp(-1, 1));
    CCClipOut* out3 = (CCClipOut*)in3->reverse()->autorelease();
    CCClipIn* in4 = CCClipIn::create(2, ccp(-1, -1));
    CCClipOut* out4 = (CCClipOut*)in4->reverse()->autorelease();
    CCClipIn* in5 = CCClipIn::create(2, ccp(1, -1));
    CCClipOut* out5 = (CCClipOut*)in5->reverse()->autorelease();
    CCClipIn* in6 = CCClipIn::create(2, ccp(0, -1));
    CCClipOut* out6 = (CCClipOut*)in6->reverse()->autorelease();
    CCClipIn* in7 = CCClipIn::create(2, ccp(0, 1));
    CCClipOut* out7 = (CCClipOut*)in7->reverse()->autorelease();
    CCClipIn* in8 = CCClipIn::create(2, ccp(-1, 0));
    CCClipOut* out8 = (CCClipOut*)in8->reverse()->autorelease();
    n->runAction(CCSequence::create(in1,
                                    out1,
                                    in2,
                                    out2,
                                    in3,
                                    out3,
                                    in4,
                                    out4,
                                    in5,
                                    out5,
                                    in6,
                                    out6,
                                    in7,
                                    out7,
                                    in8,
                                    out8,
                                    NULL));
}

std::string CommonClipInOut::subtitle()
{
    return "Clip In & Out";
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
// Missile
//
//------------------------------------------------------------------
void CommonMissile::onEnter()
{
    CommonDemo::onEnter();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    // target
    CCSprite* grossini = CCSprite::create("Images/grossini.png");
    grossini->setPosition(ccp(origin.x + visibleSize.width / 8,
                              origin.y + visibleSize.height / 8));
    grossini->setTag(1);
    addChild(grossini);
    
    // move target
    CCMoveTo* m1 = CCMoveTo::create(2, ccp(origin.x + visibleSize.width * 7 / 8,
                                           origin.y + visibleSize.height / 8));
    CCMoveTo* m2 = CCMoveTo::create(2, ccp(origin.x + visibleSize.width * 7 / 8,
                                           origin.y + visibleSize.height * 7 / 8));
    CCMoveTo* m3 = CCMoveTo::create(2, ccp(origin.x + visibleSize.width / 8,
                                           origin.y + visibleSize.height * 7 / 8));
    CCMoveTo* m4 = CCMoveTo::create(2, ccp(origin.x + visibleSize.width / 8,
                                           origin.y + visibleSize.height / 8));
    CCSequence* seq = CCSequence::create(m1, m2, m3, m4, NULL);
    grossini->runAction(CCRepeatForever::create(seq));
    
    // missile
    CCSprite* m = CCSprite::create("Images/r1.png");
    m->setPosition(ccp(origin.x + visibleSize.width / 2,
                       origin.y + visibleSize.height / 2));
    addChild(m);
    
    // run missile action
    CCMissile* action = CCMissile::create(130, grossini, 0,
                                          CCCallFunc::create(this, callfunc_selector(CommonMissile::onHit)));
    m->runAction(action);
}

void CommonMissile::onHit() {
    getChildByTag(1)->stopAllActions();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCLabelTTF* label = CCLabelTTF::create("Hit!!", "Helvetica", 24);
    label->setPosition(ccp(origin.x + visibleSize.width / 2,
                           origin.y + visibleSize.height / 2));
    addChild(label);
}

std::string CommonMissile::subtitle()
{
    return "Missile";
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
	
	CCRichLabelTTF* label = CCRichLabelTTF::create("H[color=ff00ff00][i][b][u]e我[/u][/b][/i][/color]llo [size=20][font=font/Arial Rounded MT Bold.ttf][color=ffffffff]W[/color]orld![/font][/size]",
                                                   "Helvetica",
                                                   30);
	label->enableStroke(ccBLUE, 0.5f);
	label->setPosition(ccp(origin.x + visibleSize.width / 2,
						   origin.y + visibleSize.height / 5));
	label->enableShadow(CCSizeMake(-10, -10), 0xafffff00, 4);
	label->setFontFillColor(ccc3(179, 179, 179));
	addChild(label);
    
    label = CCRichLabelTTF::create("Hello, [image=Images/a.png scale=2]\ufffc[/image], [u]Grossini[/u][image=Images/grossini.png scaley=0.5 scalex=0.3]\ufffc[/image]",
                                   "Helvetica",
                                   20);
	label->setPosition(ccp(origin.x + visibleSize.width / 2,
						   origin.y + visibleSize.height * 2 / 5));
	addChild(label);
    
    label = CCRichLabelTTF::create("H[i][b][u]ell[/u][/b][/i]o[size=20][font=font/Arial Rounded MT Bold.ttf]H[i]ell[/i]o[/font][/size]",
                                   "Helvetica",
                                   30);
    label->setColor(ccc4(0, 0, 255, 255), ccc4(0, 255, 0, 255), ccp(1, 0));
	label->setPosition(ccp(origin.x + visibleSize.width / 2,
						   origin.y + visibleSize.height * 3 / 5));
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
// Shake
//
//------------------------------------------------------------------
void CommonShake::onEnter()
{
    CommonDemo::onEnter();

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	CCSprite* s = CCSprite::create("Images/grossini.png");
	s->setPosition(ccp(origin.x + visibleSize.width / 2,
					   origin.y + visibleSize.height / 2));
	addChild(s);
	
	s->runAction(CCRepeatForever::create(CCShake::create(1, 5)));
}

std::string CommonShake::subtitle()
{
    return "Shake";
}

//------------------------------------------------------------------
//
// Scroll View
//
//------------------------------------------------------------------
void CommonScrollView::onEnter()
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
    scroll->setBounceable(false);
    addChild(scroll);
	
	CCLayer* content = createScrollContent(size);
    scroll->addChild(content);
    scroll->setContentSize(content->getContentSize());
    scroll->setContentOffset(scroll->minContainerOffset());
}

CCLayer* CommonScrollView::createScrollContent(const CCSize& size) {
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

std::string CommonScrollView::subtitle()
{
    return "Scroll View (Support Fling)";
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

//------------------------------------------------------------------
//
// Tree Fade In/Out
//
//------------------------------------------------------------------
void CommonTreeFadeInOut::onEnter()
{
    CommonDemo::onEnter();
	
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	CCSprite* s = CCSprite::create("Images/grossini.png");
	s->setPosition(ccp(origin.x + visibleSize.width / 2,
					   origin.y + visibleSize.height / 2));
	addChild(s);
	
	CCSprite* s2 = CCSprite::create("Images/grossini.png");
	s->addChild(s2);
	
	CCSprite* s3 = CCSprite::create("Images/grossini.png");
	s2->addChild(s3);
	
	s->runAction(CCRepeatForever::create(CCSequence::createWithTwoActions(CCTreeFadeOut::create(2), CCTreeFadeIn::create(2))));
}

std::string CommonTreeFadeInOut::subtitle()
{
    return "Tree Fade In/Out";
}
