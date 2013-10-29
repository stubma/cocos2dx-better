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

CCLayerMultiplexEx::CCLayerMultiplexEx() {
}

CCLayerMultiplexEx::~CCLayerMultiplexEx() {
}

CCLayerMultiplexEx * CCLayerMultiplexEx::create(CCLayer * layer, ...) {
    va_list args;
    va_start(args,layer);
	
    CCLayerMultiplexEx * pMultiplexLayer = new CCLayerMultiplexEx();
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
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

CCLayerMultiplexEx* CCLayerMultiplexEx::createWithArray(CCArray* arrayOfLayers) {
    CCLayerMultiplexEx* pRet = new CCLayerMultiplexEx();
    if (pRet && pRet->initWithArray(arrayOfLayers))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

void CCLayerMultiplexEx::switchToLayerAt(unsigned int n, bool cleanup) {
	CCAssert( n < m_pLayers->count(), "Invalid index in MultiplexLayer switchTo message" );
	
    this->removeChild((CCNode*)m_pLayers->objectAtIndex(m_nEnabledLayer), cleanup);
	
    m_nEnabledLayer = n;
	
    this->addChild((CCNode*)m_pLayers->objectAtIndex(n));
}

CCLayer* CCLayerMultiplexEx::layerAt(int n) {
    CCAssert( n < m_pLayers->count(), "Invalid index in MultiplexLayer layerAt message" );
    return (CCLayer*)m_pLayers->objectAtIndex(n);
}

NS_CC_END