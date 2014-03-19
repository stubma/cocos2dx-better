/*
 * Copyright (c) 2010 WiYun Inc.
 * Author: luma(stubma@gmail.com)
 *
 * For all entities this program is free software; you can redistribute
 * it and/or modify it under the terms of the 'WiEngine' license with
 * the additional provision that 'WiEngine' must be credited in a manner
 * that can be be observed by end users, for example, in the credits or during
 * start up. (please find WiEngine logo in sdk's logo folder)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "CBTMXLayerInfo.h"

NS_CC_BEGIN

CBTMXLayerInfo::CBTMXLayerInfo() :
m_tiles(NULL) {
}

CBTMXLayerInfo::~CBTMXLayerInfo() {
	CC_SAFE_FREE(m_tiles);
}

CBTMXLayerInfo* CBTMXLayerInfo::create() {
	CBTMXLayerInfo* li = new CBTMXLayerInfo();
	return (CBTMXLayerInfo*)li->autorelease();
}

void CBTMXLayerInfo::addProperty(const string& key, const string& value) {
	if(!m_properties.objectForKey(key)) {
		m_properties.setObject(CCString::create(value), key);
	}
}

string CBTMXLayerInfo::getProperty(const string& key) {
	CCString* p = (CCString*)m_properties.objectForKey(key);
	if(p)
		return p->getCString();
	else
		return "";
}

NS_CC_END
