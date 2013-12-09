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
#ifndef __CCAuroraManager_h__
#define __CCAuroraManager_h__

#include "cocos2d.h"
#include "CCAuroraFileData.h"
#include "CCAFCAnimation.h"
#include "CCAFCClipMapping.h"

using namespace std;

NS_CC_BEGIN

/**
 * @class CCAuroraManager
 *
 * Manager of BSprite file
 */
class CC_DLL CCAuroraManager : public CCObject {
private:
	/**
	 * sprite file data list
	 */
	typedef vector<CCAuroraFileData*> AuroraFileDataPtrList;
	AuroraFileDataPtrList m_auroraFileDataList;

	/**
	 * scale of current file data
	 */
	float m_resScale;

private:
	CCAuroraFileData* getAuroraFileData(const string& asPath);

	/**
	 * resolve original value
	 *
	 * @param v original value in sprite file
	 * @return value scaled
	 */
	float resolve(short v);
	
	/**
	 * parse all modules in a frame, it may recursively called because frame module 
	 * may be a reference to other frame
	 * 
	 * @param afd \link CCAuroraFileData CCAuroraFileData\endlink
	 * @param auroraFrame \link CCAuroraFrame CCAuroraFrame\endlink
	 * @param afcFrame \link CCAFCFrame CCAFCFrame\endlink
	 * @param offsetX x offset, only used for hyper frame
	 * @param offsetY y offset, only used for hyper frame
	 * @param mapping clip mapping rules, can be NULL
	 */
	void parseFrameModules(CCAuroraFileData* afd, CCAuroraFrame* auroraFrame, CCAFCFrame* afcFrame, int offsetX, int offsetY, CCAFCClipMapping* mapping);

	/**
	 * parse aurora module and save info into afc clip
	 *
	 * @param auroraModule \link CCAuroraModule CCAuroraModule\endlink
	 * @param auroraFrameModule \link CCAuroraFrameModule CCAuroraFrameModule\endlink
	 * @param afcClip \link CCAFCClip CCAFCClip\endlink
	 * @param index clip index
	 * @param offsetX x offset
	 * @param offsetY y offset
	 */
	void parseModule(CCAuroraModule* auroraModule, CCAuroraFrameModule* auroraFrameModule, CCAFCClip* afcClip, int index, int offsetX, int offsetY);

protected:
	CCAuroraManager();

public:
	virtual ~CCAuroraManager();
	static CCAuroraManager* getInstance();

	/**
	 * Load BSprite from a path. If already loaded, returns it directly.
	 *
	 * @param asPath path of BSprite file
	 * @return \link CCAuroraFileData CCAuroraFileData\endlink
	 */
	CCAuroraFileData* load(const char* asPath);

	/**
	 * Get \link CCAFCAnimation CCAFCAnimation\endlink data at specified index
	 *
	 * @param afd \link CCAuroraFileData CCAuroraFileData\endlink
	 * @param animIndex animation index
	 * @param mapping clip mapping rules, can be NULL
	 * @return \link CCAFCAnimation CCAFCAnimation\endlink
	 */
	CCAFCAnimation* getAnimationData(CCAuroraFileData* afd, int animIndex, CCAFCClipMapping* mapping);

	/**
	 * release all animation file data
	 */
	void releaseAllAnimationFileData();
};

NS_CC_END

#endif // __CCAuroraManager_h__
