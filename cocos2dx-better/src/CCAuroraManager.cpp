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
#include "CCAuroraManager.h"
#include "CCAuroraLoader.h"
#include "CCAuroraConstants.h"

NS_CC_BEGIN

CCAuroraManager* gAuroraManager = NULL;

CCAuroraManager::CCAuroraManager() :
		m_resScale(1) {
}

CCAuroraManager::~CCAuroraManager() {
	releaseAllAnimationFileData();
	gAuroraManager = NULL;
}

CCAuroraManager* CCAuroraManager::getInstance() {
	if(gAuroraManager == NULL) {
		gAuroraManager = new CCAuroraManager();
	}
	return gAuroraManager;
}

void CCAuroraManager::releaseAllAnimationFileData() {
	for(AuroraFileDataPtrList::iterator iter = m_auroraFileDataList.begin(); iter != m_auroraFileDataList.end(); iter++) {
		(*iter)->release();
	}
	m_auroraFileDataList.clear();
}

CCAuroraFileData* CCAuroraManager::load(const char* spxPath) {
	// search cache first
	CCAuroraFileData* data = getAuroraFileData(spxPath);
	if(data != NULL) {
		return data;
	}

	data = CCAuroraLoader::load(spxPath);
	if(data != NULL) {
		m_auroraFileDataList.push_back(data);
		data->retain();
	}

	return data;
}

CCAuroraFileData* CCAuroraManager::getAuroraFileData(const string& asPath) {
	for(AuroraFileDataPtrList::iterator iter = m_auroraFileDataList.begin(); iter != m_auroraFileDataList.end(); iter++) {
		if(asPath == (*iter)->m_path) {
			return *iter;
		}
	}
	return NULL;
}

float CCAuroraManager::resolve(short v) {
	return v * m_resScale;
}

void CCAuroraManager::parseModule(CCAuroraModule* auroraModule, CCAuroraFrameModule* auroraFrameModule, CCAFCClip* afcClip, int index, int offsetX, int offsetY) {
	CCAFCClipData& afcClipData = afcClip->getData();

	// set index
	afcClip->setIndex(index);

	// set module info
	switch(auroraModule->type) {
		case BSM_IMAGE:
			// set type
			afcClip->setType(AFC_CLIP_IMAGE);

			// image index
			afcClipData.i.imageIndex = auroraModule->imageIndex;

			// clip pos
			// BSprite y axis is reversed with opengl y axis, and origin is top left corner
			afcClipData.clipPos = ccpt(resolve(auroraFrameModule->x + auroraModule->w / 2 + offsetX),
					resolve(-auroraFrameModule->y - auroraModule->h / 2 - offsetY));

			// save image rect
			afcClipData.i.rect = ccr(resolve(auroraModule->x), resolve(auroraModule->y),
					resolve(auroraModule->w), resolve(auroraModule->h));

			// set flip flag
			afcClipData.i.flipX = (auroraFrameModule->flags & BS_FLIP_X) != 0;
			if((auroraFrameModule->flags & BS_FLIP_Y) != 0) {
				afcClipData.i.flipX = !afcClipData.i.flipX;
				afcClipData.i.rotation = 180;
			}

			break;
		case BSM_RECT:
			// set type
			afcClip->setType(AFC_CLIP_RECT);

			// clip pos
			// BSprite y axis is reversed with opengl y axis, and origin is top left corner
			afcClipData.clipPos = ccpt(resolve(auroraFrameModule->x + auroraModule->w / 2 + offsetX),
					resolve(-auroraFrameModule->y - auroraModule->h / 2 - offsetY));

			// set rect size
			afcClipData.r.size.width = resolve(auroraModule->w);
			afcClipData.r.size.height = resolve(auroraModule->h);

			break;
		case BSM_FILL_RECT:
			// set type
			afcClip->setType(AFC_CLIP_RECT);

			// clip pos
			// BSprite y axis is reversed with opengl y axis, and origin is top left corner
			afcClipData.clipPos = ccpt(resolve(auroraFrameModule->x + auroraModule->w / 2 + offsetX),
					resolve(-auroraModule->h / 2 - auroraFrameModule->y - offsetY));

			// set rect size
			afcClipData.r.size.width = resolve(auroraModule->w);
			afcClipData.r.size.height = resolve(auroraModule->h);

			// set color
			afcClipData.r.color = auroraModule->color;

			break;
        default:
            break;
	}
}

