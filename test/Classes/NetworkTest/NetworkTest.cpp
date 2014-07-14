#include "NetworkTest.h"
#include "../testResource.h"
#include "cocos2d.h"
#include "cocos2d-better.h"

TESTLAYER_CREATE_FUNC(NetworkFileDownloader);
TESTLAYER_CREATE_FUNC(NetworkHttpGet);
TESTLAYER_CREATE_FUNC(NetworkTCP);
TESTLAYER_CREATE_FUNC(NetworkUDP);

static NEWTESTFUNC createFunctions[] = {
    CF(NetworkFileDownloader),
    CF(NetworkHttpGet),
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
// File Downloader
//
//------------------------------------------------------------------
NetworkFileDownloader::~NetworkFileDownloader() {
    CCFileDownloader::purge();
}

void NetworkFileDownloader::onEnter()
{
    NetworkDemo::onEnter();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    // file name
    m_fileLabel = CCLabelTTF::create("Current file", "Helvetica", 36 / CC_CONTENT_SCALE_FACTOR());
    m_fileLabel->setPosition(ccp(origin.x + visibleSize.width / 2,
                             origin.y + visibleSize.height / 2 + 40 / CC_CONTENT_SCALE_FACTOR()));
    addChild(m_fileLabel);
    
    // label
    m_label = CCLabelTTF::create("Getting", "Helvetica", 30 / CC_CONTENT_SCALE_FACTOR());
    m_label->setPosition(ccp(origin.x + visibleSize.width / 2,
                             origin.y + visibleSize.height / 2));
    addChild(m_label);
    
    // start request in asynchronous mode
    CCFileDownloader* d = CCFileDownloader::getInstance();
    d->addFile("http://mirrors.cnnic.cn/apache/ant/binaries/RELEASE-NOTES-1.2"); // small file
    d->addFile("http://mirrors.cnnic.cn/apache/ant/binaries/apache-ant-1.9.1-bin.zip"); // not exist, 0 size
    d->addFile("http://mirrors.cnnic.cn/apache/ant/binaries/apache-ant-1.9.3-bin.zip"); // 8 mega bytes
    d->start();
    
    // update
    scheduleUpdate();
}

void NetworkFileDownloader::onExit() {
    NetworkDemo::onEnter();
}

std::string NetworkFileDownloader::subtitle() {
    return "File Downloader";
}

void NetworkFileDownloader::update(float delta) {
    CCFileDownloader* d = CCFileDownloader::getInstance();
    if(d->isDownloading()) {
        m_fileLabel->setString(d->getCurrentDownloadingFileName().c_str());
        char buf[64];
        sprintf(buf, "%ld/%ld", d->getCurrentDownloadedSize(), d->getCurrentDownloadingFileSize());
        m_label->setString(buf);
    } else {
        m_fileLabel->setString("Done");
        m_label->setString("");
    }
}

//------------------------------------------------------------------
//
// Http Client - Get
//
//------------------------------------------------------------------
NetworkHttpGet::~NetworkHttpGet() {
    CC_SAFE_RELEASE(m_client);
}

void NetworkHttpGet::onEnter()
{
    NetworkDemo::onEnter();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    // label
    m_label = CCLabelTTF::create("Getting", "Helvetica", 36 / CC_CONTENT_SCALE_FACTOR());
    m_label->setPosition(ccp(origin.x + visibleSize.width / 2,
                             origin.y + visibleSize.height / 2));
    addChild(m_label);
    
    // cancel
	CCMenuItemLabel* item1 = CCMenuItemLabel::create(CCLabelTTF::create("Cancel", "Helvetica", 40 / CC_CONTENT_SCALE_FACTOR()),
													 this,
													 menu_selector(NetworkHttpGet::onCancelClicked));
	item1->setPosition(origin.x + visibleSize.width / 2,
                       origin.y + visibleSize.height / 2 - 60 / CC_CONTENT_SCALE_FACTOR());
	CCMenu* menu = CCMenu::create(item1, NULL);
	menu->setAnchorPoint(CCPointZero);
	menu->setPosition(CCPointZero);
	addChild(menu);
    
    // start request in asynchronous mode
    CBHttpClient* client = CBHttpClient::create();
    CBHttpRequest* req = CBHttpRequest::create();
    req->setTag(1);
    req->setUrl("http://mirrors.cnnic.cn/apache//ant/binaries/apache-ant-1.9.3-bin.zip");
    req->setMethod(CBHttpRequest::kHttpGet);
    client->asyncExecute(req);
    m_client = client;
    CC_SAFE_RETAIN(m_client);
    
    // listener
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(NetworkHttpGet::onHttpDone), kCCNotificationHttpRequestCompleted, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(NetworkHttpGet::onHttpData), kCCNotificationHttpDataReceived, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(NetworkHttpGet::onHttpHeaders), kCCNotificationHttpDidReceiveResponse, NULL);
}

