#include "AnimTest.h"
#include "../testResource.h"
#include "cocos2d.h"
#include "cocos2d-better.h"

TESTLAYER_CREATE_FUNC(AnimMotionWelder);
TESTLAYER_CREATE_FUNC(AnimArctic);
TESTLAYER_CREATE_FUNC(AnimAuroraGT);
TESTLAYER_CREATE_FUNC(AnimSpriteX);
TESTLAYER_CREATE_FUNC(AnimSpriteX2011);
TESTLAYER_CREATE_FUNC(AnimClipMappingAuroraGT);

static NEWTESTFUNC createFunctions[] = {
    CF(AnimMotionWelder),
    CF(AnimArctic),
    CF(AnimAuroraGT),
    CF(AnimSpriteX),
    CF(AnimSpriteX2011),
    CF(AnimClipMappingAuroraGT)
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

void AnimTestScene::runThisTest()
{
    sceneIdx = -1;
    addChild(nextAction());

    CCDirector::sharedDirector()->replaceScene(this);
}


std::string AnimDemo::title()
{
    return "AnimTest";
}

std::string AnimDemo::subtitle()
{
    return "";
}

void AnimDemo::onEnter()
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
    CCMenuItemImage *item1 = CCMenuItemImage::create(s_pPathB1, s_pPathB2, this, menu_selector(AnimDemo::backCallback) );
    CCMenuItemImage *item2 = CCMenuItemImage::create(s_pPathR1, s_pPathR2, this, menu_selector(AnimDemo::restartCallback) );
    CCMenuItemImage *item3 = CCMenuItemImage::create(s_pPathF1, s_pPathF2, this, menu_selector(AnimDemo::nextCallback) );

    CCMenu *menu = CCMenu::create(item1, item2, item3, NULL);

    menu->setPosition(CCPointZero);
    item1->setPosition(ccp(VisibleRect::center().x - item2->getContentSize().width*2, VisibleRect::bottom().y+item2->getContentSize().height/2));
    item2->setPosition(ccp(VisibleRect::center().x, VisibleRect::bottom().y+item2->getContentSize().height/2));
    item3->setPosition(ccp(VisibleRect::center().x + item2->getContentSize().width*2, VisibleRect::bottom().y+item2->getContentSize().height/2));

    addChild(menu, 1);
}

void AnimDemo::onExit()
{
    CCLayer::onExit();
}

