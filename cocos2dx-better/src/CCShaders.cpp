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
#include "ccShader_blur_vert.h"
#include "ccShader_blur_frag.h"
#include "ccShader_laser_vert.h"
#include "ccShader_laser_frag.h"
#include "ccShader_lighting_frag.h"
#include "ccShader_lighting_vert.h"

#define LOAD_PROGRAM_IF(name) \
	if(key == kCCShader_##name) \
		p->initWithVertexShaderByteArray(ccShader_##name##_vert, ccShader_##name##_frag)
#define ADD_UNIFORM(name) \
	sUniform_pos_CC_##name = p->getUniformLocationForName(kCCUniform_##name)

NS_CC_BEGIN

void CCShaders::loadCustomShader(const string& key) {
    if(!CCShaderCache::sharedShaderCache()->programForKey(key.c_str())) {
		// load shader
        CCGLProgram* p = new CCGLProgram();
		p->autorelease();
		LOAD_PROGRAM_IF(flash);
		LOAD_PROGRAM_IF(blur);
		LOAD_PROGRAM_IF(laser);
        LOAD_PROGRAM_IF(lighting);
		
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
			ADD_UNIFORM(flashColor);
			ADD_UNIFORM(flashTime);
		} else if(key == kCCShader_blur) {
			ADD_UNIFORM(blurSize);
			ADD_UNIFORM(blurSubtract);
		} else if(key == kCCShader_lighting) {
            ADD_UNIFORM(lightingMul);
            ADD_UNIFORM(lightingAdd);
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

void CCShaders::setBlur(CCSize nodeSize, CCSize blurSize, ccColor4F blurSubtract) {
	loadCustomShader(kCCShader_blur);
    CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_blur);
    p->use();
	p->setUniformLocationWith2f(sUniform_pos_CC_blurSize, blurSize.width / nodeSize.width, blurSize.height / nodeSize.height);
	p->setUniformLocationWith4f(sUniform_pos_CC_blurSubtract, blurSubtract.r, blurSubtract.g, blurSubtract.b, blurSubtract.a);
}

void CCShaders::setLighting(ccColor4B mul, ccColor3B add) {
    loadCustomShader(kCCShader_lighting);
    CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_lighting);
    p->use();
	p->setUniformLocationWith4f(sUniform_pos_CC_lightingMul, mul.r / 255.0f, mul.g / 255.0f, mul.b / 255.0f, mul.a / 255.0f);
	p->setUniformLocationWith3f(sUniform_pos_CC_lightingAdd, add.r / 255.0f, add.g / 255.0f, add.b / 255.0f);
}

NS_CC_END