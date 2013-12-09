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
#include "CCSPXManager.h"
#include "CCSPXLoader.h"

NS_CC_BEGIN

CCSPXManager* gSPXManager = NULL;

CCSPXManager::CCSPXManager() :
		m_resScale(1) {
}

CCSPXManager::~CCSPXManager() {
	releaseAllAnimationFileData();
	gSPXManager = NULL;
}

CCSPXManager* CCSPXManager::getInstance() {
	if(gSPXManager == NULL) {
		gSPXManager = new CCSPXManager();
	}
	return gSPXManager;
}

void CCSPXManager::releaseAllAnimationFileData() {
	for(SPXFileDataPtrList::iterator iter = m_spxFileDataList.begin(); iter != m_spxFileDataList.end(); iter++) {
		(*iter)->release();
	}
	m_spxFileDataList.clear();
}

CCSPXFileData* CCSPXManager::load(const char* spxPath) {
	// search cache first
	CCSPXFileData* data = getSPXFileData(spxPath);
	if(data != NULL) {
		return data;
	}

	data = CCSPXLoader::load(spxPath);
	if(data != NULL) {
		m_spxFileDataList.push_back(data);
		data->retain();
	}

	return data;
}

CCSPXFileData* CCSPXManager::getSPXFileData(const string& spxPath) {
	for(SPXFileDataPtrList::iterator iter = m_spxFileDataList.begin(); iter != m_spxFileDataList.end(); iter++) {
		if(spxPath == (*iter)->m_path) {
			return *iter;
		}
	}
	return NULL;
}

float CCSPXManager::resolve(short v) {
	return v * m_resScale;
}

void CCSPXManager::parseTile(CCSPXTile* spxTile, CCSPXFrameTile* spxFrameTile, CCAFCClip* afcClip, int index) {
	CCAFCClipData& afcClipData = afcClip->getData();

	// set type
	afcClip->setType(AFC_CLIP_IMAGE);

	// clip index
	afcClip->setIndex(index);

	// image index
	afcClipData.i.imageIndex = 0;

	// image rect
	afcClipData.i.rect = ccr(resolve(spxTile->x), resolve(spxTile->y), resolve(spxTile->w), resolve(spxTile->h));

	// transform
	switch(spxFrameTile->transform) {
		case SPX_TRANS_MIRROR_ROT180:
			afcClipData.i.flipX = true;
			afcClipData.i.rotation = 180;
			break;
		case SPX_TRANS_MIRROR:
			afcClipData.i.flipX = true;
			break;
		case SPX_TRANS_ROT180:
			afcClipData.i.rotation = 180;
			break;
		case SPX_TRANS_MIRROR_ROT270:
			afcClipData.i.flipX = true;
			afcClipData.i.rotation = 270;
			break;
		case SPX_TRANS_ROT90:
			afcClipData.i.rotation = 90;
			break;
		case SPX_TRANS_ROT270:
			afcClipData.i.rotation = 270;
			break;
		case SPX_TRANS_MIRROR_ROT90:
			afcClipData.i.flipX = true;
			afcClipData.i.rotation = 90;
			break;
		default:
			break;
	}

	// set clip center position
	// SpriteX y axis is reversed with opengl y axis, and tile pos is top left corner
	// because we want to get center position, must consider the texture rect size based on rotation
	switch(spxFrameTile->transform) {
		case SPX_TRANS_NONE:
		case SPX_TRANS_MIRROR_ROT180:
		case SPX_TRANS_MIRROR:
		case SPX_TRANS_ROT180:
			afcClipData.clipPos = ccpt(resolve(spxFrameTile->x + spxTile->w / 2), resolve(-spxFrameTile->y - spxTile->h / 2));
			break;
		case SPX_TRANS_MIRROR_ROT270:
		case SPX_TRANS_ROT90:
		case SPX_TRANS_ROT270:
		case SPX_TRANS_MIRROR_ROT90:
			afcClipData.clipPos = ccpt(resolve(spxFrameTile->x + spxTile->h / 2), resolve(-spxFrameTile->y - spxTile->w / 2));
			break;
	}
}

