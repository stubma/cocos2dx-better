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

#include "CCLongPressGestureRecognizer.h"

NS_CC_BEGIN

bool CCLongPressGestureRecognizer::init()
{
    isRecognizing = false;
    currTouch = NULL;
    currEvent = NULL;
    
    setMinimumPressDuration(kLongPressMinDuration);
    
    return true;
}

CCLongPressGestureRecognizer::~CCLongPressGestureRecognizer()
{
    
}

void CCLongPressGestureRecognizer::timerDidEnd(float dt)
{
    CCLongPress * longPress = CCLongPress::create();
    longPress->location = currLocation;
    
    gestureRecognized(longPress);
    if (cancelsTouchesInView) stopTouchesPropagation(createSetWithTouch(currTouch), currEvent); //cancel touch over other views
    
    stopGestureRecognition();
}

bool CCLongPressGestureRecognizer::ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent)
{
    if (isRecognizing) {
        stopGestureRecognition();
        return false;
    }
    
    currLocation = pTouch->getLocation();
    if (!isPositionBetweenBounds(currLocation)) return false;
    
    currEvent = pEvent;
    currTouch = pTouch;
    
    schedule(schedule_selector(CCLongPressGestureRecognizer::timerDidEnd), minimumPressDuration);
    
    isRecognizing = true;
    return true;
}

void CCLongPressGestureRecognizer::ccTouchEnded(CCTouch * pTouch, CCEvent * pEvent)
{
    stopGestureRecognition();
}

void CCLongPressGestureRecognizer::stopGestureRecognition()
{
    if (!isRecognizing) return;
    
    currTouch = NULL;
    currEvent = NULL;
    unschedule(schedule_selector(CCLongPressGestureRecognizer::timerDidEnd));
    isRecognizing = false;
}

NS_CC_END