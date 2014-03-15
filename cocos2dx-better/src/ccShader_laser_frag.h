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
#ifndef __ccShader_laser_frag_h__
#define __ccShader_laser_frag_h__

/// by default, cocos2d-x set CC_Time as vec4 and y is seconds
const char* ccShader_laser_frag = "\n\
	#ifdef GL_ES\n\
		precision lowp float;\n\
	#endif\n\
	\n\
	varying vec2 v_texCoord;\n\
	varying vec4 v_fragmentColor;\n\
	\n\
	void main()	{\n\
		float fPosX = v_texCoord.x;\n\
		float fPosY = v_texCoord.y;\n\
		\n\
		float fR = 1.0;\n\
		float fG = 1.0;\n\
		float fB = 1.0;\n\
		float fA = 1.0;\n\
		\n\
		float fTime = abs(sin(CC_Time.y));\n\
		if(fTime < 0.3)	{\n\
			fTime = 0.3;\n\
		}\n\
		\n\
		fR = fTime / abs(fPosX - 0.5) * 0.6;\n\
		fG = fR * 0.3 * fTime;\n\
		fB = fR * (1.0 - fPosY) * fTime;\n\
		vec3 Color = vec3(fR, fG, fB);\n\
		\n\
		float fScale = 1.0;\n\
		float fPercent = 0.2;\n\
		float fDis = fPercent;\n\
		if(fPosX < fPercent) {\n\
			fDis = fPosX;\n\
		}\n\
		if(fPosX > 1.0 - fPercent) {\n\
			fDis = 1.0 - fPosX;\n\
		}\n\
		fScale = fDis / fPercent;\n\
		fA *= fScale;\n\
		\n\
		gl_FragColor.rgb = Color;\n\
		gl_FragColor.a = fA;\n\
	}";

#endif // __ccShader_laser_frag_h__
