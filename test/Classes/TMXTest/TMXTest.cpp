#include "TMXTest.h"
#include "../testResource.h"
#include "cocos2d.h"

TESTLAYER_CREATE_FUNC(TMXHexagonalDemo);
TESTLAYER_CREATE_FUNC(TMXIsometricDemo);
TESTLAYER_CREATE_FUNC(TMXOrthogonalDemo);
TESTLAYER_CREATE_FUNC(TMXOrthogonalColorDemo);
TESTLAYER_CREATE_FUNC(TMXOrthogonalEditDemo);
TESTLAYER_CREATE_FUNC(TMXOrthogonalFlipDemo);

static NEWTESTFUNC createFunctions[] = {
	CF(TMXHexagonalDemo),
    CF(TMXIsometricDemo),
	CF(TMXOrthogonalDemo),
	CF(TMXOrthogonalColorDemo),
	CF(TMXOrthogonalEditDemo),
	CF(TMXOrthogonalFlipDemo)
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
// Tile map base class
//
//------------------------------------------------------------------
void TMXBaseDemo::onEnter()
{
    TMXDemo::onEnter();
	
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	// add map
	m_map = createMap();
	m_map->setPosition(origin);
	m_map->setDebugDrawObjects(true);
	addChild(m_map);
	
	// touch flag
	m_sprite = CCSprite::create("Images/a.png");
	m_sprite->setVisible(false);
	m_map->addChild(m_sprite, MAX_INT);
	
	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
}

bool TMXBaseDemo::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
	m_lastLoc = pTouch->getLocation();
	
	return true;
}

void TMXBaseDemo::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {
	CCPoint location = pTouch->getLocation();
	CCPoint delta = ccpSub(location, m_lastLoc);
	m_lastLoc = location;
	
	CCPoint pos = m_map->getPosition();
	m_map->setPosition(ccpAdd(pos, delta));
}

void TMXBaseDemo::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
	
}

void TMXBaseDemo::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {
	
}

//------------------------------------------------------------------
//
// Isometric map
//
//------------------------------------------------------------------
void TMXIsometricDemo::onEnter()
{
    TMXBaseDemo::onEnter();
	
	// print some properties
	CBTMXObjectGroup* og = m_map->getObjectGroup("Object Layer 1");
	CCLOG("object group property: weather: %s", og->getProperty("weather").c_str());
	CCLOG("object count: %d", og->getObjectCount());
	CBTMXObject* obj = og->getObjectAt(0);
	CCLOG("first object name: %s, type: %s, favorite: %s",
		  obj->getName().c_str(),
		  obj->getType().c_str(),
		  obj->getProperty("favorite").c_str());
}

CBTMXTileMap* TMXIsometricDemo::createMap() {
	return CBTMXTileMap::createWithXMLFile("tmx/iso_test1.tmx");
}

std::string TMXIsometricDemo::subtitle()
{
    return "Isometric";
}

bool TMXIsometricDemo::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
	TMXBaseDemo::ccTouchBegan(pTouch, pEvent);
	
	CBTMXLayer* layer = m_map->getLayerAt(0);
	CCPoint loc = layer->convertToNodeSpace(m_lastLoc);
	ccPosition d = layer->getTileCoordinateAt(loc.x, loc.y);
	CCPoint pos = layer->getPositionAt(d.x, d.y);
	if(d.x != -1) {
		m_sprite->setVisible(true);
		m_sprite->setPosition(ccp(pos.x + layer->getTileWidth() / 2, pos.y + layer->getTileHeight() / 2));
	} else {
		m_sprite->setVisible(false);
	}
	
	return true;
}

//------------------------------------------------------------------
//
// Hexagonal map
//
//------------------------------------------------------------------
void TMXHexagonalDemo::onEnter()
{
    TMXBaseDemo::onEnter();
}

CBTMXTileMap* TMXHexagonalDemo::createMap() {
	return CBTMXTileMap::createWithXMLFile("tmx/hexa_test.tmx");
}

std::string TMXHexagonalDemo::subtitle()
{
    return "Hexagonal";
}

bool TMXHexagonalDemo::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
	TMXBaseDemo::ccTouchBegan(pTouch, pEvent);
	
	CBTMXLayer* layer = (CBTMXLayer*)m_map->getLayerAt(0);
	CCPoint loc = layer->convertToNodeSpace(m_lastLoc);
	ccPosition d = layer->getTileCoordinateAt(loc.x, loc.y);
	
	// place sprite
	CCPoint pos = layer->getPositionAt(d.x, d.y);
	if(d.x != -1) {
		m_sprite->setVisible(true);
		m_sprite->setPosition(ccp(pos.x + layer->getTileWidth() / 2, pos.y + layer->getTileHeight() / 2));
	} else {
		m_sprite->setVisible(false);
	}
	
	// perform rotation
	CCSprite* sprite = layer->tileAt(d.x, d.y);
	if(sprite) {
		sprite->stopAllActions();
		sprite->setRotation(0);
		CCRotateBy* rotate = CCRotateBy::create(2.f, 360);
		sprite->runAction(rotate);
	}
	
	return true;
}

//------------------------------------------------------------------
//
// Orthogonal map
//
//------------------------------------------------------------------
void TMXOrthogonalDemo::onEnter()
{
    TMXBaseDemo::onEnter();
}

CBTMXTileMap* TMXOrthogonalDemo::createMap() {
	return CBTMXTileMap::createWithXMLFile("tmx/ortho_multi_tileset.tmx");
}

