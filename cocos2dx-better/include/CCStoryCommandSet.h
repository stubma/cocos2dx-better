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

// for designer
#ifdef CC_STORY_DESIGNER
extern vector<string> gUsedSpriteFrameNames;
extern vector<string> gUsedImageNames;
extern vector<string> gUsedArmatureNames;
#endif

/////////////////////////////////////
// command method for lua - start
/////////////////////////////////////

/**
 * 设定游戏的基础大小, 如果不设定, 则沿用游戏的当前大小设定. 在剧情播放完成后会
 * 恢复原来的大小
 *
 * \note
 * 一般情况下不需要调用此方法
 *
 * @param w 宽度
 * @param h 高度
 */
extern void winsize(float w, float h);

/**
 * 设定游戏的适配模式, 如果不设定, 则沿用游戏的当前大小设定.
 *
 * \note
 * 由于cocos2d-x的设计问题, 在剧情播放完成后无法恢复原来的适配模式.
 *
 * \note
 * 一般情况下不需要调用此方法
 * 
 * @param p 适配模式常量
 */
extern void policy(ResolutionPolicy p);

/**
 * 显示一条剧情对话, 剧情对话本质上是一段文本, 但是其是一个一个字逐渐显示的.
 * 剧情对话的颜色, 字体大小, 位置, 锚点都有专门的命令进行控制. 这主要是考虑到
 * 这些设置一般不变, 所以统一控制可以减少msg命令的参数个数
 *
 * @param name 说话人的名称, 名称也是一类特殊的文本, 有专门的命令控制它的颜色, 字体大小, 锚点和位置.
 * @param s 对话的内容
 * @param wait 对话全部显示完毕后等待的时间, 小于0表示等待用户点击再消失
 */
extern void msg(string name, string s, float wait = 1);

/**
 * 设置剧情对话字体的大小, 由于剧情脚本是顺序执行的, 所以它不会影响命令之前的对话字体
 * 
 * @param size 字体大小
 */
extern void msgsize(float size);

/**
 * 设置剧情对话的颜色, 由于剧情脚本是顺序执行的, 所以它不会影响命令之前的对话颜色
 * 
 * @param c 格式为argb8888
 */
extern void msgcolor(unsigned int c);

/**
 * 剧情对话的坐标, 由于剧情脚本是顺序执行的, 所以它不会影响命令之前的对话坐标
 * 
 * @param x x坐标
 * @param y y坐标
 */
extern void msgpos(float x, float y);

/**
 * 剧情对话节点的锚点, 缺省描点是在中心. 由于剧情脚本是顺序执行的, 所以它不会影响命令之前的对话锚点.
 * 锚点使用相对值表示的, 即0表示左边或下边, 1表示右边或上边.
 * 
 * @param x x锚点
 * @param y y锚点
 */
extern void msganchor(float x, float y);

/**
 * 设置剧情对话节点的最大行宽, 缺省是不限制(也就是单行, 除非强制换行), 由于剧情脚本是顺序执行
 * 的, 所以它不会影响命令之前的对话行宽.
 *
 * @param w 行宽
 */
extern void msgwidth(float w);

/**
 * 设置剧情对话节点的文本水平对齐方式, 缺省是居中, 由于剧情脚本是顺序执行
 * 的, 所以它不会影响命令之前的对话对齐.
 *
 * @param a 可以为left, center, right
 */
extern void msgalign(CCTextAlignment a);

/**
 * 设置剧情对话节点的字体, 由于剧情脚本是顺序执行
 * 的, 所以它不会影响命令之前的对话字体
 *
 * @param fontName 字体名称
 */
extern void msgfont(string fontName);

/**
 * 名称的坐标, 由于剧情脚本是顺序执行的, 所以它不会影响命令之前的名称坐标
 *
 * @param x x坐标
 * @param y y坐标
 */
extern void namepos(float x, float y);

/**
 * 设置名称的颜色, 由于剧情脚本是顺序执行的, 所以它不会影响命令之前的名称颜色
 *
 * @param c 格式为argb8888
 */
extern void namecolor(unsigned int c);

/**
 * 设置名称字体的大小, 由于剧情脚本是顺序执行的, 所以它不会影响命令之前的名称字体
 *
 * @param size 字体大小
 */
extern void namesize(float size);

