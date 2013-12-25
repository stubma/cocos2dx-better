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
#include "CCCatmullRomSprite.h"
#include "CCTextureAtlasEx.h"

NS_CC_BEGIN

CCCatmullRomSprite::CCCatmullRomSprite(CCSprite* sprite) :
m_sprite(NULL),
m_dirty(false),
m_tension(0.5f),
m_atlas(NULL) {
	CCAssert(sprite != NULL, "CCCatmullRomSprite doesn't accept NULL sprite");
    
	// save sprite
	m_sprite = sprite;
	CC_SAFE_RETAIN(m_sprite);
    
	// atlas
	m_atlas = CCTextureAtlasEx::createWithTexture(m_sprite->getTexture(), kDefaultSpriteBatchCapacity);
	CC_SAFE_RETAIN(m_atlas);
    
	// shader program
	setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
    
	// set content size
	setContentSize(CCSizeZero);
    
    // update some member variables
    m_patternLength = m_sprite->getContentSize().width;
    m_patternWidth = m_sprite->getContentSize().height;
}

CCCatmullRomSprite::~CCCatmullRomSprite() {
	CC_SAFE_RELEASE(m_sprite);
	CC_SAFE_RELEASE(m_atlas);
}

CCCatmullRomSprite* CCCatmullRomSprite::create(const char* fileImage) {
	return CCCatmullRomSprite::createWithSprite(CCSprite::create(fileImage));
}

CCCatmullRomSprite* CCCatmullRomSprite::createWithTexture(CCTexture2D* tex) {
	return CCCatmullRomSprite::createWithSprite(CCSprite::createWithTexture(tex));
}

CCCatmullRomSprite* CCCatmullRomSprite::createWithSpriteFrameName(const char* name) {
	return createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name));
}

CCCatmullRomSprite* CCCatmullRomSprite::createWithSpriteFrame(CCSpriteFrame* frame) {
	return CCCatmullRomSprite::createWithSprite(CCSprite::createWithSpriteFrame(frame));
}

CCCatmullRomSprite* CCCatmullRomSprite::createWithSprite(CCSprite* sprite) {
	CCCatmullRomSprite* s = new CCCatmullRomSprite(sprite);
	return (CCCatmullRomSprite*)s->autorelease();
}

void CCCatmullRomSprite::setOpacity(GLubyte opacity) {
	m_sprite->setOpacity(opacity);
	m_dirty = true;
}

void CCCatmullRomSprite::draw() {
    // at least we need two points
    if(m_controlPoints.getCount() < 2)
        return;
    
	// build atlas
	if(m_dirty) {
		updateAtlas();
		m_dirty = false;
	}
	
	// profile start
	CC_PROFILER_START_CATEGORY(kCCProfilerCategorySprite, "CCCatmullRomSprite - draw");
	
	// setup
    CC_NODE_DRAW_SETUP();
	
	// blend func
	ccBlendFunc bf = m_sprite->getBlendFunc();
    ccGLBlendFunc(bf.src, bf.dst);
	
    // draw
	if(m_atlas)
		m_atlas->drawQuadsEx();
	
	// profile end
    CC_PROFILER_STOP_CATEGORY(kCCProfilerCategorySprite, "CCCatmullRomSprite - draw");
}

void CCCatmullRomSprite::addControlPoint(const CCPoint& p) {
    m_controlPoints.addPoint(p.x, p.y);
    m_dirty = true;
}

void CCCatmullRomSprite::setControlPoints(CCPointArray& pa) {
    m_controlPoints.clear();
    const vector<CCPoint*>* plist = pa.getControlPoints();
    for(vector<CCPoint*>::const_iterator iter = plist->begin(); iter != plist->end(); iter++) {
        m_controlPoints.addPoint((*iter)->x, (*iter)->y);
    }
    m_dirty = true;
}

