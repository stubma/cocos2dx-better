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
#ifndef __CCLayerMultiplexEx__
#define __CCLayerMultiplexEx__

#include "cocos2d.h"

NS_CC_BEGIN

/**
 * In general, class with Ex suffix is my workaround for cocos2d design defect
 * This class is a new implementation for CCLayerMultiplex and it use setVisible
 * to control sub layer visibility. Original class use addChild/removeChild and that
 * is defective because sub layer may lose onExit event. Maybe you don't think losing
 * onExit matters but in some situation it causes memory leak.
 */
class CC_DLL CCLayerMultiplexEx : public CCLayer {
protected:
    unsigned int m_nEnabledLayer;
    CCArray*     m_pLayers;
	
public:
    /**
     * @js ctor
     * @lua NA
     */
    CCLayerMultiplexEx();
	
    /**
     * @js NA
     * @lua NA
     */
    virtual ~CCLayerMultiplexEx();
	
    /**
     * @js NA
     */
    static CCLayerMultiplexEx* create();
    
    /** creates a CCMultiplexLayer with an array of layers.
     * @since v2.1
     * @js NA
     */
    static CCLayerMultiplexEx* createWithArray(CCArray* arrayOfLayers);
	
    /** creates a CCLayerMultiplex with one or more layers using a variable argument list.
     * @lua NA
     */
    static CCLayerMultiplexEx* create(CCLayer* layer, ... );
	
    /**
     * lua script can not init with undetermined number of variables
     * so add these functions to be used with lua.
     */
    static CCLayerMultiplexEx* createWithLayer(CCLayer* layer);
	
    void addLayer(CCLayer* layer);
	
    /** initializes a MultiplexLayer with one or more layers using a variable argument list.
     *  @js NA
     *  @lua NA
     */
    bool initWithLayers(CCLayer* layer, va_list params);
	
    /** initializes a CCMultiplexLayer with an array of layers
     *  @since v2.1
     *  @lua NA
     */
    bool initWithArray(CCArray* arrayOfLayers);
	
    /** switches to a certain layer indexed by n.
	 The current (old) layer will be removed from it's parent with 'cleanup:YES'.
	 */
    void switchTo(unsigned int n);
    
    /// get layer at specified index
    CCLayer* layerAt(int n);

	// get visible layer index
	int getEnabledLayer() { return m_nEnabledLayer;	}
};

NS_CC_END

#endif /* defined(__CCLayerMultiplexEx__) */