void CCAuroraManager::parseFrameModules(CCAuroraFileData* afd, CCAuroraFrame* auroraFrame, CCAFCFrame* afcFrame, int offsetX, int offsetY, CCAFCClipMapping* mapping) {
	for(int j = 0; j < auroraFrame->moduleCount; j++) {
		CCAFCClip* afcClip = CCAFCClip::create();
		
		// aurora frame module
		CCAuroraFrameModule* auroraFrameModule = afd->m_frameModules + j + auroraFrame->firstModuleIndex;
		
		// get index, maybe module or frame index
		int index = ((auroraFrameModule->flags & BS_INDEX_EX_MASK) << BS_INDEX_EX_SHIFT) | auroraFrameModule->index;
		
		// is hyper frame?
		if((auroraFrameModule->flags & BS_HYPER_FM) != 0) {
			CCAuroraFrame* hyperFrame = afd->m_frames + index;
			parseFrameModules(afd, hyperFrame, afcFrame, auroraFrameModule->x, auroraFrameModule->y, mapping);
		} else {
			// need search clip mapping first
			CCAFCClipMappingRule* rule = NULL;
			if(mapping)
				rule = mapping->findRule(index);
			
			// depend on this rule, build clip way different
			if(!rule) {
				// parse aurora module
				CCAuroraModule* auroraModule = afd->m_modules + index;
				parseModule(auroraModule, auroraFrameModule, afcClip, index, offsetX, offsetY);
			} else {
				switch(rule->type) {
					case AFC_CMR_INTERNAL_CLIP:
					{
						// redirect index
						index = rule->icr.destClipIndex;
						
						// parse aurora module
						CCAuroraModule* auroraModule = afd->m_modules + index;
						parseModule(auroraModule, auroraFrameModule, afcClip, index, offsetX, offsetY);
						break;
					}
					case AFC_CMR_EXTERNAL_CLIP:
					{
						// get external file data
						CCAuroraFileData* externalAFD = getAuroraFileData(rule->ecr.path);
						if(!externalAFD)
							continue;

						// parse aurora module
						CCAuroraModule* auroraModule = externalAFD->m_modules + rule->ecr.destClipIndex;
						parseModule(auroraModule, auroraFrameModule, afcClip, rule->ecr.destClipIndex, offsetX, offsetY);
						
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
						clipData.clipPos = ccpt(resolve(auroraFrameModule->x) + rule->ear.pos.x,
								resolve(-auroraFrameModule->y) + rule->ear.pos.y);
						
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
}

CCAFCAnimation* CCAuroraManager::getAnimationData(CCAuroraFileData* afd, int animIndex, CCAFCClipMapping* mapping) {
	CCAFCAnimation* afcAnim = CCAFCAnimation::create();

	// save scale
	m_resScale = afd->m_resScale;

	// current action
	CCAuroraAnimation* auroraAnim = afd->m_animations + animIndex;

	// read all frames
	float prevOffsetX = 0;
	float prevOffsetY = 0;
	ccRect* collisionRect = afd->m_collisionRects;
	for(int i = 0; i < auroraAnim->frameCount; i++) {
		// create frame
		CCAFCFrame* afcFrame = CCAFCFrame::create();

		// animation frame
		CCAuroraAnimationFrame* auroraAnimFrame = afd->m_animationFrames + i + auroraAnim->firstFrameIndex;

		// set delay
		afcFrame->setUseTickDelay(true);
		afcFrame->setDelay(auroraAnimFrame->delay);

		// save offset, we need convert it to offset relative to previous frame
		// aurora y axis is reversed to opengl y axis, so negate it
		if(i > 0) {
			afcFrame->setIncrementation(ccpt(resolve(auroraAnimFrame->offsetX - prevOffsetX), resolve(-auroraAnimFrame->offsetY - prevOffsetY)));
		}
		prevOffsetX = auroraAnimFrame->offsetX;
		prevOffsetY = -auroraAnimFrame->offsetY;

		// aurora frame
		int index = ((auroraAnimFrame->flags & BS_INDEX_EX_MASK) << BS_INDEX_EX_SHIFT) | auroraAnimFrame->index;
		CCAuroraFrame* auroraFrame = afd->m_frames + index;

		// if frame flipped?
		afcFrame->setFlipX((auroraAnimFrame->flags & BS_FLIP_X) != 0);
		afcFrame->setFlipY((auroraAnimFrame->flags & BS_FLIP_Y) != 0);

		// read all modules
		parseFrameModules(afd, auroraFrame, afcFrame, 0, 0, mapping);

		// add collision rects
		for(int j = 0; j < auroraFrame->collisionRectCount; j++, collisionRect++) {
			// create clip
			CCAFCClip* afcClip = CCAFCClip::create();
			CCAFCClipData& afcClipData = afcClip->getData();

			// set type
			afcClip->setType(AFC_CLIP_COLLISION_RECT);

			// set clip pos
			afcClipData.clipPos = ccpt(resolve(collisionRect->x + collisionRect->width / 2),
					resolve(-collisionRect->y - collisionRect->height / 2));

			// set rect size
			afcClipData.cr.size.width = resolve(collisionRect->width);
			afcClipData.cr.size.height = resolve(collisionRect->height);

			// add clip
			afcFrame->addClip(afcClip);
		}

		// add frame
		afcAnim->addFrame(afcFrame);
	}

	return afcAnim;
}

NS_CC_END