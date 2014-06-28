#include "StoryTest.h"
#include "../testResource.h"
#include "cocos2d.h"
#include "cocos2d-better.h"

TESTLAYER_CREATE_FUNC(Story1);

static NEWTESTFUNC createFunctions[] = {
    CF(Story1)
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

void StoryTestScene::runThisTest()
{
    sceneIdx = -1;
    addChild(nextAction());

    CCDirector::sharedDirector()->replaceScene(this);
}


std::string StoryDemo::title()
{
    return "StoryTest";
}

std::string StoryDemo::subtitle()
{
    return "";
}

void StoryDemo::onEnter()
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
    CCMenuItemImage *item1 = CCMenuItemImage::create(s_pPathB1, s_pPathB2, this, menu_selector(StoryDemo::backCallback) );
    CCMenuItemImage *item2 = CCMenuItemImage::create(s_pPathR1, s_pPathR2, this, menu_selector(StoryDemo::restartCallback) );
    CCMenuItemImage *item3 = CCMenuItemImage::create(s_pPathF1, s_pPathF2, this, menu_selector(StoryDemo::nextCallback) );

    CCMenu *menu = CCMenu::create(item1, item2, item3, NULL);

    menu->setPosition(CCPointZero);
    item1->setPosition(ccp(VisibleRect::center().x - item2->getContentSize().width*2, VisibleRect::bottom().y+item2->getContentSize().height/2));
    item2->setPosition(ccp(VisibleRect::center().x, VisibleRect::bottom().y+item2->getContentSize().height/2));
    item3->setPosition(ccp(VisibleRect::center().x + item2->getContentSize().width*2, VisibleRect::bottom().y+item2->getContentSize().height/2));

    addChild(menu, 1);
}

void StoryDemo::onExit()
{
    CCLayer::onExit();
}

void StoryDemo::restartCallback(CCObject* pSender)
{
    CCScene* s = new StoryTestScene();
    s->addChild( restartAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void StoryDemo::nextCallback(CCObject* pSender)
{
    CCScene* s = new StoryTestScene();
    s->addChild( nextAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void StoryDemo::backCallback(CCObject* pSender)
{
    CCScene* s = new StoryTestScene();
    s->addChild( backAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

//------------------------------------------------------------------
//
// Story parsing
//
//------------------------------------------------------------------
void Story1::onEnter()
{
    StoryDemo::onEnter();
    
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("Files/Cowboy.ExportJson");
    
    CCStoryLayer* storyLayer = CCStoryLayer::create();
    addChild(storyLayer);
    storyLayer->preloadStoryFile("Files/story_test.lua");
    storyLayer->playStory();
}

void Story1::onExit() {
    StoryDemo::onExit();
    
    CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo("Files/Cowboy.ExportJson");
}

std::string Story1::subtitle()
{
    return "Story 1";
}