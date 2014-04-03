#include "DBTest.h"
#include "../testResource.h"
#include "cocos2d.h"

TESTLAYER_CREATE_FUNC(DBCreateDatabase);
TESTLAYER_CREATE_FUNC(DBSQLFile);
TESTLAYER_CREATE_FUNC(DBTransaction);

static NEWTESTFUNC createFunctions[] = {
    CF(DBCreateDatabase),
	CF(DBSQLFile),
	CF(DBTransaction)
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

void DBTestScene::runThisTest()
{
    sceneIdx = -1;
    addChild(nextAction());

    CCDirector::sharedDirector()->replaceScene(this);
}


std::string DBDemo::title()
{
    return "DBTest";
}

std::string DBDemo::subtitle()
{
    return "";
}

void DBDemo::onEnter()
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
    CCMenuItemImage *item1 = CCMenuItemImage::create(s_pPathB1, s_pPathB2, this, menu_selector(DBDemo::backCallback) );
    CCMenuItemImage *item2 = CCMenuItemImage::create(s_pPathR1, s_pPathR2, this, menu_selector(DBDemo::restartCallback) );
    CCMenuItemImage *item3 = CCMenuItemImage::create(s_pPathF1, s_pPathF2, this, menu_selector(DBDemo::nextCallback) );

    CCMenu *menu = CCMenu::create(item1, item2, item3, NULL);

    menu->setPosition(CCPointZero);
    item1->setPosition(ccp(VisibleRect::center().x - item2->getContentSize().width*2, VisibleRect::bottom().y+item2->getContentSize().height/2));
    item2->setPosition(ccp(VisibleRect::center().x, VisibleRect::bottom().y+item2->getContentSize().height/2));
    item3->setPosition(ccp(VisibleRect::center().x + item2->getContentSize().width*2, VisibleRect::bottom().y+item2->getContentSize().height/2));

    addChild(menu, 1);
}

void DBDemo::onExit()
{
    CCLayer::onExit();
}

