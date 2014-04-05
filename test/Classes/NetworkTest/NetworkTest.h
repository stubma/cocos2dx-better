#ifndef _NetworkTest_H_
#define _NetworkTest_H_

#include "../testBasic.h"
#include "cocos2d-better.h"

using namespace std;
USING_NS_CC;

// the class inherit from TestScene
// every Scene each test used must inherit from TestScene,
// make sure the test have the menu item for back to main menu
class NetworkTestScene : public TestScene
{
public:
    virtual void runThisTest();
};

class NetworkDemo : public CCLayer
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

class NetworkFileDownloader : public NetworkDemo
{
public:
    virtual ~NetworkFileDownloader();
    virtual void onEnter();
    virtual void onExit();
    virtual string subtitle();
    
    void onHttpDone(CBHttpResponse* response);
};

class NetworkTCP : public NetworkDemo
{
private:
	CCTCPSocketHub* m_hub;
	CCLabelTTF* m_recv;
	
private:
	void onSendClicked(CCObject* sender);
	void onTCPSocketConnected(CCTCPSocket* s);
    void onTCPSocketDisonnected(CCTCPSocket* s);
    void onPacketReceived(CCPacket* p);
    
public:
    virtual ~NetworkTCP();
    virtual void onEnter();
    virtual void onExit();
    virtual string subtitle();
};

class NetworkUDP : public NetworkDemo
{
private:
	CCUDPSocketHub* m_hub;
	CCLabelTTF* m_recv;
	
private:
	void onSendClicked(CCObject* sender);
    void onUDPSocketConnected(CCUDPSocket* s);
    void onUDPSocketDisonnected(CCUDPSocket* s);
    void onPacketReceived(CCPacket* p);
    
public:
    virtual ~NetworkUDP();
    virtual void onEnter();
    virtual void onExit();
    virtual string subtitle();
};

#endif