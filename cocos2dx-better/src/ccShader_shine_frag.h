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
#ifndef __ccShader_shine_frag_h__
#define __ccShader_shine_frag_h__

#define kCCUniform_shineWidth "CC_shineWidth"
#define kCCUniform_shineSpeed "CC_shineSpeed"
#define kCCUniform_shineXY1 "CC_shineXY1"
#define kCCUniform_shineXY2 "CC_shineXY2"
#define kCCUniform_shineColor1 "CC_shineColor1"
#define kCCUniform_shineColor2 "CC_shineColor2"
#define kCCUniform_shineColor3 "CC_shineColor3"
#define kCCUniform_shinePositions "CC_shinePositions"
#define kCCUniform_shineInterval "CC_shineInterval"

static GLint sUniform_pos_CC_shineWidth = -1;
static GLint sUniform_pos_CC_shineSpeed = -1;
static GLint sUniform_pos_CC_shineInterval = -1;
static GLint sUniform_pos_CC_shineXY1 = -1;
static GLint sUniform_pos_CC_shineXY2 = -1;
static GLint sUniform_pos_CC_shineColor1 = -1;
static GLint sUniform_pos_CC_shineColor2 = -1;
static GLint sUniform_pos_CC_shineColor3 = -1;
static GLint sUniform_pos_CC_shinePositions = -1;

const char* ccShader_shine_frag = "\n\
    #ifdef GL_ES\n\
        precision lowp float;\n\
    #endif\n\
    \n\
    varying vec4 v_fragmentColor;\n\
    varying vec2 v_texCoord;\n\
    varying vec4 v_position;\n\
    uniform sampler2D CC_Texture0;\n\
    uniform float CC_shineWidth;\n\
    uniform float CC_shineSpeed;\n\
    uniform float CC_shineInterval;\n\
    uniform vec2 CC_shineXY1;\n\
    uniform vec2 CC_shineXY2;\n\
    uniform vec4 CC_shineColor1;\n\
    uniform vec4 CC_shineColor2;\n\
    uniform vec4 CC_shineColor3;\n\
    uniform vec3 CC_shinePositions;\n\
    \n\
    void main() {\n\
        gl_FragColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord); \n\
    }";

#endif
