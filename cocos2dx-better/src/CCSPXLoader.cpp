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
#include "CCSPXLoader.h"
#include <stdlib.h>
#include "CCSPXFrame.h"
#include "CCSPXAction.h"
#include <math.h>
#include "CCMoreMacros.h"
#include "CCUtils.h"
 
NS_CC_BEGIN

// sprite x file version, 3.3
#define SPX_VERSION 34

// endian
#define SPX_BIG_ENDIAN 1
#define SPX_LITTLE_ENDIAN 0

// byte order of current parsing file
static bool s_bigEndian;

// data in parsing
static const char* s_data;

// read position in s_data
static int s_pos;

static short readShort() {
	short ret = *(short*)(s_data + s_pos);
	if(s_bigEndian)
		ret = betoh16(ret);
	else
		ret = letoh16(ret);
	s_pos += sizeof(short);
	return ret;
}

static int readInt() {
	int ret = *(int*)(s_data + s_pos);
	if(s_bigEndian)
		ret = betoh32(ret);
	else
		ret = letoh32(ret);
	s_pos += sizeof(int);
	return ret;
}

static int readByte() {
	return s_data[s_pos++] & 0xFF;
}

CCSPXFileData* CCSPXLoader::load(const char* data, size_t length, float resScale) {
	// verify sig
	s_data = data;
	if(s_data[0] != 'S' || s_data[1] != 'P' || s_data[2] != 'R' || s_data[3] != 'X') {
		CCLOGERROR("CCSPXLoader::load: Invalid SpriteX file header");
		return NULL;
	}
	s_pos = 4;

	// verify version
	if(readByte() != SPX_VERSION) {
		CCLOGERROR("CCSPXLoader::load: unsupported SpriteX file version: %d", s_data[s_pos]);
		return NULL;
	}

	// check byte order
	s_bigEndian = readByte() == SPX_BIG_ENDIAN;

	// create data object
	CCSPXFileData* spx = CCSPXFileData::create();

	// save scale
	spx->m_resScale = resScale;

	// get tile count
	spx->m_tileCount = readInt();

	// read all tile info
	spx->m_tiles = (CCSPXTile*)malloc(spx->m_tileCount * sizeof(CCSPXTile));
	for(int i = 0; i < spx->m_tileCount; i++) {
		// read x, y, w, h
		spx->m_tiles[i].x = readShort();
		spx->m_tiles[i].y = readShort();
		spx->m_tiles[i].w = readShort();
		spx->m_tiles[i].h = readShort();
	}

	// get frame count
	spx->m_frameCount = readInt();

	// read all frames
	for(int i = 0; i < spx->m_frameCount; i++) {
		// create frame
		CCSPXFrame* frame = new CCSPXFrame();

		// read frame info
		frame->m_frameTileCount = readInt();
		frame->m_collisionCount = readInt();
		frame->m_referencePointCount = readInt();
		frame->m_height = abs(readShort() - readShort());
		frame->m_width = abs(readShort() - readShort());

		// read frame tiles
		if(frame->m_frameTileCount > 0) {
			frame->m_tiles = (CCSPXFrameTile*)malloc(sizeof(CCSPXFrameTile) * frame->m_frameTileCount);
			for(int i = 0; i < frame->m_frameTileCount; i++) {
				frame->m_tiles[i].index = readShort();
				frame->m_tiles[i].x = readShort();
				frame->m_tiles[i].y = readShort();
				frame->m_tiles[i].transform = (CCSPXTransform)readShort();
			}
		}

		// read frame collision
		if(frame->m_collisionCount) {
			frame->m_collisions = (CCSPXCollision*)malloc(sizeof(CCSPXCollision) * frame->m_collisionCount);
			for(int i = 0; i < frame->m_collisionCount; i++) {
				frame->m_collisions[i].x = readShort();
				frame->m_collisions[i].y = readShort();
				frame->m_collisions[i].width = readShort();
				frame->m_collisions[i].height = readShort();
			}
		}

		// read frame reference points
		if(frame->m_referencePointCount > 0) {
			frame->m_refPoints = (CCSPXReferencePoint*)malloc(sizeof(CCSPXReferencePoint) * frame->m_referencePointCount);
			for(int i = 0; i < frame->m_referencePointCount; i++) {
				frame->m_refPoints[i].x = readShort();
				frame->m_refPoints[i].y = readShort();
			}
		}

		// add frame
		spx->m_frames.push_back(frame);
	}

	// get action count
	spx->m_actionCount = readInt();

	// read all actions
	for(int i = 0; i < spx->m_actionCount; i++) {
		// create action
		CCSPXAction* action = new CCSPXAction();

		// read frame count
		action->m_frameCount = readInt();

		// read delay
		int delay = readByte();
		float duration = delay / 1000.f;
		bool uniformDelay = delay != 1;

		// read transform
		action->m_transform = (CCSPXTransform)readInt();

		// read frame info
		int maxTile = 0;
		action->m_frames = (CCSPXActionFrame*)malloc(sizeof(CCSPXActionFrame) * action->m_frameCount);
		for(int i = 0; i < action->m_frameCount; i++) {
			action->m_frames[i].index = readShort();
			if(uniformDelay)
				action->m_frames[i].duration = duration;
			else
				action->m_frames[i].duration = readShort() / 1000.f;

			// check max tile
			CCSPXFrame* f = spx->getFrameAt(action->m_frames[i].index);
			maxTile = MAX(maxTile, f->m_frameTileCount);
		}

		// set max frame tile
		action->m_maxFrameTile = maxTile;

		// add action
		spx->m_actions.push_back(action);
	}

	return spx;
}

CCSPXFileData* CCSPXLoader::load(const char* spxPath) {
	unsigned long len;
	char* data = (char*)CCFileUtils::sharedFileUtils()->getFileData(spxPath, "rb", &len);
	CCSPXFileData* spx = load(data, (size_t)len, 1 / CC_CONTENT_SCALE_FACTOR());
	spx->m_path = spxPath;
	free(data);
	return spx;
}

NS_CC_END