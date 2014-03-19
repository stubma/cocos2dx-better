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
#ifndef __CBTMXObjectGroup_h__
#define __CBTMXObjectGroup_h__

#include "CBTMXObject.h"

using namespace std;

NS_CC_BEGIN

/**
 * Object group in TMX map, also known as object layer
 *
 * \note
 * This is a re-implementation for TMX map. Cocos2d-x TMX support is defective, so I write my own.
 * To avoid name conflict, I use CB prefix which stands for cocos2dx-better
 */
class CC_DLL CBTMXObjectGroup : public CCObject {
protected:
	CBTMXObjectGroup();
	
public:
	virtual ~CBTMXObjectGroup();
	static CBTMXObjectGroup* create();
	
	/**
	 * Add property to object group
	 *
	 * @param key property name
	 * @param value property value
	 */
	void addProperty(const string& key, const string& value);
	
	/**
	 * Query property of object group
	 *
	 * @param name property name
	 * @return property value, or NULL if not found
	 */
	string getProperty(const string& name);
	
	/**
	 * Create a new CBTMXObject
	 *
	 * @return CBTMXObject instance, which is already added into object array
	 */
	CBTMXObject* newObject();
	
	/**
	 * Get object by name
	 *
	 * @param name object name
	 * @return CBTMXObject instance, or NULL if not found
	 */
	CBTMXObject* getObject(const string& name);
	
	/**
	 * Get object at specified index. If an object doesn't have a name, we can get it
	 * by index. The index increases from zero and follow the order object appears in TMX file
	 *
	 * @param index index of object
	 * @return CBTMXObject instance, or NULL if index is invalid
	 */
	CBTMXObject* getObjectAt(int index);
	
	/**
	 * Get object count in object group
	 *
	 * @return object count
	 */
	int getObjectCount() { return m_objects.count(); }
	
	/// name
	CC_SYNTHESIZE(string, m_name, Name);
	
	/// x offset of object group
	CC_SYNTHESIZE(float, m_offsetX, OffsetX);
	
	/// y offset of object group
	CC_SYNTHESIZE(float, m_offsetY, OffsetY);
	
	/// object list, every object is a CBTMXObject instance
	CC_SYNTHESIZE_PASS_BY_REF(CCArray, m_objects, Objects);
	
	/// property dict
	CC_SYNTHESIZE_PASS_BY_REF_NC(CCDictionary, m_properties, Properties);
	
	/// color of object group, in argb8888 format, but alpha is always 0xff
	CC_SYNTHESIZE(int, m_color, Color);
	
	/// opacity of object group, from 0 to 1
	CC_SYNTHESIZE_SETTER(float, m_opacity, Opacity);
};

NS_CC_END

#endif // __CBTMXObjectGroup_h__
