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
#ifndef __CCBlur__
#define __CCBlur__

#include "cocos2d.h"

NS_CC_BEGIN

/**
 * Blur a sprite. You need set a delta blur size
 */
class CC_DLL CCBlur : public CCActionInterval {
private:
    /// old program
    CCGLProgram* m_oldProgram;
    
    /// start blur size
	CCSize m_startBlurSize;
	
	/// end blur size
	CCSize m_endBlurSize;
	
	/// delta size
	CCSize m_deltaBlurSize;
    
protected:
    CCBlur();
    
public:
    virtual ~CCBlur();
    
    /**
     * create a CCBlur instance
     *
     * @param duration action duration time
     * @param c the Blur color
     */
    static CCBlur* create(float duration, CCSize startBlurSize, CCSize endBlurSize);
    
    /// init self
    bool initWithBlurSize(float d, CCSize startBlurSize, CCSize endBlurSize);
    
    // override super
    virtual void update(float time);
    virtual void startWithTarget(CCNode *pTarget);
    virtual void stop();
    virtual CCObject* copyWithZone(CCZone* pZone);
    virtual CCActionInterval* reverse();
};

NS_CC_END

#endif /* defined(__CCBlur__) */
