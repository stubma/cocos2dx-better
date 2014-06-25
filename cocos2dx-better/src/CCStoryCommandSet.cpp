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
#include "CCStoryCommandSet.h"

CCArray gStoryCommandSet;

void msg(string s) {
    CCStoryCommand* c = CCStoryCommand::create(CCStoryCommand::MSG);
    c->m_param.msg.s = CCUtils::copy(s.c_str());
    gStoryCommandSet.addObject(c);
}

NS_CC_BEGIN

CCStoryCommand::CCStoryCommand() :
m_type(UNKNOWN) {
    
}

CCStoryCommand::~CCStoryCommand() {
    switch(m_type) {
        case MSG:
            free((void*)m_param.msg.s);
            break;
        default:
            break;
    }
}

CCStoryCommand* CCStoryCommand::create(Type type) {
    CCStoryCommand* c = new CCStoryCommand();
    if(c->initWithCommand(type)) {
        c->autorelease();
        return c;
    }
    CC_SAFE_RELEASE(c);
    return NULL;
}

bool CCStoryCommand::initWithCommand(Type type) {
    m_type = type;
    memset(&m_param, 0, sizeof(m_param));
    
    return true;
}

NS_CC_END