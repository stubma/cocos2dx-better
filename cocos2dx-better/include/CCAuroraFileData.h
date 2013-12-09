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
#ifndef __CCAuroraFileData_h__
#define __CCAuroraFileData_h__

#include "CCAFCFileData.h"
#include "CCAuroraConstants.h"

NS_CC_BEGIN

class CCAuroraLoader;
class CCAuroraManager;

/**
 * @struct CCAuroraModule
 *
 * module info of aurora sprite
 */
typedef struct CCAuroraModule {
	/**
	 * type of module
	 */
	CCAuroraModuleType type;

	/**
	 * image index, only meaningful for image module
	 */
	int imageIndex;

	/**
	 * color of module, useless for some types
	 */
	int color;

	/**
	 * tile x position in atlas
	 */
	short x;

	/**
	 * tile y position in atlas
	 */
	short y;

	/**
	 * module width
	 */
	short w;

	/**
	 * module height
	 */
	short h;
} CCAuroraModule;

/**
 * @struct CCAuroraFrameModule
 *
 * frame module info
 */
typedef struct CCAuroraFrameModule {
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
	 * palette index
	 */
	short palette;

	/**
	 * module flag, bits 6, 7 are extension for module index
	 */
	unsigned char flags;
} CCAuroraFrameModule;

/**
 * @struct CCAuroraFrame
 *
 * frame info in aurora file
 */
typedef struct CCAuroraFrame {
	/**
	 * module count in frame
	 */
	short moduleCount;

	/**
	 * index of first module. The index indicates data position in \c m_frameModules
	 * of \link CCAuroraFileData CCAuroraFileData\endlink
	 */
	short firstModuleIndex;

	/**
	 * count of collision rect belongs to this frame
	 */
	short collisionRectCount;

	/**
	 * bound rect of frame
	 */
	ccRect bound;
} CCAuroraFrame;

/**
 * @typedef CCAuroraAnimationFrame
 *
 * \if Enlgish
 * info of frame in an animation
 */
typedef struct CCAuroraAnimationFrame {
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
	 * y offset of frame. Y axis of aurora is reversed to opengl, so need negate it
	 * when calculate AFC frame offset
	 */
	short offsetY;

	/**
	 * flags of frame, bits 6, 7 are extension for frame index
	 */
	unsigned char flags;
} CCAuroraAnimationFrame;

/**
 * info of aurora animation
 */
typedef struct CCAuroraAnimation {
	/**
	 * frame count in this animation
	 */
	short frameCount;

	/**
	 * index of first frame in animation frames list. The index indicates data position in
	 * \c m_animationFrames of \link CCAuroraFileData CCAuroraFileData\endlink
	 */
	short firstFrameIndex;
} CCAuroraAnimation;

/**
 * @class CCAuroraFileData
 *
 * File data object for export file of AuroraGT, a gameloft game designer. The exported file is called
 * BSprite, and it uses little endian byte order. In BSprite, positive direction of x axis is right,
 * positive of y is down and original point is top left corner
 */
class CC_DLL CCAuroraFileData : public CCAFCFileData {
	friend class CCAuroraLoader;
	friend class CCAuroraManager;

public:
	/**
	 * version of aurora file format
	 */
	int m_version;

	/**
	 * flags of aurora sprite file
	 */
	int m_flags;

	/**
	 * image atlas count
	 */
	int m_imageCount;

	/**
	 * total number of modules
	 */
	int m_moduleCount;

	/**
	 * data of all modules
	 */
	CCAuroraModule* m_modules;

	/**
	 * frame module count of all frames
	 */
	int m_frameModuleCount;

	/**
	 * data of all frame modules
	 */
	CCAuroraFrameModule* m_frameModules;

	/**
	 * collision rect count
	 */
	int m_collisionRectCount;

	/**
	 * buffer of collision rects
	 */
	ccRect* m_collisionRects;

	/**
	 * frame count
	 */
	int m_frameCount;

	/**
	 * data of all frames
	 */
	CCAuroraFrame* m_frames;

	/**
	 * frame count in all animations
	 */
	int m_animationFrameCount;

	/**
	 * data of all animation frames
	 */
	CCAuroraAnimationFrame* m_animationFrames;

	/**
	 * count of animations
	 */
	int m_animationCount;

	/**
	 * data of all animations
	 */
	CCAuroraAnimation* m_animations;

protected:
	CCAuroraFileData();

public:
	virtual ~CCAuroraFileData();
	static CCAuroraFileData* create();
};

NS_CC_END

#endif // __CCAuroraFileData_h__
