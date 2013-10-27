/****************************************************************************
 Author: Luma (stubma@gmail.com)
 
 https://github.com/stubma/cocos2dx-common
 
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
#ifndef __CCSpriteEx__
#define __CCSpriteEx__

#include "cocos2d.h"

NS_CC_BEGIN

/**
 * In general, class with Ex suffix is my workaround for cocos2d design defect
 */
class CC_DLL CCSpriteEx : public CCSprite {
protected:
	CCSpriteEx();
	
public:
	virtual ~CCSpriteEx();
	
	// just copy super
	static CCSpriteEx* create();
    static CCSpriteEx* create(const char *pszFileName);
    static CCSpriteEx* create(const char *pszFileName, const CCRect& rect);
    static CCSpriteEx* createWithTexture(CCTexture2D *pTexture);
    static CCSpriteEx* createWithTexture(CCTexture2D *pTexture, const CCRect& rect);
    static CCSpriteEx* createWithSpriteFrame(CCSpriteFrame *pSpriteFrame);
    static CCSpriteEx* createWithSpriteFrameName(const char *pszSpriteFrameName);
	
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
};

NS_CC_END

#endif /* defined(__CCSpriteEx__) */
