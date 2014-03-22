/****************************************************************************
 Author: Luma (stubma@gmail.com)
 
 https://github.com/stubma/cocos2dx-better
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#include "CBTMXTileMap.h"
#include "CBTMXObjectGroup.h"
#include "CBTMXLayer.h"
#include "CBTMXLayerInfo.h"
#include "CBTMXLoader.h"
#include "CBTMXTileSetInfo.h"
#include "CBTMXObjectDebugRenderer.h"

NS_CC_BEGIN

#define TAG_OBJECT_LAYER 0x7f7f7f7f

CBTMXTileMap::~CBTMXTileMap() {
	CC_SAFE_RELEASE(m_mapInfo);
}

CBTMXTileMap::CBTMXTileMap() :
m_mapWidth(0),
m_mapHeight(0),
m_tileWidth(0),
m_tileHeight(0),
m_debugDrawObjects(false),
m_mapInfo(NULL) {
}

CBTMXTileMap* CBTMXTileMap::createWithXMLFile(const string& file) {
	CBTMXTileMap* tmx = new CBTMXTileMap();
	if(tmx->initWithXMLFile(file)) {
		return (CBTMXTileMap*)tmx->autorelease();
	}
	
	tmx->release();
	return NULL;
}

bool CBTMXTileMap::initWithXMLFile(const string& file) {
	if(!CCNodeRGBA::init())
		return false;
	
	// set anchor
	setAnchorPoint(CCPointZero);
	ignoreAnchorPointForPosition(false);
	
	// map info
	m_mapInfo = CBTMXLoader::create()->load(file);
	CC_SAFE_RETAIN(m_mapInfo);
	
	// save info
	m_tileWidth = m_mapInfo->getTileWidth();
	m_tileHeight = m_mapInfo->getTileHeight();
	m_mapWidth = m_mapInfo->getMapWidth();
	m_mapHeight = m_mapInfo->getMapHeight();
    
    // append texture to tileset and set tileset size
	CCObject* obj;
	CCARRAY_FOREACH(&m_mapInfo->getTileSets(), obj) {
		CBTMXTileSetInfo* tileset = (CBTMXTileSetInfo*)obj;
		CCTexture2D* tex = CCTextureCache::sharedTextureCache()->addImage(tileset->getSourceImagePath().c_str());
		tileset->setTexture(tex);
		tileset->setImageHeight(tex->getContentSize().height);
		tileset->setImageWidth(tex->getContentSize().width);
	}
	
	// create tmx layer
	int idx = 0;
	CCARRAY_FOREACH(&m_mapInfo->getLayers(), obj) {
		CBTMXLayer* layer = CBTMXLayer::create(idx, m_mapInfo);
		if(!layer)
			continue;
		
		// add
		addChild(layer, idx, idx);
		idx++;
		CCLOG("%f", layer->getPositionX());
		
		// set map size
		CCSize size = getContentSize();
		CCSize layerSize = layer->getContentSize();
		size.width = MAX(size.width, layerSize.width);
		size.height = MAX(size.height, layerSize.height);
		setContentSize(size);
	}
	
	return true;
}

CBTMXLayer* CBTMXTileMap::getLayer(const string& name) {
	CCArray* children = getChildren();
	int cc = getChildrenCount();
	for(int i = 0; i < cc; i++) {
		CCNode* child = (CCNode*)children->objectAtIndex(i);
		CBTMXLayer* layer = dynamic_cast<CBTMXLayer*>(child);
		if(layer != NULL) {
			if(layer->getLayerInfo()->getName() == name)
				return layer;
		}
	}
	return NULL;
}

CBTMXLayer* CBTMXTileMap::getLayerAt(int index) {
	CCArray* children = getChildren();
	int cc = getChildrenCount();
	for(int i = 0; i < cc; i++) {
		CCNode* child = (CCNode*)children->objectAtIndex(i);
		CBTMXLayer* layer = dynamic_cast<CBTMXLayer*>(child);
		if(layer != NULL) {
			index--;
			if(index < 0)
				return layer;
		}
	}
	return NULL;
}

CBTMXObjectGroup* CBTMXTileMap::getObjectGroup(const string& name) {
	CCObject* obj;
	CCARRAY_FOREACH(&m_mapInfo->getObjectGroups(), obj) {
		CBTMXObjectGroup* og = (CBTMXObjectGroup*)obj;
		if(og->getName() == name) {
			return og;
		}
	}
	return NULL;
}

string CBTMXTileMap::getProperty(const string& name) {
	return m_mapInfo->getProperty(name);
}

string CBTMXTileMap::getTileProperty(int gid, const string& name) {
	return m_mapInfo->getTileProperty(gid, name);
}

void CBTMXTileMap::setDebugDrawObjects(bool flag) {
	if(m_debugDrawObjects != flag) {
		m_debugDrawObjects = flag;
		
		// add object layer or remove it
		if(m_debugDrawObjects) {
			CBTMXObjectDebugRenderer* r = CBTMXObjectDebugRenderer::create(this);
			addChild(r, TAG_OBJECT_LAYER, MAX_INT);
		} else {
			removeChildByTag(TAG_OBJECT_LAYER);
		}
	}
}

CCPoint CBTMXTileMap::nodeToTMXSpace(CCPoint p) {
	switch(m_mapInfo->getOrientation()) {
		case kCBTMXOrientationOrthogonal:
		case kCBTMXOrientationHexagonal:
			// ortho and hex is simple, just convert origin
			return ccp(p.x, getContentSize().height - p.y);
		case kCBTMXOrientationIsometric:
		{
			// iso map origin is at top vertex of (0, 0) tile, and x&y axis follows edges of that tile
			float x = m_mapHeight * m_tileHeight - p.y - (m_mapWidth * m_tileWidth / 4 - p.x / 2);
			float y = m_mapHeight * m_tileHeight - p.y + (m_mapWidth * m_tileWidth / 4 - p.x / 2);
			return ccp(x, y);
		}
		default:
			return p;
	}
}

CCPoint CBTMXTileMap::tmxToNodeSpace(CCPoint p) {
	switch(m_mapInfo->getOrientation()) {
		case kCBTMXOrientationOrthogonal:
		case kCBTMXOrientationHexagonal:
			// ortho and hex is simple, just convert origin
			return ccp(p.x, getContentSize().height - p.y);
		case kCBTMXOrientationIsometric:
		{
			// iso map origin is at top vertex of (0, 0) tile, and x&y axis follows edges of that tile
			float x = m_mapWidth * m_tileWidth / 2 + p.x - p.y;
			float y = (m_mapHeight * m_tileHeight * 2 - p.x - p.y) / 2;
			return ccp(x, y);
		}
		default:
			return p;
	}
}

NS_CC_END