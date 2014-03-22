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
#include "CBTMXTileSetInfo.h"
#include "CBTMXMapInfo.h"

NS_CC_BEGIN

CBTMXTileSetInfo::CBTMXTileSetInfo() :
m_texture(NULL),
m_imageHeight(0),
m_imageWidth(0),
m_spacing(0),
m_margin(0),
m_tileHeight(0),
m_tileWidth(0),
m_firstGid(0) {
}

CBTMXTileSetInfo::~CBTMXTileSetInfo() {
}

CBTMXTileSetInfo* CBTMXTileSetInfo::create() {
	CBTMXTileSetInfo* t = new CBTMXTileSetInfo();
	return (CBTMXTileSetInfo*)t->autorelease();
}

CCRect CBTMXTileSetInfo::getRect(int gid) {
	CCRect rect = CCRectZero;
	rect.size.width = m_tileWidth;
	rect.size.height = m_tileHeight;
	
	gid &= kCBTMXTileFlagFlipMask;
	gid -= m_firstGid;
	int maxX = (int)((m_imageWidth - m_margin * 2 + m_spacing) / (m_tileWidth + m_spacing));
	rect.origin.x = (gid % maxX) * (m_tileWidth + m_spacing) + m_margin;
	rect.origin.y = (gid / maxX) * (m_tileHeight + m_spacing) + m_margin;
	
	return rect;
}

NS_CC_END
