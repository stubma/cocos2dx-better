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
#include "CCSlider.h"

NS_CC_BEGIN

CCSlider::CCSlider() :
m_thumbSprite(NULL),
m_backgroundSprite(NULL),
m_progressSprite(NULL),
m_value(0),
m_intValue(0),
m_minimumValue(0),
m_maximumValue(1),
m_minimumAllowedValue(0),
m_maximumAllowedValue(1),
m_discreteMode(false) {
}

CCSlider::~CCSlider() {
    
}

CCSlider* CCSlider::create(CCSprite* bg, CCSprite* thumb, CCSprite* progress) {
    CCSlider* s = new CCSlider();
    if(s->initWithSprites(bg, thumb, progress)) {
        return (CCSlider*)s->autorelease();
    }
    
    s->release();
    return NULL;
}

bool CCSlider::initWithSprites(CCSprite* bg, CCSprite* thumb, CCSprite* progress) {
    if(!CCControl::init())
        return false;
    
    // add component
    m_backgroundSprite = bg;
    m_thumbSprite = thumb;
    m_progressSprite = progress;
    addChild(m_backgroundSprite);
    if(m_progressSprite)
        addChild(m_progressSprite, 1);
    addChild(m_thumbSprite, 2);
    
    // update
    updateLayout();
    
    // trigger first value change event
    setValue(m_minimumValue);
    
    // event
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    
    return true;
}

void CCSlider::setBackgroundSprite(CCSprite* bg) {
    CCAssert(bg != NULL, "Slider background can't be NULL");
    
    // remove old and add new
    if(m_backgroundSprite) {
        m_backgroundSprite->removeFromParent();
        m_backgroundSprite = NULL;
    }
    m_backgroundSprite = bg;
    addChild(m_backgroundSprite);
    
    // necessary update
    updateLayout();
    updateSpritesForValueChange();
}

void CCSlider::setThumbSprite(CCSprite* thumb) {
    CCAssert(thumb != NULL, "Slider thumb can't be NULL");
    
    // remove old and add new
    if(m_thumbSprite) {
        m_thumbSprite->removeFromParent();
        m_thumbSprite = NULL;
    }
    m_thumbSprite = thumb;
    addChild(m_backgroundSprite, 2);
    
    // necessary update
    updateLayout();
    updateSpritesForValueChange();
}

void CCSlider::setProgressSprite(CCSprite* progress) {
    // remove old and add new
    if(m_progressSprite) {
        m_progressSprite->removeFromParent();
        m_progressSprite = NULL;
    }
    m_progressSprite = progress;
    addChild(m_progressSprite, 1);
    
    // necessary update
    updateLayout();
    updateSpritesForValueChange();
}

void CCSlider::setValue(float v) {
    m_value = clampf(v, m_minimumValue, m_maximumValue);
    if(m_discreteMode) {
        int intValue = (int)(m_value + 0.5f);
        if(m_intValue != intValue) {
            m_intValue = intValue;
            m_value = intValue;
            updateSpritesForValueChange();
            sendActionsForControlEvents(CCControlEventValueChanged);
        }
    } else {
        m_intValue = (int)m_value;
        updateSpritesForValueChange();
        sendActionsForControlEvents(CCControlEventValueChanged);
    }
}

void CCSlider::setMinimumValue(float min) {
    m_minimumValue = min;
    m_minimumAllowedValue = min;
    if(m_minimumValue >= m_maximumValue) {
        m_maximumValue = m_minimumValue + 1.0f;
    }
    setValue(m_value);
}

void CCSlider::setMaximumValue(float max) {
    m_maximumValue = max;
    m_maximumAllowedValue = max;
    if(m_maximumValue <= m_minimumValue) {
        m_minimumValue = m_maximumValue - 1.0f;
    }
    setValue(m_value);
}

