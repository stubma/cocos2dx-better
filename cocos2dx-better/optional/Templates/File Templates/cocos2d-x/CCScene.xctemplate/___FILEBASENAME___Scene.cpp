//
//  ___FILENAME___
//  ___PROJECTNAME___
//
//  Created on ___DATE___.
//___COPYRIGHT___
//

#include "___FILEBASENAME___Scene.h"

___FILEBASENAME___::___FILEBASENAME___() {
}

___FILEBASENAME___::~___FILEBASENAME___() {
}

CCScene* ___FILEBASENAME___::scene() {
    // 'scene' is an autorelease object
    CCScene* scene = CCScene::create();
    
    // 'layer' is an autorelease object
    ___FILEBASENAME___* layer = ___FILEBASENAME___::create();
	
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

bool ___FILEBASENAME___::init() {
    // super init first
    if (!CCLayer::init()) {
        return false;
    }
    
    return true;
}

void ___FILEBASENAME___::onEnter() {
    CCLayer::onEnter();
}

void ___FILEBASENAME___::onExit() {
	CCLayer::onExit();
}
