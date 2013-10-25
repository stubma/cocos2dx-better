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

#ifndef CCPanGestureRecognizer_h
#define CCPanGestureRecognizer_h

#include "CCGestureRecognizer.h"

NS_CC_BEGIN

//this class is used for storing information about the pan gesture
class CC_DLL CCPan : public CCObject
{
public:
    bool init() {return true;}
    CREATE_FUNC(CCPan);
    CCPoint location;
    CCPoint delta;
};

class CC_DLL CCPanGestureRecognizer : public CCGestureRecognizer
{
public:
    bool init();
    ~CCPanGestureRecognizer();
    CREATE_FUNC(CCPanGestureRecognizer);
    
    virtual bool ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent);
    virtual void ccTouchMoved(CCTouch * pTouch, CCEvent * pEvent);
    virtual void ccTouchEnded(CCTouch * pTouch, CCEvent * pEvent);
};

NS_CC_END

#endif