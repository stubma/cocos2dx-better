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
#include "CCShaders.h"
#include "ccShader_flash_vert.h"
#include "ccShader_flash_frag.h"

using namespace std;

NS_CC_BEGIN

void CCShaders::loadCustomShaders() {
	loadCustomShader(kCCShader_flash);
}

void CCShaders::loadCustomShader(const string& key) {
    if(!CCShaderCache::sharedShaderCache()->programForKey(key.c_str())) {
		// load shader
        CCGLProgram* p = new CCGLProgram();
		p->autorelease();
		if(key == kCCShader_flash) {
			p->initWithVertexShaderByteArray(ccShader_flash_vert, ccShader_flash_frag);
		}
		
		// add attribute
		if(false) {
			// non-default situation
		} else {
			p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
			p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
			p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
		}

		// link it
        p->link();
		
		// add custom uniform
		if(key == kCCShader_flash) {
			sUniform_pos_CC_flashColor = p->getUniformLocationForName(kCCUniformFlashColor);
			sUniform_pos_CC_flashTime = p->getUniformLocationForName(kCCUniformFlashTime);
		}
		
		// add standard uniforms
        p->updateUniforms();
		
		// add program
        CCShaderCache::sharedShaderCache()->addProgram(p, key.c_str());
    }
}

CCGLProgram* CCShaders::programForKey(const string& key) {
	loadCustomShader(key);
	return CCShaderCache::sharedShaderCache()->programForKey(key.c_str());
}

void CCShaders::setFlash(float r, float g, float b, float t) {
	loadCustomShader(kCCShader_flash);
    CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_flash);
    p->use();
    p->setUniformLocationWith3f(sUniform_pos_CC_flashColor, r, g, b);
    p->setUniformLocationWith1f(sUniform_pos_CC_flashTime, t);
}

NS_CC_END