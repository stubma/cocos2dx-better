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
#ifndef __CCSPXFrame_h__
#define __CCSPXFrame_h__

#include "cocos2d.h"
#include "CCSPXConstants.h"

NS_CC_BEGIN

class CCSPXLoader;

/**
 * @typedef CCSPXFrameTile
 *
 * tile info in a frame
 */
typedef struct CCSPXFrameTile {
	/**
	 * index of tile
	 */
	int index;

	/**
	 * tile x position in frame
	 */
	int x;

	/**
	 * tile y position in frame
	 */
	int y;

	/**
	 * transform of this tile
	 */
	CCSPXTransform transform;
} CCSPXFrameTile;

/**
 * @typedef CCSPXCollision
 *
 * collision region
 */
typedef struct CCSPXCollision {
	/**
	 * x position of collision region
	 */
	int x;

	/**
	 * y position of collision region
	 */
	int y;

	/**
	 * width of collision region
	 */
	int width;

	/**
	 * height of collision region
	 */
	int height;
} CCSPXCollision;

/**
 * @typedef CCSPXReferencePoint
 *
 * reference point
 */
typedef struct CCSPXReferencePoint {
	/**
	 * x location of reference point
	 */
	int x;

	/**
	 * y location of reference point
	 */
	int y;
} CCSPXReferencePoint;

/**
 * frame of SpriteX animation
 */
class CC_DLL CCSPXFrame : public CCObject {
	friend class CCSPXLoader;

public:
	/**
	 * width of frame
	 */
	int m_width;

	/**
	 * height of frame
	 */
	int m_height;

	/**
	 * tile count in frame
	 */
	int m_frameTileCount;

	/**
	 * collision rect count in frame
	 */
	int m_collisionCount;

	/**
	 * reference point count in frame
	 */
	int m_referencePointCount;

	/**
	 * tile count in frame
	 */
	CCSPXFrameTile* m_tiles;

	/**
	 * collision rect info in frame
	 */
	CCSPXCollision* m_collisions;

	/**
	 * reference point in frame
	 */
	CCSPXReferencePoint* m_refPoints;

private:
	CCSPXFrame();

public:
	virtual ~CCSPXFrame();
};

NS_CC_END

#endif // __CCSPXFrame_h__
