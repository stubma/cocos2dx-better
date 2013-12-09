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

#include "CCPanGestureRecognizer.h"

NS_CC_BEGIN

bool CCPanGestureRecognizer::init()
{
    return true;
}

CCPanGestureRecognizer::~CCPanGestureRecognizer()
{
    
}

bool CCPanGestureRecognizer::ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent)
{
    if (isRecognizing) {
        isRecognizing = false;
        return false;
    }
    
    CCPoint loc = pTouch->getLocation();
    if (!isPositionBetweenBounds(loc)) return false;
    
    isRecognizing = true;
    return true;
}

void CCPanGestureRecognizer::ccTouchMoved(CCTouch * pTouch, CCEvent * pEvent)
{
    CCPan * pan = CCPan::create();
    pan->location = pTouch->getLocation();
    pan->delta = pTouch->getDelta();
    gestureRecognized(pan);
}

void CCPanGestureRecognizer::ccTouchEnded(CCTouch * pTouch, CCEvent * pEvent)
{
    isRecognizing = false;
    
    //cancel touch over other views if necessary
    if (cancelsTouchesInView) {
        stopTouchesPropagation(createSetWithTouch(pTouch), pEvent);
    }
}

NS_CC_END