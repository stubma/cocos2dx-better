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
#include "CCUtils.h"

CCArray gStoryCommandSet;

void msg(string s) {
    CCStoryCommand* c = CCStoryCommand::create(CCStoryCommand::MSG);
    c->m_param.msg.s = CCUtils::copy(s.c_str());
    gStoryCommandSet.addObject(c);
}

void msgsize(float size) {
    CCStoryCommand* c = CCStoryCommand::create(CCStoryCommand::MSG_SIZE);
    c->m_param.msgsize.size = size;
    gStoryCommandSet.addObject(c);
}

void msgcolor(unsigned int c) {
    CCStoryCommand* cmd = CCStoryCommand::create(CCStoryCommand::MSG_COLOR);
    cmd->m_param.msgcolor.c = c;
    gStoryCommandSet.addObject(cmd);
}

void msgpos(float x, float y) {
    CCStoryCommand* c = CCStoryCommand::create(CCStoryCommand::MSG_POS);
    c->m_param.msgpos.x = x;
    c->m_param.msgpos.y = y;
    gStoryCommandSet.addObject(c);
}

void msganchor(float x, float y) {
    CCStoryCommand* c = CCStoryCommand::create(CCStoryCommand::MSG_ANCHOR);
    c->m_param.msganchor.x = x;
    c->m_param.msganchor.y = y;
    gStoryCommandSet.addObject(c);
}

void namepos(float x, float y) {
    CCStoryCommand* c = CCStoryCommand::create(CCStoryCommand::NAME_POS);
    c->m_param.namepos.x = x;
    c->m_param.namepos.y = y;
    gStoryCommandSet.addObject(c);
}

void namecolor(unsigned int c) {
    CCStoryCommand* cmd = CCStoryCommand::create(CCStoryCommand::NAME_COLOR);
    cmd->m_param.namecolor.c = c;
    gStoryCommandSet.addObject(cmd);
}

void namesize(float size) {
    CCStoryCommand* c = CCStoryCommand::create(CCStoryCommand::NAME_SIZE);
    c->m_param.namesize.size = size;
    gStoryCommandSet.addObject(c);
}

void nameanchor(float x, float y) {
    CCStoryCommand* c = CCStoryCommand::create(CCStoryCommand::NAME_ANCHOR);
    c->m_param.nameanchor.x = x;
    c->m_param.nameanchor.y = y;
    gStoryCommandSet.addObject(c);
}

void move() {
    
}

void wait() {
    
}

void fadein() {
    
}

void fadeout() {
    
}

void dialog() {
    
}

void blink() {
    
}

void shake() {
    
}

void scale() {
    
}

void img() {
    
}

void arm() {
    
}

void armplay() {
    
}

void initangle() {
    
}

void rotate() {
    
}

void bg() {
    
}

void bgcolor(unsigned int c) {
    CCStoryCommand* cmd = CCStoryCommand::create(CCStoryCommand::BG_COLOR);
    cmd->m_param.bgcolor.c = c;
    gStoryCommandSet.addObject(cmd);
}

void bgm() {
    
}

void sound() {
    
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