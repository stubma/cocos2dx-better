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
#ifndef __CBTMXTileSetInfo_h__
#define __CBTMXTileSetInfo_h__

#include "cocos2d.h"
#include "CCMoreMacros.h"

using namespace std;

NS_CC_BEGIN

/**
 * Info of a tile set
 *
 * \note
 * This is a re-implementation for TMX map. Cocos2d-x TMX support is defective, so I write my own.
 * To avoid name conflict, I use CB prefix which stands for cocos2dx-better
 */
class CC_DLL CBTMXTileSetInfo : public CCObject {
protected:
	CBTMXTileSetInfo();
	
public:
	virtual ~CBTMXTileSetInfo();
	static CBTMXTileSetInfo* create();
	
	/**
	 * get tile image rect in atlas, the origin is top-left
	 *
	 * @param gid grid id, you must subtract firstGid to get correct gid
	 * @return the tile image rect in atlas and the origin is top-left
	 */
	CCRect getRect(int gid);
	
	/// tileset name
	CC_SYNTHESIZE(string, m_name, Name);
	
	/// the start gid of this tileset, it increases by the column order and starts from zero
	CC_SYNTHESIZE(int, m_firstGid, FirstGid);
	
	/// tile width in pixels
	CC_SYNTHESIZE(float, m_tileWidth, TileWidth);
	
	/// tile height in pixels
	CC_SYNTHESIZE(float, m_tileHeight, TileHeight);
	
	/// spacing between tiles in tileset atlas image, in pixels
	CC_SYNTHESIZE(float, m_spacing, Spacing);
	
	/// margin of tileset atlas image, in pixels
	CC_SYNTHESIZE(float, m_margin, Margin);
	
	/// tileset source image path, can be relative or absolute, depends on tmx file path
	CC_SYNTHESIZE(string, m_sourceImagePath, SourceImagePath);
	
	/// atlas image pixel width
	CC_SYNTHESIZE(float, m_imageWidth, ImageWidth);
	
	/// atlas image pixel height
	CC_SYNTHESIZE(float, m_imageHeight, ImageHeight);
	
	/// texture
	CC_SYNTHESIZE(CCTexture2D*, m_texture, Texture);
};

NS_CC_END

#endif // __CBTMXTileSetInfo_h__
