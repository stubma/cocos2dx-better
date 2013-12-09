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
#ifndef __CCSPX3Constants_h__
#define __CCSPX3Constants_h__

#include "CCSPXConstants.h"

NS_CC_BEGIN

/**
 * @typedef CCSPX3ActionMode
 *
 * Mode of action
 */
typedef enum {
	/**
	 * animation uses time to define frame duration
	 */
	SPX_DELAY = 1,

	/**
	 * all frames in animation are displayed for one frame time
	 */
	SPX_NODELAY
} CCSPX3ActionMode;

/**
 * mode of tile sets
 */
typedef enum {
	/**
	 * every tile has same size, so it only saves tile size and rows/columns in atlas.
	 * That means the file size can be smaller
	 */
	SPX_TILE_INDEX = 1,

	/**
	 * every tile is defined by position and size
	 */
	SPX_TILE_CLIP
} CCSPX3TileSetMode;

/**
 * @typedef CCSPX3PatchType
 *
 * Type of patch, patch here is just equal with tile
 *
 * \note
 * Collision rectangle is not a tile in SpriteX 2011 and SpriteX 2011 processes
 * collision rectangle alone
 */
typedef enum {
	/**
	 * patch is a tile, i.e., at image in atlas
	 */
	SPX_PATCH_TILE = 1,

	/**
	 * patch is a shape
	 */
	SPX_PATCH_SHAPE
} CCSPX3PatchType;

/**
 * @typedef CCSPX3ShapeType
 *
 * type of shape
 */
typedef enum {
	SPX_POINT = 0x01,
	SPX_LINE = 0x02,
	SPX_RECT = 0x03,
	SPX_TRIANGLE = 0x04,
	SPX_POLYGON = 0x05,
	SPX_ELLIPSE = 0x06,
	SPX_ROUNDRECT = 0x08,
	SPX_ARC = 0x09,
	SPX_PIE = 0x0A
} CCSPX3ShapeType;

NS_CC_END

#endif // __CCSPX3Constants_h__
