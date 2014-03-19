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
#include "CCAFCSprite.h"
#include "CCAFCAnimation.h"
#include "CCAFCFileData.h"
#include "CCAFCClipMapping.h"
#include <stdarg.h>
#include "CBTextureAtlas.h"
#include "CCUtils.h"

NS_CC_BEGIN

CCAFCSprite::CCAFCSprite() :
		m_animationData(NULL),
        m_drawMarkers(NULL),
		m_curAnimationIndex(-1),
		m_curFrame(0),
		m_numOfFrame(0),
		m_frameDuration(0),
		m_isTickDelay(false),
		m_forceTickMode(false),
		m_elapsed(0),
		m_unitInterval(0),
		m_isReverse(false),
		m_loop(0),
		m_isAnimationEnded(false),
		m_paused(false),
		m_debugDrawFrameRect(false),
		m_debugDrawCollisionRect(false),
		m_callback(NULL),
		m_frameOffset(CCPointZero),
		m_ignoreFrameOffset(false),
		m_flipX(false),
		m_flipY(false) {
    // blend func
    m_sBlendFunc.src = CC_BLEND_SRC;
    m_sBlendFunc.dst = CC_BLEND_DST;
            
    // shader program
    setShaderProgram(CCShaderCache::sharedShaderCache()->
                     programForKey(kCCShader_PositionTextureColor));
}

CCAFCSprite::~CCAFCSprite() {
	for(SpritePtrList::iterator iter = m_spriteList.begin(); iter != m_spriteList.end(); iter++) {
		(*iter)->release();
	}
	for(SpriteBatchNodePtrList::iterator iter = m_sheetList.begin(); iter != m_sheetList.end(); iter++) {
		(*iter)->release();
	}
	for(AFCClipMappingPtrList::iterator iter = m_mappingList.begin(); iter != m_mappingList.end(); iter++) {
		(*iter)->release();
	}
	CC_SAFE_RELEASE(m_animationData);
    
    if(m_drawMarkers) {
        free(m_drawMarkers);
        m_drawMarkers = NULL;
    }
}

CCSpriteBatchNode* CCAFCSprite::createBatchNode(CCTexture2D* tex) {
	CCSpriteBatchNode* bn = CCSpriteBatchNode::createWithTexture(tex);
	CBTextureAtlas* atlas = new CBTextureAtlas();
	atlas->initWithTexture(tex, kDefaultSpriteBatchCapacity);
	atlas->autorelease();
	bn->setTextureAtlas(atlas);
	return bn;
}

void CCAFCSprite::invokeOnAFCAnimationFrameChanged() {
	if(m_callback) {
		m_callback->onAFCSpriteAnimationFrameChanged(this);
	}
}

void CCAFCSprite::invokeOnAFCAnimationEnded() {
	if(m_callback) {
		m_callback->onAFCSpriteAnimationEnded(this);
	}
}

void CCAFCSprite::setAnimationData(CCAFCAnimation* data) {
	CC_SAFE_RETAIN(data);
	CC_SAFE_RELEASE(m_animationData);
	m_animationData = data;
}

void CCAFCSprite::initSpriteFromAnimationData() {
	int maxClipNum = 0;
	for(int i = 0; i < m_numOfFrame; i++) {
		CCAFCFrame* fd = m_animationData->getFrameAt(i);
		maxClipNum = MAX(fd->getClipCount(), maxClipNum);
	}

	int numDifference = maxClipNum - m_spriteList.size();
	for(int i = 0; i < numDifference; i++) {
		CCSprite* sprite = CCSprite::create();
		sprite->setColor(getColor());
		m_spriteList.push_back(sprite);
		sprite->retain();
	}
}

