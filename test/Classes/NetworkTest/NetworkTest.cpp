#include "NetworkTest.h"
#include "../testResource.h"
#include "cocos2d.h"
#include "cocos2d-better.h"

TESTLAYER_CREATE_FUNC(NetworkTCP);

static NEWTESTFUNC createFunctions[] = {
    CF(NetworkTCP)
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

void NetworkTestScene::runThisTest()
{
    sceneIdx = -1;
    addChild(nextAction());

    CCDirector::sharedDirector()->replaceScene(this);
}


std::string NetworkDemo::title()
{
    return "NetworkTest";
}

std::string NetworkDemo::subtitle()
{
    return "";
}

void NetworkDemo::onEnter()
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
    CCMenuItemImage *item1 = CCMenuItemImage::create(s_pPathB1, s_pPathB2, this, menu_selector(NetworkDemo::backCallback) );
    CCMenuItemImage *item2 = CCMenuItemImage::create(s_pPathR1, s_pPathR2, this, menu_selector(NetworkDemo::restartCallback) );
    CCMenuItemImage *item3 = CCMenuItemImage::create(s_pPathF1, s_pPathF2, this, menu_selector(NetworkDemo::nextCallback) );

    CCMenu *menu = CCMenu::create(item1, item2, item3, NULL);

    menu->setPosition(CCPointZero);
    item1->setPosition(ccp(VisibleRect::center().x - item2->getContentSize().width*2, VisibleRect::bottom().y+item2->getContentSize().height/2));
    item2->setPosition(ccp(VisibleRect::center().x, VisibleRect::bottom().y+item2->getContentSize().height/2));
    item3->setPosition(ccp(VisibleRect::center().x + item2->getContentSize().width*2, VisibleRect::bottom().y+item2->getContentSize().height/2));

    addChild(menu, 1);
}

void NetworkDemo::onExit()
{
    CCLayer::onExit();
}

void NetworkDemo::restartCallback(CCObject* pSender)
{
    CCScene* s = new NetworkTestScene();
    s->addChild( restartAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void NetworkDemo::nextCallback(CCObject* pSender)
{
    CCScene* s = new NetworkTestScene();
    s->addChild( nextAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void NetworkDemo::backCallback(CCObject* pSender)
{
    CCScene* s = new NetworkTestScene();
    s->addChild( backAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

//------------------------------------------------------------------
//
// TCP Socket
//
//------------------------------------------------------------------
void NetworkTCP::onEnter()
{
    NetworkDemo::onEnter();

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	// you can run a echo server to test the tcp socket
	// you can find a echo server demo in gevent samples
	CCMenuItemLabel* item1 = CCMenuItemLabel::create(CCLabelTTF::create("Send Hello", "Helvetica", 40 / CC_CONTENT_SCALE_FACTOR()),
													 this,
													 menu_selector(NetworkTCP::onSendClicked));
	item1->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	
	CCMenu* menu = CCMenu::create(item1, NULL);
	menu->setAnchorPoint(CCPointZero);
	menu->setPosition(CCPointZero);
	addChild(menu);
	
	m_hub = CCTCPSocketHub::create();
	m_hub->createSocket("192.168.1.104", 6000, 1);
	m_hub->registerCallback(1, this);
	addChild(m_hub);
}

void NetworkTCP::onSendClicked(CCObject* sender) {
	CCByteBuffer packet;
	packet.writeLine("Hello");
	m_hub->sendPacket(1, &packet);
}

void NetworkTCP::onTCPSocketConnected(int tag) {
	CCLOG("connected: %d", tag);
}

void NetworkTCP::onTCPSocketDisconnected(int tag) {
	CCLOG("disconnected: %d", tag);
}

void NetworkTCP::onTCPSocketData(int tag, CCByteBuffer& bb) {
	string ret;
	bb.readLine(ret);
	if(!ret.empty())
		CCLOG("get data: %s", ret.c_str());
}

std::string NetworkTCP::subtitle()
{
    return "TCP Socket";
}