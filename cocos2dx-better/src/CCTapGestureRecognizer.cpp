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

#include "CCTapGestureRecognizer.h"

NS_CC_BEGIN

bool CCTapGestureRecognizer::init()
{
    setNumberOfTapsRequired(1);
    taps = 0;
    return true;
}

CCTapGestureRecognizer::~CCTapGestureRecognizer()
{
    
}

bool CCTapGestureRecognizer::ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent)
{
    if (isRecognizing && taps==0) {
        stopGestureRecognition();
        return false;
    }
    
    initialPosition = pTouch->getLocation();
    
    if (!isPositionBetweenBounds(initialPosition)) return false;
    
    CCTime::gettimeofdayCocos2d(&startTime, NULL);
    
    if (taps>0 && taps<numberOfTapsRequired) {
        float distance = distanceBetweenPoints(finalPosition, initialPosition); //distance between taps
        double duration = CCTime::timersubCocos2d(&endTime, &startTime); //duration between taps
        if (duration>kTapMaxDurationBetweenTaps || distance>kTapMaxDistanceBetweenTaps) {
            stopGestureRecognition();
        }
    }
    
    isRecognizing = true;
    return true;
}

void CCTapGestureRecognizer::ccTouchEnded(CCTouch * pTouch, CCEvent * pEvent)
{
    //calculate duration
    CCTime::gettimeofdayCocos2d(&endTime, NULL);
    double duration = CCTime::timersubCocos2d(&startTime, &endTime); //duration of tap in milliseconds
    
    //calculate distance
    finalPosition = pTouch->getLocation();
    float distance = distanceBetweenPoints(initialPosition, finalPosition);
    
    //tap was successful
    if (duration<=kTapMaxDuration && distance<=kTapMaxDistance) {
        taps++;
        if (taps==numberOfTapsRequired) {
            CCTap * tap = CCTap::create();
            tap->location = initialPosition;
            
            gestureRecognized(tap);
            if (cancelsTouchesInView) stopTouchesPropagation(createSetWithTouch(pTouch), pEvent); //cancel touch over other views
            stopGestureRecognition();
        }
    }
    else {
        stopGestureRecognition();
    }
}

void CCTapGestureRecognizer::stopGestureRecognition()
{
    taps = 0;
    isRecognizing = false;
}

NS_CC_END