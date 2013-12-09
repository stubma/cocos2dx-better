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
#ifndef __CCSPXManager_h__
#define __CCSPXManager_h__

#include "cocos2d.h"
#include "CCSPXFileData.h"
#include "CCAFCAnimation.h"
#include "CCAFCClipMapping.h"

using namespace std;

NS_CC_BEGIN

/**
 * @class CCSPXManager
 *
 * Manager of SpriteX file
 */
class CC_DLL CCSPXManager : public CCObject {
private:
	/**
	 * sprite file data list
	 */
	typedef vector<CCSPXFileData*> SPXFileDataPtrList;
	SPXFileDataPtrList m_spxFileDataList;

	/**
	 * scale of current file data
	 */
	float m_resScale;

private:
	CCSPXFileData* getSPXFileData(const string& spxPath);

	/**
	 * resolve original value
	 *
	 * @param v original value in sprite file
	 * @return value scaled
	 */
	float resolve(short v);

	/**
	 * Resolve a tile info
	 *
	 * @param spxTile tile data of SpriteX
	 * @param spxFrameTile frame tile data of SpriteX
	 * @param afcClip clip of SpriteX
	 * @param index clip index
	 */
	void parseTile(CCSPXTile* spxTile, CCSPXFrameTile* spxFrameTile, CCAFCClip* afcClip, int index);

protected:
	CCSPXManager();

public:
	virtual ~CCSPXManager();
	static CCSPXManager* getInstance();

	/**
	 * Load SpriteX from a path. If already loaded, returns it directly.
	 *
	 * @param spxPath path of SpriteX file
	 * @return \link CCSPXFileData CCSPXFileData\endlink
	 */
	CCSPXFileData* load(const char* spxPath);

	/**
	 * Get \link CCAFCAnimation CCAFCAnimation\endlink data at specified index
	 *
	 * @param spx \link CCSPXFileData CCSPXFileData\endlink
	 * @param animIndex animation index
	 * @param mapping clip mapping rules, can be NULL
	 * @return \link CCAFCAnimation CCAFCAnimation\endlink
	 */
	CCAFCAnimation* getAnimationData(CCSPXFileData* spx, int animIndex, CCAFCClipMapping* mapping);

	/**
	 * release all animation file data
	 */
	void releaseAllAnimationFileData();
};

NS_CC_END

#endif // __CCSPXManager_h__
