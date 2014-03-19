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
#ifndef __CBLayerMultiplex__
#define __CBLayerMultiplex__

#include "cocos2d.h"

NS_CC_BEGIN

/**
 * This class is a new implementation for CCLayerMultiplex and it use setVisible
 * to control sub layer visibility. Original class use addChild/removeChild and that
 * is defective because sub layer may lose onExit event. Maybe you don't think losing
 * onExit matters but in some situation it causes memory leak.
 *
 * \note
 * Sometimes I find defect in cocos2d-x and want to create a new class to workaround it,
 * so I will choose CB prefix instead of CC. I use Ex suffix before but I don't like it anymore.
 * CB stands cocos2dx-better.
 */
class CC_DLL CBLayerMultiplex : public CCLayer {
protected:
    unsigned int m_nEnabledLayer;
    CCArray*     m_pLayers;
	
public:
    /**
     * @js ctor
     * @lua NA
     */
    CBLayerMultiplex();
	
    /**
     * @js NA
     * @lua NA
     */
    virtual ~CBLayerMultiplex();
	
    /**
     * @js NA
     */
    static CBLayerMultiplex* create();
    
    /** creates a CCMultiplexLayer with an array of layers.
     * @since v2.1
     * @js NA
     */
    static CBLayerMultiplex* createWithArray(CCArray* arrayOfLayers);
	
    /** creates a CCLayerMultiplex with one or more layers using a variable argument list.
     * @lua NA
     */
    static CBLayerMultiplex* create(CCLayer* layer, ... );
	
    /**
     * lua script can not init with undetermined number of variables
     * so add these functions to be used with lua.
     */
    static CBLayerMultiplex* createWithLayer(CCLayer* layer);
	
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

#endif /* defined(__CBLayerMultiplex__) */
