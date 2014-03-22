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
#ifndef __CBTMXLayerInfo_h__
#define __CBTMXLayerInfo_h__

#include "cocos2d.h"
#include "CCMoreMacros.h"

using namespace std;

NS_CC_BEGIN

/**
 * Info of a TMX layer
 *
 * \note
 * This is a re-implementation for TMX map. Cocos2d-x TMX support is defective, so I write my own.
 * To avoid name conflict, I use CB prefix which stands for cocos2dx-better
 */
class CC_DLL CBTMXLayerInfo : public CCObject {
protected:
	CBTMXLayerInfo();
	
public:
	virtual ~CBTMXLayerInfo();
	static CBTMXLayerInfo* create();
	
	/**
	 * Get a property value by a key
	 *
	 * @param key key string
	 * @return value string, or empty string if not found
	 */
	string getProperty(const string& key);
	
	/**
	 * Add a property
	 *
	 * @param key key string
	 * @param value value string
	 */
	void addProperty(const string& key, const string& value);
	
	/// layer name
	CC_SYNTHESIZE(string, m_name, Name);
	
	/// tile width of layer
	CC_SYNTHESIZE(int, m_layerWidth, LayerWidth);
	
	/// tile height of layer
	CC_SYNTHESIZE(int, m_layerHeight, LayerHeight);
	
	/// tile gid array
	CC_SYNTHESIZE(int*, m_tiles, Tiles);
	
	/// is layer visible
	CC_SYNTHESIZE_BOOL(m_visible, Visible);
	
	/// layer alpha
	CC_SYNTHESIZE(int, m_alpha, Alpha);
	
	/// property dict
	CC_SYNTHESIZE_PASS_BY_REF_NC(CCDictionary, m_properties, Properties);
	
	/// x offset
	CC_SYNTHESIZE(float, m_offsetX, OffsetX);
	
	/// y offset
	CC_SYNTHESIZE(float, m_offsetY, OffsetY);
};

NS_CC_END

#endif // __CBTMXLayerInfo_h__
