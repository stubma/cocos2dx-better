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
#ifndef __CBArmature_h__
#define __CBArmature_h__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC_EXT;

NS_CC_BEGIN

/**
 * to fix
 * 1. use CBTextureAtlas to avoid vbo compatibility issue
 * 2. add a pre draw function so that you can do something before sprite is render, for example, set
 *   parameters for a custom shader
 */
class CC_DLL CBArmature : public CCArmature {
public:
    
    /**
     * Allocates and initializes an armature.
     * @return An initialized armature which is marked as "autorelease".
     */
    static CBArmature *create();
    
    /**
     * Allocates an armature, and use the CCArmatureData named name in CCArmatureDataManager to initializes the armature.
     *
     * @param  name CCArmature will use the name to find the CCArmatureData to initializes it.
     * @return A initialized armature which is marked as "autorelease".
     */
    static CBArmature *create(const char *name);
    
    static CBArmature *create(const char *name, CCBone *parentBone);
    
public:
    /**
     *  @js ctor
     */
    CBArmature();
    /**
     *  @js NA
     */
    ~CBArmature(void);
    
    virtual void draw();
    
    virtual CCTextureAtlas* getTexureAtlasWithTexture(CCTexture2D *texture);
    
    /// predraw function
    CC_SYNTHESIZE_RETAIN(CCCallFuncO*, m_preDrawFunction, PreDrawFunction);
};

NS_CC_END

#endif //__CBArmature_h__

