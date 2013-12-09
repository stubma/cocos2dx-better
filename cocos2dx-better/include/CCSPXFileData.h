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
#ifndef __CCSPXFileData_h__
#define __CCSPXFileData_h__

#include "CCAFCFileData.h"
#include "CCSPXFrame.h"
#include "CCSPXAction.h"

using namespace std;

NS_CC_BEGIN

class CCSPXLoader;
class CCSPXManager;

/**
 * @typedef CCSPXTile
 *
 * tile size info in SpriteX
 */
typedef struct CCSPXTile {
	/**
	 * tile image x position in atlas
	 */
	short x;

	/**
	 * tile image y position in atlas
	 */
	short y;

	/**
	 * tile image width in atlas
	 */
	short w;

	/**
	 * tile image height in atlas
	 */
	short h;
} CCSPXTile;

/**
 * @class CCSPXFileData
 *
 * A bean for SpriteX file data. In SpriteX, positive direction of x axis is right,
 * positive of y is down and original point is top left corner
 */
class CC_DLL CCSPXFileData : public CCAFCFileData {
	friend class CCSPXLoader;
	friend class CCSPXManager;

private:
	/**
	 * all frame
	 */
	typedef vector<CCSPXFrame*> SPXFramePtrList;
	SPXFramePtrList m_frames;

	/**
	 * all animations
	 */
	typedef vector<CCSPXAction*> SPXActionPtrList;
	SPXActionPtrList m_actions;

public:
	/**
	 * tile count
	 */
	int m_tileCount;

	/**
	 * frame count
	 */
	int m_frameCount;

	/**
	 * animation count
	 */
	int m_actionCount;

	/**
	 * data of all tiles
	 */
	CCSPXTile* m_tiles;

protected:
	CCSPXFileData();

public:
	virtual ~CCSPXFileData();
	static CCSPXFileData* create();

	/**
	 * Get frame at an index
	 *
	 * @param index frame index
	 * @return \link CCSPXFrame CCSPXFrame\endlink, or NULL if index is not valid
	 */
	CCSPXFrame* getFrameAt(int index) { return m_frames.at(index); }

	/**
	 * Get animation at an index
	 *
	 * @param index animation index
	 * @return \link CCSPXAction CCSPXAction\endlink, or NULL if index is not valid
	 */
	CCSPXAction* getActionAt(int index) { return m_actions.at(index); }
};

NS_CC_END

#endif // __CCSPXFileData_h__
