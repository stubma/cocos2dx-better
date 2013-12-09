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

#ifndef CCGestureRecognizer_h
#define CCGestureRecognizer_h

#include "cocos2d.h"

NS_CC_BEGIN

class CC_DLL CCGestureRecognizer : public CCLayer
{
public:
	CCGestureRecognizer();
	~CCGestureRecognizer();
	void setTarget(CCObject * tar, SEL_CallFuncO sel);

	//setParent is called after the layer is added as a child
	virtual void setParent(CCNode*p);
protected:
	CCRect frame;
	bool isRecognizing;

	void gestureRecognized(CCObject * gesture);
	void stopTouchesPropagation(CCSet * pTouches, CCEvent * pEvent);

	//utility methods
	bool isPositionBetweenBounds(CCPoint pos);
	float distanceBetweenPoints(CCPoint p1, CCPoint p2);
	CCSet * createSetWithTouch(CCTouch * pTouch);

	virtual void registerWithTouchDispatcher(void);

	//mandatory methods for subclasses
	virtual bool ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent)=0;
	virtual void ccTouchMoved(CCTouch * pTouch, CCEvent * pEvent)=0;
	virtual void ccTouchEnded(CCTouch * pTouch, CCEvent * pEvent)=0;

	//if gesture is correctly recognized, cancel touch over other views (default: false)
	//NOTE: subclasses must check the value and implement it correctly
	CC_SYNTHESIZE(bool, cancelsTouchesInView, CancelsTouchesInView);
    
private:
	SEL_CallFuncO selector;
	CCObject * target;
	CCTouchDispatcher * dispatcher;
};

NS_CC_END

#endif
