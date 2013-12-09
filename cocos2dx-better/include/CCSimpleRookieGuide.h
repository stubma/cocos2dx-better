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
#ifndef __CCSimpleRookieGuide__
#define __CCSimpleRookieGuide__

#include "cocos2d.h"
#include "CCRookieGuide.h"

NS_CC_BEGIN

/**
 * A simple rookie guide customization. It just show a label and an arrow
 */
class CC_DLL CCSimpleRookieGuide : public CCRookieGuide {
protected:
    CCSimpleRookieGuide();
    
public:
    virtual ~CCSimpleRookieGuide();
    
    static CCSimpleRookieGuide* create();
    
    // override super
    virtual void fadeIn(float duration, float delay = 0);
    
    /**
     * align arrow to a region center, with specified distance and angle
     *
     * @param regionIndex index of region
     * @param distance distance between region center and arrow sprite center
     * @param degree degree of the vector whose origin is region center and point 
     *      to arrow center. Positive value is counter-clockwise
     * @param arrowPresetRotation take an rightward arrow as 0 degree, the preset degree of your
     *      arrow sprite. Positive value is counter-clockwise
     */
    void pointToRegionCenter(int regionIndex, float distance, float degree, float arrowPresetRotation = 0);
    
    /// place hint relative to arrow, relative to arrow center
    void shiftHint(float dx, float dy);
    
    /// arrow node
    CC_SYNTHESIZE_SETTER(CCSprite*, m_arrow, Arrow);
    CC_SYNTHESIZE_SETTER(CCNode*, m_hint, Hint);
};

NS_CC_END

#endif /* defined(__CCSimpleRookieGuide__) */