void CCSlider::updateLayout() {
    // self
    CCSize bgSize = m_backgroundSprite->getContentSize();
    CCSize thumbSize = m_thumbSprite->getContentSize();
    CCSize size = CCSizeMake(bgSize.width,
                             MAX(bgSize.height, thumbSize.height));
    setContentSize(size);
    setAnchorPoint(ccp(0.5f, 0.5f));
    ignoreAnchorPointForPosition(false);
    
    // background
    m_backgroundSprite->ignoreAnchorPointForPosition(false);
    m_backgroundSprite->setAnchorPoint(ccp(0.5f, 0.5f));
    m_backgroundSprite->setPosition(ccp(size.width / 2, size.height / 2));
    
    // the progress bar
    if(m_progressSprite) {
        m_progressSprite->ignoreAnchorPointForPosition(false);
        m_progressSprite->setAnchorPoint(ccp(0, 0.5f));
        m_progressSprite->setPosition(ccp(0, size.height / 2));
    }
    
    // thumb
    m_thumbSprite->ignoreAnchorPointForPosition(false);
    m_thumbSprite->setPosition(ccp(0.0f, size.height / 2));
}

void CCSlider::updateSpritesForValueChange() {
    // update thumb position for new value
    if(m_thumbSprite) {
        float percent = (m_value - m_minimumValue) / (m_maximumValue - m_minimumValue);
        CCPoint pos = m_thumbSprite->getPosition();
        pos.x = percent * m_backgroundSprite->getContentSize().width;
        m_thumbSprite->setPosition(pos);
        
        // Stretches content proportional to newLevel
        if(m_progressSprite) {
            CCRect textureRect = m_progressSprite->getTextureRect();
            textureRect = CCRectMake(textureRect.origin.x, textureRect.origin.y, pos.x, textureRect.size.height);
            m_progressSprite->setTextureRect(textureRect, m_progressSprite->isTextureRectRotated(), textureRect.size);
        }
    }
}

bool CCSlider::isTouchInside(CCTouch* touch) {
    // location in parent space
    CCPoint touchLocation = touch->getLocation();
    touchLocation = getParent()->convertToNodeSpace(touchLocation);
    
    // rect in parent space
    CCRect rect = boundingBox();
    
    // check contain or not
    return rect.containsPoint(touchLocation);
}

CCPoint CCSlider::locationFromTouch(CCTouch* touch) {
    // location in self space
    CCPoint touchLocation = touch->getLocation();
    touchLocation = convertToNodeSpace(touchLocation);
    
    // clamp
    touchLocation.x = clampf(touchLocation.x, 0, m_backgroundSprite->getContentSize().width);
    
    // return
    return touchLocation;
}

bool CCSlider::ccTouchBegan(CCTouch* touch, CCEvent* pEvent) {
    if(!isTouchInside(touch)) {
        return false;
    }
    
    CCPoint location = locationFromTouch(touch);
    sliderBegan(location);
    return true;
}

void CCSlider::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent) {
    CCPoint location = locationFromTouch(pTouch);
    sliderMoved(location);
}

void CCSlider::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent) {
    sliderEnded(CCPointZero);
}

void CCSlider::ccTouchCancelled(CCTouch* pTouch, CCEvent* pEvent) {
    sliderEnded(CCPointZero);
}

float CCSlider::valueForLocation(CCPoint location) {
    float percent = location.x / m_backgroundSprite->getContentSize().width;
    return MAX(MIN(m_minimumValue + percent * (m_maximumValue - m_minimumValue), m_maximumAllowedValue),
               m_minimumAllowedValue);
}

void CCSlider::sliderBegan(CCPoint location) {
    setSelected(true);
    getThumbSprite()->setColor(ccGRAY);
    setValue(valueForLocation(location));
}

void CCSlider::sliderMoved(CCPoint location) {
    setValue(valueForLocation(location));
}

void CCSlider::sliderEnded(CCPoint location) {
    if (isSelected()) {
        setValue(valueForLocation(m_thumbSprite->getPosition()));
    }
    getThumbSprite()->setColor(ccWHITE);
    setSelected(false);
}

NS_CC_END