/**
 * 名称节点的锚点, 缺省描点是在中心. 由于剧情脚本是顺序执行的, 所以它不会影响命令之前的名称锚点.
 * 锚点使用相对值表示的, 即0表示左边或下边, 1表示右边或上边.
 *
 * @param x x锚点
 * @param y y锚点
 */
extern void nameanchor(float x, float y);

/**
 * 设置名称节点的字体, 由于剧情脚本是顺序执行
 * 的, 所以它不会影响命令之前的名称字体
 *
 * @param fontName 字体名称
 */
extern void namefont(string fontName);

/**
 * 设置标签的颜色, 由于剧情脚本是顺序执行的, 所以它不会影响命令之前的标签颜色.
 * 除了剧情对话和名称之外的文本, 统一由一些命令控制它们的颜色, 字体大小和描点.
 *
 * @param c 格式为argb8888
 */
extern void labelcolor(unsigned int c);

/**
 * 设置标签字体的大小, 由于剧情脚本是顺序执行的, 所以它不会影响命令之前的标签字体.
 * 标签是指除了剧情对话和名称之外的所有文本
 *
 * @param size 字体大小
 */
extern void labelsize(float size);

/**
 * 设置标签节点的字体, 由于剧情脚本是顺序执行
 * 的, 所以它不会影响命令之前的标签字体
 *
 * @param fontName 字体名称
 */
extern void labelfont(string fontName);

/**
 * 标签节点的锚点, 缺省描点是在中心. 由于剧情脚本是顺序执行的, 所以它不会影响命令之前的标签锚点.
 * 锚点使用相对值表示的, 即0表示左边或下边, 1表示右边或上边. 标签是指除了剧情对话和名称之外的所有文本
 *
 * @param x x锚点
 * @param y y锚点
 */
extern void labelanchor(float x, float y);

/**
 * 设置标签节点的最大行宽, 缺省是不限制(也就是单行, 除非强制换行), 由于剧情脚本是顺序执行
 * 的, 所以它不会影响命令之前的标签行宽.
 *
 * @param w 行宽
 */
extern void labelwidth(float w);

/**
 * 设置标签节点的文本水平对齐方式, 缺省是居中, 由于剧情脚本是顺序执行
 * 的, 所以它不会影响命令之前的标签对齐.
 *
 * @param a 可以为left, center, right
 */
extern void labelalign(CCTextAlignment a);

/**
 * 对某个角色执行一个移动动作
 * 
 * @param name 角色的名称
 * @param x 要移动到的x位置
 * @param y 要移动到的y位置
 * @param time 移动完成的时间
 */
extern void move(string name, float x, float y, float time);

/**
 * 等待多少秒再继续执行剧情
 * 
 * @param time 等待时间
 */
extern void wait(float time);

/**
 * 等待某个动画播放完一次, 这个命令的上一条命令必须是armplay, 否则会提示出错.
 */
extern void waitarm();

/**
 * 等待用户点击任意位置, 再继续执行剧情
 */
extern void waitclick();

/**
 * 对某个角色执行淡入效果
 *
 * @param name 角色名称
 * @param time 淡入完成时间
 */
extern void fadein(string name, float time);

/**
 * 对某个角色执行淡出效果
 *
 * @param name 角色名称
 * @param time 淡出完成时间
 */
extern void fadeout(string name, float time);

/**
 * 对某个角色执行闪光效果
 *
 * @param name 角色名称
 * @param color 闪光颜色
 * @param times 闪光次数
 * @param time 闪光时间
 */
extern void flash(string name, unsigned int color, float times, float time);

/**
 * 对某个角色执行闪烁效果
 *
 * @param name 角色名称
 * @param times 闪烁次数
 * @param time 闪烁时间
 */
extern void blink(string name, float times, float time);

/**
 * 对某个角色执行抖动效果
 * 
 * @param name 角色名称
 * @param apmlitude 抖动的幅度
 * @param time 抖动时间
 */
extern void shake(string name, float amplitude, float time);

/**
 * 对某个角色执行缩放动画
 *
 * @param name 角色名称
 * @param to 缩放到多少倍, 1表示原大小, 2表示2倍
 * @param time 持续时间
 */
extern void scaleto(string name, float to, float time);

