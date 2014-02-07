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
#ifndef __CCSPX3Frame_h__
#define __CCSPX3Frame_h__

#include "cocos2d.h"
#include "CCSPX3TileSet.h"

NS_CC_BEGIN

/**
 * @struct CCSPX3Patch
 *
 * Clip in SpriteX 2011, it calls them patch. There is two types of patch:
 * image or shape.
 */
typedef struct CCSPX3Patch {
	/**
	 * patch type
	 */
	CCSPX3PatchType type;

	union {
		/**
		 * for image type tile
		 */
		struct {
			/**
			 * index of tile set
			 */
			short tileSetIndex;

			/**
			 * index of tile
			 */
			short tileIndex;

			/**
			 * tile x position in frame
			 */
			short x;

			/**
			 * tile y position in frame
			 */
			short y;

			/**
			 * transform of this tile
			 */
			CCSPXTransform transform;
		} tile;

		/**
		 * for shape type tile
		 */
		struct {
			/**
			 * drawing mode, not used for now
			 */
			int mode;

			/**
			 * shape type
			 */
			CCSPX3ShapeType shape;

			/**
			 * transform of this tile
			 */
			CCSPXTransform transform;

			/**
			 * color of shape
			 */
		    int color;

		    /**
		     * vertex count of shape
		     */
		    short vertexCount;

		    /**
		     * count of shape parameters, first is vertices, then followed by
		     * other parameters
		     */
		    short paramCount;

		    /**
		     * buffer of parameters
		     */
		    short* params;
		} shape;
	};
} CCSPX3Patch;

/**
 * @class CCSPX3Frame
 *
 * frame in SpriteX 2011
 */
class CC_DLL CCSPX3Frame : public CCObject {
public:
	/**
	 * flag of frame
	 */
	int m_flags;

	/**
	 * frame rect in animation
	 */
	CCSPX3Tile m_rect;

	/**
	 * count of patch
	 */
	int m_patchCount;

	/**
	 * data of all patch
	 */
	CCSPX3Patch* m_patches;

	/**
	 * count of collision rectangle
	 */
	int m_collisionCount;

	/**
	 * data of all collision rectangles
	 */
	CCSPX3Tile* m_collisionRects;

protected:
	CCSPX3Frame();

public:
    virtual ~CCSPX3Frame();
    static CCSPX3Frame* create();
};

NS_CC_END

#endif // __CCSPX3Frame_h__