void CCAFCSprite::draw() {
    CC_PROFILER_START_CATEGORY(kCCProfilerCategorySprite, "CCAFCSprite - draw");
    
    CC_NODE_DRAW_SETUP();
    
    ccGLBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);
    
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex);
    
    // check draw marker
    if(m_drawMarkers) {
        memset(m_drawMarkers, 0, m_sheetList.size() * sizeof(int));
    } else {
        m_drawMarkers = (int*)calloc(m_sheetList.size(), sizeof(int));
        for(SpriteBatchNodePtrList::iterator iter = m_sheetList.begin(); iter != m_sheetList.end(); iter++) {
            (*iter)->setUserData(m_drawMarkers + (iter - m_sheetList.begin()));
        }
    }
    
    // update children transform
    for(SpriteBatchNodePtrList::iterator iter = m_sheetList.begin(); iter != m_sheetList.end(); iter++) {
        arrayMakeObjectsPerformSelector((*iter)->getChildren(), updateTransform, CCSprite*);
    }
    
    // draw by clip order
    int numOfQuads = 0;
    CCSpriteBatchNode* lastSheet = NULL;
    for(SpritePtrList::iterator iter = m_spriteList.begin(); iter != m_spriteList.end(); iter++) {
        CCSprite* sprite = *iter;
		if(sprite->isVisible()) {
			CCSpriteBatchNode* sheet = (CCSpriteBatchNode*)sprite->getParent();
			if(sheet != lastSheet) {
				// draw last sheet
				if(numOfQuads != 0) {
                    CBTextureAtlas* atlas = (CBTextureAtlas*)lastSheet->getTextureAtlas();
                    int* pMarker = (int*)lastSheet->getUserData();
                    atlas->drawNumberOfQuadsEx(numOfQuads, *pMarker);
                    *pMarker += numOfQuads;
				}
                
				// change last sheet and reset number of quads
				lastSheet = sheet;
				numOfQuads = 1;
			} else {
				numOfQuads++;
			}
		}
	}
    
    // draw last sheet
	if(numOfQuads != 0) {
        CBTextureAtlas* atlas = (CBTextureAtlas*)lastSheet->getTextureAtlas();
        int* pMarker = (int*)lastSheet->getUserData();
        atlas->drawNumberOfQuadsEx(numOfQuads, *pMarker);
	}
    
    // debug draw frame rect
	if(m_debugDrawFrameRect) {
		CCRect bound = getFrameRect();
        ccDrawColor4B(0, 255, 0, 255);
        ccDrawRect(bound.origin, ccpAdd(bound.origin,
                                        ccp(bound.size.width, bound.size.height)));
		ccDrawColor4B(255, 255, 255, 255);
	}
    
    // debug draw collision rect
    if(m_debugDrawCollisionRect) {
        ccDrawColor4B(0, 255, 0, 255);
		CCAFCAnimation* anim = getCurrentAnimationData();
		if(anim) {
			CCAFCFrame* frame = anim->getFrameAt(m_curFrame);
			if(frame) {
				int count = frame->getClipCount();
				for (int i = 0; i < count; i++) {
					CCAFCClip* clip = frame->getClipAt(i);
					if(clip->getType() == AFC_CLIP_COLLISION_RECT) {
						CCAFCClipData& data = clip->getData();
                        CCPoint origin = ccp(data.clipPos.x - data.cr.size.width / 2,
                                             data.clipPos.y - data.cr.size.height / 2);
                        CCPoint dest = ccp(data.cr.size.width,
                                           data.cr.size.height);
                        
						// check flip flag
						if(m_flipX) {
							origin.x = -origin.x - dest.x;
						}
						if(m_flipY) {
							origin.y = -origin.y - dest.y;
						}
                        
						// draw it
                        ccDrawRect(origin, dest);
					}
				}
			}
		}
        ccDrawColor4B(255, 255, 255, 255);
	}
    
    CC_INCREMENT_GL_DRAWS(1);
    
    CC_PROFILER_STOP_CATEGORY(kCCProfilerCategorySprite, "CCAFCSprite - draw");
}

