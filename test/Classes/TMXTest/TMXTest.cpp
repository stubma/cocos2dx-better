#include "TMXTest.h"
#include "../testResource.h"
#include "cocos2d.h"

TESTLAYER_CREATE_FUNC(TMXISOParsing);

static NEWTESTFUNC createFunctions[] = {
    CF(TMXISOParsing)
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

void TMXTestScene::runThisTest()
{
    sceneIdx = -1;
    addChild(nextAction());

    CCDirector::sharedDirector()->replaceScene(this);
}


std::string TMXDemo::title()
{
    return "TMXTest";
}

std::string TMXDemo::subtitle()
{
    return "";
}

void TMXDemo::onEnter()
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
    CCMenuItemImage *item1 = CCMenuItemImage::create(s_pPathB1, s_pPathB2, this, menu_selector(TMXDemo::backCallback) );
    CCMenuItemImage *item2 = CCMenuItemImage::create(s_pPathR1, s_pPathR2, this, menu_selector(TMXDemo::restartCallback) );
    CCMenuItemImage *item3 = CCMenuItemImage::create(s_pPathF1, s_pPathF2, this, menu_selector(TMXDemo::nextCallback) );

    CCMenu *menu = CCMenu::create(item1, item2, item3, NULL);

    menu->setPosition(CCPointZero);
    item1->setPosition(ccp(VisibleRect::center().x - item2->getContentSize().width*2, VisibleRect::bottom().y+item2->getContentSize().height/2));
    item2->setPosition(ccp(VisibleRect::center().x, VisibleRect::bottom().y+item2->getContentSize().height/2));
    item3->setPosition(ccp(VisibleRect::center().x + item2->getContentSize().width*2, VisibleRect::bottom().y+item2->getContentSize().height/2));

    addChild(menu, 1);
}

void TMXDemo::onExit()
{
    CCLayer::onExit();
}

void TMXDemo::restartCallback(CCObject* pSender)
{
    CCScene* s = new TMXTestScene();
    s->addChild( restartAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void TMXDemo::nextCallback(CCObject* pSender)
{
    CCScene* s = new TMXTestScene();
    s->addChild( nextAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void TMXDemo::backCallback(CCObject* pSender)
{
    CCScene* s = new TMXTestScene();
    s->addChild( backAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

//------------------------------------------------------------------
//
// Isometric map
//
//------------------------------------------------------------------
void TMXISOParsing::onEnter()
{
    TMXDemo::onEnter();

	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	// add map
	m_map = CBTMXTileMap::createWithXMLFile("tmx/iso_test1.tmx");
	m_map->setPosition(origin);
	m_map->setDebugDrawObjects(true);
	addChild(m_map);
	
	// print some properties
	CBTMXObjectGroup* og = m_map->getObjectGroup("Object Layer 1");
	CCLOG("object group property: weather: %s", og->getProperty("weather").c_str());
	CCLOG("object count: %d", og->getObjectCount());
	CBTMXObject* obj = og->getObjectAt(0);
	CCLOG("first object name: %s, type: %s, favorite: %s",
		  obj->getName().c_str(),
		  obj->getType().c_str(),
		  obj->getProperty("favorite").c_str());
	
	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
}

std::string TMXISOParsing::subtitle()
{
    return "Isometric";
}

bool TMXISOParsing::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
	m_lastLoc = pTouch->getLocation();
	return true;
}

void TMXISOParsing::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {
	CCPoint location = pTouch->getLocation();
	CCPoint delta = ccpSub(location, m_lastLoc);
	m_lastLoc = location;
	
	CCPoint pos = m_map->getPosition();
	m_map->setPosition(ccpAdd(pos, delta));
}

void TMXISOParsing::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
	
}

void TMXISOParsing::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {
	
}