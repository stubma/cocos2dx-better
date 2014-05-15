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
#ifndef __ccShader_matrix_frag_h__
#define __ccShader_matrix_frag_h__

#define kCCUniform_colorMatrix "CC_colorMatrix"

static GLint sUniform_pos_CC_colorMatrix = -1;

const char* ccShader_matrix_frag = "\n\
    #ifdef GL_ES \n\
        precision lowp float; \n\
    #endif \n\
    \n\
    varying vec4 v_fragmentColor; \n\
    varying vec2 v_texCoord; \n\
    uniform sampler2D CC_Texture0; \n\
    uniform mat4 CC_colorMatrix; \n\
    \n\
    void main()	{ \n\
        gl_FragColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord); \n\
        gl_FragColor = CC_colorMatrix * gl_FragColor; \n\
    }";

#endif // __ccShader_matrix_frag_h__
