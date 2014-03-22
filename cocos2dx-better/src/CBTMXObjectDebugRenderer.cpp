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
#include "CBTMXObjectDebugRenderer.h"
#include "CBTMXTileMap.h"
#include "CBTMXObjectGroup.h"
#include "CBTMXObject.h"

NS_CC_BEGIN

CBTMXObjectDebugRenderer::CBTMXObjectDebugRenderer() :
m_map(NULL) {
}

CBTMXObjectDebugRenderer::~CBTMXObjectDebugRenderer() {
}

CBTMXObjectDebugRenderer* CBTMXObjectDebugRenderer::create(CBTMXTileMap* m) {
	CBTMXObjectDebugRenderer* l = new CBTMXObjectDebugRenderer();
	if(l->initWithMap(m)) {
		return (CBTMXObjectDebugRenderer*)l->autorelease();
	}
	l->release();
	return NULL;
}

bool CBTMXObjectDebugRenderer::initWithMap(CBTMXTileMap* map) {
	m_map = map;
	return true;
}

void CBTMXObjectDebugRenderer::draw() {
	CBTMXMapInfo* mapInfo = m_map->getMapInfo();
	CCObject* obj;
	CCARRAY_FOREACH(&mapInfo->getObjectGroups(), obj) {
		CBTMXObjectGroup* group = (CBTMXObjectGroup*)obj;
		int oc = group->getObjectCount();
		for(int i = 0; i < oc; i++) {
			CBTMXObject* obj = group->getObjectAt(i);
			
			// get object x, y, those x, y is in tmx coordinates
			CCPoint loc = obj->getPosition();
			
			// is object a polygon?
			bool isNormal = obj->getShape() == CBTMXObject::NORMAL;
			bool isClosed = obj->getShape() == CBTMXObject::POLYGON;
			
			// fill vertex buffer
			m_vertices.clear();
			if(isNormal) {
				// manual add point, remember currently it should be in tmx coordinate space
				CCSize s = obj->getSize();
				m_vertices.addPoint(0, 0);
				m_vertices.addPoint(0, s.height);
				m_vertices.addPoint(s.width, s.height);
				m_vertices.addPoint(s.width, 0);
				m_vertices.addPoint(0, 0);
			} else {
				// add points
				m_vertices.addPoints(obj->getPoints());
				
				// if closed, add first point
				if(isClosed) {
					m_vertices.addPoint(obj->getPoints().getPointAt(0));
				}
			}
			
			// convert all points from tmx space to node space
			CCPoint* buffer = m_vertices.getBuffer();
			for(int i = 0; i < m_vertices.getCount(); i++) {
				CCPoint p = m_map->tmxToNodeSpace(ccp(buffer[i].x + loc.x, buffer[i].y + loc.y));
				buffer[i].x = p.x;
				buffer[i].y = p.y;
			}
			
			// draw lines
			ccDrawColor4B(0, 255, 0, 255);
			ccDrawPoly(buffer, m_vertices.getCount(), false);
		}
	}
}

NS_CC_END