void CCAFCSprite::setBlendFunc(ccBlendFunc blendFunc) {
    m_sBlendFunc = blendFunc;
    for(SpriteBatchNodePtrList::iterator iter = m_sheetList.begin(); iter != m_sheetList.end(); iter++) {
		(*iter)->setBlendFunc(blendFunc);
	}
}

void CCAFCSprite::setColor(const ccColor3B& color) {
    CCNodeRGBA::setColor(color);
    for(SpritePtrList::iterator iter = m_spriteList.begin(); iter != m_spriteList.end(); iter++) {
		(*iter)->setColor(color);
	}
}

void CCAFCSprite::replaceTextures(CCTexture2D* tex, ...) {
	// release old batch nodes
	for(SpritePtrList::iterator iter = m_spriteList.begin(); iter != m_spriteList.end(); iter++) {
		(*iter)->release();
	}
	m_spriteList.clear();
	for(SpriteBatchNodePtrList::iterator iter = m_sheetList.begin(); iter != m_sheetList.end(); iter++) {
		(*iter)->release();
	}
	m_sheetList.clear();

	// for first texture
	CCSpriteBatchNode* sheet = CCAFCSprite::createBatchNode(tex);
	m_sheetList.push_back(sheet);
	sheet->retain();

	// create new sprite batch node for new set of textures
	va_list textures;
	va_start(textures, tex);
	for(CCTexture2D* t = va_arg(textures, CCTexture2D*); t != NULL; t = va_arg(textures, CCTexture2D*)) {
		sheet = CCAFCSprite::createBatchNode(t);
		m_sheetList.push_back(sheet);
		sheet->retain();
	}
	va_end(textures);
    
    // release draw marker
    if(m_drawMarkers) {
        free(m_drawMarkers);
        m_drawMarkers = NULL;
    }

	// start animation from very beginning
	if(m_curAnimationIndex >= 0)
		playAnimation(m_curAnimationIndex);
}

void CCAFCSprite::replaceTextures(CCTexture2D** tex, int count) {
	// create new sprite batch node for new set of textures
	SpriteBatchNodePtrList tmp;
	for(int i = 0; i < count; i++) {
		CCSpriteBatchNode* sheet = CCAFCSprite::createBatchNode(tex[i]);
		tmp.push_back(sheet);
		sheet->retain();
	}
	
	// release old batch nodes
	for(SpritePtrList::iterator iter = m_spriteList.begin(); iter != m_spriteList.end(); iter++) {
		(*iter)->release();
	}
	m_spriteList.clear();
	for(SpriteBatchNodePtrList::iterator iter = m_sheetList.begin(); iter != m_sheetList.end(); iter++) {
		(*iter)->release();
	}
	m_sheetList.clear();
	m_sheetList = tmp;
    
    // release draw marker
    if(m_drawMarkers) {
        free(m_drawMarkers);
        m_drawMarkers = NULL;
    }

	// start animation from very beginning
	if(m_curAnimationIndex >= 0)
		playAnimation(m_curAnimationIndex);
}

void CCAFCSprite::initFrameOffset() {
	m_frameOffset.x = m_frameOffset.y = 0;
	if(m_isReverse) {
		// because reverse, so we need calculate frame offset first
		int frameCount = m_animationData->getFrameCount();
		for(int i = 0; i < frameCount; i++) {
			CCAFCFrame* frameData = m_animationData->getFrameAt(i);
			ccPoint inc = frameData->getIncrementation();
			m_frameOffset.x += inc.x;
			m_frameOffset.y += inc.y;
		}
	}
}

void CCAFCSprite::adjustFrameOffset(CCAFCFrame* frameData) {
	ccPoint inc = frameData->getIncrementation();
	if(m_isReverse) {
		m_frameOffset.x -= inc.x;
		m_frameOffset.y -= inc.y;
	} else {
		m_frameOffset.x += inc.x;
		m_frameOffset.y += inc.y;
	}
}

