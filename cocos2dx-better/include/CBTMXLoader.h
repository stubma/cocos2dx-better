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
#ifndef __CBTMXLoader__
#define __CBTMXLoader__

#include "cocos2d.h"

using namespace std;

NS_CC_BEGIN

class CBTMXMapInfo;

/// xml parser of tmx file
class CC_DLL CBTMXLoader : public CCObject, public CCSAXDelegator {
private:
	/// tag of tmx file
	typedef enum {
		UNKNOWN,
		MAP,
		TILESET,
		TILE,
		IMAGE,
		LAYER,
		DATA,
		PROPERTIES,
		PROPERTY,
		OBJECTGROUP,
		OBJECT,
		POLYLINE,
		POLYGON
	} TMXTag;
	
	// attr name of tmx tag
	typedef enum {
		ATTR_UNKNOWN,
		ATTR_VERSION,
		ATTR_ORIENTATION,
		ATTR_WIDTH,
		ATTR_HEIGHT,
		ATTR_TILEWIDTH,
		ATTR_TILEHEIGHT,
		ATTR_NAME,
		ATTR_FIRSTGID,
		ATTR_SPACING,
		ATTR_MARGIN,
		ATTR_VISIBLE,
		ATTR_OPACITY,
		ATTR_X,
		ATTR_Y,
		ATTR_TYPE,
		ATTR_COLOR,
		ATTR_POINTS
	} TMXAttr;
	
private:
	/// tmx dir
	string m_tmxDir;
	
	/// map
	CBTMXMapInfo* m_map;
	
	/// tag stack
	vector<TMXTag> m_tags;
	
	/// last gid
	int m_lastGid;
	
	/// compressed
	bool m_compressed;
	
private:
	/// internal load method, return false if failed
	bool load(const char* data, int length);
	
	// tag operation
	void pushTag(TMXTag tag);
	void popTag();
	TMXTag topTag();
	TMXTag parentTag();
	TMXTag grandpaTag();
	TMXTag getTag(const char* name);
	TMXAttr getAttr(const char* name);
	
	// find a attribute value
	const char* valueForKey(const char** atts, const char* name);
	
protected:
	CBTMXLoader();
	
public:
	virtual ~CBTMXLoader();
	static CBTMXLoader* create();
	
	// override super
	virtual void startElement(void* ctx, const char* name, const char** atts);
    virtual void endElement(void* ctx, const char* name);
    virtual void textHandler(void* ctx, const char* s, int len);
	
	/// load a tmx map from a tmx file relative path, return NULL if failed
	CBTMXMapInfo* load(const string& tmxFile);
	
	/// load a tmx map from a tmx raw data, so you must specify a resource base path, return NULL if failed
	CBTMXMapInfo* load(const char* tmxData, int length, const string& resourcePath);
};

NS_CC_END

#endif /* defined(__CBTMXLoader__) */
