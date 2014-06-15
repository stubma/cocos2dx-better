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
#include "CCSPX3Manager.h"
#include "CCSPX3Loader.h"

NS_CC_BEGIN

CCSPX3Manager* gSPX3Manager = NULL;

CCSPX3Manager::CCSPX3Manager() :
		m_resScale(1) {
}

CCSPX3Manager::~CCSPX3Manager() {
	releaseAllAnimationFileData();
	gSPX3Manager = NULL;
}

CCSPX3Manager* CCSPX3Manager::getInstance() {
	if(gSPX3Manager == NULL) {
		gSPX3Manager = new CCSPX3Manager();
	}
	return gSPX3Manager;
}

void CCSPX3Manager::releaseAllAnimationFileData() {
	for(SPX3FileDataPtrList::iterator iter = m_spxFileDataList.begin(); iter != m_spxFileDataList.end(); iter++) {
		(*iter)->release();
	}
	m_spxFileDataList.clear();
}

CCSPX3FileData* CCSPX3Manager::load(const char* spxPath) {
	// search cache first
	CCSPX3FileData* data = getSPX3FileData(spxPath);
	if(data != NULL) {
		return data;
	}

	data = CCSPX3Loader::load(spxPath);
	if(data != NULL) {
		m_spxFileDataList.push_back(data);
		data->retain();
	}

	return data;
}

CCSPX3FileData* CCSPX3Manager::getSPX3FileData(const string& spxPath) {
	for(SPX3FileDataPtrList::iterator iter = m_spxFileDataList.begin(); iter != m_spxFileDataList.end(); iter++) {
		if((*iter)->m_path == spxPath) {
			return *iter;
		}
	}
	return NULL;
}

float CCSPX3Manager::resolve(short v) {
	return v * m_resScale;
}

