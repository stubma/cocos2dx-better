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
#ifndef __CCArcticFileData_h__
#define __CCArcticFileData_h__

#include "CCAFCFileData.h"

NS_CC_BEGIN

class CCArcticLoader;
class CCArcticManager;

/**
 * @struct CCArcticModule
 *
 * module info of arctic sprite
 */
typedef struct CCArcticModule {
	/**
	 * image index
	 */
	int imageIndex;

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
} CCArcticModule;

/**
 * @struct CCArcticFrameModule
 *
 * frame module info
 */
typedef struct CCArcticFrameModule {
	/**
	 * index of module
	 */
	short index;

	/**
	 * module x position in frame
	 */
	short x;

	/**
	 * module y position in frame
	 */
	short y;

	/**
	 * module flag, bits 6, 7 are extension for module index
	 */
	unsigned char flags;
} CCArcticFrameModule;

/**
 * @struct CCArcticFrame
 *
 * frame info in arctic file
 */
typedef struct CCArcticFrame {
	/**
	 * module count in frame
	 */
	short moduleCount;

	/**
	 * index of first module. The index indicates data position in \c m_frameModules
	 * of \link CCArcticFileData CCArcticFileData\endlink
	 */
	short firstModuleIndex;
	
	/**
	 * count of collision rect belongs to this frame
	 */
	short collisionRectCount;
	
	/**
	 * buffer of collision rects, the (x, y) is top left corner
	 */
	ccRect* collisionRects;
} CCArcticFrame;

/**
 * @typedef CCArcticAnimationFrame
 *
 * \if Enlgish
 * info of frame in an animation
 */
typedef struct CCArcticAnimationFrame {
	/**
	 * index of frame
	 */
	short index;

	/**
	 * delay time of this frame, it is not a normal time value and it is
	 * very similar with motion welder. So actually we need set an unit
	 * interval to evaluate final time
	 */
	short delay;

	/**
	 * x offset of frame
	 */
	short offsetX;

	/**
	 * y offset of frame. Y axis of arctic is reversed to opengl, so need negate it
	 * when calculate AFC frame offset
	 */
	short offsetY;

	/**
	 * flags of frame, bits 6, 7 are extension for frame index
	 */
	unsigned char flags;
} CCArcticAnimationFrame;

/**
 * info of arctic animation
 */
typedef struct CCArcticAnimation {
	/**
	 * frame count in this animation
	 */
	short frameCount;

	/**
	 * index of first frame in animation frames list. The index indicates data position in
	 * \c m_animationFrames of \link CCArcticFileData CCArcticFileData\endlink
	 */
	short firstFrameIndex;
} CCArcticAnimation;

/**
 * @class CCArcticFileData
 *
 * File data object for Arctic editor. It uses little endian byte order. In ASprite,
 * positive direction of x axis is right, positive of y is down and original point is
 * top left corner
 */
class CC_DLL CCArcticFileData : public CCAFCFileData {
	friend class CCArcticLoader;
	friend class CCArcticManager;

public:
	/**
	 * version of arctic file format
	 */
	int m_version;

	/**
	 * flags of arctic sprite file
	 */
	int m_flags;

	/**
	 * total number of modules
	 */
	int m_moduleCount;

	/**
	 * data of all modules
	 */
	CCArcticModule* m_modules;

	/**
	 * frame module count of all frames
	 */
	int m_frameModuleCount;

	/**
	 * data of all frame modules
	 */
	CCArcticFrameModule* m_frameModules;

	/**
	 * frame count
	 */
	int m_frameCount;

	/**
	 * data of all frames
	 */
	CCArcticFrame* m_frames;

	/**
	 * frame count in all animations
	 */
	int m_animationFrameCount;

	/**
	 * data of all animation frames
	 */
	CCArcticAnimationFrame* m_animationFrames;

	/**
	 * count of animations
	 */
	int m_animationCount;

	/**
	 * data of all animations
	 */
	CCArcticAnimation* m_animations;

protected:
	CCArcticFileData();

public:
	virtual ~CCArcticFileData();
	static CCArcticFileData* create();
};

NS_CC_END

#endif // __CCArcticFileData_h__
