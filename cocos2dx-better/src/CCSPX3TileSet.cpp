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
#include "CCSPX3TileSet.h"

NS_CC_BEGIN

CCSPX3TileSet::CCSPX3TileSet() :
		m_mode(SPX_TILE_CLIP),
		m_imageIndex(-1),
		m_tileWidth(0),
		m_tileHeight(0),
		m_tileColumns(0),
		m_tileRows(0),
		m_tileCount(0),
		m_tiles(NULL) {
}

CCSPX3TileSet::~CCSPX3TileSet() {
	if(m_tiles)
		free(m_tiles);
}

CCSPX3TileSet* CCSPX3TileSet::create() {
	CCSPX3TileSet* ts = new CCSPX3TileSet();
	return (CCSPX3TileSet*)ts->autorelease();
}

NS_CC_END