void CCAFCSprite::addChild(CCNode* child, int z, int tag) {
    CCSpriteBatchNode* bn = dynamic_cast<CCSpriteBatchNode*>(child);
    if(!bn){
        CCLOGWARN("CCMWSprite::addChild: Can't add a non-CCSpriteBatchNode as child");
        return;
    }

    CCNode::addChild(child, z, tag);
}

void CCAFCSprite::playAnimation(int index, CCAFCClipMapping* mapping) {
	// validate index
	if(index < 0 || index >= getAnimationCount()) {
		CCLOGWARN("CCAFCSprite::playAnimation: index %d is out of range", index);
		return;
	}

	// refresh animation data
	setAnimationData(getAnimationAt(index, mapping));

	// reset frame index and ensure sprite pool is big enough
	m_numOfFrame = m_animationData->getFrameCount();
	initSpriteFromAnimationData();

	// reset loop count
	m_tmpLoop = m_loop;

	// set first frame, if reverse, need start from last frame
	m_isAnimationEnded = false;
	initFrameOffset();
	if(m_isReverse) {
		setFrameIndex(m_numOfFrame - 1);
	} else {
		setFrameIndex(0);
	}

	// save current animation index
	m_curAnimationIndex = index;
}

void CCAFCSprite::playAnimation(int index, int mappingTag) {
	playAnimation(index, getClipMappingByTag(mappingTag));
}

void CCAFCSprite::setFrameIndex(int index) {
	bool frameChanged = m_curFrame != index;
	m_curFrame = index;

	// clear all clip sprite from batch node
	for(SpritePtrList::iterator iter = m_spriteList.begin(); iter != m_spriteList.end(); iter++) {
		CCSprite* sprite = *iter;
		if(sprite->isVisible()) {
			CCSpriteBatchNode* sheet = (CCSpriteBatchNode*)sprite->getParent();
			if(sheet)
				sheet->removeAllChildren();
		} else {
			break;
		}
	}

	// get frame data and adjust frame offset
	CCAFCFrame* frameData = m_animationData->getFrameAt(index);
	adjustFrameOffset(frameData);
    
    // is frame flipped
    bool frameFlipX = frameData->isFlipX();
    bool frameFlipY = frameData->isFlipY();
    
    // flip or not?
    bool flipX = m_flipX != frameFlipX;
    bool flipY = m_flipY != frameFlipY;

	// since not all clip data are image clip
	int imageClipIndex = 0;
	for (int i = 0; i < frameData->getClipCount(); i++) {
		CCAFCClip* clip = frameData->getClipAt(i);
		CCAFCClipData& clipData = clip->getData();

		// it is a image clip
		CCAFCClipType clipType = clip->getType();
		if(clipType == AFC_CLIP_IMAGE) {
			// get clip pos with frame increment added
			CCPoint clipPos = ccpAdd(ccp2CCP(clipData.clipPos), m_ignoreFrameOffset ? CCPointZero : m_frameOffset);

			// check flip flag
			if(flipX)
				clipPos.x = -clipPos.x;
			if(flipY)
				clipPos.y = -clipPos.y;

			// get related sprite sheet, if the sheet in data is NULL, then we use original sheet
			CCSpriteBatchNode* sheet = clipData.i.sheet;
			if(!sheet)
				sheet = m_sheetList.at(clipData.i.imageIndex);

			// create sprite frame and set it to clip sprite
			CCSpriteFrame* tempSpriteFrame = CCSpriteFrame::createWithTexture(sheet->getTexture(), ccr2CCR(clipData.i.rect));
			CCSprite* sprite = m_spriteList.at(imageClipIndex);
			sprite->setDisplayFrame(tempSpriteFrame);
			sprite->setVisible(true);

			// set sprite position, because clipPos is center position so just set it
			sprite->setPosition(ccp(clipPos.x, clipPos.y));

			// set flipX flag
			sprite->setFlipX(clipData.i.flipX);

			// check CCAFCSprite flip settings
			// no flipY needed for sprite because flipY is just a flipX with 180 degree rotation
			float rotation = clipData.i.rotation;
			if(flipX) {
				sprite->setFlipX(!sprite->isFlipX());
				rotation = 360 - rotation;
			}
			if(flipY) {
				sprite->setFlipX(!sprite->isFlipX());
				rotation = 180 - rotation;
			}

			// rotate it
			sprite->setRotation(rotation);

			// add sprite to sheet
			sheet->addChild(sprite);
			sprite->setColor(getColor());
			imageClipIndex++;
		}
	}

	// reset elapse and duration
	m_elapsed = 0;
	m_frameDuration = frameData->getDelay();

	// use tick mode or not
	m_isTickDelay = m_forceTickMode ? true : frameData->isUseTickDelay();

	// if force tick mode but frame is not, then we need reset duration to 1 tick
	if(m_forceTickMode && !frameData->isUseTickDelay())
		m_frameDuration = 1;

	// finally we can compute the real duration
	if(m_isTickDelay && m_unitInterval != 0)
		m_frameDuration *= m_unitInterval;

	// notify callback
	if(frameChanged)
		invokeOnAFCAnimationFrameChanged();

	/*
	 * cached sprite may be more than current frame clips, so hide unused sprites
	 */
	for (int i = imageClipIndex; i < m_spriteList.size(); i++) {
		CCSprite* sprite = m_spriteList.at(i);
		sprite->setVisible(false);
	}
}

