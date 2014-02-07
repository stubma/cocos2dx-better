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
#include "CCArcticLoader.h"
#include "CCArcticConstants.h"
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

static void skip(size_t len) {
	s_pos += len;
}

CCArcticFileData* CCArcticLoader::load(const char* data, size_t length, float resScale) {
	CCArcticFileData* afd = CCArcticFileData::create();
	s_data = data;
	s_pos = 0;

	// set scale
	afd->m_resScale = resScale;

	// version
	afd->m_version = readShort();

	// skip id for other editor
	if(afd->m_version > 0x30)
		skip(4);

	// flags
	afd->m_flags = readInt();

	// read all modules
	if((afd->m_flags & AS_MODULES) != 0) {
		// module count
		afd->m_moduleCount = readShort();

		if(afd->m_moduleCount > 0) {
			// allocate memory
			afd->m_modules = (CCArcticModule*)calloc(afd->m_moduleCount, sizeof(CCArcticModule));

			for(int i = 0; i < afd->m_moduleCount; i++) {
				CCArcticModule* module = afd->m_modules + i;

				// image index
				if((afd->m_flags & AS_MODULES_IMG) != 0) {
					module->imageIndex = readByte();
				}

				// location
				if((afd->m_flags & AS_MODULES_XY) != 0) {
					if((afd->m_flags & AS_MODULES_XY_SHORT) != 0) {
						module->x = readShort();
						module->y = readShort();
					} else {
						module->x = readByte();
						module->y = readByte();
					}
				}

				// size
				if((afd->m_flags & AS_MODULES_WH_SHORT) != 0) {
					module->w = readShort();
					module->h = readShort();
				} else {
					module->w = readByte();
					module->h = readByte();
				}
			}
		}
	}

	// read all frames
	if((afd->m_flags & AS_FRAMES) != 0) {
		// count of frame modules
		afd->m_frameModuleCount = readShort();

		// read all frame modules
		if(afd->m_frameModuleCount > 0) {
			afd->m_frameModules = (CCArcticFrameModule*)calloc(afd->m_frameModuleCount, sizeof(CCArcticFrameModule));
			for(int i = 0; i < afd->m_frameModuleCount; i++) {
				CCArcticFrameModule* fm = afd->m_frameModules + i;
				fm->index = readByte();
				if((afd->m_flags & AS_FM_OFF_SHORT) != 0) {
					fm->x = readShort();
					fm->y = readShort();
				} else {
					fm->x = readByte();
					fm->y = readByte();
				}
				fm->flags = readByte();
			}
		}

		// count of frames
		afd->m_frameCount = readShort();

		// read all frames
		if(afd->m_frameCount > 0) {
			afd->m_frames = (CCArcticFrame*)calloc(afd->m_frameCount, sizeof(CCArcticFrame));
			for(int i = 0; i < afd->m_frameCount; i++) {
				CCArcticFrame* f = afd->m_frames + i;
				f->moduleCount = readByte();
				f->firstModuleIndex = readShort();
			}
		}
		
		// read collision rect of this frame
		if((afd->m_flags & AS_FRAME_COL_RC) != 0) {
			for(int i = 0; i < afd->m_frameCount; i++) {
				CCArcticFrame* f = afd->m_frames + i;
				f->collisionRectCount = readByte();
				if(f->collisionRectCount > 0) {
					f->collisionRects = (ccRect*)calloc(f->collisionRectCount, sizeof(ccRect));
					ccRect* r = f->collisionRects;
					for(int j = 0; j < f->collisionRectCount; j++, r++) {
						r->x = readShort();
						r->y = readShort();
						r->width = readShort();
						r->height = readShort();
					}
				}
			}
		}
	}

	// read all animations
	if((afd->m_flags & AS_ANIMS) != 0) {
		// count of animation frames
		afd->m_animationFrameCount = readShort();

		// read all animation frames
		if(afd->m_animationFrameCount > 0) {
			afd->m_animationFrames = (CCArcticAnimationFrame*)calloc(afd->m_animationFrameCount, sizeof(CCArcticAnimationFrame));
			for(int i = 0; i < afd->m_animationFrameCount; i++) {
				CCArcticAnimationFrame* af = afd->m_animationFrames + i;
				af->index = readByte();
				af->delay = readByte();
				if((afd->m_flags & AS_AF_OFF_SHORT) != 0) {
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
			afd->m_animations = (CCArcticAnimation*)calloc(afd->m_animationCount, sizeof(CCArcticAnimation));
			for(int i = 0; i < afd->m_animationCount; i++) {
				CCArcticAnimation* a = afd->m_animations + i;
				
				// number of animation frame
				if((afd->m_flags & AS_AF_NUM_SHORT) != 0) 
					a->frameCount = readShort();
				else
					a->frameCount = readByte();
				
				// first animation index
				a->firstFrameIndex = readShort();
			}
		}
	}

	return afd;
}

CCArcticFileData* CCArcticLoader::load(const char* asPath) {
	unsigned long len;
	char* data = (char*)CCFileUtils::sharedFileUtils()->getFileData(asPath, "rb", &len);
	CCArcticFileData* as = load(data, (size_t)len, 1 / CC_CONTENT_SCALE_FACTOR());
	as->m_path = asPath;
	free(data);
	return as;
}

NS_CC_END