#include "NetworkTest.h"
#include "../testResource.h"
#include "cocos2d.h"
#include "cocos2d-better.h"

TESTLAYER_CREATE_FUNC(NetworkTCP);
TESTLAYER_CREATE_FUNC(NetworkUDP);

static NEWTESTFUNC createFunctions[] = {
    CF(NetworkTCP),
	CF(NetworkUDP)
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
NetworkTCP::~NetworkTCP() {
    m_hub->stopAll();
}

void NetworkTCP::onExit() {
    NetworkDemo::onExit();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->removeObserver(this, kCCNotificationTCPSocketConnected);
    nc->removeObserver(this, kCCNotificationTCPSocketDisconnected);
    nc->removeObserver(this, kCCNotificationPacketReceived);
}

void NetworkTCP::onEnter()
{
    NetworkDemo::onEnter();

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	// to test this sample, find echoserver.py in server folder, run it with python
	// you need install greenlet and gevent
	CCMenuItemLabel* item1 = CCMenuItemLabel::create(CCLabelTTF::create("Send Hello", "Helvetica", 40 / CC_CONTENT_SCALE_FACTOR()),
													 this,
													 menu_selector(NetworkTCP::onSendClicked));
	item1->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	
	CCMenu* menu = CCMenu::create(item1, NULL);
	menu->setAnchorPoint(CCPointZero);
	menu->setPosition(CCPointZero);
	addChild(menu);
	
	// received content
	m_recv = CCLabelTTF::create("", "Helvetica", 20 / CC_CONTENT_SCALE_FACTOR());
	m_recv->setPosition(ccp(origin.x + visibleSize.width / 2,
							origin.y + visibleSize.height / 5));
	addChild(m_recv);
    
    // notification observer
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, callfuncO_selector(NetworkTCP::onTCPSocketConnected), kCCNotificationTCPSocketConnected, NULL);
    nc->addObserver(this, callfuncO_selector(NetworkTCP::onTCPSocketDisonnected), kCCNotificationTCPSocketDisconnected, NULL);
    nc->addObserver(this, callfuncO_selector(NetworkTCP::onPacketReceived), kCCNotificationPacketReceived, NULL);
	
	// change ip to your server
	// registerCallback must be invoked before createSocket otherwise connect event is lost
	m_hub = CCTCPSocketHub::create();
    m_hub->setRawPolicy(true);
	m_hub->createSocket("172.16.96.60", 6000, 1);
}

void NetworkTCP::onSendClicked(CCObject* sender) {
	static int index = 0;
	char buf[32];
	sprintf(buf, "Hello: %d\r\n", index++);
	CCPacket* p = CCPacket::createRawPacket(buf, strlen(buf));
	m_hub->sendPacket(1, p);
}

void NetworkTCP::onTCPSocketConnected(CCTCPSocket* s) {
    CCLOG("connected: %d", s->getTag());
}

void NetworkTCP::onTCPSocketDisonnected(CCTCPSocket* s) {
    CCLOG("disconnected: %d", s->getTag());
}

void NetworkTCP::onPacketReceived(CCPacket* p) {
    string ret;
    CCByteBuffer bb(p->getBuffer(), p->getPacketLength(), p->getPacketLength());
	bb.readLine(ret);
	if(!ret.empty()) {
		char buf[65535];
		sprintf(buf, "Client get: %s", ret.c_str());
		m_recv->setString(buf);
	}
}

std::string NetworkTCP::subtitle()
{
    return "TCP Socket - Echo Server";
}

//------------------------------------------------------------------
//
// UDP Socket
//
//------------------------------------------------------------------
NetworkUDP::~NetworkUDP() {
    CC_SAFE_RELEASE(m_hub);
}

void NetworkUDP::onEnter()
{
    NetworkDemo::onEnter();
	
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	// to test this sample, find udp_server.py in server folder, run it with python
	// you need install greenlet and gevent
	CCMenuItemLabel* item1 = CCMenuItemLabel::create(CCLabelTTF::create("Send Hello", "Helvetica", 40 / CC_CONTENT_SCALE_FACTOR()),
													 this,
													 menu_selector(NetworkUDP::onSendClicked));
	item1->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	
	CCMenu* menu = CCMenu::create(item1, NULL);
	menu->setAnchorPoint(CCPointZero);
	menu->setPosition(CCPointZero);
	addChild(menu);
	
	// received content
	m_recv = CCLabelTTF::create("", "Helvetica", 20 / CC_CONTENT_SCALE_FACTOR());
	m_recv->setPosition(ccp(origin.x + visibleSize.width / 2,
							origin.y + visibleSize.height / 5));
	addChild(m_recv);
	
	// change ip to your server
	// registerCallback must be invoked before createSocket otherwise bound event is lost
	m_hub = CCUDPSocketHub::create();
	m_hub->registerCallback(1, this);
	m_hub->createSocket("192.168.1.104", 9000, 1);
    CC_SAFE_RETAIN(m_hub);
}

void NetworkUDP::onSendClicked(CCObject* sender) {
	static int index = 0;
	char buf[32];
	sprintf(buf, "Hello: %d", index++);
	CCByteBuffer packet;
	packet.writeLine(buf);
	m_hub->sendPacket(1, &packet);
}

void NetworkUDP::onUDPSocketBound(int tag) {
	CCLOG("bound: %d", tag);
}

void NetworkUDP::onUDPSocketClosed(int tag) {
	CCLOG("closed: %d", tag);
}

void NetworkUDP::onUDPSocketData(int tag, CCByteBuffer& bb) {
	string ret;
	bb.readLine(ret);
	if(!ret.empty()) {
		char buf[65535];
		sprintf(buf, "Client get: %s", ret.c_str());
		m_recv->setString(buf);
	}
}

std::string NetworkUDP::subtitle()
{
    return "UDP Socket";
}