void CCAFCSprite::setFlipX(bool flipX) {
	m_flipX = flipX;

	// if node is running, update frame immediately
	if(isRunning()) {
		setFrameIndex(m_curFrame);
	}
}

void CCAFCSprite::setFlipY(bool flipY) {
	m_flipY = flipY;

	// if node is running, update frame immediately
	if(isRunning()) {
		setFrameIndex(m_curFrame);
	}
}

void CCAFCSprite::tick(float delta) {
	if(m_paused || (m_isAnimationEnded && m_tmpLoop == 0)) {
		return;
	}

	// add delta to elapsed time
	// or add 1 seconds for tick mode
	if(m_isTickDelay && m_unitInterval == 0) {
		m_elapsed += 1;
	} else {
		m_elapsed += delta;
	}

	if(m_elapsed >= m_frameDuration) {
		//end of animation in reverse mode
		int nextFrame;
		bool ended = false;
		if(m_isReverse && m_curFrame == 0) {
			nextFrame = m_numOfFrame - 1;
			ended = true;
		} else if(!m_isReverse && m_curFrame == m_numOfFrame - 1) {
			// end of animation in forward mode
			nextFrame = 0;
			ended = true;
		} else {
			nextFrame = m_curFrame + (m_isReverse ? -1 : 1);
		}

		// if animation ended
		if(ended) {
			// re-init frame offset
			initFrameOffset();

			// if the animation doesn't need looping
			// set m_isAnimationEnded to true
			if(m_tmpLoop == 0) {
				m_isAnimationEnded = ended;
			} else if(m_tmpLoop > 0) {
				m_tmpLoop--;
			}

			// notify callback in every loop
			invokeOnAFCAnimationEnded();
		}

		// if not end, set next frame
		if(!m_isAnimationEnded) {
			setFrameIndex(nextFrame);
		}
	}
}

void CCAFCSprite::setLoopCount(int loop) {
	// to make loop count correct, subtract one for first loop
	if(loop > 0)
		loop--;

	m_loop = loop;
	m_tmpLoop = loop;
}

void CCAFCSprite::setUnitInterval(float interval) {
	m_unitInterval = interval;

	// adjust current frame delay
	if(m_animationData) {
		CCAFCFrame* frameData = m_animationData->getFrameAt(m_curFrame);
		if(frameData) {
			if(m_isTickDelay)
				m_frameDuration = frameData->getDelay() * m_unitInterval;
		}
	}
}

