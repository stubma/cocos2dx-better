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
#ifndef __ccShader_flash_frag_h__
#define __ccShader_flash_frag_h__

#define kCCUniform_flashColor "CC_flashColor"
#define kCCUniform_flashTime "CC_flashTime"

static GLint sUniform_pos_CC_flashColor = -1;
static GLint sUniform_pos_CC_flashTime = -1;

const char* ccShader_flash_frag = "\n\
	#ifdef GL_ES \n\
	precision lowp float; \n\
	#endif \n\
	\n\
	varying vec4 v_fragmentColor; \n\
	varying vec2 v_texCoord; \n\
	uniform sampler2D CC_Texture0; \n\
	uniform vec3 CC_flashColor; \n\
	uniform float CC_flashTime; \n\
	\n\
	void main()	{ \n\
		gl_FragColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord); \n\
		if(gl_FragColor.a > 0.0) { \n\
			vec3 deltaColor = (CC_flashColor - gl_FragColor.xyz) * CC_flashTime * gl_FragColor.a; \n\
			gl_FragColor.xyz += deltaColor; \n\
		} \n\
	}";

#endif // __ccShader_flash_frag_h__
