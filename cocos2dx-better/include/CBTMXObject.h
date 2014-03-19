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
#ifndef __CBTMXObject_h__
#define __CBTMXObject_h__

#include "cocos2d.h"
#include "CCPointList.h"
#include "CCMoreMacros.h"

using namespace std;

NS_CC_BEGIN

/**
 * Object in TMX object group
 *
 * \note
 * This is a re-implementation for TMX map. Cocos2d-x TMX support is defective, so I write my own. 
 * To avoid name conflict, I use CB prefix which stands for cocos2dx-better
 */
class CC_DLL CBTMXObject : public CCObject {
public:
	/// object type
	enum Type {
		/// Normal object, just a collection of key-value pairs
		NORMAL,
		
		/// Polygon object, can get vertex points
		POLYGON,
		
		/// Polyline object, first and last point is not connected
		POLYLINE
	};
	
protected:
	CBTMXObject();
	
public:
	virtual ~CBTMXObject();
	static CBTMXObject* create();
	
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
	
	/**
	 * A quick way to get object position
	 *
	 * @return position, in tmx map space. For orthogonal and hexagonal, the origin is top-left,
	 *		for isometic, the origin is the top rhombus
	 */
	CCPoint getPosition();
	
	/**
	 * A quick way to get object size, but not all objects have a size
	 *
	 * @return size of object
	 */
	CCSize getSize();
	
	/// property dict
	CC_SYNTHESIZE_PASS_BY_REF_NC(CCDictionary, m_properties, Properties);
	
	/// point list, only used for POLYGON or POLYLINE object
	CC_SYNTHESIZE_PASS_BY_REF_NC(CCPointList, m_points, Points);
	
	/// object type
	CC_SYNTHESIZE(Type, m_type, Type);
};

NS_CC_END

#endif // __CBTMXObject_h__
