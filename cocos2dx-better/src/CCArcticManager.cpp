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
#include "CCArcticManager.h"
#include "CCArcticLoader.h"
#include "CCArcticConstants.h"

NS_CC_BEGIN

CCArcticManager* gArcticManager = NULL;

CCArcticManager::CCArcticManager() :
		m_resScale(1) {
}

CCArcticManager::~CCArcticManager() {
	releaseAllAnimationFileData();
	gArcticManager = NULL;
}

CCArcticManager* CCArcticManager::getInstance() {
	if(gArcticManager == NULL) {
		gArcticManager = new CCArcticManager();
	}
	return gArcticManager;
}

void CCArcticManager::releaseAllAnimationFileData() {
	for(ArcticFileDataPtrList::iterator iter = m_arcticFileDataList.begin(); iter != m_arcticFileDataList.end(); iter++) {
		(*iter)->release();
	}
	m_arcticFileDataList.clear();
}

CCArcticFileData* CCArcticManager::load(const char* spxPath) {
	// search cache first
	CCArcticFileData* data = getArcticFileData(spxPath);
	if(data != NULL) {
		return data;
	}

	data = CCArcticLoader::load(spxPath);
	if(data != NULL) {
		m_arcticFileDataList.push_back(data);
		data->retain();
	}

	return data;
}

CCArcticFileData* CCArcticManager::getArcticFileData(const string& asPath) {
	for(ArcticFileDataPtrList::iterator iter = m_arcticFileDataList.begin(); iter != m_arcticFileDataList.end(); iter++) {
		if(asPath == (*iter)->m_path) {
			return *iter;
		}
	}
	return NULL;
}

float CCArcticManager::resolve(short v) {
	return v * m_resScale;
}

void CCArcticManager::parseModule(CCArcticModule* arcticModule, CCArcticFrameModule* arcticFrameModule, CCAFCClip* afcClip, int index, int offsetX, int offsetY) {
	CCAFCClipData& afcClipData = afcClip->getData();

	// set type
	afcClip->setType(AFC_CLIP_IMAGE);

	// set index
	afcClip->setIndex(index);

	// image index
	afcClipData.i.imageIndex = arcticModule->imageIndex;

	// clip pos
	// ASprite y axis is reversed with opengl y axis, and origin is top left corner
	afcClipData.clipPos = ccpt(resolve(arcticFrameModule->x + arcticModule->w / 2 + offsetX),
			resolve(-arcticFrameModule->y - arcticModule->h / 2 - offsetY));

	// save image rect
	afcClipData.i.rect = ccr(resolve(arcticModule->x), resolve(arcticModule->y), resolve(arcticModule->w), resolve(arcticModule->h));

	// set flip flag
	afcClipData.i.flipX = (arcticFrameModule->flags & AS_FLIP_X) != 0;
	if((arcticFrameModule->flags & AS_FLIP_Y) != 0) {
		afcClipData.i.flipX = !afcClipData.i.flipX;
		afcClipData.i.rotation = 180;
	}
}