/**
 * 创建一个图片角色, 即精灵角色(Sprite), 该角色的图片可以来自于某张大图(一般由TexturePacker创建), 
 * 也可以来自某个图片文件. 会首先尝试使用大图, 如果找不到帧, 则会尝试找寻文件
 *
 * \note
 * 这个角色本质上是一个CCSprite类, 角色可以有一个名字, 其它命令可以通过名字引用这个角色.
 *
 * @param name 角色名字
 * @param frameName 角色图片的帧名, 如果帧不存在, 则会尝试找寻文件
 * @param x 角色的初始x位置
 * @param y 角色的初始y位置
 */
extern void img(string name, string frameName, float x, float y);

/**
 * 创建一个标签角色, 本质上是创建了一个CCRichLabelTTF对象. 标签的颜色, 字体大小, 锚点都受专门命令
 * 的统一控制, 这是为了减少每次创建标签时需要提供的参数. 
 * 
 * @param name 角色名称
 * @param text 标签文字, 可以是CCRichLabelTTF支持的富文本格式
 * @param x 初始x位置
 * @param y 初始y位置
 */
extern void label(string name, string text, float x, float y);

/**
 * 创建一个动画角色, 本质上是创建了一个CBArmature对象. 创建的动画角色缺省不播放任何动画,
 * 一般要跟一个armplay命令来播放动画.
 *
 * @param name 角色名称
 * @param armName 动画描述文件名称(无ExportJson扩展名)
 * @param x 初始x位置
 * @param y 初始y位置
 */
extern void arm(string name, string armName, float x, float y);

/**
 * 让某个动画角色播放动画
 *
 * @param name 角色名称
 * @param animName 动画名称
 */
extern void armplay(string name, string animName);

/**
 * 改变某个角色的旋转角度
 *
 * @param name 角色名称
 * @param degree 角度, 正值表示顺时针, 负值表示逆时针
 */
extern void angle(string name, float degree);

/**
 * 对某个角色执行旋转动画
 *
 * @param name 角色名称
 * @param delta 要旋转的角度
 * @param time 持续时间
 */
extern void rotateby(string name, float delta, float time);

/**
 * 设置背景的配置, 背景是一种特殊的图片角色, 背景没有名称, 显示在最下面. 背景的锚点在中间, 目前不能更改.
 * 如果不调用此名称, 背景的位置就是在屏幕中间.
 * 
 * @param x 背景位置
 * @param y 背景位置
 */
extern void bgpos(float x, float y);

/**
 * 指定背景图片, 图片必须来自某个大图(通常由TexturePacker创建)
 * 
 * @param frameName 图片帧名
 */
extern void bg(string frameName);

/**
 * 指定背景颜色, 背景颜色的优先级高于背景图片, 如果同时设置了背景图片和背景颜色, 且背景颜色无透明度,
 * 则背景图片将不可见.
 *
 * @param c argb8888格式
 */
extern void bgcolor(unsigned int c);

/**
 * 设定某个角色的渲染顺序, 或者叫z order值, 值大者将显示在值小者的上面
 *
 * @param name 角色名称
 * @param zOrder 渲染顺序
 */
extern void z(string name, int zOrder);

/**
 * 播放背景音乐, 如果当前有背景音乐, 将停止当前的背景音乐
 *
 * \note
 * 文件路径会被首先映射到外部存储器, 如果不存在再定位到内部, 这样可以支持下载更新的情况.
 *
 * @param musicFile 背景音乐文件路径
 */
extern void bgm(string musicFile);

/**
 * 播放音效
 *
 * \note
 * 文件路径会被首先映射到外部存储器, 如果不存在再定位到内部, 这样可以支持下载更新的情况.
 *
 * @param soundFile 音效文件路径
 */
extern void sound(string soundFile);

/**
 * 删除一个角色
 *
 * @param name 角色名称
 */
extern void remove(string name);

/**
 * 显示某个角色
 *
 * @param name 角色名称
 */
extern void show(string name);

/**
 * 隐藏某个角色
 *
 * @param name 角色名称
 */
extern void hide(string name);

/**
 * 水平翻转某个角色
 *
 * @param name 角色名称
 */
extern void flipx(string name);

/**
 * 垂直翻转某个角色
 *
 * @param name 角色名称
 */
extern void flipy(string name);

/**
 * 设置角色的位置
 *
 * @param name 角色名称
 * @param x x位置
 * @param y y位置
 */
