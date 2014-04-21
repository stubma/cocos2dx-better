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
#ifndef __CBDrawNode__
#define __CBDrawNode__

#include "cocos2d.h"

NS_CC_BEGIN

/**
 * to fix:
 * 1. vbo or vao has compatibility issue on some android model, so just use basic render method
 */
class CC_DLL CBDrawNode : public CCNode {
protected:    
    unsigned int    m_uBufferCapacity;
    GLsizei         m_nBufferCount;
    ccV2F_C4B_T2F   *m_pBuffer;
    
    ccBlendFunc     m_sBlendFunc;
    
    bool            m_bDirty;
    
public:
    static CBDrawNode* create();
    virtual ~CBDrawNode();
    
    virtual bool init();
    virtual void draw();
    
    /** draw a dot at a position, with a given radius and color */
    void drawDot(const CCPoint &pos, float radius, const ccColor4F &color);
    
    /** draw a segment with a radius and color */
    void drawSegment(const CCPoint &from, const CCPoint &to, float radius, const ccColor4F &color);
    
    /** draw a polygon with a fill color and line color
     * @code
     * when this funciton bound to js,the input params are changed
     * js:var drawPolygon(var verts, var fillColor,var borderWidth,var borderColor)
     * @endcode
     */
    void drawPolygon(CCPoint *verts, unsigned int count, const ccColor4F &fillColor, float borderWidth, const ccColor4F &borderColor);
    
    /** Clear the geometry in the node's buffer. */
    void clear();
    /**
     * @js NA
     */
    ccBlendFunc getBlendFunc() const;
    /**
     * @code
     * when this function bound to js ,the input param is change
     * js:var setBlendFunc(var src,var dst)
     * @endcode
     */
    void setBlendFunc(const ccBlendFunc &blendFunc);
    
    CBDrawNode();
    
    /** listen the event that coming to foreground on Android
     * @js NA
     */
    void listenBackToForeground(CCObject *obj);
private:
    void ensureCapacity(unsigned int count);
    void render();
};

NS_CC_END

#endif /* defined(__CBDrawNode__) */
