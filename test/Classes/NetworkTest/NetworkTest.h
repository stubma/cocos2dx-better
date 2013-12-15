#ifndef _NetworkTest_H_
#define _NetworkTest_H_

#include "../testBasic.h"
#include "cocos2d-better.h"

using namespace std;
USING_NS_CC;


enum
{
    NETWORK_TCP = 0,
    NETWORK_LAYER_COUNT,
};


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

class NetworkTCP : public NetworkDemo, public CCTCPSocketListener
{
private:
	CCTCPSocketHub* m_hub;
	CCLabelTTF* m_recv;
	
private:
	void onSendClicked(CCObject* sender);
	
public:
    virtual void onEnter();
    virtual string subtitle();

	// CCTCPSocketListener
	virtual void onTCPSocketConnected(int tag);
	virtual void onTCPSocketDisconnected(int tag);
	virtual void onTCPSocketData(int tag, CCByteBuffer& bb);
};

#endif