extern void pos(string name, float x, float y);

/**
 * 设置角色的位置, 该位置以某个节点为参考得到(称为参考角色), 比如参考角色的(1, 1), 即表示
 * 将角色的位置设置成参考角色的右上角位置.
 *
 * @param name 角色名称
 * @param refName 参考角色名称
 * @param relativeX 参考角色的相对x位置
 * @param relativeY 参考角色的相对y位置
 */
extern void npos(string name, string refName, float relativeX, float relativeY);

/**
 * 设置角色的位置, 该位置以屏幕为参考得到(实质上是可见区域), 比如(1, 1)表示屏幕的右上角
 *
 * @param name 角色名称
 * @param relativeX 屏幕的相对x位置
 * @param relativeY 屏幕的相对y位置
 */
extern void spos(string name, float relativeX, float relativeY);

/**
 * 设置角色的锚点
 *
 * @param name 角色名称
 * @param x x锚点
 * @param y y锚点
 */
extern void anchor(string name, float x, float y);

/**
 * 设置角色的颜色
 * 
 * @param name 角色名称
 * @param c argb8888格式
 */
extern void color(string name, unsigned int c);

/**
 * 设置角色的缩放
 * 
 * @param name 角色名称
 * @param x x方向缩放
 * @param y y方向缩放
 */
extern void scale(string name, float x, float y);

/**
 * 根据名称获得一个参数, 参数的类型是字符串.
 *
 * @param key 参数键值
 * @return 参数值. 如果参数不存在则返回空字符串
 */
extern string p(string key);

/////////////////////////////////////
// command method for lua - end
/////////////////////////////////////

NS_CC_BEGIN

/// one command
class CCStoryCommand : public CCObject {
public:
    enum Type {
        UNKNOWN,
        WIN_SIZE,
        POLICY,
        MSG,
        MSG_SIZE,
        MSG_COLOR,
        MSG_POS,
        MSG_ANCHOR,
        MSG_WIDTH,
        MSG_ALIGN,
        MSG_FONT,
        NAME_POS,
        NAME_COLOR,
        NAME_SIZE,
        NAME_ANCHOR,
        NAME_FONT,
        LABEL_COLOR,
        LABEL_SIZE,
        LABEL_FONT,
        LABEL_ANCHOR,
        LABEL_WIDTH,
        LABEL_ALIGN,
        MOVE,
        WAIT,
        WAIT_ARM,
        WAIT_CLICK,
        FADE_IN,
        FADE_OUT,
        FLASH,
        BLINK,
        SHAKE,
        SCALE_TO,
        IMG,
        LABEL,
        ARM,
        ARM_PLAY,
        ANGLE,
        ROTATE_BY,
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
        POS,
        NODE_POS,
        SCREEN_POS,
        ANCHOR,
        COLOR,
        SCALE
    };
    
public:
    /// command parameters
    union {
        struct {
            float w;
            float h;
        } winsize;
        
        struct {
            ResolutionPolicy p;
        } policy;
        
        struct {
            const char* name;
            const char* s;
            float wait;
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
            float w;
        } msgwidth;
        
        struct {
            CCTextAlignment a;
        } msgalign;
        
        struct {
            const char* fontName;
        } msgfont;
        
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
            const char* fontName;
        } namefont;
        
        struct {
            unsigned int c;
        } labelcolor;
        
        struct {
            float size;
        } labelsize;
        
        struct {
            const char* fontName;
        } labelfont;
        
        struct {
            float x;
            float y;
        } labelanchor;
        
        struct {
            float w;
        } labelwidth;
        
        struct {
            CCTextAlignment a;
        } labelalign;
        
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
            float times;
            float time;
        } blink;
        
        struct {
            const char* name;
            float amplitude;
            float time;
        } shake;
        
        struct {
            const char* name;
            float delta;
            float time;
        } rotateby;
        
        struct {
            const char* name;
            float to;
            float time;
        } scaleto;
        
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
        
        struct {
            float x;
            float y;
            const char* name;
            const char* refName;
        } npos;
        
        struct {
            float x;
            float y;
            const char* name;
        } spos;
        
        struct {
            float x;
            float y;
            const char* name;
        } anchor;
        
        struct {
            unsigned int c;
            const char* name;
        } color;
        
        struct {
            const char* name;
            float x;
            float y;
        } scale;
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
