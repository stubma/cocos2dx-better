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
extern void labelcolor(unsigned int c);
extern void labelsize(float size);
extern void labelanchor(float x, float y);
extern void move(string name, float x, float y, float time);
extern void wait(float time);
extern void waitarm();
extern void waitclick();
extern void fadein(string name, float time);
extern void fadeout(string name, float time);
extern void flash(string name, unsigned int color, float times, float time);
extern void shake(string name, float amplitude, float time);
extern void scale(string name, float to, float time);
extern void img(string name, string frameName, float x, float y);
extern void label(string name, string text, float x, float y);
extern void arm(string name, string armName, float x, float y);
extern void armplay(string name, string animName);
extern void angle(string name, float degree);
extern void rotate(string name, float delta, float time);
extern void bgpos(float x, float y);
extern void bg(string frameName);
extern void bgcolor(unsigned int c);
extern void z(string name, int zOrder);
extern void bgm(string musicFile);
extern void sound(string soundFile);
extern void remove(string name);
extern void show(string name);
extern void hide(string name);
extern void flipx(string name);
extern void flipy(string name);
extern void pos(string name, float x, float y);

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
        LABEL_COLOR,
        LABEL_SIZE,
        LABEL_ANCHOR,
        MOVE,
        WAIT,
        WAIT_ARM,
        WAIT_CLICK,
        FADE_IN,
        FADE_OUT,
        FLASH,
        SHAKE,
        SCALE,
        IMG,
        LABEL,
        ARM,
        ARM_PLAY,
        ANGLE,
        ROTATE,
        BG_POS,
        BG,
        BG_COLOR,
        Z,
        BGM,
        SOUND,
        REMOVE,
        SHOW,
        HIDE,
        FLIP_X,
        FLIP_Y,
        POS
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
            unsigned int c;
        } labelcolor;
        
        struct {
            float size;
        } labelsize;
        
        struct {
            float x;
            float y;
        } labelanchor;
        
        struct {
            const char* name;
            float x;
            float y;
            float time;
        } move;
        
        struct {
            float time;
        } wait;
        
        struct {
            const char* name;
            unsigned int c;
            float times;
            float time;
        } flash;
        
        struct {
            const char* name;
            float amplitude;
            float time;
        } shake;
        
        struct {
            const char* name;
            float delta;
            float time;
        } rotate;
        
        struct {
            const char* name;
            float to;
            float time;
        } scale;
        
        struct {
            const char* name;
            float time;
        } fadein;
        
        struct {
            const char* name;
            float time;
        } fadeout;
        
        struct {
            float x;
            float y;
            const char* name;
            const char* frameName;
        } img;
        
        struct {
            float x;
            float y;
            const char* name;
            const char* text;
        } label;
        
        struct {
            float x;
            float y;
            const char* name;
            const char* armName;
        } arm;
        
        struct {
            const char* name;
            const char* animName;
        } armplay;
        
        struct {
            const char* name;
            float degree;
        } angle;
        
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
        
        struct {
            const char* musicFile;
        } bgm;
        
        struct {
            const char* soundFile;
        } sound;
        
        struct {
            const char* name;
            int z;
        } z;
        
        struct {
            const char* name;
        } remove;
        
        struct {
            const char* name;
        } show;
        
        struct {
            const char* name;
        } hide;
        
        struct {
            const char* name;
        } flipx;
        
        struct {
            const char* name;
        } flipy;
        
        struct {
            float x;
            float y;
            const char* name;
        } pos;
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
