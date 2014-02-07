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
#include "CCAuroraLoader.h"
#include "CCAuroraConstants.h"
#include <stdlib.h>
#include <math.h>
#include "CCMoreMacros.h"
#include "CCUtils.h"

NS_CC_BEGIN

/// data is in parsing
static const char* s_data;

/// position of read
static int s_pos;

static int readByte() {
	return s_data[s_pos++] & 0xFF;
}

static short readShort() {
	short ret = *(short*)(s_data + s_pos);
	ret = letoh16(ret);
	s_pos += sizeof(short);
	return ret;
}

static int readInt() {
	int ret = *(int*)(s_data + s_pos);
	ret = letoh32(ret);
	s_pos += sizeof(int);
	return ret;
}

static int peekByte() {
	return s_data[s_pos];
}

static void skip(size_t len) {
	s_pos += len;
}

CCAuroraFileData* CCAuroraLoader::load(const char* data, size_t length, float resScale) {
	CCAuroraFileData* afd = CCAuroraFileData::create();
	s_data = data;
	s_pos = 0;

	// set scale
	afd->m_resScale = resScale;

	// version
	afd->m_version = readShort();

	// currently we only support version 3
	if(afd->m_version != BS_VERSION_3) {
		CCLOGWARN("CCAuroraLoader::load: currently cocos2dx-better only support version 3 BSprite file");
		return afd;
	}

	// flags
	afd->m_flags = readInt();

	// read all modules
	if((afd->m_flags & BS_MODULES) != 0) {
		// module count
		afd->m_moduleCount = readShort();

		if(afd->m_moduleCount > 0) {
			// allocate memory
			afd->m_modules = (CCAuroraModule*)calloc(afd->m_moduleCount, sizeof(CCAuroraModule));

			for(int i = 0; i < afd->m_moduleCount; i++) {
				CCAuroraModule* module = afd->m_modules + i;

				// check first byte to decide what the module is
				int head = peekByte();
				if(head == 0xFF) {
					module->type = BSM_RECT;
					readByte();
					module->color = readInt();
					module->w = readByte();
					module->h = readByte();
				} else if(head == 0xFE) {
					module->type = BSM_FILL_RECT;
					readByte();
					module->color = readInt();
					module->w = readByte();
					module->h = readByte();
				} else if(head == 0xFD) {
					module->type = BSM_MARKER;
					readByte();
					module->w = readByte();
					module->h = readByte();
				} else {
					module->type = BSM_IMAGE;
					if((afd->m_flags & BS_MODULES_IMG) != 0) 
						module->imageIndex = readByte();
					if((afd->m_flags & BS_MODULES_XY_SHORT) != 0) {
						module->x = readShort();
						module->y = readShort();
					} else if((afd->m_flags & BS_MODULES_XY) != 0) {
						module->x = readByte();
						module->y = readByte();
					}
					if((afd->m_flags & BS_MODULES_WH_SHORT) != 0) {
						module->w = readShort();
						module->h = readShort();
					} else {
						module->w = readByte();
						module->h = readByte();
					}

					// decide image count
					afd->m_imageCount = MAX(afd->m_imageCount, module->imageIndex + 1);
				}
			}
		}
	}

	// read all frames
	if((afd->m_flags & BS_FRAMES) != 0) {
		// count of frame modules
		afd->m_frameModuleCount = readShort();

		// read all frame modules
		if(afd->m_frameModuleCount > 0) {
			afd->m_frameModules = (CCAuroraFrameModule*)calloc(afd->m_frameModuleCount, sizeof(CCAuroraFrameModule));
			for(int i = 0; i < afd->m_frameModuleCount; i++) {
				CCAuroraFrameModule* fm = afd->m_frameModules + i;
				fm->index = readByte();
				if((afd->m_flags & BS_FM_OFF_SHORT) != 0) {
					fm->x = readShort();
					fm->y = readShort();
				} else {
					fm->x = readByte();
					fm->y = readByte();
				}
				if((afd->m_flags & BS_FM_PALETTE) != 0)
					fm->palette = readByte();
				fm->flags = readByte();
			}
		}

		// read all collision rect
		if((afd->m_flags & BS_FRAME_RECTS) != 0) {
			afd->m_collisionRectCount = readShort();
			if(afd->m_collisionRectCount > 0) {
				afd->m_collisionRects = (ccRect*)calloc(afd->m_collisionRectCount, sizeof(ccRect));
				for(int i = 0; i < afd->m_collisionRectCount; i++) {
					ccRect* r = afd->m_collisionRects + i;
					r->x = readByte();
					r->y = readByte();
					r->width = readByte();
					r->height = readByte();
				}
			}
		}

		// read all frames
		afd->m_frameCount = readShort();
		if(afd->m_frameCount > 0) {
			afd->m_frames = (CCAuroraFrame*)calloc(afd->m_frameCount, sizeof(CCAuroraFrame));
			for(int i = 0; i < afd->m_frameCount; i++) {
				CCAuroraFrame* f = afd->m_frames + i;

				// module count
				if((afd->m_flags & BS_NFM_1_BYTE) != 0)
					f->moduleCount = readByte();
				else
					f->moduleCount = readShort();

				// first module index
				if((afd->m_flags & BS_NO_FM_START) == 0)
					f->firstModuleIndex = readShort();

				// collision rect count
				if((afd->m_flags & BS_FRAME_RECTS) != 0)
					f->collisionRectCount = readByte();
			}

			// read frame bounds
			if((afd->m_flags & BS_SKIP_FRAME_RC) == 0) {
				for(int i = 0; i < afd->m_frameCount; i++) {
					CCAuroraFrame* f = afd->m_frames + i;
					f->bound.x = readByte();
					f->bound.y = readByte();
					f->bound.width = readByte();
					f->bound.height = readByte();
				}
			}

			// skip collision rect, the collision rect is actually frame rects and we already load it before
			if((afd->m_flags & BS_FRAME_COLL_RC) != 0) {
				skip(4 * afd->m_frameCount);
			}
		}
	}

	// read all animations
	if((afd->m_flags & BS_ANIMS) != 0) {
		// count of animation frames
		afd->m_animationFrameCount = readShort();

		// read all animation frames
		if(afd->m_animationFrameCount > 0) {
			afd->m_animationFrames = (CCAuroraAnimationFrame*)calloc(afd->m_animationFrameCount, sizeof(CCAuroraAnimationFrame));
			for(int i = 0; i < afd->m_animationFrameCount; i++) {
				CCAuroraAnimationFrame* af = afd->m_animationFrames + i;
				af->index = readByte();
				af->delay = readByte();
				if((afd->m_flags & BS_AF_OFF_SHORT) != 0) {
					af->offsetX = readShort();
					af->offsetY = readShort();
				} else {
					af->offsetX = readByte();
					af->offsetY = readByte();
				}
				af->flags = readByte();
			}
		}

		// count of animations
		afd->m_animationCount = readShort();

		// read all animations
		if(afd->m_animationCount > 0) {
			afd->m_animations = (CCAuroraAnimation*)calloc(afd->m_animationCount, sizeof(CCAuroraAnimation));
			for(int i = 0; i < afd->m_animationCount; i++) {
				CCAuroraAnimation* a = afd->m_animations + i;

				// number of frame
				if((afd->m_flags & BS_NAF_1_BYTE) != 0)
					a->frameCount = readByte();
				else
					a->frameCount = readShort();

				// first animation frame inex
				if((afd->m_flags & BS_NO_AF_START) == 0)
					a->firstFrameIndex = readShort();
			}
		}
	}

	return afd;
}

CCAuroraFileData* CCAuroraLoader::load(const char* asPath) {
	unsigned long len;
	char* data = (char*)CCFileUtils::sharedFileUtils()->getFileData(asPath, "rb", &len);
	CCAuroraFileData* as = load(data, (size_t)len, 1 / CC_CONTENT_SCALE_FACTOR());
	as->m_path = asPath;
	free(data);
	return as;
}

NS_CC_END