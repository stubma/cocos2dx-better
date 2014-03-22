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
#include "CBTMXMapInfo.h"
#include "CBTMXTileSetInfo.h"

NS_CC_BEGIN

CBTMXMapInfo::CBTMXMapInfo() {
}

CBTMXMapInfo::~CBTMXMapInfo() {
}

CBTMXMapInfo* CBTMXMapInfo::create() {
	CBTMXMapInfo* m = new CBTMXMapInfo();
	return (CBTMXMapInfo*)m->autorelease();
}

string CBTMXMapInfo::getProperty(const string& key) {
	CCString* p = (CCString*)m_properties.objectForKey(key);
	if(p)
		return p->getCString();
	else
		return "";
}

void CBTMXMapInfo::addProperty(const string& key, const string& value) {
	if(!m_properties.objectForKey(key)) {
		m_properties.setObject(CCString::create(value), key);
	}
}

int CBTMXMapInfo::getTileSetIndex(int gid) {
	int pureGid = gid & kCBTMXTileFlagFlipMask;
	int count = m_tilesets.count();
	for(int i = 0; i < count; i++) {
		CBTMXTileSetInfo* tileset = (CBTMXTileSetInfo*)m_tilesets.objectAtIndex(i);
		if(tileset->getFirstGid() > pureGid)
			return i - 1;
	}
	return m_tilesets.count() - 1;
}

string CBTMXMapInfo::getTileProperty(int gid, const string& key) {
	int pureGid = gid & kCBTMXTileFlagFlipMask;
	char buf[32];
	sprintf(buf, "%d", pureGid);
	CCDictionary* props = (CCDictionary*)m_tileProperties.objectForKey(buf);
	if(props) {
		CCString* p = (CCString*)props->objectForKey(key);
		if(p)
			return p->getCString();
		else
			return "";
	} else {
		return "";
	}
}

void CBTMXMapInfo::addTileProperty(int gid, const string& key, const string& value) {
	int pureGid = gid & kCBTMXTileFlagFlipMask;
	char buf[32];
	sprintf(buf, "%d", pureGid);
	CCDictionary* props = (CCDictionary*)m_tileProperties.objectForKey(buf);
	if(!props) {
		props = CCDictionary::create();
		m_tileProperties.setObject(props, buf);
	}
	props->setObject(CCString::create(value), key);
}
	
NS_CC_END
