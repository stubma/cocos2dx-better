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
#ifndef __ccShader_blur_frag_h__
#define __ccShader_blur_frag_h__

#define kCCUniform_blurSize "CC_blurSize"
#define kCCUniform_blurSubtract "CC_blurSubtract"

static GLint sUniform_pos_CC_blurSize = -1;
static GLint sUniform_pos_CC_blurSubtract = -1;

const char* ccShader_blur_frag = "\n\
	#ifdef GL_ES \n\
		precision lowp float;\n\
	#endif\n\
	\n\
	varying vec4 v_fragmentColor;\n\
	varying vec2 v_texCoord;\n\
	uniform sampler2D CC_Texture0;\n\
	uniform vec2 CC_blurSize;\n\
	uniform vec4 CC_blurSubtract;\n\
	\n\
	void main() {\n\
		vec4 sum = vec4(0.0);\n\
		sum += texture2D(CC_Texture0, v_texCoord - 4.0 * CC_blurSize) * 0.05;\n\
		sum += texture2D(CC_Texture0, v_texCoord - 3.0 * CC_blurSize) * 0.09;\n\
		sum += texture2D(CC_Texture0, v_texCoord - 2.0 * CC_blurSize) * 0.12;\n\
		sum += texture2D(CC_Texture0, v_texCoord - 1.0 * CC_blurSize) * 0.15;\n\
		sum += texture2D(CC_Texture0, v_texCoord) * 0.16;\n\
		sum += texture2D(CC_Texture0, v_texCoord + 1.0 * CC_blurSize) * 0.15;\n\
		sum += texture2D(CC_Texture0, v_texCoord + 2.0 * CC_blurSize) * 0.12;\n\
		sum += texture2D(CC_Texture0, v_texCoord + 3.0 * CC_blurSize) * 0.09;\n\
		sum += texture2D(CC_Texture0, v_texCoord + 4.0 * CC_blurSize) * 0.05;\n\
		\n\
		gl_FragColor = (sum - CC_blurSubtract) * v_fragmentColor;\n\
	}";

#endif // __ccShader_blur_frag_h__
