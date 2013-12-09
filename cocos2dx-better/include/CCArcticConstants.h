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
#ifndef __CCArcticConstants_h__
#define __CCArcticConstants_h__

NS_CC_BEGIN

/**
 * @typedef CCArcticFlags
 *
 * flags used in ASprite file
 */
typedef enum {
	/// export encoded images for each module
	AS_IMAGE_MODULES = (1 << 0),

	/// export PNG additional info (CRCs) for each module
	AS_PNG_CRC = (1 << 1),

	// keep original palette (do not optimize colors)
	AS_KEEP_PAL = (1 << 2),

	// move transparency as the first color(s)
	AS_TRANSP_FIRST = (1 << 4),

	// move transparency as the last color(s)
	AS_TRANSP_LAST = (1 << 5),

	// export alpha data for each module
	AS_IMAGE_ALPHA = (1 << 6),

	// export modules (w and h)
	AS_MODULES = (1 << 8),

	// export w, h for each module as short
	AS_MODULES_WH_SHORT = (1 << 9),

	// export image index for each module
	AS_MODULES_IMG = (1 << 10),

	// export x, y for each module
	AS_MODULES_XY = (1 << 11),

	// export x, y for each module as short
	AS_MODULES_XY_SHORT = (1 << 12),

	// export frames
	AS_FRAMES = (1 << 16),

	// export fm offsets as short (default is byte)
	AS_FM_OFF_SHORT = (1 << 17),

	// export fm number as short (default is byte)
	AS_FM_NUM_SHORT = (1 << 18),

	// export frame rect
	AS_FRAME_RC = (1 << 19),

	// export collision rect
	AS_FRAME_COL_RC = (1 << 20),

	// export anims
	AS_ANIMS = (1 << 24),

	// export af offsets as short (default is byte)
	AS_AF_OFF_SHORT = (1 << 25),

	// export af number as short (default is byte)
	AS_AF_NUM_SHORT = (1 << 26)
} CCArcticFlags;

/**
 * @typedef CCArcticFrameModuleFlags
 *
 * Flags used in arctic frame module
 */
typedef enum {
	/**
	 * flipped in x axis
	 */
	AS_FLIP_X = 0x01,

	/**
	 * flipped in y axis
	 */
	AS_FLIP_Y = 0x02,

	/**
	 * rotoated by 90 degree, in clockwise direction
	 */
	AS_ROT_90 = 0x04,

	/**
	 * reversed
	 */
	AS_USER0 = 0x10,

	/**
	 * reversed
	 */
	AS_USER1 = 0x20,

	/**
	 * it is not a module, it is a reference to other frame
	 */
	AS_HYPER_FM = 0x10,

	/**
	 * attach box
	 */
	AS_ATTACK_BOX = 0x40,

	/**
	 * extended index mask
	 */
	AS_INDEX_EX_MASK = 0xC0
} CCArcticFrameModuleFlags;

/**
 * how many bits should extended index shift to right
 */
#define AS_INDEX_EX_SHIFT 2

NS_CC_END

#endif // __CCArcticConstants_h__