void DBDemo::restartCallback(CCObject* pSender)
{
    CCScene* s = new DBTestScene();
    s->addChild( restartAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void DBDemo::nextCallback(CCObject* pSender)
{
    CCScene* s = new DBTestScene();
    s->addChild( nextAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void DBDemo::backCallback(CCObject* pSender)
{
    CCScene* s = new DBTestScene();
    s->addChild( backAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

//------------------------------------------------------------------
//
// Create Database
//
//------------------------------------------------------------------
void DBCreateDatabase::onEnter()
{
    DBDemo::onEnter();

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	CCLabelTTF* label1 = CCLabelTTF::create("Create Database", "Helvetica", 24);
	CCMenuItemLabel* item1 = CCMenuItemLabel::create(label1, this, menu_selector(DBCreateDatabase::onCreateDatabaseClicked));
	CCLabelTTF* label2 = CCLabelTTF::create("Delete Database", "Helvetica", 24);
	CCMenuItemLabel* item2 = CCMenuItemLabel::create(label2, this, menu_selector(DBCreateDatabase::onDeleteDatabaseClicked));
	CCMenu* menu = CCMenu::create(item1, item2, NULL);
	menu->alignItemsVertically();
	menu->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	addChild(menu);
	
	m_hintLabel = CCLabelTTF::create("value is", "Helvetica", 14);
	m_hintLabel->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 6));
	addChild(m_hintLabel);
}

std::string DBCreateDatabase::subtitle()
{
    return "Create Database";
}

void DBCreateDatabase::onCreateDatabaseClicked() {
	string path = CCUtils::externalize("create_database_test.db");
	
	// check existence of database
	if(CCUtils::isPathExistent(path)) {
		m_hintLabel->setString("database already exists, so won't create it again");
	} else {
		// create database and open it
		CCDatabase* db = CCDatabase::create(path);
		db->open();
		
		// create a test table into it
		int value = 1000 * CCRANDOM_0_1();
		db->executeUpdate("CREATE TABLE test (_id INTEGER PRIMARY KEY autoincrement, test_column INTEGER)");
		db->executeUpdate("INSERT INTO test (test_column) VALUES (%d)", value);
		
		// the database is auto released before make returns, and it will be closed when it is deconstructed
		// so no need to call close here, just update hint label
		char buf[64];
		sprintf(buf, "value is %d", value);
		m_hintLabel->setString(buf);
	}
}

void DBCreateDatabase::onDeleteDatabaseClicked() {
	string path = CCUtils::externalize("create_database_test.db");
	
	if(CCUtils::deleteFile(path)) {
		m_hintLabel->setString("database file deleted, you can create it again");
	} else {
		m_hintLabel->setString("failed to delete database file");
	}
}

//------------------------------------------------------------------
//
// SQL File
//
//------------------------------------------------------------------
void DBSQLFile::onEnter()
{
    DBDemo::onEnter();
	
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	CCLabelTTF* label1 = CCLabelTTF::create("Execute SQL File", "Helvetica", 24);
	CCMenuItemLabel* item1 = CCMenuItemLabel::create(label1, this, menu_selector(DBSQLFile::onExecuteSQLFileClicked));
	CCLabelTTF* label2 = CCLabelTTF::create("Delete Database", "Helvetica", 24);
	CCMenuItemLabel* item2 = CCMenuItemLabel::create(label2, this, menu_selector(DBSQLFile::onDeleteDatabaseClicked));
	CCMenu* menu = CCMenu::create(item1, item2, NULL);
	menu->alignItemsVertically();
	menu->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	addChild(menu);
	
	m_hintLabel = CCLabelTTF::create("", "Helvetica", 14);
	m_hintLabel->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 6));
	addChild(m_hintLabel);
}

string DBSQLFile::subtitle()
{
    return "SQL File";
}

void DBSQLFile::onExecuteSQLFileClicked() {
	string path = CCUtils::externalize("sqlfile_test.db");
	if(!CCUtils::isPathExistent(path)) {
		// create database and open it
		CCDatabase* db = CCDatabase::create(path);
		db->open();
		
		// execute sql file now
		if(db->executeSQL("Files/create_database.sql"))
			m_hintLabel->setString("Executed successful, please manually verify the database");
		else
			m_hintLabel->setString("SQL file is failed to be executed, nothing applied to database");
	}
}

void DBSQLFile::onDeleteDatabaseClicked() {
	string path = CCUtils::externalize("sqlfile_test.db");
	
	if(CCUtils::deleteFile(path)) {
		m_hintLabel->setString("database file deleted, you can create it again");
	} else {
		m_hintLabel->setString("failed to delete database file");
	}
}

//------------------------------------------------------------------
//
// Transaction
//
//------------------------------------------------------------------
DBTransaction::DBTransaction() :
		m_db(NULL) {
}

DBTransaction::~DBTransaction() {
	CC_SAFE_RELEASE(m_db);
}

void DBTransaction::onEnter()
{
    DBDemo::onEnter();
	
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	CCLabelTTF* label1 = CCLabelTTF::create("Insert One Row", "Helvetica", 24);
	CCMenuItemLabel* item1 = CCMenuItemLabel::create(label1, this, menu_selector(DBTransaction::onInsertClicked));
	CCLabelTTF* label2 = CCLabelTTF::create("Commit", "Helvetica", 24);
	CCMenuItemLabel* item2 = CCMenuItemLabel::create(label2, this, menu_selector(DBTransaction::onCommitClicked));
	CCLabelTTF* label3 = CCLabelTTF::create("Rollback", "Helvetica", 24);
	CCMenuItemLabel* item3 = CCMenuItemLabel::create(label3, this, menu_selector(DBTransaction::onRollbackClicked));
	CCMenu* menu = CCMenu::create(item1, item2, item3, NULL);
	menu->alignItemsVertically();
	menu->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	addChild(menu);
	
	m_hintLabel = CCLabelTTF::create("row count: 0", "Helvetica", 14);
	m_hintLabel->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 6));
	addChild(m_hintLabel);
	
	// open database, if it is not there, will create it
	string path = CCUtils::externalize("transaction_test.db");
	m_db = CCDatabase::create(path);
	m_db->open();
	m_db->retain();
	
	// if no test table, create it
	if(!m_db->tableExists("test"))
		m_db->executeUpdate("CREATE TABLE test (_id INTEGER PRIMARY KEY autoincrement, test_column INTEGER)");
	
	// query the value inserted
	int rowCount = m_db->intForQuery("SELECT count() FROM test");
	char buf[64];
	sprintf(buf, "row count: %d", rowCount);
	m_hintLabel->setString(buf);
}

string DBTransaction::subtitle()
{
    return "Transaction";
}

void DBTransaction::onInsertClicked() {
	// if not in transaction, start transaction
	if(!m_db->isInTransaction())
		m_db->beginTransaction();
	
	// execute insertion
	m_db->executeUpdate("INSERT INTO test (test_column) VALUES (%d)", 1000 * CCRANDOM_0_1());
}

void DBTransaction::onCommitClicked() {
	if(m_db->commit()) {
		// query the value inserted
		int rowCount = m_db->intForQuery("SELECT count() from test");
		char buf[64];
		sprintf(buf, "row count: %d", rowCount);
		m_hintLabel->setString(buf);
	} else {
		m_hintLabel->setString("commit transaction failed");
	}
}

void DBTransaction::onRollbackClicked() {
	if(m_db->rollback()) {
		m_hintLabel->setString("rollback transaction successful");
	} else {
		m_hintLabel->setString("rollback transaction failed");
	}
}