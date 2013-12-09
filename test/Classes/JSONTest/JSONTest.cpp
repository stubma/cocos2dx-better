#include "JSONTest.h"
#include "../testResource.h"
#include "cocos2d.h"
#include "cocos2d-better.h"

TESTLAYER_CREATE_FUNC(JSONParsing);

static NEWTESTFUNC createFunctions[] = {
    CF(JSONParsing)
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

void JSONTestScene::runThisTest()
{
    sceneIdx = -1;
    addChild(nextAction());

    CCDirector::sharedDirector()->replaceScene(this);
}


std::string JSONDemo::title()
{
    return "JSONTest";
}

std::string JSONDemo::subtitle()
{
    return "";
}

void JSONDemo::onEnter()
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
    CCMenuItemImage *item1 = CCMenuItemImage::create(s_pPathB1, s_pPathB2, this, menu_selector(JSONDemo::backCallback) );
    CCMenuItemImage *item2 = CCMenuItemImage::create(s_pPathR1, s_pPathR2, this, menu_selector(JSONDemo::restartCallback) );
    CCMenuItemImage *item3 = CCMenuItemImage::create(s_pPathF1, s_pPathF2, this, menu_selector(JSONDemo::nextCallback) );

    CCMenu *menu = CCMenu::create(item1, item2, item3, NULL);

    menu->setPosition(CCPointZero);
    item1->setPosition(ccp(VisibleRect::center().x - item2->getContentSize().width*2, VisibleRect::bottom().y+item2->getContentSize().height/2));
    item2->setPosition(ccp(VisibleRect::center().x, VisibleRect::bottom().y+item2->getContentSize().height/2));
    item3->setPosition(ccp(VisibleRect::center().x + item2->getContentSize().width*2, VisibleRect::bottom().y+item2->getContentSize().height/2));

    addChild(menu, 1);
}

void JSONDemo::onExit()
{
    CCLayer::onExit();
}

void JSONDemo::restartCallback(CCObject* pSender)
{
    CCScene* s = new JSONTestScene();
    s->addChild( restartAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void JSONDemo::nextCallback(CCObject* pSender)
{
    CCScene* s = new JSONTestScene();
    s->addChild( nextAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void JSONDemo::backCallback(CCObject* pSender)
{
    CCScene* s = new JSONTestScene();
    s->addChild( backAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

//------------------------------------------------------------------
//
// Create Database
//
//------------------------------------------------------------------
void JSONParsing::onEnter()
{
    JSONDemo::onEnter();

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
    CCJSONObject* jo = CCJSONObject::create("Files/test_json");
    CCJSONArray* ja = jo->optJSONArray("f");
    CCJSONObject* jo2 = ja->optJSONObject(5);
    string stringVal = jo2->optString("f2");
    int intVal = jo2->optInt("f2");
    float floatVal = jo2->optFloat("f2");
    bool boolVal = jo2->optBool("f2");  // only when string is "true", the boolean value will be true, otherwise it is false
    
    char buf[256];
    sprintf(buf, "JSON parsing result\nsource file: Files/test_json\n\nkey: f2\nstring value: \"%s\"\nbool value: %s\nint value: %d\nfloat value: %.2f",
            +            stringVal.c_str(), boolVal ? "true" : "false", intVal, floatVal);
    CCLabelTTF* label = CCLabelTTF::create(buf, "Helvetica", 16);
    label->setPosition(ccp(origin.x + visibleSize.width / 2,
                           +                           origin.y + visibleSize.height / 2));
    addChild(label);
}

std::string JSONParsing::subtitle()
{
    return "JSON Parsing";
}