void CCArcticManager::parseFrameModules(CCArcticFileData* afd, CCArcticFrame* arcticFrame, CCAFCFrame* afcFrame, int offsetX, int offsetY, CCAFCClipMapping* mapping) {
	for(int j = 0; j < arcticFrame->moduleCount; j++) {
		CCAFCClip* afcClip = CCAFCClip::create();
		
		// arctic frame module
		CCArcticFrameModule* arcticFrameModule = afd->m_frameModules + j + arcticFrame->firstModuleIndex;
		
		// index, maybe module or hyper frame
		int index = ((arcticFrameModule->flags & AS_INDEX_EX_MASK) << AS_INDEX_EX_SHIFT) | arcticFrameModule->index;
		
		// is hyper frame?
		if((arcticFrameModule->flags & AS_HYPER_FM) != 0) {
			CCArcticFrame* hyperFrame = afd->m_frames + index;
			parseFrameModules(afd, hyperFrame, afcFrame, arcticFrameModule->x, arcticFrameModule->y, mapping);
		} else {
			// need search clip mapping first
			CCAFCClipMappingRule* rule = NULL;
			if(mapping)
				rule = mapping->findRule(index);
			
			// depend on this rule, build clip way different
			if(!rule) {
				// arctic module
				CCArcticModule* arcticModule = afd->m_modules + index;
				parseModule(arcticModule, arcticFrameModule, afcClip, index, offsetX, offsetY);

				// set index
				afcClip->setIndex(index);
			} else {
				switch(rule->type) {
					case AFC_CMR_INTERNAL_CLIP:
					{
						// if internal mapping, redirect clip index
						if(rule)
							index = rule->icr.destClipIndex;

						// arctic module
						CCArcticModule* arcticModule = afd->m_modules + index;
						parseModule(arcticModule, arcticFrameModule, afcClip, index, offsetX, offsetY);

						break;
					}
					case AFC_CMR_EXTERNAL_CLIP:
					{
						// get external file data
						CCArcticFileData* externalAFD = getArcticFileData(rule->ecr.path);
						if(!externalAFD)
							continue;

						// parse arctic module
						CCArcticModule* arcticModule = externalAFD->m_modules + rule->ecr.destClipIndex;
						parseModule(arcticModule, arcticFrameModule, afcClip, rule->ecr.destClipIndex, offsetX, offsetY);

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
						clipData.clipPos = ccpt(resolve(arcticFrameModule->x) + rule->ear.pos.x,
								resolve(-arcticFrameModule->y) + rule->ear.pos.y);

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
	}
	
	// collision rects
	if(arcticFrame->collisionRectCount > 0) {
		ccRect* cr = arcticFrame->collisionRects;
		for(int i = 0; i < arcticFrame->collisionRectCount; i++, cr++) {
			CCAFCClip* afcClip = CCAFCClip::create();
			CCAFCClipData& afcClipData = afcClip->getData();
			
			// set type
			afcClip->setType(AFC_CLIP_COLLISION_RECT);
			
			// clip pos
			// ASprite y axis is reversed with opengl y axis, and origin is top left corner
			afcClipData.clipPos = ccpt(resolve(cr->x + cr->width / 2 + offsetX),
					resolve(-cr->y - cr->height / 2 - offsetY));
			
			// set rect
			afcClipData.cr.size = ccsz(resolve(cr->width), resolve(cr->height));
			
			// add clip
			afcFrame->addClip(afcClip);
		}
	}
}

CCAFCAnimation* CCArcticManager::getAnimationData(CCArcticFileData* afd, int animIndex, CCAFCClipMapping* mapping) {
	CCAFCAnimation* afcAnim = CCAFCAnimation::create();

	// save scale
	m_resScale = afd->m_resScale;

	// current action
	CCArcticAnimation* arcticAnim = afd->m_animations + animIndex;

	// read all frames
	float prevOffsetX = 0;
	float prevOffsetY = 0;
	for(int i = 0; i < arcticAnim->frameCount; i++) {
		// create frame
		CCAFCFrame* afcFrame = CCAFCFrame::create();

		// animation frame
		CCArcticAnimationFrame* arcticAnimFrame = afd->m_animationFrames + i + arcticAnim->firstFrameIndex;

		// set delay
		afcFrame->setUseTickDelay(true);
		afcFrame->setDelay(arcticAnimFrame->delay);

		// save offset, we need convert it to offset relative to previous frame
		// arctic y axis is reversed to opengl y axis, so negate it
		if(i > 0) {
			afcFrame->setIncrementation(ccpt(resolve(arcticAnimFrame->offsetX - prevOffsetX), resolve(-arcticAnimFrame->offsetY - prevOffsetY)));
		}
		prevOffsetX = arcticAnimFrame->offsetX;
		prevOffsetY = -arcticAnimFrame->offsetY;

		// arctic frame
		int index = ((arcticAnimFrame->flags & AS_INDEX_EX_MASK) << AS_INDEX_EX_SHIFT) | arcticAnimFrame->index;
		CCArcticFrame* arcticFrame = afd->m_frames + index;

		// if frame flipped?
		afcFrame->setFlipX((arcticAnimFrame->flags & AS_FLIP_X) != 0);
		afcFrame->setFlipY((arcticAnimFrame->flags & AS_FLIP_Y) != 0);

		// read all modules
		parseFrameModules(afd, arcticFrame, afcFrame, 0, 0, mapping);

		// add frame
		afcAnim->addFrame(afcFrame);
	}

	return afcAnim;
}

NS_CC_END