CCAFCAnimation* CCSPXManager::getAnimationData(CCSPXFileData* spx, int animIndex, CCAFCClipMapping* mapping) {
	CCAFCAnimation* afcAnim = CCAFCAnimation::create();

	// save scale
	m_resScale = spx->m_resScale;

	// current action
	CCSPXAction* spxAction = spx->getActionAt(animIndex);

	// read all frames
	for(int i = 0; i < spxAction->m_frameCount; i++) {
		// create frame
		CCAFCFrame* afcFrame = CCAFCFrame::create();

		// spx frame in action
		CCSPXActionFrame* spxActionFrame = spxAction->m_frames + i;

		// set delay
		afcFrame->setDelay(spxActionFrame->duration);

		// spx frame
		CCSPXFrame* spxFrame = spx->getFrameAt(spxActionFrame->index);

		// read all image clip
		for(int j = 0; j < spxFrame->m_frameTileCount; j++) {
			CCAFCClip* afcClip = CCAFCClip::create();

			// tile in frame
			CCSPXFrameTile* spxFrameTile = spxFrame->m_tiles + j;

			// need search clip mapping first
			int index = spxFrameTile->index;
			CCAFCClipMappingRule* rule = NULL;
			if(mapping)
				rule = mapping->findRule(index);

			if(!rule) {
				// parse tile into afc clip
				CCSPXTile* spxTile = spx->m_tiles + index;
				parseTile(spxTile, spxFrameTile, afcClip, index);
			} else {
				switch(rule->type) {
					case AFC_CMR_INTERNAL_CLIP:
					{
						// if internal mapping, redirect clip index
						if(rule)
							index = rule->icr.destClipIndex;

						// parse tile into afc clip
						CCSPXTile* spxTile = spx->m_tiles + index;
						parseTile(spxTile, spxFrameTile, afcClip, index);

						break;
					}
					case AFC_CMR_EXTERNAL_CLIP:
					{
						// get external file data
						CCSPXFileData* externalAFD = getSPXFileData(rule->ecr.path);
						if(!externalAFD)
							continue;

						// parse arctic module
						CCSPXTile* spxTile = externalAFD->m_tiles + rule->ecr.destClipIndex;
						parseTile(spxTile, spxFrameTile, afcClip, rule->ecr.destClipIndex);

						// need redirect sheet to external sheet
						afcClip->getData().i.sheet = rule->ecr.sheet;

						break;
					}
					case AFC_CMR_EXTERNAL_ATLAS:
					{
						CCAFCClipData& clipData = afcClip->getData();

						// sheet
						clipData.i.sheet = rule->ear.sheet;

						// clip pos
						clipData.clipPos = ccpt(resolve(spxFrameTile->x) + rule->ear.pos.x,
								resolve(-spxFrameTile->y) + rule->ear.pos.y);

						// clip image rect
						clipData.i.rect = rule->ear.texRect;

						// flip flag
						clipData.i.flipX = rule->ear.flipX;
						clipData.i.rotation = rule->ear.rotation;

						break;
					}
				}
			}

			// add clip
			afcFrame->addClip(afcClip);
		}

		// real all collision clip
		for(int j = 0; j < spxFrame->m_collisionCount; j++) {
			CCAFCClip* afcClip = CCAFCClip::create();
			CCAFCClipData& afcClipData = afcClip->getData();

			// spx collision
			CCSPXCollision* spxCollision = spxFrame->m_collisions + j;

			// set type
			afcClip->setType(AFC_CLIP_COLLISION_RECT);

			// set clip pos
			// SpriteX y axis is reversed with opengl y axis, and tile pos is top left corner
			afcClipData.clipPos = ccpt(resolve(spxCollision->x + spxCollision->width / 2), resolve(-spxCollision->y - spxCollision->height / 2));

			// collision rect
			afcClipData.cr.size = ccsz(resolve(spxCollision->width), resolve(spxCollision->height));

			// add clip
			afcFrame->addClip(afcClip);
		}

		// real all point clip
		for(int j = 0; j < spxFrame->m_referencePointCount; j++) {
			CCAFCClip* afcClip = CCAFCClip::create();
			CCAFCClipData& afcClipData = afcClip->getData();

			// reference point
			CCSPXReferencePoint* spxRefPoint = spxFrame->m_refPoints + j;

			// set type
			afcClip->setType(AFC_CLIP_POINT);

			// set clip pos
			afcClipData.clipPos = ccpt(resolve(spxRefPoint->x), resolve(-spxRefPoint->y));

			// add clip
			afcFrame->addClip(afcClip);
		}

		// add frame
		afcAnim->addFrame(afcFrame);
	}

	return afcAnim;
}

NS_CC_END