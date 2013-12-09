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
#ifndef __CCSPX3Manager_h__
#define __CCSPX3Manager_h__

#include "cocos2d.h"
#include "CCSPX3FileData.h"
#include "CCAFCAnimation.h"
#include "CCAFCClipMapping.h"

using namespace std;

NS_CC_BEGIN

/**
 * @class CCSPX3Manager
 *
 * Manager of SpriteX 2011 file
 */
class CC_DLL CCSPX3Manager : public CCObject {
private:
	/**
	 * sprite file data list
	 */
	typedef vector<CCSPX3FileData*> SPX3FileDataPtrList;
	SPX3FileDataPtrList m_spxFileDataList;

	/**
	 * scale of current file data
	 */
	float m_resScale;

private:
	CCSPX3FileData* getSPX3FileData(const string& spxPath);

	/**
	 * resolve original value
	 *
	 * @param v original value in sprite file
	 * @return value scaled
	 */
	float resolve(short v);

protected:
	CCSPX3Manager();

public:
	virtual ~CCSPX3Manager();
	static CCSPX3Manager* getInstance();

	/**
	 * Load SpriteX 2011 from a path. If already loaded, returns it directly.
	 *
	 * @param spxPath path of SpriteX 2011 file
	 * @return \link CCSPX3FileData CCSPX3FileData\endlink
	 */
	CCSPX3FileData* load(const char* spxPath);

	/**
	 * Get \link CCAFCAnimation CCAFCAnimation\endlink data at specified index
	 *
	 * @param spx \link CCSPX3FileData CCSPX3FileData\endlink
	 * @param animIndex animation index
	 * @param mapping clip mapping rules, can be NULL
	 * @return \link CCAFCAnimation CCAFCAnimation\endlink
	 */
	CCAFCAnimation* getAnimationData(CCSPX3FileData* spx, int animIndex, CCAFCClipMapping* mapping);

	/**
	 * release all animation file data
	 */
	void releaseAllAnimationFileData();
};

NS_CC_END

#endif // __CCSPX3Manager_h__