std::string TMXOrthogonalDemo::subtitle()
{
    return "Orthogonal - Multiple Tilesets";
}

bool TMXOrthogonalDemo::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
	TMXBaseDemo::ccTouchBegan(pTouch, pEvent);
	
	CBTMXLayer* layer = (CBTMXLayer*)m_map->getLayerAt(0);
	CCPoint loc = layer->convertToNodeSpace(m_lastLoc);
	ccPosition d = layer->getTileCoordinateAt(loc.x, loc.y);
	
	// place sprite
	CCPoint pos = layer->getPositionAt(d.x, d.y);
	if(d.x != -1) {
		m_sprite->setVisible(true);
		m_sprite->setPosition(ccp(pos.x + layer->getTileWidth() / 2, pos.y + layer->getTileHeight() / 2));
	} else {
		m_sprite->setVisible(false);
	}
	
	// perform rotation
	CCSprite* sprite = layer->tileAt(d.x, d.y);
	if(sprite) {
		sprite->stopAllActions();
		sprite->setRotation(0);
		CCRotateBy* rotate = CCRotateBy::create(2.f, 360);
		sprite->runAction(rotate);
	}
	
	return true;
}

//------------------------------------------------------------------
//
// Orthogonal - change color
//
//------------------------------------------------------------------
void TMXOrthogonalColorDemo::onEnter()
{
    TMXBaseDemo::onEnter();
}

CBTMXTileMap* TMXOrthogonalColorDemo::createMap() {
	return CBTMXTileMap::createWithXMLFile("tmx/ortho_multi_tileset.tmx");
}

std::string TMXOrthogonalColorDemo::subtitle()
{
    return "Orthogonal - Change Color";
}

bool TMXOrthogonalColorDemo::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
	TMXBaseDemo::ccTouchBegan(pTouch, pEvent);
	
	CBTMXLayer* layer = (CBTMXLayer*)m_map->getLayerAt(0);
	CCPoint loc = layer->convertToNodeSpace(m_lastLoc);
	ccPosition d = layer->getTileCoordinateAt(loc.x, loc.y);
	layer->setTileColorAt(cc4BLUE, d.x, d.y);
	
	return true;
}

//------------------------------------------------------------------
//
// Orthogonal, touch to add or delete
//
//------------------------------------------------------------------
void TMXOrthogonalEditDemo::onEnter()
{
    TMXBaseDemo::onEnter();
}

CBTMXTileMap* TMXOrthogonalEditDemo::createMap() {
	return CBTMXTileMap::createWithXMLFile("tmx/ortho_flip_test.tmx");
}

std::string TMXOrthogonalEditDemo::subtitle()
{
    return "Orthogonal - Touch to Add/Delete";
}

bool TMXOrthogonalEditDemo::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
	TMXBaseDemo::ccTouchBegan(pTouch, pEvent);
	
	CBTMXLayer* layer = (CBTMXLayer*)m_map->getLayerAt(0);
	CCPoint loc = layer->convertToNodeSpace(m_lastLoc);
	ccPosition d = layer->getTileCoordinateAt(loc.x, loc.y);
	int gid = layer->getGidAt(d.x, d.y);
	if(gid <= 0) {
		// add
		layer->setTileAt(CCRANDOM_0_X_INT(40) + 1, d.x, d.y);
	} else {
		// delete
		layer->removeTileAt(d.x, d.y);
	}
	
	return true;
}

//------------------------------------------------------------------
//
// Orthogonal, to test flip
//
//------------------------------------------------------------------
void TMXOrthogonalFlipDemo::onEnter()
{
    TMXBaseDemo::onEnter();
}

CBTMXTileMap* TMXOrthogonalFlipDemo::createMap() {
	return CBTMXTileMap::createWithXMLFile("tmx/ortho_flip_test.tmx");
}

std::string TMXOrthogonalFlipDemo::subtitle()
{
    return "Orthogonal - Tile Flip";
}

bool TMXOrthogonalFlipDemo::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
	TMXBaseDemo::ccTouchBegan(pTouch, pEvent);
	
	CBTMXLayer* layer = (CBTMXLayer*)m_map->getLayerAt(0);
	CCPoint loc = layer->convertToNodeSpace(m_lastLoc);
	ccPosition d = layer->getTileCoordinateAt(loc.x, loc.y);
	int gid = layer->getGidAt(d.x, d.y);
	int pureGid = gid & kCBTMXTileFlagFlipMask;
	if((gid & kCBTMXTileFlagFlipAll) == kCBTMXTileFlagFlipAll)
		layer->updateTileAt(pureGid, d.x, d.y);
	else if((gid & kCBTMXTileFlagFlipH) == kCBTMXTileFlagFlipH)
		layer->updateTileAt(pureGid | kCBTMXTileFlagFlipV, d.x, d.y);
	else if((gid & kCBTMXTileFlagFlipV) == kCBTMXTileFlagFlipV)
		layer->updateTileAt(pureGid | kCBTMXTileFlagFlipDiagonal, d.x, d.y);
	else if((gid & kCBTMXTileFlagFlipDiagonal) == kCBTMXTileFlagFlipDiagonal)
		layer->updateTileAt(pureGid | kCBTMXTileFlagFlipAll, d.x, d.y);
	else
		layer->updateTileAt(pureGid | kCBTMXTileFlagFlipH, d.x, d.y);
	
	return true;
}