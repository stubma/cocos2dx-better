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

#ifndef CCTapGestureRecognizer_h
#define CCTapGestureRecognizer_h

#include "CCGestureRecognizer.h"

#define kTapMaxDuration 150
#define kTapMaxDurationBetweenTaps 220
#define kTapMaxDistance 20
#define kTapMaxDistanceBetweenTaps 20

NS_CC_BEGIN

//this class is used for storing information about the tap gesture
class CC_DLL CCTap : public CCObject
{
public:
    bool init() {return true;}
    CREATE_FUNC(CCTap);
    CCPoint location;
};

class CC_DLL CCTapGestureRecognizer : public CCGestureRecognizer
{
public:
    bool init();
    ~CCTapGestureRecognizer();
    CREATE_FUNC(CCTapGestureRecognizer);
    
    virtual bool ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent);
    virtual void ccTouchMoved(CCTouch * pTouch, CCEvent * pEvent){};
    virtual void ccTouchEnded(CCTouch * pTouch, CCEvent * pEvent);
protected:
    CC_SYNTHESIZE(unsigned int, numberOfTapsRequired, NumberOfTapsRequired);
private:
    int taps;
    CCPoint initialPosition, finalPosition;
    struct cc_timeval startTime, endTime;
    
    void stopGestureRecognition();
};

NS_CC_END

#endif