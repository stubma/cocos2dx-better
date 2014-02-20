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
#include "CCFlash.h"
#include "ccShader_flash_frag.h"
#include "ccShader_flash_vert.h"

NS_CC_BEGIN

#define SHADER_KEY "ccShader_flash"
#define kCCUniformFlashColor "CC_flashColor"
#define kCCUniformFlashTime "CC_flashTime"

static GLint CC_flashColor_pos = -1;
static GLint CC_flashTime_pos = -1;

CCFlash::CCFlash() :
m_oldProgram(NULL) {
    // cache shader if not
    if(!CCShaderCache::sharedShaderCache()->programForKey(SHADER_KEY)) {
        CCGLProgram* p = new CCGLProgram();
        p->initWithVertexShaderByteArray(ccShader_flash_vert, ccShader_flash_frag);
        p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
        p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
        p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
        p->link();
        CC_flashColor_pos = p->getUniformLocationForName(kCCUniformFlashColor);
        CC_flashTime_pos = p->getUniformLocationForName(kCCUniformFlashTime);
        p->updateUniforms();
        p->autorelease();
        CCShaderCache::sharedShaderCache()->addProgram(p, SHADER_KEY);
    }
}

CCFlash::~CCFlash() {
}

CCFlash* CCFlash::create(float duration, ccColor3B c) {
    CCFlash* f = new CCFlash();
    f->initWithDurationAndColor(duration, c);
    return (CCFlash*)f->autorelease();
}

bool CCFlash::initWithDurationAndColor(float d, ccColor3B c) {
    setDuration(d);
    m_color = c;
    
    // float color
    m_r = m_color.r / 255.0f;
    m_g = m_color.g / 255.0f;
    m_b = m_color.b / 255.0f;
    
    return true;
}

void CCFlash::update(float time) {
    // time percentage
    float t;
    if(time > 0.5f) {
        t = 1 - (time - 0.5f) / 0.5f;
    } else {
        t = time / 0.5f;
    }
    
    // shader, set
    CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey(SHADER_KEY);
    p->use();
    p->setUniformLocationWith3f(CC_flashColor_pos, m_r, m_g, m_b);
    p->setUniformLocationWith1f(CC_flashTime_pos, t);
}

void CCFlash::startWithTarget(CCNode *pTarget) {
    CCActionInterval::startWithTarget(pTarget);
    
    // save old program
    if(!m_oldProgram) {
        m_oldProgram = pTarget->getShaderProgram();
    }
    
    // set new program
    pTarget->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(SHADER_KEY));
}

void CCFlash::stop() {
    getTarget()->setShaderProgram(m_oldProgram);
    CCActionInterval::stop();
}

CCActionInterval* CCFlash::reverse() {
    return CCFlash::create(getDuration(), m_color);
}

CCObject* CCFlash::copyWithZone(CCZone *pZone) {
    CCZone* pNewZone = NULL;
    CCFlash* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) {
        // in case of being called at sub class
        pCopy = (CCFlash*)(pZone->m_pCopyObject);
    } else {
        pCopy = new CCFlash();
        pZone = pNewZone = new CCZone(pCopy);
    }
	
    CCActionInterval::copyWithZone(pZone);
    pCopy->initWithDurationAndColor(m_fDuration, m_color);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

NS_CC_END