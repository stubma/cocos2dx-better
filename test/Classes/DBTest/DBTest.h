#ifndef _DBTest_H_
#define _DBTest_H_

#include "../testBasic.h"
#include "cocos2d-better.h"

using namespace std;
USING_NS_CC;


// the class inherit from TestScene
// every Scene each test used must inherit from TestScene,
// make sure the test have the menu item for back to main menu
class DBTestScene : public TestScene
{
public:
    virtual void runThisTest();
};

class DBDemo : public CCLayer
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

class DBCreateDatabase : public DBDemo
{
private:
	CCLabelTTF* m_hintLabel;
	
public:
    virtual void onEnter();
    virtual string subtitle();
	
	void onCreateDatabaseClicked();
	void onDeleteDatabaseClicked();
};

class DBSQLFile : public DBDemo
{
private:
	CCLabelTTF* m_hintLabel;
	
public:
    virtual void onEnter();
    virtual string subtitle();
	
	void onExecuteSQLFileClicked();
	void onDeleteDatabaseClicked();
};

class DBTransaction : public DBDemo
{
private:
	CCLabelTTF* m_hintLabel;
	CCDatabase* m_db;
	
public:
	DBTransaction();
	virtual ~DBTransaction();
    virtual void onEnter();
    virtual string subtitle();
	
	void onInsertClicked();
	void onCommitClicked();
	void onRollbackClicked();
};

#endif