void AnimDemo::restartCallback(CCObject* pSender)
{
    CCScene* s = new AnimTestScene();
    s->addChild( restartAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void AnimDemo::nextCallback(CCObject* pSender)
{
    CCScene* s = new AnimTestScene();
    s->addChild( nextAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void AnimDemo::backCallback(CCObject* pSender)
{
    CCScene* s = new AnimTestScene();
    s->addChild( backAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

//------------------------------------------------------------------
//
// Motion Welder
//
//------------------------------------------------------------------
void AnimMotionWelder::onEnter()
{
    AnimDemo::onEnter();
    
    // surface
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    // texture
    CCTexture2D* tex1 = CCTextureCache::sharedTextureCache()->addImage("Files/mongo.png");
    
    // animation 1
    m_sprite1 = CCMWSprite::create("Files/test_motion_welder.anu", 0, tex1, NULL);
    m_sprite1->setLoopCount(-1);
    m_sprite1->setPosition(ccp(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height * 5 / 6));
    m_sprite1->setUnitInterval(0.1f);
    m_sprite1->setDebugDrawFrameRect(true);
    m_sprite1->setDebugDrawCollisionRect(true);
    addChild(m_sprite1);
    
    // animation 2
    m_sprite2 = CCMWSprite::create("Files/test_motion_welder.anu", 1, tex1, NULL);
    m_sprite2->setLoopCount(-1);
    m_sprite2->setPosition(ccp(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height * 4 / 6));
    m_sprite2->setUnitInterval(0.1f);
    m_sprite2->setDebugDrawFrameRect(true);
    m_sprite2->setDebugDrawCollisionRect(true);
    addChild(m_sprite2);
    
    // animation 2 but flip x
    m_sprite3 = CCMWSprite::create("Files/test_motion_welder.anu", 1, tex1, NULL);
    m_sprite3->setLoopCount(-1);
    m_sprite3->setFlipX(true);
    m_sprite3->setPosition(ccp(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height * 3 / 6));
    m_sprite3->setUnitInterval(0.1f);
    m_sprite3->setDebugDrawFrameRect(true);
    m_sprite3->setDebugDrawCollisionRect(true);
    addChild(m_sprite3);
    
    // animation 2 but ignore frame offset
    m_sprite4 = CCMWSprite::create("Files/test_motion_welder.anu", 1, tex1, NULL);
    m_sprite4->setLoopCount(-1);
    m_sprite4->setIgnoreFrameOffset(true);
    m_sprite4->setPosition(ccp(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height * 2 / 6));
    m_sprite4->setUnitInterval(0.1f);
    m_sprite4->setDebugDrawFrameRect(true);
    m_sprite4->setDebugDrawCollisionRect(true);
    addChild(m_sprite4);
    
    // animation 2 but ignore frame offset and flip x
    m_sprite5 = CCMWSprite::create("Files/test_motion_welder.anu", 1, tex1, NULL);
    m_sprite5->setLoopCount(-1);
    m_sprite5->setIgnoreFrameOffset(true);
    m_sprite5->setFlipX(true);
    m_sprite5->setPosition(ccp(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height / 6));
    m_sprite5->setUnitInterval(0.1f);
    m_sprite5->setDebugDrawFrameRect(true);
    m_sprite5->setDebugDrawCollisionRect(true);
    addChild(m_sprite5);
    
    scheduleUpdate();
    
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
}

bool AnimMotionWelder::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    m_sprite1->setPaused(!m_sprite1->isPaused());
    m_sprite2->setPaused(!m_sprite2->isPaused());
    m_sprite3->setPaused(!m_sprite3->isPaused());
    m_sprite4->setPaused(!m_sprite4->isPaused());
    m_sprite5->setPaused(!m_sprite5->isPaused());
    return false;
}

void AnimMotionWelder::update(float dt) {
    m_sprite1->tick(dt);
    m_sprite2->tick(dt);
    m_sprite3->tick(dt);
    m_sprite4->tick(dt);
    m_sprite5->tick(dt);
}

std::string AnimMotionWelder::subtitle()
{
    return "Motion Welder";
}

//------------------------------------------------------------------
//
// Arctic
//
//------------------------------------------------------------------
void AnimArctic::onEnter()
{
    AnimDemo::onEnter();
    
    // surface
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    // texture
    CCTextureCache* tc = CCTextureCache::sharedTextureCache();
    CCTexture2D* tex1 = tc->addImage("Files/fighterboby001.png");
    CCTexture2D* tex2 = tc->addImage("Files/fighterarmor001.png");
    CCTexture2D* tex3 = tc->addImage("Files/fighterweapon001.png");
    
    // animation 1
    m_sprite1 = CCArcticSprite::create("Files/fighter.aspr", 0, tex1, tex2, tex3, NULL);
    m_sprite1->setLoopCount(-1);
    m_sprite1->setPosition(ccp(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height * 3 / 4));
    m_sprite1->setUnitInterval(0.1f);
    m_sprite1->setDebugDrawFrameRect(true);
    m_sprite1->setDebugDrawCollisionRect(true);
    addChild(m_sprite1);
    
    // animation 2
    m_sprite2 = CCArcticSprite::create("Files/fighter.aspr", 1, tex1, tex2, tex3, NULL);
    m_sprite2->setLoopCount(-1);
    m_sprite2->setPosition(ccp(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height * 2 / 4));
    m_sprite2->setUnitInterval(0.1f);
    m_sprite2->setDebugDrawFrameRect(true);
    m_sprite2->setDebugDrawCollisionRect(true);
    addChild(m_sprite2);
    
    // animation 3
    m_sprite3 = CCArcticSprite::create("Files/fighter.aspr", 2, tex1, tex2, tex3, NULL);
    m_sprite3->setLoopCount(-1);
    m_sprite3->setPosition(ccp(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height / 4));
    m_sprite3->setUnitInterval(0.1f);
    m_sprite3->setDebugDrawFrameRect(true);
    m_sprite3->setDebugDrawCollisionRect(true);
    addChild(m_sprite3);
    
    scheduleUpdate();
    
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
}

bool AnimArctic::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    m_sprite1->setPaused(!m_sprite1->isPaused());
    m_sprite2->setPaused(!m_sprite2->isPaused());
    m_sprite3->setPaused(!m_sprite3->isPaused());
    return false;
}

void AnimArctic::update(float dt) {
    m_sprite1->tick(dt);
    m_sprite2->tick(dt);
    m_sprite3->tick(dt);
}

std::string AnimArctic::subtitle()
{
    return "Arctic";
}

//------------------------------------------------------------------
//
// Aurora GT
//
//------------------------------------------------------------------
void AnimAuroraGT::onEnter()
{
    AnimDemo::onEnter();
    
    // surface
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    // texture
    CCTextureCache* tc = CCTextureCache::sharedTextureCache();
    CCTexture2D* tex = tc->addImage("Files/prince.png");
    
    // animation 1
    m_sprite1 = CCAuroraSprite::create("Files/prince.bsprite", 78, tex, NULL);
    m_sprite1->setLoopCount(-1);
    m_sprite1->setPosition(ccp(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height * 3 / 4));
    m_sprite1->setUnitInterval(0.1f);
    m_sprite1->setDebugDrawFrameRect(true);
    m_sprite1->setDebugDrawCollisionRect(true);
    addChild(m_sprite1);
    
    // animation 2
    m_sprite2 = CCAuroraSprite::create("Files/prince.bsprite", 99, tex, NULL);
    m_sprite2->setLoopCount(-1);
    m_sprite2->setPosition(ccp(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height * 2 / 4));
    m_sprite2->setUnitInterval(0.1f);
    m_sprite2->setDebugDrawFrameRect(true);
    m_sprite2->setDebugDrawCollisionRect(true);
    addChild(m_sprite2);
    
    // animation 3
    m_sprite3 = CCAuroraSprite::create("Files/prince.bsprite", 66, tex, NULL);
    m_sprite3->setLoopCount(-1);
    m_sprite3->setPosition(ccp(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height / 4));
    m_sprite3->setUnitInterval(0.1f);
    m_sprite3->setDebugDrawFrameRect(true);
    m_sprite3->setDebugDrawCollisionRect(true);
    addChild(m_sprite3);
    
    scheduleUpdate();
    
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
}

bool AnimAuroraGT::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    m_sprite1->setPaused(!m_sprite1->isPaused());
    m_sprite2->setPaused(!m_sprite2->isPaused());
    m_sprite3->setPaused(!m_sprite3->isPaused());
    return false;
}

void AnimAuroraGT::update(float dt) {
    m_sprite1->tick(dt);
    m_sprite2->tick(dt);
    m_sprite3->tick(dt);
}

std::string AnimAuroraGT::subtitle()
{
    return "AuroraGT";
}

//------------------------------------------------------------------
//
// SpriteX
//
//------------------------------------------------------------------
void AnimSpriteX::onEnter()
{
    AnimDemo::onEnter();
    
    // surface
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    // texture
    CCTextureCache* tc = CCTextureCache::sharedTextureCache();
    CCTexture2D* tex = tc->addImage("Files/spx_test.png");
    
    // animation 1
    // SpriteX doesn't support tick mode, but you can use setForceTickMode
    // to enforce using tick mode. this animation use a large unit interval
    // so the animation plays slow
    m_sprite1 = CCSPXSprite::create("Files/spx_test.sprite", tex, 0);
    m_sprite1->setLoopCount(-1);
    m_sprite1->setPosition(ccp(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height * 7 / 8));
    m_sprite1->setForceTickMode(true);
    m_sprite1->setUnitInterval(0.5f);
    addChild(m_sprite1);
    
    // animation 2
    m_sprite2 = CCSPXSprite::create("Files/spx_test.sprite", tex, 1);
    m_sprite2->setLoopCount(-1);
    m_sprite2->setPosition(ccp(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height * 6 / 8));
    addChild(m_sprite2);
    
    // animation 3, 反着放
    m_sprite3 = CCSPXSprite::create("Files/spx_test.sprite", tex, 2);
    m_sprite3->setLoopCount(-1);
    m_sprite3->setPosition(ccp(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height * 5 / 8));
    m_sprite3->setReverse(true);
    addChild(m_sprite3);
    
    // animation 4
    m_sprite4 = CCSPXSprite::create("Files/spx_test.sprite", tex, 3);
    m_sprite4->setLoopCount(-1);
    m_sprite4->setPosition(ccp(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height * 4 / 8));
    addChild(m_sprite4);
    
    // animation 5, y轴倒转播放
    m_sprite5 = CCSPXSprite::create("Files/spx_test.sprite", tex, 4);
    m_sprite5->setLoopCount(-1);
    m_sprite5->setPosition(ccp(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height * 3 / 8));
    m_sprite5->setFlipY(true);
    addChild(m_sprite5);
    
    // animation 6
    m_sprite6 = CCSPXSprite::create("Files/spx_test.sprite", tex, 5);
    m_sprite6->setLoopCount(-1);
    m_sprite6->setPosition(ccp(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height * 2 / 8));
    addChild(m_sprite6);
    
    // animation 7
    m_sprite7 = CCSPXSprite::create("Files/spx_test.sprite", tex, 6);
    m_sprite7->setLoopCount(-1);
    m_sprite7->setPosition(ccp(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height / 8));
    addChild(m_sprite7);
    
    scheduleUpdate();
    
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
}

bool AnimSpriteX::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    m_sprite1->setPaused(!m_sprite1->isPaused());
    m_sprite2->setPaused(!m_sprite2->isPaused());
    m_sprite3->setPaused(!m_sprite3->isPaused());
    m_sprite4->setPaused(!m_sprite4->isPaused());
    m_sprite5->setPaused(!m_sprite5->isPaused());
    m_sprite6->setPaused(!m_sprite6->isPaused());
    m_sprite7->setPaused(!m_sprite7->isPaused());
    return false;
}

void AnimSpriteX::update(float dt) {
    m_sprite1->tick(dt);
    m_sprite2->tick(dt);
    m_sprite3->tick(dt);
    m_sprite4->tick(dt);
    m_sprite5->tick(dt);
    m_sprite6->tick(dt);
    m_sprite7->tick(dt);
}

std::string AnimSpriteX::subtitle()
{
    return "SpriteX";
}

//------------------------------------------------------------------
//
// SpriteX 2011
//
//------------------------------------------------------------------
void AnimSpriteX2011::onEnter()
{
    AnimDemo::onEnter();
    
    // surface
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    // texture
    CCTextureCache* tc = CCTextureCache::sharedTextureCache();
    CCTexture2D* tex = tc->addImage("Files/spx3_test.png");
    
    // animation 1
    // SpriteX doesn't support tick mode, but you can use setForceTickMode
    // to enforce using tick mode. this animation use a large unit interval
    // so the animation plays slow
    m_sprite1 = CCSPX3Sprite::create("Files/spx3_test.sprite", 0, tex, NULL);
    m_sprite1->setLoopCount(-1);
    m_sprite1->setPosition(ccp(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height * 7 / 8));
    m_sprite1->setForceTickMode(true);
    m_sprite1->setUnitInterval(0.5f);
    addChild(m_sprite1);
    
    // animation 2
    m_sprite2 = CCSPX3Sprite::create("Files/spx3_test.sprite", 1, tex, NULL);
    m_sprite2->setLoopCount(-1);
    m_sprite2->setPosition(ccp(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height * 6 / 8));
    addChild(m_sprite2);
    
    // animation 3, 反着放
    m_sprite3 = CCSPX3Sprite::create("Files/spx3_test.sprite", 2, tex, NULL);
    m_sprite3->setLoopCount(-1);
    m_sprite3->setPosition(ccp(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height * 5 / 8));
    m_sprite3->setReverse(true);
    addChild(m_sprite3);
    
    // animation 4
    m_sprite4 = CCSPX3Sprite::create("Files/spx3_test.sprite", 3, tex, NULL);
    m_sprite4->setLoopCount(-1);
    m_sprite4->setPosition(ccp(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height * 4 / 8));
    addChild(m_sprite4);
    
    // animation 5, y轴倒转播放
    m_sprite5 = CCSPX3Sprite::create("Files/spx3_test.sprite", 4, tex, NULL);
    m_sprite5->setLoopCount(-1);
    m_sprite5->setPosition(ccp(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height * 3 / 8));
    m_sprite5->setFlipY(true);
    addChild(m_sprite5);
    
    // animation 6
    m_sprite6 = CCSPX3Sprite::create("Files/spx3_test.sprite", 5, tex, NULL);
    m_sprite6->setLoopCount(-1);
    m_sprite6->setPosition(ccp(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height * 2 / 8));
    addChild(m_sprite6);
    
    // animation 7
    m_sprite7 = CCSPX3Sprite::create("Files/spx3_test.sprite", 6, tex, NULL);
    m_sprite7->setLoopCount(-1);
    m_sprite7->setPosition(ccp(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height / 8));
    addChild(m_sprite7);
    
    scheduleUpdate();
    
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
}

bool AnimSpriteX2011::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    m_sprite1->setPaused(!m_sprite1->isPaused());
    m_sprite2->setPaused(!m_sprite2->isPaused());
    m_sprite3->setPaused(!m_sprite3->isPaused());
    m_sprite4->setPaused(!m_sprite4->isPaused());
    m_sprite5->setPaused(!m_sprite5->isPaused());
    m_sprite6->setPaused(!m_sprite6->isPaused());
    m_sprite7->setPaused(!m_sprite7->isPaused());
    return false;
}

void AnimSpriteX2011::update(float dt) {
    m_sprite1->tick(dt);
    m_sprite2->tick(dt);
    m_sprite3->tick(dt);
    m_sprite4->tick(dt);
    m_sprite5->tick(dt);
    m_sprite6->tick(dt);
    m_sprite7->tick(dt);
}

std::string AnimSpriteX2011::subtitle()
{
    return "SpriteX 2011";
}

//------------------------------------------------------------------
//
// Clip Mapping (Aurora GT)
//
//------------------------------------------------------------------
void AnimClipMappingAuroraGT::onEnter()
{
    AnimDemo::onEnter();
    
    // surface
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    // texture
    CCTextureCache* tc = CCTextureCache::sharedTextureCache();
    CCTexture2D* tex = tc->addImage("Files/prince.png");
    
    // animation 1
    m_sprite1 = CCAuroraSprite::create("Files/prince.bsprite", 78, tex, NULL);
    m_sprite1->setLoopCount(-1);
    m_sprite1->setPosition(ccp(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height / 2));
    m_sprite1->setUnitInterval(0.1f);
    m_sprite1->setDebugDrawFrameRect(true);
    m_sprite1->setDebugDrawCollisionRect(true);
    addChild(m_sprite1);
    
    // create clip mapping and add them to sprite
    CCAFCClipMapping* mapping = CCAFCClipMapping::createWithAuroraGT(1, "Files/prince_m00.mmp");
    m_sprite1->addClipMapping(mapping);
    mapping = CCAFCClipMapping::createWithAuroraGT(2, "Files/prince_m01.mmp");
    m_sprite1->addClipMapping(mapping);
    mapping = CCAFCClipMapping::createWithAuroraGT(3, "Files/prince_m02.mmp");
    m_sprite1->addClipMapping(mapping);
    mapping = CCAFCClipMapping::createWithAuroraGT(4, "Files/prince_m03.mmp");
    m_sprite1->addClipMapping(mapping);
    mapping = CCAFCClipMapping::createWithAuroraGT(5, "Files/prince_m04.mmp");
    m_sprite1->addClipMapping(mapping);
    mapping = CCAFCClipMapping::createWithAuroraGT(6, "Files/prince_m05.mmp");
    m_sprite1->addClipMapping(mapping);
    mapping = CCAFCClipMapping::createWithAuroraGT(7, "Files/prince_m06.mmp");
    m_sprite1->addClipMapping(mapping);
    mapping = CCAFCClipMapping::createWithAuroraGT(8, "Files/prince_m07.mmp");
    m_sprite1->addClipMapping(mapping);
    
    CCLabelTTF* label = CCLabelTTF::create("Switch Clip Mapping", "Helvetica", 16);
    CCMenuItemLabel* item = CCMenuItemLabel::create(label,
                                                    this,
                                                    menu_selector(AnimClipMappingAuroraGT::onSwitchClipMapping));
    item->setPosition(ccp(origin.x + visibleSize.width / 2,
                          origin.y + visibleSize.height / 7));
    CCMenu* menu = CCMenu::create(item, NULL);
    menu->setPosition(CCPointZero);
    addChild(menu);
    
    scheduleUpdate();
    
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
}

void AnimClipMappingAuroraGT::onSwitchClipMapping(CCObject* sender) {
    switch(m_nextMapping) {
        case 0:
            m_sprite1->playAnimation(78);
            break;
        default:
            m_sprite1->playAnimation(78, m_nextMapping);
            break;
    }
    
    m_nextMapping++;
    m_nextMapping %= 9;
}

bool AnimClipMappingAuroraGT::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    m_sprite1->setPaused(!m_sprite1->isPaused());
    return false;
}

void AnimClipMappingAuroraGT::update(float dt) {
    m_sprite1->tick(dt);
}

std::string AnimClipMappingAuroraGT::subtitle()
{
    return "Clip Mapping (AuroraGT)";
}