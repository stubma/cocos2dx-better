/****************************************************************************
Copyright (c) 2013 Artavazd Barseghyan

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

#include "CCPinchGestureRecognizer.h"

NS_CC_BEGIN

bool CCPinchGestureRecognizer::init()
{
    touchNumber = 0;
    lastDistance = 0;
    
    touches = CCArray::create();
    touches->retain();
    
    return true;
}

CCPinchGestureRecognizer::~CCPinchGestureRecognizer()
{
    touches->release();
}

bool CCPinchGestureRecognizer::ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent)
{
    if (isRecognizing || !isPositionBetweenBounds(pTouch->getLocation())) {
        return false;
    }
    
    touchNumber++;
    touches->addObject(pTouch);
    
    //start recognizing after that 2 fingers are touching
    if (touchNumber==2) {
        isRecognizing = true;
    }
    
    return true;
}

void CCPinchGestureRecognizer::ccTouchMoved(CCTouch * pTouch, CCEvent * pEvent)
{
    if (!isRecognizing) {
        return;
    }
    
    CCTouch * touch1 = (CCTouch*)touches->objectAtIndex(0);
    CCTouch * touch2 = (CCTouch*)touches->objectAtIndex(1);
    CCPoint delta1 = touch1->getDelta();
    CCPoint delta2 = touch2->getDelta();
    
    if ((fabs(delta1.x)<kPinchThreshold && fabs(delta1.y)<kPinchThreshold) || (fabs(delta2.x)<kPinchThreshold && fabs(delta2.y)<kPinchThreshold)) {
        return;
    }
    
    float distance = distanceBetweenPoints(touch1->getLocation(), touch2->getLocation());
    if (!lastDistance) {
        lastDistance = distance;
        return;
    }
    
    CCPinch * pinch = CCPinch::create();
    
    //decide type of pinch
    if (lastDistance<=distance) {
        pinch->type = kPinchGestureRecognizerTypeOpen;
    }
    else {
        pinch->type = kPinchGestureRecognizerTypeClose;
    }
    
    if ((delta1.x>0 && delta2.x<0) || (delta1.x<0 && delta2.x>0) || (delta1.y>0 && delta2.y<0) || (delta1.y<0 && delta2.y>0)) {
        gestureRecognized(pinch);
    }
}

void CCPinchGestureRecognizer::ccTouchEnded(CCTouch * pTouch, CCEvent * pEvent)
{
    isRecognizing = false;
    lastDistance = 0;
    touchNumber--;
    touches->removeObject(pTouch);
    
    //cancel touch over other views if necessary
    if (cancelsTouchesInView) {
        stopTouchesPropagation(createSetWithTouch(pTouch), pEvent);
    }
}

NS_CC_END