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
#ifndef __CCCatmullRomSprite_h__
#define __CCCatmullRomSprite_h__

#include "cocos2d.h"
#include "CCPointList.h"

using namespace std;

NS_CC_BEGIN

class CBTextureAtlas;

/**
 * A node which can render a image pattern as a strip, the strip can be curve in
 * in any shape. The strip can be described by a series of control points.
 *
 * \note
 * Because it is a curve, so this sprite has a zero content size. However you can
 * get its point list to calculate its minimum bounding box
 *
 * \note
 * When use texture in an atlas, you should add 1 pixel Extrude. TexturePacker
 * supports Extrude option.
 */
class CC_DLL CCCatmullRomSprite : public CCNodeRGBA, public CCBlendProtocol {
private:
	/// dirty flag
	bool m_dirty;
	
	/// atlas
	CBTextureAtlas* m_atlas;
    
    /// control point list
    CCPointList m_controlPoints;
    
    /// point list
    CCPointList m_points;
    
    /// curve tension
    float m_tension;
    
    /// pattern length
    float m_patternLength;
    
    /// pattern width
    float m_patternWidth;
    
    /// segment point start index
    vector<int> m_segmentPointIndices;
    
    /// segment quad start index
    vector<int> m_segmentQuadIndices;
    
    /// segment visible flag list
    map<int, bool> m_segmentVisibilities;
    
    /// true means all segments are visible
    bool m_allVisible;
	
private:
	/// update atlas
	void updateAtlas();
    
    /// populate all points from control points
    void populatePoints(const CCPointList& controlPoints, CCPointList& points);
    
    /// populate a quad
    void populateQuad(const CCPoint& bl, const CCPoint& br, const CCPoint& tl, const CCPoint& tr, float texStartP, float texEndP, float verStartP, float verEndP);
	
public:
	CCCatmullRomSprite(CCSprite* sprite);
	virtual ~CCCatmullRomSprite();
	
	static CCCatmullRomSprite* create(const char* fileImage);
	static CCCatmullRomSprite* createWithTexture(CCTexture2D* tex);
	static CCCatmullRomSprite* createWithSpriteFrameName(const char* name);
	static CCCatmullRomSprite* createWithSpriteFrame(CCSpriteFrame* frame);
	static CCCatmullRomSprite* createWithSprite(CCSprite* sprite);
	
    // override super
	virtual void setOpacity(GLubyte opacity);
    inline void setBlendFunc(ccBlendFunc blendFunc) { m_sprite->setBlendFunc(blendFunc); }
    inline ccBlendFunc getBlendFunc(void) { return m_sprite->getBlendFunc(); }
	virtual void draw();
    
    /**
     * add a control point to point list, cause sprite redraw
     * 
     * @param p point to be added
     */
    void addControlPoint(const CCPoint& p);
    
    /**
     * set a new point list, cause sprite redraw
     * 
     * @param pa point list
     */
    void setControlPoints(CCPointArray& pa);
    
    /// get control point count
    int getControlPointCount() { return m_controlPoints.getCount(); }
    
    /// get segment count
    int getSegmentCount() { return MAX(0, getControlPointCount() - 1); }
    
    /// set segment visibility
    void setSegmentVisible(int sIndex, bool visible);
    
    /// is a segment visible? if index is invalid, it returns true
    bool isSegmentVisible(int sIndex);
    
    /// clear segment visibility flag, set them all visible
    void resetSegmentVisibility();
    
    /// set tension of curve, the bigger it is, the more flat the curve is
    void setTension(float t);
	
	/// get internal sprite
	CC_SYNTHESIZE_READONLY(CCSprite*, m_sprite, PatternSprite);
};

NS_CC_END

#endif // __CCCatmullRomSprite_h__