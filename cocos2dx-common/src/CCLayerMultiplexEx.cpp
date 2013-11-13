/****************************************************************************
 Author: Luma (stubma@gmail.com)
 
 https://github.com/stubma/cocos2dx-common
 
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
#include "CCLayerMultiplexEx.h"

NS_CC_BEGIN

CCLayerMultiplexEx::CCLayerMultiplexEx() :
m_nEnabledLayer(0),
m_pLayers(NULL) {
}

CCLayerMultiplexEx::~CCLayerMultiplexEx() {
	CC_SAFE_RELEASE(m_pLayers);
}

CCLayerMultiplexEx* CCLayerMultiplexEx::create(CCLayer * layer, ...) {
    va_list args;
    va_start(args, layer);
	
    CCLayerMultiplexEx* pMultiplexLayer = new CCLayerMultiplexEx();
    if(pMultiplexLayer && pMultiplexLayer->initWithLayers(layer, args))
    {
        pMultiplexLayer->autorelease();
        va_end(args);
        return pMultiplexLayer;
    }
	
    va_end(args);
    CC_SAFE_DELETE(pMultiplexLayer);
    return NULL;
}

CCLayerMultiplexEx* CCLayerMultiplexEx::createWithLayer(CCLayer* layer) {
    return CCLayerMultiplexEx::create(layer, NULL);
}

CCLayerMultiplexEx* CCLayerMultiplexEx::create() {
    CCLayerMultiplexEx* pRet = new CCLayerMultiplexEx();
    if (pRet && pRet->init()) {
        pRet->autorelease();
    } else {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

CCLayerMultiplexEx* CCLayerMultiplexEx::createWithArray(CCArray* arrayOfLayers)
{
    CCLayerMultiplexEx* pRet = new CCLayerMultiplexEx();
    if (pRet && pRet->initWithArray(arrayOfLayers)) {
        pRet->autorelease();
    } else {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

void CCLayerMultiplexEx::addLayer(CCLayer* layer) {
    CCAssert(m_pLayers, "");
    m_pLayers->addObject(layer);
	addChild(layer);
	layer->setVisible(false);
}

bool CCLayerMultiplexEx::initWithLayers(CCLayer *layer, va_list params) {
    if (CCLayer::init()) {
        m_pLayers = CCArray::createWithCapacity(5);
        m_pLayers->retain();
        m_pLayers->addObject(layer);
		addChild(layer);
		layer->setVisible(false);
		
        CCLayer* l = va_arg(params,CCLayer*);
        while(l) {
            m_pLayers->addObject(l);
			addChild(l);
			l->setVisible(false);
            l = va_arg(params, CCLayer*);
        }
		
        m_nEnabledLayer = 0;
		((CCNode*)m_pLayers->objectAtIndex(m_nEnabledLayer))->setVisible(true);
		
        return true;
    }
	
    return false;
}

bool CCLayerMultiplexEx::initWithArray(CCArray* arrayOfLayers) {
    if (CCLayer::init()) {
        m_pLayers = CCArray::createWithCapacity(arrayOfLayers->count());
        m_pLayers->addObjectsFromArray(arrayOfLayers);
        m_pLayers->retain();
		
		CCObject* obj;
        CCARRAY_FOREACH(m_pLayers, obj) {
			CCNode* node = (CCNode*)obj;
			addChild(node);
			node->setVisible(false);
		}
		
		m_nEnabledLayer = 0;
		((CCNode*)m_pLayers->objectAtIndex(m_nEnabledLayer))->setVisible(true);
		
        return true;
    }
    return false;
}

void CCLayerMultiplexEx::switchTo(unsigned int n) {
    CCAssert( n < m_pLayers->count(), "Invalid index in MultiplexLayer switchTo message" );
	
    ((CCNode*)m_pLayers->objectAtIndex(m_nEnabledLayer))->setVisible(false);
	
    m_nEnabledLayer = n;
	
    ((CCNode*)m_pLayers->objectAtIndex(n))->setVisible(true);
}

CCLayer* CCLayerMultiplexEx::layerAt(int n) {
    CCAssert( n < m_pLayers->count(), "Invalid index in MultiplexLayer layerAt message" );
    return (CCLayer*)m_pLayers->objectAtIndex(n);
}

NS_CC_END