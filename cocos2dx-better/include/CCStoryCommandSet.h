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
#ifndef __CCStoryCommandSet__
#define __CCStoryCommandSet__

#include "cocos2d.h"
#include <string>

USING_NS_CC;
using namespace std;

// global command set
extern CCArray gStoryCommandSet;

// command method for lua
extern void msg(string name, string s);
extern void msgsize(float size);
extern void msgcolor(unsigned int c);
extern void msgpos(float x, float y);
extern void msganchor(float x, float y);
extern void namepos(float x, float y);
extern void namecolor(unsigned int c);
extern void namesize(float size);
extern void nameanchor(float x, float y);
extern void move();
extern void wait();
extern void fadein();
extern void fadeout();
extern void dialog();
extern void blink();
extern void shake();
extern void scale();
extern void img();
extern void arm();
extern void armplay();
extern void initangle();
extern void rotate();
extern void bgpos(float x, float y);
extern void bg(string frameName);
extern void bgcolor(unsigned int c);
extern void bgm();
extern void sound();

NS_CC_BEGIN

/// one command
class CCStoryCommand : public CCObject {
public:
    enum Type {
        UNKNOWN,
        MSG,
        MSG_SIZE,
        MSG_COLOR,
        MSG_POS,
        MSG_ANCHOR,
        NAME_POS,
        NAME_COLOR,
        NAME_SIZE,
        NAME_ANCHOR,
        MOVE,
        WAIT,
        FADE_IN,
        FADE_OUT,
        DIALOG,
        BLINK,
        SHAKE,
        SCALE,
        IMG,
        ARM,
        ARM_PLAY,
        INIT_ANGLE,
        ROTATE,
        BG_POS,
        BG,
        BG_COLOR,
        BGM,
        SOUND
    };
    
public:
    /// command parameters
    union {
        struct {
            const char* name;
            const char* s;
        } msg;
        
        struct {
            float size;
        } msgsize;
        
        struct {
            unsigned int c;
        } msgcolor;
        
        struct {
            float x;
            float y;
        } msgpos;
        
        struct {
            float x;
            float y;
        } msganchor;
        
        struct {
            float size;
        } namesize;
        
        struct {
            unsigned int c;
        } namecolor;
        
        struct {
            float x;
            float y;
        } namepos;
        
        struct {
            float x;
            float y;
        } nameanchor;
        
        struct {
            float x;
            float y;
        } bgpos;
        
        struct {
            const char* frameName;
        } bg;
        
        struct {
            unsigned int c;
        } bgcolor;
    } m_param;
    
public:
    CCStoryCommand();
    virtual ~CCStoryCommand();
    static CCStoryCommand* create(Type type);
    
    virtual bool initWithCommand(Type type);
    
    CC_SYNTHESIZE(Type, m_type, Type);
};

NS_CC_END

#endif /* defined(__CCStoryCommandSet__) */