CCAFCAnimation* CCSPX3Manager::getAnimationData(CCSPX3FileData* spx, int animIndex, CCAFCClipMapping* mapping) {
	CCAFCAnimation* afcAnim = CCAFCAnimation::create();

	// save scale
	m_resScale = spx->m_resScale;

	// current action
	CCSPX3Action* spxAction = spx->getActionAt(animIndex);

	// read all frames
	for(int i = 0; i < spxAction->m_frameCount; i++) {
		// create frame
		CCAFCFrame* afcFrame = CCAFCFrame::create();

		// spx frame
		CCSPX3Frame* spxFrame = spx->getFrameAt(spxAction->m_frameIndices[i]);

		// set delay
		if(spxAction->m_mode == SPX_DELAY) {
			afcFrame->setDelay(spxAction->m_delays[i]);
		} else {
			afcFrame->setUseTickDelay(true);
			afcFrame->setDelay(1);
		}

		// read all clip excepts collision rectangle
		CCSPX3Patch* patch = spxFrame->m_patches;
		for(int j = 0; j < spxFrame->m_patchCount; j++, patch++) {
			CCAFCClip* afcClip = CCAFCClip::create();
			CCAFCClipData& afcClipData = afcClip->getData();

			// add clip
			if(patch->type == SPX_PATCH_TILE) {
				// image clip
				afcClip->setType(AFC_CLIP_IMAGE);

				// spx tile set
				CCSPX3TileSet* spxTileSet = spx->getTileSetAt(patch->tile.tileSetIndex);

				// image index
				afcClipData.i.imageIndex = spxTileSet->m_imageIndex;

				// decide clip rect
				if(spxTileSet->m_mode == SPX_TILE_INDEX) {
					int row = patch->tile.tileSetIndex / spxTileSet->m_tileColumns;
					int col = patch->tile.tileSetIndex % spxTileSet->m_tileRows;
					afcClipData.i.rect = ccr(resolve(spxTileSet->m_tileWidth * col),
							resolve(spxTileSet->m_tileHeight * row),
							resolve(spxTileSet->m_tileWidth),
							resolve(spxTileSet->m_tileHeight));

					// set clip center position
					// SpriteX y axis is reversed with opengl y axis, and tile pos is top left corner
					// because we want to get center position, must consider the texture rect size based on rotation
					switch(patch->tile.transform) {
						case SPX_TRANS_NONE:
						case SPX_TRANS_MIRROR_ROT180:
						case SPX_TRANS_MIRROR:
						case SPX_TRANS_ROT180:
							afcClipData.clipPos =  ccpt(resolve(patch->tile.x + spxTileSet->m_tileWidth / 2),
									resolve(-patch->tile.y - spxTileSet->m_tileHeight / 2));
							break;
						case SPX_TRANS_MIRROR_ROT270:
						case SPX_TRANS_ROT90:
						case SPX_TRANS_ROT270:
						case SPX_TRANS_MIRROR_ROT90:
							afcClipData.clipPos =  ccpt(resolve(patch->tile.x + spxTileSet->m_tileHeight / 2),
									resolve(-patch->tile.y - spxTileSet->m_tileWidth / 2));
							break;
					}
				} else {
					CCSPX3Tile* tile = spxTileSet->m_tiles + patch->tile.tileIndex;
					afcClipData.i.rect = ccr(resolve(tile->left()), resolve(tile->top()), resolve(tile->width()), resolve(tile->height()));

					// set clip center position
					// SpriteX y axis is reversed with opengl y axis, and tile pos is top left corner
					// because we want to get center position, must consider the texture rect size based on rotation
					switch(patch->tile.transform) {
						case SPX_TRANS_NONE:
						case SPX_TRANS_MIRROR_ROT180:
						case SPX_TRANS_MIRROR:
						case SPX_TRANS_ROT180:
							afcClipData.clipPos = ccpt(resolve(patch->tile.x + tile->width() / 2), resolve(-patch->tile.y - tile->height() / 2));
							break;
						case SPX_TRANS_MIRROR_ROT270:
						case SPX_TRANS_ROT90:
						case SPX_TRANS_ROT270:
						case SPX_TRANS_MIRROR_ROT90:
							afcClipData.clipPos = ccpt(resolve(patch->tile.x + tile->height() / 2), resolve(-patch->tile.y - tile->width() / 2));
							break;
					}
				}

				// transform
				switch(patch->tile.transform) {
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
			} else {
				// shape clip
				switch(patch->shape.shape) {
					case SPX_POINT:
						afcClip->setType(AFC_CLIP_POINT);
						afcClipData.clipPos = ccpt(resolve(patch->shape.params[0]), resolve(-patch->shape.params[1]));
						break;
					case SPX_LINE:
						afcClip->setType(AFC_CLIP_LINE);
						afcClipData.clipPos = ccpt(resolve(patch->shape.params[0]), resolve(-patch->shape.params[1]));
						afcClipData.l.endPoint.x = resolve(patch->shape.params[2]);
						afcClipData.l.endPoint.y = resolve(-patch->shape.params[3]);
						break;
					case SPX_RECT:
					{
						// rect clip
						afcClip->setType(AFC_CLIP_RECT);

						// color
						afcClipData.r.color = patch->shape.color;

						// corner
						short x1 = resolve(patch->shape.params[0]);
						short y1 = resolve(-patch->shape.params[1]);
						short x2 = resolve(patch->shape.params[2]);
						short y2 = resolve(-patch->shape.params[3]);

						// size
						afcClipData.r.size.width = resolve(abs(x1 - x2));
						afcClipData.r.size.height = resolve(abs(y1 - y2));

						// clip center pos
						afcClipData.clipPos = ccpt((x1 + x2) / 2, (y1 + y2) / 2);

						break;
					}
					case SPX_TRIANGLE:
					{
						// triangle clip
						afcClip->setType(AFC_CLIP_TRIANGLE);

						// points
						afcClipData.clipPos = ccpt(resolve(patch->shape.params[0]), resolve(-patch->shape.params[1]));
						afcClipData.t.pt2.x = resolve(patch->shape.params[2]);
						afcClipData.t.pt2.y = resolve(-patch->shape.params[3]);
						afcClipData.t.pt3.x = resolve(patch->shape.params[4]);
						afcClipData.t.pt3.y = resolve(-patch->shape.params[5]);

						break;
					}
					case SPX_ELLIPSE:
					{
						// ellipse clip
						afcClip->setType(AFC_CLIP_ELLIPSE);

						// color
						afcClipData.e.color = patch->shape.color;

						// corner
						short x1 = resolve(patch->shape.params[0]);
						short y1 = resolve(-patch->shape.params[1]);
						short x2 = resolve(patch->shape.params[2]);
						short y2 = resolve(-patch->shape.params[3]);

						// size
						afcClipData.e.size.width = resolve(abs(x1 - x2));
						afcClipData.e.size.height = resolve(abs(y1 - y2));

						// clip center pos
						afcClipData.clipPos = ccpt((x1 + x2) / 2, (y1 + y2) / 2);

						// angle
						afcClipData.e.startAngle = 0;
						afcClipData.e.endAngle = 360;
						break;
					}
					case SPX_ROUNDRECT:
					{
						// roundrect clip
						afcClip->setType(AFC_CLIP_ROUNDRECT);

						// color
						afcClipData.rr.color = patch->shape.color;

						// corner
						short x1 = resolve(patch->shape.params[0]);
						short y1 = resolve(-patch->shape.params[1]);
						short x2 = resolve(patch->shape.params[2]);
						short y2 = resolve(-patch->shape.params[3]);

						// size
						afcClipData.rr.size.width = resolve(abs(x1 - x2));
						afcClipData.rr.size.height = resolve(abs(y1 - y2));

						// clip center pos
						afcClipData.clipPos = ccpt((x1 + x2) / 2, (y1 + y2) / 2);

						// arc size
						afcClipData.rr.arcWidth = resolve(patch->shape.params[4]);
						afcClipData.rr.arcHeight = resolve(patch->shape.params[5]);

						break;
					}
					case SPX_POLYGON:
					case SPX_ARC:
					case SPX_PIE:
						// cocos2dx-better currently doesn't support such shapes
						break;
				}
			}

			// add clip
			afcFrame->addClip(afcClip);
		}

		// process all collision rect
		CCSPX3Tile* collision = spxFrame->m_collisionRects;
		for(int j = 0; j < spxFrame->m_collisionCount; j++, collision++) {
			CCAFCClip* afcClip = CCAFCClip::create();
			CCAFCClipData& afcClipData = afcClip->getData();

			// type
			afcClip->setType(AFC_CLIP_COLLISION_RECT);

			// clip pos
			afcClipData.clipPos = ccpt(resolve(collision->left() + collision->width() / 2),
					resolve(-collision->bottom() - collision->height() / 2));

			// size
			afcClipData.cr.size.width = resolve(collision->width());
			afcClipData.cr.size.height = resolve(collision->height());

			// add clip
			afcFrame->addClip(afcClip);
		}

		// add frame
		afcAnim->addFrame(afcFrame);
	}

	return afcAnim;
}

NS_CC_END