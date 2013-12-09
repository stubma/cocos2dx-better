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
#ifndef __CCAuroraConstants_h__
#define __CCAuroraConstants_h__

NS_CC_BEGIN

/**
 * @typedef CCAuroraFlags
 *
 * Flags used in BSprite file
 */
typedef enum {
	// export modules
	BS_MODULES = 1,

	// export x, y for each module
	BS_MODULES_XY = 1 << 1,

	// export image index for each module
	BS_MODULES_IMG = 1 << 2,

	// export w, h for each module as short
	BS_MODULES_WH_SHORT = 1 << 4,

	// export x, y for each module as short
	BS_MODULES_XY_SHORT = 1 << 5,

	// export frames
	BS_FRAMES = 1 << 8,

	// don't export first frame module index
	BS_NO_FM_START = 1 << 9,

	// export fm offsets as shorts
	BS_FM_OFF_SHORT = 1 << 10,

	// export number of frame module as byte
	BS_NFM_1_BYTE = 1 << 11,

	// do not export frame bound rect
	BS_SKIP_FRAME_RC = 1 << 12,

	// export frame collision rect
	BS_FRAME_COLL_RC = 1 << 13,

	// export palette used by the module
	BS_FM_PALETTE = 1 << 14,

	// export frame rects
	BS_FRAME_RECTS = 1 << 15,

	// export anims
	BS_ANIMS = 1 << 16,

	// don't export first animation frame index
	BS_NO_AF_START = 1 << 17,

	// export animation frame offsets as shorts
	BS_AF_OFF_SHORT = 1 << 18,

	// export number of animation frame as byte
	BS_NAF_1_BYTE = 1 << 19,

	// export encoded images for each module
	BS_MODULE_IMAGES = 1 << 24,

	// export PNG additional info (CRCs) for each module
	BS_PNG_CRC = 1 << 25,

	// keep original palette (do not optimize colors)
	BS_KEEP_PAL = 1 << 26,

	// move transparency as the first color(s)
	BS_TRANSP_FIRST = 1 << 27,

	// move transparency as the last color(s)
	BS_TRANSP_LAST = 1 << 28,

	// export sprite image as an PNG with CRCs for each palette
	BS_SINGLE_IMAGE	= 1 << 29,
} CCAuroraFlags;

/**
 * @typedef CCAuroraModuleType
 *
 * type of module in BSprite
 */
typedef enum {
	// unknown type
	BSM_UNKNOWN,

	// image module
	BSM_IMAGE,

	// it is a rect
	BSM_RECT,

	// it is a filled rect
	BSM_FILL_RECT,

	// it is a marker
	BSM_MARKER
} CCAuroraModuleType;

/**
 * @typedef CCAuroraFrameModuleFlags
 *
 * Flags used in arctic frame module
 */
typedef enum {
	/**
	 * flipped in x axis
	 */
	BS_FLIP_X = 0x01,

	/**
	 * flipped in y axis
	 */
	BS_FLIP_Y = 0x02,

	/**
	 * rotoated by 90 degree, in clockwise direction
	 */
	BS_ROT_90 = 0x04,

	/**
	 * reversed
	 */
	BS_USER0 = 0x10,

	/**
	 * reversed
	 */
	BS_USER1 = 0x20,

	/**
	 * it is not a module, it is a reference to other frame
	 */
	BS_HYPER_FM = 0x10,

	/**
	 * attach box
	 */
	BS_ATTACK_BOX = 0x40,

	/**
	 * extended index mask
	 */
	BS_INDEX_EX_MASK = 0xC0
} CCAuroraFrameModuleFlags;

/**
 * how many bits should extended index shift to right
 */
#define BS_INDEX_EX_SHIFT 2

/**
 * Version 3 of BSprite file
 */
#define BS_VERSION_3 0x03DF

NS_CC_END

#endif // __CCAuroraConstants_h__
