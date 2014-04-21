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
#ifndef __CCRookieGuide__
#define __CCRookieGuide__

#include "cocos2d.h"
#include "CCMoreMacros.h"
#include "CBDrawNode.h"

using namespace std;

NS_CC_BEGIN

/**
 * It provides basic framework to define a rookie guide UI. You should
 * subclass it to add visual element. The base class takes care of touch
 * event and general UI rendering.
 * 
 * \par
 * A rookie guide layer will have preset clickable regions which are totally
 * transparent.
 */
class CC_DLL CCRookieGuide : public CCLayer {
protected:
    typedef struct {
        CCRect r;
        CCCallFunc* func;
        
        // only valid when func is NULL
        bool removeOnTouch;
    } Region;
    typedef vector<Region> RegionList;
    
private:
    /// clicked region
    Region* m_clickedRegion;
    
protected:
    CCRookieGuide();
    
    /**
     * only invoked when you call fadeIn, subclass can override it for
     * custom behavior
     */
    virtual void onFadeInDone();
    
public:
    virtual ~CCRookieGuide();
    
    static CCRookieGuide* create();
    
    /// override super
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    /**
     * fade in whole rookie layer, this method should be invoked after
     * bg color is set
     *
     * @param duration fade in action duration time
     * @param delay delay time before fade in action, default is zero
     */
    virtual void fadeIn(float duration, float delay = 0);
    
    /**
     * add a clickable region, the region must in world space. 
     *
     * \note
     * When a region is clickable , the rookie guide layer will be auto
     * removed after it is clicked
     *
     * @param r region rectangle
     * @param func function to be invoked when region is clicked. This 
     *      can be NULL if no special action need to be performed. When
     *      it is NULL, this region is not clickable.
     * @param removeOnTouch true means guide layer will be removed on touch began
     *      this flag is valid only when function is NULL
     */
    void addRegion(const CCRect& r, CCCallFunc* func, bool removeOnTouch = false);
    
    /**
     * add a clickable region by a node, the node's global bounding box
     * will be the region rectangle
     *
     * \note
     * When a region is clickable , the rookie guide layer will be auto
     * removed after it is clicked
     *
     * @param n node to be the region
     * @param func function to be invoked when region is clicked. This
     *      can be NULL if no special action need to be performed. When
     *      it is NULL, this region is not clickable.
     * @param removeOnTouch true means guide layer will be removed on touch began
     *      this flag is valid only when function is NULL
     */
    void addRegion(CCNode* n, CCCallFunc* func, bool removeOnTouch = false);
    
    /// set a region removed on touch, that will clear its function if has
    void setRegionRemoveOnTouch(int index);
	
	/// get rectangle of region at given index
	const CCRect& getRegionRect(int index);
    
    /// disable region check until time passed
    void enableRegionCheckAfter(float seconds);
    
    /// clickable area
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(RegionList, m_regions, Regions);
    
    /// bg color
    CC_SYNTHESIZE_PASS_BY_REF_SETTER(ccColor4B, m_bgColor, BgColor);
    
    /// any touch mode, means touch anywhere can end the guide
    CC_SYNTHESIZE_BOOL(m_anyTouchMode, AnyTouchMode);
	
	/// true means no region checking, that will eat all touch event
	CC_SYNTHESIZE_BOOL(m_shouldCheckRegion, ShouldCheckRegion);
	
	/// clipper
	CC_SYNTHESIZE(CCClippingNode*, m_clipper, Clipper);
	
	/// content layer
	CC_SYNTHESIZE(CCLayerColor*, m_content, Content);
	
	/// stencil node
	CC_SYNTHESIZE(CBDrawNode*, m_stencil, Stencil);
    
    /// function invoked on enter and exit
    CC_SYNTHESIZE_RETAIN(CCCallFunc*, m_onExitFunc, OnExitFunc);
    CC_SYNTHESIZE_RETAIN(CCCallFunc*, m_onEnterFunc, OnEnterFunc);
};

NS_CC_END

#endif /* defined(__CCRookieGuide__) */
