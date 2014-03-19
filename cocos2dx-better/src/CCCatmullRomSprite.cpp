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
#include "CBTextureAtlas.h"
#include "CCUtils.h"
#include "CCMoreMacros.h"

NS_CC_BEGIN

CCCatmullRomSprite::CCCatmullRomSprite(CCSprite* sprite) :
m_sprite(NULL),
m_dirty(false),
m_allVisible(true),
m_tension(0.1f),
m_atlas(NULL) {
	CCAssert(sprite != NULL, "CCCatmullRomSprite doesn't accept NULL sprite");
    
	// save sprite
	m_sprite = sprite;
	CC_SAFE_RETAIN(m_sprite);
    
	// atlas
	m_atlas = CBTextureAtlas::createWithTexture(m_sprite->getTexture(), kDefaultSpriteBatchCapacity);
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
	if(m_atlas) {
        if(m_allVisible) {
            m_atlas->drawQuadsEx();
        } else {
            int startIndex = 0;
            int sc = getSegmentCount();
            for(int i = 0; i < sc; i++) {
                bool visible = isSegmentVisible(i);
                if(!visible) {
                    int endIndex = m_segmentQuadIndices[i];
                    m_atlas->drawNumberOfQuadsEx(endIndex - startIndex, startIndex);
                    startIndex = m_segmentQuadIndices[i + 1];
                }
            }
            
            // last
            if(m_atlas->getTotalQuads() > startIndex) {
                m_atlas->drawNumberOfQuadsEx(m_atlas->getTotalQuads() - startIndex, startIndex);
            }
        }
    }
	
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
    
    // clear
	m_atlas->removeAllQuads();
    m_segmentQuadIndices.clear();
    
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
    int segIndex = 0;
    for(int i = 2; i < pc; i++) {
        // save quad index
        if(m_segmentPointIndices[segIndex] == i - 2) {
            m_segmentQuadIndices.push_back(m_atlas->getTotalQuads());
            segIndex++;
        }
        
        // third point
        CCPoint p2 = m_points.getPointAt(i);
        
        // 3 vectors, from 0 to 1, from 1 to 2, from 1 to 0
        CCPoint v01 = ccpSub(p1, p0);
        CCPoint v12 = ccpSub(p2, p1);
        CCPoint v10 = ccpSub(p0, p1);
        
        // angle in center of v01 and v12, then rotate 90 degrees
        float r = (ccpToAngle(v01) + ccpToAngle(v12)) / 2 - M_PI_2;
        
        // the vector of center divider
        CCPoint m = ccp(cosf(r), sinf(r));
        
        // angle between center and v10
        float rm01 = ccpToAngle(m) - ccpToAngle(v10);
        
        // the actual base width of joint
        // but we must prevent the base width to be too large
        float s = sinf(rm01);
        float w = MAX_FLOAT;
        if(s != 0)
            w = fabsf(halfWidth / s);
        w = MIN(halfWidth * 2, w);
        
        // a corner situation when v01 is in third quadrant and v12 is in fourth quadrant
        if(v01.x < 0 && SIGN(v01.x) == SIGN(v12.x) && SIGN(v01.y) != SIGN(v12.y)) {
            r = M_PI + r;
        }
		
		// populate tl and tr
		CCPoint br, tr;
        br.x = p1.x + w * cosf(r);
        br.y = p1.y + w * sinf(r);
        tr.x = p1.x - w * cosf(r);
        tr.y = p1.y - w * sinf(r);

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
    quad.bl.vertices.x = CCUtils::lerp(bl.x, br.x, verStartP);
    quad.bl.vertices.y = CCUtils::lerp(bl.y, br.y, verStartP);
    quad.br.vertices.x = CCUtils::lerp(bl.x, br.x, verEndP);
    quad.br.vertices.y = CCUtils::lerp(bl.y, br.y, verEndP);
    quad.tl.vertices.x = CCUtils::lerp(tl.x, tr.x, verStartP);
    quad.tl.vertices.y = CCUtils::lerp(tl.y, tr.y, verStartP);
    quad.tr.vertices.x = CCUtils::lerp(tl.x, tr.x, verEndP);
    quad.tr.vertices.y = CCUtils::lerp(tl.y, tr.y, verEndP);
    
    // save texcoords, bl and tl
    if(m_sprite->isTextureRectRotated()) {
        float blv = quad.bl.texCoords.v;
        float brv = quad.br.texCoords.v;
        float tlv = quad.tl.texCoords.v;
        float trv = quad.tr.texCoords.v;
        quad.bl.texCoords.v = CCUtils::lerp(blv, brv, texStartP);
        quad.tl.texCoords.v = CCUtils::lerp(tlv, trv, texStartP);
        quad.br.texCoords.v = CCUtils::lerp(blv, brv, texEndP);
        quad.tr.texCoords.v = CCUtils::lerp(tlv, trv, texEndP);
    } else {
        float blu = quad.bl.texCoords.u;
        float bru = quad.br.texCoords.u;
        float tlu = quad.tl.texCoords.u;
        float tru = quad.tr.texCoords.u;
        quad.bl.texCoords.u = CCUtils::lerp(blu, bru, texStartP);
        quad.tl.texCoords.u = CCUtils::lerp(tlu, tru, texStartP);
        quad.br.texCoords.u = CCUtils::lerp(blu, bru, texEndP);
        quad.tr.texCoords.u = CCUtils::lerp(tlu, tru, texEndP);
    }
    
    // add quad
    while(m_atlas->getTotalQuads() >= m_atlas->getCapacity())
        m_atlas->resizeCapacity(m_atlas->getCapacity() * 2);
    m_atlas->insertQuad(&quad, m_atlas->getTotalQuads());
}

void CCCatmullRomSprite::populatePoints(const CCPointList& controlPoints, CCPointList& points) {
    // clear
    points.clear();
    m_segmentPointIndices.clear();
    
    // populate points segment by segment
    int totalSeg = controlPoints.getCount() - 1;
    for(int curSeg = 0; curSeg < totalSeg; curSeg++) {
        // add start index of segment
        m_segmentPointIndices.push_back(points.getCount());
        
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
    
    // last placeholder
    m_segmentPointIndices.push_back(points.getCount());
}

void CCCatmullRomSprite::setTension(float t) {
    m_tension = t;
    m_dirty = true;
}

bool CCCatmullRomSprite::isSegmentVisible(int sIndex) {
    map<int, bool>::iterator iter = m_segmentVisibilities.find(sIndex);
    if(iter != m_segmentVisibilities.end())
        return iter->second;
    else
        return true;
}

void CCCatmullRomSprite::setSegmentVisible(int sIndex, bool visible) {
    if(!visible) {
        m_segmentVisibilities[sIndex] = visible;
        m_allVisible = false;
    } else {
        map<int, bool>::iterator iter = m_segmentVisibilities.find(sIndex);
        if(iter != m_segmentVisibilities.end()) {
            m_segmentVisibilities.erase(iter);
        }
        m_allVisible = m_segmentVisibilities.empty();
    }
}

void CCCatmullRomSprite::resetSegmentVisibility() {
    m_segmentVisibilities.clear();
    m_allVisible = true;
}

NS_CC_END