void CCCatmullRomSprite::updateAtlas() {
    // populate points
    populatePoints(m_controlPoints, m_points);
    
    // clear atlas
	m_atlas->removeAllQuads();
    
    // basic check, at least we need two points
    int pc = m_points.getCount();
    if(pc < 2)
        return;
    
    // append a point to avoid losting last segment
    CCPoint pLast0 = m_points.getPointAt(pc - 1);
    CCPoint pLast1 = m_points.getPointAt(pc - 2);
    CCPoint pAppend = ccpAdd(pLast0, ccpSub(pLast0, pLast1));
    m_points.addPoint(pAppend);
    pc++;
    
    // first two points
    CCPoint p0 = m_points.getPointAt(0);
    CCPoint p1 = m_points.getPointAt(1);
    
    // half width of pattern
    float halfWidth = m_patternWidth / 2;

    // bl and br of first quad
    CCPoint bl, tl;
    {
        CCPoint v01 = ccpSub(p1, p0);
        float r01 = ccpToAngle(v01);
        bl.x = p0.x + halfWidth * sinf(r01);
        bl.y = p0.y - halfWidth * cosf(r01);
        tl.x = p0.x - halfWidth * sinf(r01);
        tl.y = p0.y + halfWidth * cosf(r01);
    }
    
    // current length
    float texStartP = 0;
	float texEndP;
	float headPos = 0;
    
    // populate quads
    for(int i = 2; i < pc; i++) {
        // third point
        CCPoint p2 = m_points.getPointAt(i);
        
        // to calculate the angle between y axis and enter angle split
        CCPoint v01 = ccpSub(p1, p0);
        CCPoint v12 = ccpSub(p2, p1);
        float r = (ccpToAngle(v01) + ccpToAngle(v12)) / 2 - M_PI_2;
		
		// populate tl and tr
		CCPoint br, tr;
        br.x = p1.x + halfWidth * cosf(r);
        br.y = p1.y + halfWidth * sinf(r);
        tr.x = p1.x - halfWidth * cosf(r);
        tr.y = p1.y - halfWidth * sinf(r);

        // calculate texcoords pencentage
        float segLen = ccpLength(v01);
		float remainLen = segLen;
        float initVerP = 0;
        float stepVerP = m_patternLength / segLen;
        while(remainLen > m_patternLength) {
            texEndP = texStartP;
            float p = (1 - texStartP) / (1 - texStartP + texEndP);
            populateQuad(bl, br, tl, tr, texStartP, 1, initVerP, initVerP + stepVerP * p);
            populateQuad(bl, br, tl, tr, 0, texEndP, initVerP + stepVerP * p, initVerP + stepVerP);
            initVerP += stepVerP;
            
            // cut pattern length
            remainLen -= m_patternLength;
        }
        
        // remaining length
		headPos += segLen;
		headPos = fmodf(headPos, m_patternLength);
        texEndP = headPos / m_patternLength;
        stepVerP = remainLen / segLen;
        if(texEndP <= texStartP) {
            float p = (1 - texStartP) / (1 - texStartP + texEndP);
            populateQuad(bl, br, tl, tr, texStartP, 1, initVerP, initVerP + stepVerP * p);
            populateQuad(bl, br, tl, tr, 0, texEndP, initVerP + stepVerP * p, initVerP + stepVerP);
        } else {
            populateQuad(bl, br, tl, tr, texStartP, texEndP, initVerP, 1);
        }
        
        // move forward
        p0 = p1;
        p1 = p2;
        bl = br;
        tl = tr;
        texStartP = texEndP;
    }
}

void CCCatmullRomSprite::populateQuad(const CCPoint& bl, const CCPoint& br, const CCPoint& tl, const CCPoint& tr, float texStartP, float texEndP, float verStartP, float verEndP) {
    ccV3F_C4B_T2F_Quad quad = m_sprite->getQuad();
    
    // vertices
    quad.bl.vertices.x = bl.x * (1 - verStartP) + br.x * verStartP;
    quad.bl.vertices.y = bl.y * (1 - verStartP) + br.y * verStartP;
    quad.br.vertices.x = bl.x * (1 - verEndP) + br.x * verEndP;
    quad.br.vertices.y = bl.y * (1 - verEndP) + br.y * verEndP;
    quad.tl.vertices.x = tl.x * (1 - verStartP) + tr.x * verStartP;
    quad.tl.vertices.y = tl.y * (1 - verStartP) + tr.y * verStartP;
    quad.tr.vertices.x = tl.x * (1 - verEndP) + tr.x * verEndP;
    quad.tr.vertices.y = tl.y * (1 - verEndP) + tr.y * verEndP;
    
    // save texcoords, bl and tl
    float blu = quad.bl.texCoords.u;
    float bru = quad.br.texCoords.u;
    float tlu = quad.tl.texCoords.u;
    float tru = quad.tr.texCoords.u;
    quad.bl.texCoords.u = blu * (1 - texStartP) + bru * texStartP;
    quad.tl.texCoords.u = tlu * (1 - texStartP) + tru * texStartP;
    quad.br.texCoords.u = blu * (1 - texEndP) + bru * texEndP;
    quad.tr.texCoords.u = tlu * (1 - texEndP) + tru * texEndP;
    
	CCLOG("bl: %f", quad.bl.vertices.x);
    
    // add quad
    while(m_atlas->getTotalQuads() >= m_atlas->getCapacity())
        m_atlas->resizeCapacity(m_atlas->getCapacity() * 2);
    m_atlas->insertQuad(&quad, m_atlas->getTotalQuads());
}

void CCCatmullRomSprite::populatePoints(const CCPointList& controlPoints, CCPointList& points) {
    // clear
    points.clear();
    
    // populate points segment by segment
    int totalSeg = controlPoints.getCount() - 1;
    for(int curSeg = 0; curSeg < totalSeg; curSeg++) {
        // four control points
        CCPoint cp0 = controlPoints.getPointAt(curSeg - 1);
        CCPoint cp1 = controlPoints.getPointAt(curSeg);
        CCPoint cp2 = controlPoints.getPointAt(curSeg + 1);
        CCPoint cp3 = controlPoints.getPointAt(curSeg + 2);
        
        // segment length, desired points
        float curSegLen = ccpLength(ccpSub(cp2, cp1));
        int curSegPoints = (int)ceilf(curSegLen / m_patternLength) + 1;
        float step = 1.0f / (curSegPoints - 1);
        
        // populate points for this segments
        float lt = 0;
        while(lt < 1) {
            CCPoint pos = ccCardinalSplineAt(cp0, cp1, cp2, cp3, m_tension, lt);
            points.addPoint(pos);
            lt += step;
        }
    }
    
    // last point
    points.addPoint(controlPoints.getPointAt(controlPoints.getCount() - 1));
}

void CCCatmullRomSprite::setTension(float t) {
    m_tension = t;
    m_dirty = true;
}

NS_CC_END