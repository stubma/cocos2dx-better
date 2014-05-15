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
#ifndef __ccShader_lighting_frag_h__
#define __ccShader_lighting_frag_h__

#define kCCUniform_lightingMul "CC_lightingMul"
#define kCCUniform_lightingAdd "CC_lightingAdd"

static GLint sUniform_pos_CC_lightingMul = -1;
static GLint sUniform_pos_CC_lightingAdd = -1;

const char* ccShader_lighting_frag = "\n\
    #ifdef GL_ES \n\
        precision lowp float; \n\
    #endif \n\
    \n\
    varying vec4 v_fragmentColor; \n\
    varying vec2 v_texCoord; \n\
    uniform sampler2D CC_Texture0; \n\
    uniform vec4 CC_lightingMul; \n\
    uniform vec3 CC_lightingAdd; \n\
    \n\
    void main()	{ \n\
        gl_FragColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord); \n\
        vec3 c = CC_lightingAdd * gl_FragColor.a; \n\
        gl_FragColor *= CC_lightingMul; \n\
        gl_FragColor.xyz += c; \n\
    }";

#endif // __ccShader_lighting_frag_h__
