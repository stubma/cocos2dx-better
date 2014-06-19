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
#ifndef __CCTexture2D_richlabel__
#define __CCTexture2D_richlabel__

#include "cocos2d.h"
#include "ccMoreTypes.h"
#include "CCImage_richlabel.h"
#include "CCMoreMacros.h"

NS_CC_BEGIN

/**
 * Customized CCTexture2D for color label implementation
 */
class CCTexture2D_richlabel : public CCTexture2D {
public:
    CCTexture2D_richlabel();
    virtual ~CCTexture2D_richlabel();
	
    const char* description(void);
	
	/** Initializes a texture from a string with dimensions, alignment, font name and font size */
    bool initWithRichString(const char *text,  const char *fontName, float fontSize, const CCSize& dimensions, CCTextAlignment hAlignment, CCVerticalTextAlignment vAlignment);
	
    /** Initializes a texture from a string with font name and font size */
    bool initWithRichString(const char *text, const char *fontName, float fontSize);
	
    /** Initializes a texture from a string using a text definition*/
    bool initWithRichString(const char *text, ccRichFontDefinition *textDefinition);
	
	/// shadow and stroke padding value
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(CCPoint, m_shadowStrokePadding, ShadowStrokePadding);
	
	/// link meta list
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(LinkMetaList, m_linkMetas, LinkMetas);
	
	/// image meta list
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(vector<CCRect>, m_imageRects, ImageRects);
    
    /// length of unstyled string
    CC_SYNTHESIZE(int, m_realLength, RealLength);
    
    /// true means this label has continuous effect, so we need pass elapsed time to update it
    CC_SYNTHESIZE_BOOL(m_needTime, NeedTime);
};

NS_CC_END

#endif /* defined(__CCTexture2D_richlabel__) */
