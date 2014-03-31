//
//  ___FILENAME___
//  ___PROJECTNAME___
//
//  Created on ___DATE___.
//___COPYRIGHT___
//

#include "___FILEBASENAME___.h"

___FILEBASENAME___::___FILEBASENAME___() {
	
}

___FILEBASENAME___::~___FILEBASENAME___() {
	
}

___FILEBASENAME___* ___FILEBASENAME___::create() {
	___FILEBASENAME___* l = new ___FILEBASENAME___();
	l->init();
	return (___FILEBASENAME___*)l->autorelease();
}

void ___FILEBASENAME___::onExit() {
    CCLayer::onExit();
}

void ___FILEBASENAME___::onEnter() {
    CCLayer::onEnter();
}

bool ___FILEBASENAME___::init() {
	if(!CCLayer::init())
		return false;
    
    // surface
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	return true;
}