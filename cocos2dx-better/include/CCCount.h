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
#ifndef __CCCount__
#define __CCCount__

#include "cocos2d.h"

using namespace std;

NS_CC_BEGIN

/**
 * increase or decrease a number label from a number to another.
 * the action target must implements CCLabelProtocol or CocoStudio labels, such as
 * UILabel and UILabelBMFont
 */
class CC_DLL CCCount : public CCActionInterval {
private:
    typedef enum {
        CC_LABEL_PROTOCOL,
        CCS_LABEL,
        CCS_BITMAP_LABEL
    } LabelType;
    
private:
    /// from value
    int m_from;
    
    /// to value
    int m_to;
    
    /// pattern
    string m_pattern;
    
    /// buf
    char* m_buf;
    
    /// label type
    LabelType m_lt;
    
public:
    CCCount();
    virtual ~CCCount();
    
    /**
     * create a CCCount instance
     *
     * @param duration action duration time
     * @param from start value
     * @param to end value
     * @param pattern the label text pattern, must contain one %d formatter
     */
    static CCCount* create(float duration, int from, int to, const string& pattern = "%d");
    
    /// init self
    bool initWithDuration(float d, int from, int to, const string& pattern);
    
    // override super
    virtual void update(float time);
    virtual void startWithTarget(CCNode *pTarget);
    virtual CCObject* copyWithZone(CCZone* pZone);
    virtual CCActionInterval* reverse();
};

NS_CC_END

#endif /* defined(__CCCount__) */
