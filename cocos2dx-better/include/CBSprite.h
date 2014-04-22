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
#ifndef __CBSprite__
#define __CBSprite__

#include "cocos2d.h"
#include "CCMoreMacros.h"

NS_CC_BEGIN

/**
 * This class fixed:
 * 1. sometime I want to keep blend function after change texture, original CCSprite can't
 * 2. add a pre draw function so that you can do something before sprite is render, for example, set
 *   parameters for a custom shader
 *
 * \note
 * Sometimes I find defect in cocos2d-x and want to create a new class to workaround it,
 * so I will choose CB prefix instead of CC. I use Ex suffix before but I don't like it anymore.
 * CB stands cocos2dx-better.
 */
class CC_DLL CBSprite : public CCSprite {
protected:
	CBSprite();
	
public:
	virtual ~CBSprite();
	
	// just copy super
	static CBSprite* create();
    static CBSprite* create(const char *pszFileName);
    static CBSprite* create(const char *pszFileName, const CCRect& rect);
    static CBSprite* createWithTexture(CCTexture2D *pTexture);
    static CBSprite* createWithTexture(CCTexture2D *pTexture, const CCRect& rect);
    static CBSprite* createWithSpriteFrame(CCSpriteFrame *pSpriteFrame);
    static CBSprite* createWithSpriteFrameName(const char *pszSpriteFrameName);
	
    /// to insert a pre draw callback
    virtual void draw();
    
	/**
	 * setTexture will trigger updateBlendFunc. In multipack textures (a feature supported
	 * (in TexturePacker), if frames in an animation cross different texture and sprite use
	 * non-default blend function, the blend function will be reset after setDisplayFrame.
	 * So, the workaround is overriding setTexture and checking a special flag before updating 
	 * blend function.
	 */
	virtual void setTexture(CCTexture2D *texture);
	
	/// true means don't update blend function
	CC_SYNTHESIZE_BOOL(m_shouldUpdateBlendFunc, ShouldUpdateBlendFunc);
    
    /// predraw function
    CC_SYNTHESIZE_RETAIN(CCCallFuncO*, m_preDrawFunction, PreDrawFunction);
};

NS_CC_END

#endif /* defined(__CBSprite__) */
