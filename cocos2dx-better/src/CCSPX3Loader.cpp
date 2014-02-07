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
#include "CCSPX3Loader.h"
#include <stdlib.h>
#include "CCSPX3Frame.h"
#include "CCSPX3Action.h"
#include <math.h>
#include "CCMoreMacros.h"
#include "CCUtils.h"
 
NS_CC_BEGIN

#define SPX_MIN_VERSION 11
#define SPX_MAX_VERSION 12

// data in pasring
static const char* s_data;

// read position in s_data
static int s_pos;

static short readShort() {
	short ret = *(short*)(s_data + s_pos);
	ret = betoh16(ret);
	s_pos += sizeof(short);
	return ret;
}

static int readInt() {
	int ret = *(int*)(s_data + s_pos);
	ret = betoh32(ret);
	s_pos += sizeof(int);
	return ret;
}

static int readByte() {
	return s_data[s_pos++] & 0xFF;
}

static void skip(size_t len) {
	s_pos += len;
}

CCSPX3FileData* CCSPX3Loader::load(const char* data, size_t length, float resScale) {
	// verify sig
	s_data = data;
	if(s_data[0] != 'S' || s_data[1] != 'P' || s_data[2] != 'X' || s_data[3] != '3') {
		CCLOGERROR("CCSPX3Loader::load: Invalid SpriteX 2011 file header");
		return NULL;
	}
	s_pos = 4;

	// verify version
	int version = readByte();
	if(version < SPX_MIN_VERSION || version > SPX_MAX_VERSION) {
		CCLOGERROR("CCSPX3Loader::load: unsupported SpriteX 2011 file version: %d", s_data[s_pos]);
		return NULL;
	}

	// create data object
	CCSPX3FileData* spx = CCSPX3FileData::create();

	// save version
	spx->m_version = version;

	// save scale
	spx->m_resScale = resScale;

	// image id list
	int imageIdCount = readShort();
	short* imageIds = (short*)malloc(imageIdCount * sizeof(short));
	for(int i = 0; i < imageIdCount; i++)
		imageIds[i] = readShort();

	// skip image size table
	if(version >= 12) {
		skip(2 * imageIdCount * sizeof(short));
	}

	// skip action id table
	int actionIdCount = readShort();
	skip(actionIdCount * sizeof(short));

	// read all tile set
	spx->m_tileSetCount = readShort();
	for(int i = 0; i < spx->m_tileSetCount; i++) {
		// create tileset
		CCSPX3TileSet* ts = CCSPX3TileSet::create();

		// mode
		ts->m_mode = (CCSPX3TileSetMode)readByte();

		// from image id, get image index
		short imageId = readShort();
		for(int j = 0; j < imageIdCount; j++) {
			if(imageId == imageIds[j]) {
				ts->m_imageIndex = j;
				break;
			}
		}

		// verify image index
		if(ts->m_imageIndex == -1)
			continue;

		// process by mode
		if(ts->m_mode == SPX_TILE_INDEX) {
			ts->m_tileWidth = readShort();
			ts->m_tileHeight = readShort();
			ts->m_tileColumns = readShort();
			ts->m_tileRows = readShort();
		} else if(ts->m_mode == SPX_TILE_CLIP) {
			ts->m_tileCount = readShort();
			ts->m_tiles = (CCSPX3Tile*)calloc(ts->m_tileCount, sizeof(CCSPX3Tile));
			CCSPX3Tile* tile = ts->m_tiles;
			for(int j = 0; j < ts->m_tileCount; j++, tile++) {
				tile->x1 = readShort();
				tile->y1 = readShort();
				tile->x2 = readShort();
				tile->y2 = readShort();
			}
		} else {
			continue;
		}

		// add tile set
		spx->m_tileSets.push_back(ts);
		ts->retain();
	}

	// free
	free(imageIds);

	// read all frames
	spx->m_frameCount = readShort();
	for(int i = 0; i < spx->m_frameCount; i++) {
		// create frame
		CCSPX3Frame* f = CCSPX3Frame::create();

		// flags
		f->m_flags = readInt();

		// rect
		f->m_rect.x1 = readShort();
		f->m_rect.y1 = readShort();
		f->m_rect.x2 = readShort();
		f->m_rect.y2 = readShort();

		// patch count
		f->m_patchCount = readShort();

		// read all patches
		f->m_patches = (CCSPX3Patch*)calloc(f->m_patchCount, sizeof(CCSPX3Patch));
		CCSPX3Patch* patch = f->m_patches;
		for(int j = 0; j < f->m_patchCount; j++, patch++) {
			patch->type = (CCSPX3PatchType)readByte();
			if(patch->type == SPX_PATCH_TILE) {
				patch->tile.tileSetIndex = readShort();
				patch->tile.tileIndex = readShort();
				patch->tile.transform = (CCSPXTransform)readByte();
				patch->tile.x = readShort();
				patch->tile.y = readShort();
			} else if(patch->type == SPX_PATCH_SHAPE) {
				patch->shape.mode = readByte();
				patch->shape.shape = (CCSPX3ShapeType)readByte();
				patch->shape.transform = (CCSPXTransform)readByte();
				patch->shape.color = readInt();
				patch->shape.vertexCount = readShort();
				patch->shape.paramCount = readShort();
				patch->shape.params = (short*)calloc(patch->shape.paramCount, sizeof(short));
				short* p = patch->shape.params;
				for(int k = 0; k < patch->shape.paramCount; k++, p++) {
					*p = readShort();
				}
			} else {
				continue;
			}
		}

		// read all collision
		f->m_collisionCount = readShort();
		f->m_collisionRects = (CCSPX3Tile*)calloc(f->m_collisionCount, sizeof(CCSPX3Tile));
		CCSPX3Tile* collision = f->m_collisionRects;
		for(int j = 0; j < f->m_collisionCount; j++, collision++) {
			collision->x1 = readShort();
			collision->y1 = readShort();
			collision->x2 = readShort();
			collision->y2 = readShort();
		}

		// add frame
		spx->m_frames.push_back(f);
		f->retain();
	}

	// read all actions
	spx->m_actionCount = readShort();
	for(int i = 0; i < spx->m_actionCount; i++) {
		// create action
		CCSPX3Action* a = CCSPX3Action::create();

		// mode
		a->m_mode = (CCSPX3ActionMode)readByte();

		// collide
		a->m_collisionRect.x1 = readShort();
		a->m_collisionRect.y1 = readShort();
		a->m_collisionRect.x2 = readShort();
		a->m_collisionRect.y2 = readShort();

		// frame count
		a->m_frameCount = readShort();

		// frames
		a->m_frameIndices = (short*)calloc(a->m_frameCount, sizeof(short));
		short* f = a->m_frameIndices;
		for(int j = 0; j < a->m_frameCount; j++, f++) {
			*f = readShort();
		}

		// depend on mode
		if(a->m_mode == SPX_DELAY) {
			// delays
			a->m_delays = (float*)calloc(a->m_frameCount, sizeof(float));
			float* d = a->m_delays;
			for(int j = 0; j < a->m_frameCount; j++, d++) {
				*d = readInt() / 1000.f;
			}
		} else if(a->m_mode == SPX_NODELAY) {
			// do nothing
		} else {
			// invalid mode
			continue;
		}

		// add action
		spx->m_actions.push_back(a);
		a->retain();
	}

	return spx;
}

CCSPX3FileData* CCSPX3Loader::load(const char* spxPath) {
	unsigned long len;
	char* data = (char*)CCFileUtils::sharedFileUtils()->getFileData(spxPath, "rb", &len);
	CCSPX3FileData* spx = load(data, (size_t)len, 1 / CC_CONTENT_SCALE_FACTOR());
	spx->m_path = spxPath;
	free(data);
	return spx;
}

NS_CC_END