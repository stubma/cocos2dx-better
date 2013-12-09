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
#ifndef __CCMWManager_h__
#define __CCMWManager_h__

#include "cocos2d.h"
#include "CCMWFileData.h"
#include "CCAFCAnimation.h"
#include "CCAFCClipMapping.h"

using namespace std;

NS_CC_BEGIN

/**
 * @typedef CCMWClipType
 *
 * type constant of motion welder clip
 */
typedef enum {
	ClipType_ImageNormal = 0x0000,
	ClipType_ImageFlipX = 0x0002,
	ClipType_ImageFlipY = 0x0004,
	ClipType_ImageFlipXY = 0x0006,
	ClipType_Ecllipse = 0x0001,
	ClipType_Ecllipse_Filled = 0x0003,
	ClipType_Line = 0x0005,
	ClipType_Rect = 0x0007,
	ClipType_Rect_Filled = 0x0009,
	ClipType_RoundRect = 0x000b,
	ClipType_RoundRect_Filled = 0x000d,
	ClipType_CollisionRect = 0x000f
} CCMWClipType;

/**
 * @class CCMWManager
 *
 * \par
 * Data pool of motion welder file data. It parses *.anu files and cache them.
 * This class is referenced by \link wyDirector wyDirector\endlink and it will be
 * destroyed by \link wyDirector wyDirector\endlink when game ends.
 *
 * \par
 * One anu file can contain more than one animation and one animation consists of
 * one or more frames. One Frame consists of one or more clips. So motion welder is
 * a three layer structure, that is the architecture we called AFC (Animation-Frame-Clip).
 */
class CC_DLL CCMWManager : public CCObject {
private:
	/**
	 * animation file data list
	 */
	typedef vector<CCMWFileData*> MWFileDataPtrList;
	MWFileDataPtrList m_animationFileDataList;

	/**
	 * scale of current file data
	 */
	float m_resScale;

private:
	/**
	 * Parse a motion welder clip and save info to afc clip
	 *
	 * @param animationFileData \link CCMWFileData CCMWFileData\endlink
	 * @param clipAdditionalDataIndex index of clip
	 * @param type clip type
	 * @param clipPosX x offset of clip
	 * @param clipPosY y offset of clip
	 * @param afcClip output afc clip
	 */
	bool parseClip(CCMWFileData* animationFileData, int clipAdditionalDataIndex, CCMWClipType type, float clipPosX, float clipPosY, CCAFCClip* afcClip);

protected:
	CCMWManager();

	/**
	 * resolve original value
	 *
	 * @param v original value in anu file
	 * @return value scaled
	 */
	float resolve(short v);

public:
	virtual ~CCMWManager();

	/**
	 * Get single instance
	 *
	 * @return shared \link CCMWManager CCMWManager\endlink instance
	 */
	static CCMWManager* getInstance();

	/**
	 * release all animation file data
	 */
	void releaseAllAnimationFileData();

	/**
	 * Load motion welder file data from path
	 *
	 * @param path file path
	 * @return \link CCMWFileData CCMWFileData\endlink
	 */
	CCMWFileData* load(const char* path);

	/**
	 * Get \link CCMWFileData CCMWFileData\endlink from a path. If this file data is
	 * already loaded, then returns it directly. If not, returns NULL.
	 *
	 * @param path anu file path in file system or assets
	 * @return \link CCMWFileData CCMWFileData\endlink pointer or NULL if not found
	 */
	CCMWFileData* getAnimationFileData(const string& path);

	/**
	 * Get \link CCAFCAnimation CCAFCAnimation\endlink data at specified index
	 *
	 * @param fileData \link CCMWFileData CCMWFileData\endlink
	 * @param animIndex animation index
	 * @param mapping clip mapping rules, can be NULL
	 * @return \link CCAFCAnimation CCAFCAnimation\endlink
	 */
	CCAFCAnimation* getAnimationData(CCMWFileData* fileData, int animIndex, CCAFCClipMapping* mapping);
};

NS_CC_END

#endif // __CCMWManager_h__
