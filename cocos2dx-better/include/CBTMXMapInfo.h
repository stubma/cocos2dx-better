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
#ifndef __CBTMXMapInfo_h__
#define __CBTMXMapInfo_h__

#include "cocos2d.h"
#include "CCMoreMacros.h"

using namespace std;

NS_CC_BEGIN
	
/// orientation of map
typedef enum {
	kCBTMXOrientationOrthogonal = 1,
	kCBTMXOrientationIsometric,
	kCBTMXOrientationHexagonal
} cbTMXOrientation;

/// tile flags, it is not TMX standard, just give you a way to flip tile
typedef enum {
	kCBTMXTileFlagFlipH = 0x80000000,
	kCBTMXTileFlagFlipV = 0x40000000,
	kCBTMXTileFlagFlipDiagonal = 0x20000000,
	kCBTMXTileFlagFlipAll = (kCBTMXTileFlagFlipH | kCBTMXTileFlagFlipV | kCBTMXTileFlagFlipDiagonal),
	kCBTMXTileFlagFlipMask = ~(kCBTMXTileFlagFlipAll)
} cbTileFlags;

/**
 * Info of a map
 *
 * \note
 * This is a re-implementation for TMX map. Cocos2d-x TMX support is defective, so I write my own.
 * To avoid name conflict, I use CB prefix which stands for cocos2dx-better
 */
class CC_DLL CBTMXMapInfo : public CCObject {
protected:
	CBTMXMapInfo();
	
public:
	virtual ~CBTMXMapInfo();
	static CBTMXMapInfo* create();
	
	/**
	 * Get a property value by a key
	 *
	 * @param key key string
	 * @return value string, or empty string if not found
	 */
	string getProperty(const string& key);
	
	/**
	 * Add a property
	 *
	 * @param key key string
	 * @param value value string
	 */
	void addProperty(const string& key, const string& value);
	
	/// find tileset index by a gid
	int getTileSetIndex(int gid);
	
	/// get tile property, return empty string if no property found
	string getTileProperty(int gid, const string& key);
	
	/// add tile property
	void addTileProperty(int gid, const string& key, const string& value);
	
	/// tmx filename
	CC_SYNTHESIZE(string, m_filename, TMXFilename);
	
	/// map orientation
	CC_SYNTHESIZE(cbTMXOrientation, m_orientation, Orientation);
	
	/// map width in tiles
	CC_SYNTHESIZE(int, m_mapWidth, MapWidth);
	
	/// map height in tiles
	CC_SYNTHESIZE(int, m_mapHeight, MapHeight);
	
	/// tiles width in pixels
	CC_SYNTHESIZE(float, m_tileWidth, TileWidth);
	
	/// tiles height in pixels
	CC_SYNTHESIZE(float, m_tileHeight, TileHeight);
	
	/// layer info list
	CC_SYNTHESIZE_PASS_BY_REF_NC(CCArray, m_layers, Layers);
	
	/// tileset info list
	CC_SYNTHESIZE_PASS_BY_REF_NC(CCArray, m_tilesets, TileSets);
	
	/// ObjectGroup list
	CC_SYNTHESIZE_PASS_BY_REF_NC(CCArray, m_objectGroups, ObjectGroups);
	
	/// property dict
	CC_SYNTHESIZE_PASS_BY_REF_NC(CCDictionary, m_properties, Properties);
	
	/// tile property dict, key is gid, value is a dict
	CC_SYNTHESIZE_PASS_BY_REF_NC(CCDictionary, m_tileProperties, TileProperties);
};

NS_CC_END

#endif // __CBTMXMapInfo_h__