void NetworkHttpGet::onExit() {
    NetworkDemo::onEnter();
    
    // remove listener
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, kCCNotificationHttpRequestCompleted);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, kCCNotificationHttpDataReceived);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, kCCNotificationHttpDidReceiveResponse);
}

void NetworkHttpGet::onCancelClicked(CCObject* sender) {
    m_client->cancel(1);
}

std::string NetworkHttpGet::subtitle() {
    return "Http Client - Get";
}

void NetworkHttpGet::onHttpHeaders(CBHttpResponse* response) {
    string contentLength = response->getHeader("Content-Length");
    m_fileLen = atoi(contentLength.c_str());
    m_recvLen = 0;
    CCLOG("file length is %d", m_fileLen);
    
    char buf[64];
    sprintf(buf, "%d/%d", m_recvLen, m_fileLen);
    m_label->setString(buf);
}

void NetworkHttpGet::onHttpData(CBHttpResponse* response) {
    m_recvLen += response->getData()->getSize();
    char buf[64];
    sprintf(buf, "%d/%d", m_recvLen, m_fileLen);
    m_label->setString(buf);
}

void NetworkHttpGet::onHttpDone(CBHttpResponse* response) {
    char buf[128];
    sprintf(buf, "done, success: %s", response->isSuccess() ? "true" : "false");
    m_label->setString(buf);
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
    m_hub->stopAll();
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
	
    // notification observer
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, callfuncO_selector(NetworkUDP::onUDPSocketConnected), kCCNotificationUDPSocketConnected, NULL);
    nc->addObserver(this, callfuncO_selector(NetworkUDP::onUDPSocketDisonnected), kCCNotificationUDPSocketDisconnected, NULL);
    nc->addObserver(this, callfuncO_selector(NetworkUDP::onPacketReceived), kCCNotificationPacketReceived, NULL);
    
	// change ip to your server
	// registerCallback must be invoked before createSocket otherwise bound event is lost
	m_hub = CCUDPSocketHub::create();
    m_hub->setRawPolicy(true);
	m_hub->createSocket("192.168.1.106", 9000, 1);
}

void NetworkUDP::onExit() {
    NetworkDemo::onExit();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->removeObserver(this, kCCNotificationUDPSocketConnected);
    nc->removeObserver(this, kCCNotificationUDPSocketDisconnected);
    nc->removeObserver(this, kCCNotificationPacketReceived);
}

void NetworkUDP::onUDPSocketConnected(CCUDPSocket* s) {
    CCLOG("bound: %d", s->getTag());
}

void NetworkUDP::onUDPSocketDisonnected(CCUDPSocket* s) {
    CCLOG("closed: %d", s->getTag());
}

void NetworkUDP::onPacketReceived(CCPacket* p) {
    string ret;
    CCByteBuffer bb(p->getBuffer(), p->getPacketLength(), p->getPacketLength());
	bb.readLine(ret);
	if(!ret.empty()) {
		char buf[65535];
		sprintf(buf, "Client get: %s", ret.c_str());
		m_recv->setString(buf);
	}
}

void NetworkUDP::onSendClicked(CCObject* sender) {
	static int index = 0;
	char buf[32];
	sprintf(buf, "Hello: %d\r\n", index++);
	CCPacket* p = CCPacket::createRawPacket(buf, strlen(buf));
	m_hub->sendPacket(1, p);
}

std::string NetworkUDP::subtitle() {
    return "UDP Socket";
}