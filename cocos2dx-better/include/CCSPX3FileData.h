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
#ifndef __CCSPX3FileData_h__
#define __CCSPX3FileData_h__

#include "CCAFCFileData.h"
#include "CCSPX3TileSet.h"
#include "CCSPX3Frame.h"
#include "CCSPX3Action.h"

using namespace std;

NS_CC_BEGIN

class CCSPX3Loader;
class CCSPX3Manager;

/**
 * @class CCSPX3FileData
 *
 * A bean for SpriteX 2011 exported file. In SpriteX 2011, every atlas is represented by
 * a tile set, and tile info is resolved by tile set index and tile index. But anyway,
 * SpriteX 2011 is still an AFC architecture editor.
 */
class CC_DLL CCSPX3FileData : public CCAFCFileData {
	friend class CCSPX3Loader;
	friend class CCSPX3Manager;

private:
	/**
	 * array of tile sets
	 */
	typedef vector<CCSPX3TileSet*> SPX3TileSetPtrList;
	SPX3TileSetPtrList m_tileSets;

	/**
	 * array of frames
	 */
	typedef vector<CCSPX3Frame*> SPX3FramePtrList;
	SPX3FramePtrList m_frames;

	/**
	 * array of actions
	 */
	typedef vector<CCSPX3Action*> SPX3ActionPtrList;
	SPX3ActionPtrList m_actions;

public:
	/**
	 * version of file format
	 */
	int m_version;

	/**
	 * count of tile set, every tile set responses to an atlas
	 */
	int m_tileSetCount;

	/**
	 * frame count
	 */
	int m_frameCount;

	/**
	 * animation count
	 */
	int m_actionCount;

protected:
	CCSPX3FileData();

public:
    virtual ~CCSPX3FileData();
    static CCSPX3FileData* create();

    /**
	 * Get tile set at given index
	 *
	 * @param index tile set index
	 * @return \link CCSPX3TileSet CCSPX3TileSet\endlink
	 */
	CCSPX3TileSet* getTileSetAt(int index) { return m_tileSets.at(index); }

	/**
	 * Get frame at given index
	 *
	 * @param index frame index
	 * @return \link CCSPX3Frame CCSPX3Frame\endlink
	 */
	CCSPX3Frame* getFrameAt(int index) { return m_frames.at(index); }

	/**
	 * Get animation at an index
	 *
	 * @param index animation index
	 * @return \link CCSPX3Action CCSPX3Action\endlink, or NULL if index is not valid
	 */
	CCSPX3Action* getActionAt(int index) { return m_actions.at(index); }
};

NS_CC_END

#endif // __CCSPX3FileData_h__
