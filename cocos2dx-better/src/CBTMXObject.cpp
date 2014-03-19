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
#include "CBTMXObject.h"

NS_CC_BEGIN

CBTMXObject::CBTMXObject() :
m_type(NORMAL) {
}

CBTMXObject::~CBTMXObject() {
}

CBTMXObject* CBTMXObject::create() {
	CBTMXObject* o = new CBTMXObject();
	return (CBTMXObject*)o->autorelease();
}

string CBTMXObject::getProperty(const string& key) {
	CCString* p = (CCString*)m_properties.objectForKey(key);
	return p->getCString();
}

CCPoint CBTMXObject::getPosition() {
	string xs = getProperty("x");
	string ys = getProperty("y");
	CCPoint p = CCPointZero;
	if(!xs.empty())
		sscanf(xs.c_str(), "%f", &p.x);
	if(!ys.empty())
		sscanf(ys.c_str(), "%f", &p.y);
	return p;
}

CCSize CBTMXObject::getSize() {
	string ws = getProperty("width");
	string hs = getProperty("height");
	CCSize s = CCSizeZero;
	if(!ws.empty())
		sscanf(ws.c_str(), "%f", &s.width);
	if(!hs.empty())
		sscanf(hs.c_str(), "%f", &s.height);
	return s;
}

void CBTMXObject::addProperty(const string& key, const string& value) {
	if(!m_properties.objectForKey(key)) {
		m_properties.setObject(CCString::create(value), key);
	}
}

NS_CC_END