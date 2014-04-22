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
#include "CBArmature.h"
#include "CBTextureAtlas.h"

NS_CC_BEGIN

CBArmature::CBArmature() :
m_preDrawFunction(NULL) {
}

CBArmature::~CBArmature() {
    CC_SAFE_RELEASE(m_preDrawFunction);
}

// TextureAtlas - alloc & init

CBArmature * CBArmature::create() {
    CBArmature * pArmature = new CBArmature();
    if(pArmature && pArmature->init())
    {
        pArmature->autorelease();
        return pArmature;
    }
    CC_SAFE_DELETE(pArmature);
    return NULL;
}

CBArmature * CBArmature::create(const char *name) {
    CBArmature * pArmature = new CBArmature();
    if (pArmature && pArmature->init(name))
    {
        pArmature->autorelease();
        return pArmature;
    }
    CC_SAFE_DELETE(pArmature);
    return NULL;
}

CBArmature * CBArmature::create(const char *name, CCBone *parentBone) {
    CBArmature * pArmature = new CBArmature();
    if (pArmature && pArmature->init(name, parentBone))
    {
        pArmature->autorelease();
        return pArmature;
    }
    CC_SAFE_DELETE(pArmature);
    return NULL;
}

void CBArmature::draw()
{
    if(m_preDrawFunction)
        m_preDrawFunction->execute();
    
    if (m_pParentBone == NULL && m_pBatchNode == NULL)
    {
        CC_NODE_DRAW_SETUP();
        ccGLBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);
    }
    
    CCObject *object = NULL;
    CCARRAY_FOREACH(m_pChildren, object)
    {
        if (CCBone *bone = dynamic_cast<CCBone *>(object))
        {
            CCNode *node = bone->getDisplayRenderNode();
            
            if (NULL == node)
                continue;
            
            switch (bone->getDisplayRenderNodeType())
            {
                case CS_DISPLAY_SPRITE:
                {
                    CCSkin *skin = (CCSkin *)node;
                    
                    CCTextureAtlas *textureAtlas = skin->getTextureAtlas();
                    bool blendDirty = bone->isBlendDirty();
                    if(m_pAtlas != textureAtlas || blendDirty)
                    {
                        if (m_pAtlas)
                        {
                            if(CBTextureAtlas* pAtlas = dynamic_cast<CBTextureAtlas *>(m_pAtlas)) {
                                pAtlas->drawQuadsEx();
                            } else {
                                m_pAtlas->drawQuads();
                            }
                            m_pAtlas->removeAllQuads();
                        }
                    }
                    
                    m_pAtlas = textureAtlas;
                    if (m_pAtlas->getCapacity() == m_pAtlas->getTotalQuads() && !m_pAtlas->resizeCapacity(m_pAtlas->getCapacity() * 2))
                        return;
                    
                    skin->updateTransform();
                    
                    if (blendDirty)
                    {
                        ccBlendFunc func = bone->getBlendFunc();
                        ccGLBlendFunc(func.src, func.dst);
                        
                        if(CBTextureAtlas* pAtlas = dynamic_cast<CBTextureAtlas *>(m_pAtlas)) {
                            pAtlas->drawQuadsEx();
                        } else {
                            m_pAtlas->drawQuads();
                        }
                        m_pAtlas->removeAllQuads();
                        
                        ccGLBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);
                        bone->setBlendDirty(false);
                    }
                }
                    break;
                case CS_DISPLAY_ARMATURE:
                {
                    CCArmature *armature = (CCArmature *)(node);
                    
                    CCTextureAtlas *textureAtlas = armature->getTextureAtlas();
                    if(m_pAtlas != textureAtlas)
                    {
                        if (m_pAtlas)
                        {
                            if(CBTextureAtlas* pAtlas = dynamic_cast<CBTextureAtlas *>(m_pAtlas)) {
                                pAtlas->drawQuadsEx();
                            } else {
                                m_pAtlas->drawQuads();
                            }
                            m_pAtlas->removeAllQuads();
                        }
                    }
                    armature->draw();
                    
                    m_pAtlas = armature->getTextureAtlas();
                }
                    break;
                default:
                {
                    if (m_pAtlas)
                    {
                        if(CBTextureAtlas* pAtlas = dynamic_cast<CBTextureAtlas *>(m_pAtlas)) {
                            pAtlas->drawQuadsEx();
                        } else {
                            m_pAtlas->drawQuads();
                        }
                        m_pAtlas->removeAllQuads();
                    }
                    node->visit();
                    
                    CC_NODE_DRAW_SETUP();
                    ccGLBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);
                }
                    break;
            }
        }
        else if(CCNode *node = dynamic_cast<CCNode *>(object))
        {
            if (m_pAtlas)
            {
                if(CBTextureAtlas* pAtlas = dynamic_cast<CBTextureAtlas *>(m_pAtlas)) {
                    pAtlas->drawQuadsEx();
                } else {
                    m_pAtlas->drawQuads();
                }
                m_pAtlas->removeAllQuads();
            }
            node->visit();
            
            CC_NODE_DRAW_SETUP();
            ccGLBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);
        }
    }
    
    if(m_pAtlas && !m_pBatchNode && m_pParentBone == NULL)
    {
        if(CBTextureAtlas* pAtlas = dynamic_cast<CBTextureAtlas *>(m_pAtlas)) {
            pAtlas->drawQuadsEx();
        } else {
            m_pAtlas->drawQuads();
        }
        m_pAtlas->removeAllQuads();
    }
}

CCTextureAtlas* CBArmature::getTexureAtlasWithTexture(CCTexture2D *texture) {
    int key = texture->getName();
    
    if (m_pParentBone && m_pParentBone->getArmature())
    {
        return m_pParentBone->getArmature()->getTexureAtlasWithTexture(texture);
    }
    else if (m_pBatchNode)
    {
        m_pBatchNode->getTexureAtlasWithTexture(texture);
    }
    
    CCTextureAtlas *atlas = (CCTextureAtlas *)m_pTextureAtlasDic->objectForKey(key);
    if (atlas == NULL)
    {
        atlas = CBTextureAtlas::createWithTexture(texture, 4);
        m_pTextureAtlasDic->setObject(atlas, key);
    }
    return atlas;
}

NS_CC_END