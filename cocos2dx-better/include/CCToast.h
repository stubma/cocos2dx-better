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
#ifndef __CCToast__
#define __CCToast__

#include "cocos2d.h"

using namespace std;

NS_CC_BEGIN

/**
 * Toast
 */
class CC_DLL CCToast : public CCLayer {    
protected:
    CCToast();
    
public:
    virtual ~CCToast();
    
    /**
     * Create a CCToast instance
     *
     * @param owner owner node, toast will be added to owner with max z order
     * @param content content node will display real info. You must set proper anchor and position for content node
     * @param tag tag of this toast, if not -1, this toast will not show more than one instance at the same time
     * @param duration duration time, default is 0 which means default duration, about 3 seconds
     * @param inAction the custom in action of content node, default is NULL which means the content node  
     *      will be faded in
     * @param outAction the custom out action of content node, default is NULL which means the content node
     *      will be faded out
     * @return CCToast instance
     */
    static CCToast* create(CCNode* owner,
                           CCNode* content,
                           int tag = -1,
                           float duration = 0,
                           CCFiniteTimeAction* inAction = NULL,
                           CCFiniteTimeAction* outAction = NULL);
    
    /// force remove a toast with given tag
    static void forceRemove(CCNode* owner, int tag);
};

NS_CC_END

#endif /* defined(__CCToast__) */
