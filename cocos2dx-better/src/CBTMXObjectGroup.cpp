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
#include "CBTMXObjectGroup.h"

NS_CC_BEGIN

CBTMXObjectGroup::CBTMXObjectGroup() :
m_offsetX(0),
m_offsetY(0),
m_color(0xffffffff),
m_opacity(1) {
}

CBTMXObjectGroup::~CBTMXObjectGroup() {
}

CBTMXObjectGroup* CBTMXObjectGroup::create() {
	CBTMXObjectGroup* g = new CBTMXObjectGroup();
	return (CBTMXObjectGroup*)g->autorelease();
}

void CBTMXObjectGroup::addProperty(const string& key, const string& value) {
	if(!m_properties.objectForKey(key)) {
		m_properties.setObject(CCString::create(value), key);
	}
}

string CBTMXObjectGroup::getProperty(const string& name) {
	CCString* p = (CCString*)m_properties.objectForKey(name);
	if(p)
		return p->getCString();
	else
		return "";
}

CBTMXObject* CBTMXObjectGroup::newObject() {
	CBTMXObject* to = CBTMXObject::create();
	m_objects.addObject(to);
    return to;
}

CBTMXObject* CBTMXObjectGroup::getObject(const string& name) {
	CCObject* obj;
	CCARRAY_FOREACH(&m_objects, obj) {
		CBTMXObject* to = (CBTMXObject*)obj;
		if(to->getName() == name)
			return to;
	}
	
	return NULL;
}

CBTMXObject* CBTMXObjectGroup::getObjectAt(int index) {
	if(index < 0 || index >= m_objects.count())
		return NULL;
	else
		return (CBTMXObject*)m_objects.objectAtIndex(index);
}

void CBTMXObjectGroup::setOpacity(float opacity) {
	m_opacity = MIN(1, MAX(0, opacity));
}

NS_CC_END