int CCAFCSprite::getCollisionRectCount() {
	CCAFCAnimation* anim = getCurrentAnimationData();
	if(anim) {
		CCAFCFrame* frame = anim->getFrameAt(m_curFrame);
		if(frame) {
			return frame->getClipCount(AFC_CLIP_COLLISION_RECT);
		}
	}

	return 0;
}

CCRect CCAFCSprite::getCollisionRect(int index) {
	CCRect ret = CCRectZero;
	CCAFCAnimation* anim = getCurrentAnimationData();
	if(anim) {
		CCAFCFrame* frame = anim->getFrameAt(m_curFrame);
		if(frame) {
			CCAFCClip* clip = frame->getClipAt(AFC_CLIP_COLLISION_RECT, index);
			if(clip) {
				CCAFCClipData& data = clip->getData();
				ret.size.width = data.cr.size.width;
				ret.size.height = data.cr.size.height;
				ret.origin.x = data.clipPos.x - ret.size.width / 2;
				ret.origin.y = data.clipPos.y - ret.size.height / 2;

				if(m_flipX) {
					ret.origin.x = -ret.origin.x - ret.size.width;
				}
				if(m_flipY) {
					ret.origin.y = -ret.origin.y - ret.size.height;
				}
			}
		}
	}

	return ret;
}

CCRect CCAFCSprite::getCollisionRectRelativeToParent(int index) {
	CCRect r = getCollisionRect(index);
    return CCRectApplyAffineTransform(r, nodeToParentTransform());
}

CCRect CCAFCSprite::getCollisionRectRelativeToWorld(int index) {
	CCRect r = getCollisionRect(index);
    return CCRectApplyAffineTransform(r, nodeToWorldTransform());
}

CCRect CCAFCSprite::getFrameRect() {
	CCRect ret = CCRectZero;
	bool first = true;
	CCAFCAnimation* anim = getCurrentAnimationData();
	if(anim) {
		CCAFCFrame* frame = anim->getFrameAt(m_curFrame);
		if(frame) {
			int count = frame->getClipCount(AFC_CLIP_IMAGE);
			for (int i = 0; i < count; i++) {
				CCSprite* sprite = m_spriteList.at(i);
                CCPoint origin = CCUtils::getOrigin(sprite);
                CCSize size = sprite->getContentSize();
				if(first) {
					ret = CCRectMake(origin.x, origin.y, size.width, size.height);
					first = false;
				} else {
                    CCRect frameRect = CCRectMake(origin.x, origin.y, size.width, size.height);
                    ret = CCUtils::combine(ret, frameRect);
				}
			}
		}
	}

	return ret;
}

CCRect CCAFCSprite::getFrameRectRelativeToParent() {
	CCRect r = getFrameRect();
    return CCRectApplyAffineTransform(r, nodeToParentTransform());
}

CCRect CCAFCSprite::getFrameRectRelativeToWorld() {
	CCRect r = getFrameRect();
    return CCRectApplyAffineTransform(r, nodeToWorldTransform());
}

void CCAFCSprite::addClipMapping(CCAFCClipMapping* mapping) {
	if(mapping) {
        m_mappingList.push_back(mapping);
		mapping->retain();
	}
}

void CCAFCSprite::removeClipMappingByTag(int tag) {
    for(AFCClipMappingPtrList::iterator iter = m_mappingList.begin(); iter != m_mappingList.end(); iter++) {
        CCAFCClipMapping* mapping = *iter;
		if(mapping->getTag() == tag) {
            m_mappingList.erase(iter);
			mapping->release();
			break;
		}
	}
}

CCAFCClipMapping* CCAFCSprite::getClipMappingByTag(int tag) {
    for(AFCClipMappingPtrList::iterator iter = m_mappingList.begin(); iter != m_mappingList.end(); iter++) {
        CCAFCClipMapping* mapping = *iter;
		if(mapping->getTag() == tag) {
			return mapping;
		}
	}
	return NULL;
}

NS_CC_END