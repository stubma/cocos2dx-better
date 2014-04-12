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
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC

#include "CCImage_richlabel.h"
#import <Foundation/Foundation.h>
#import <CoreText/CoreText.h>
#import <AppKit/AppKit.h>
#include <math.h>

// tag char
#define TAG_START '['
#define TAG_END ']'

// span type
typedef enum {
    UNKNOWN,
    COLOR,
    FONT,
    SIZE,
    BOLD,
    ITALIC,
    UNDERLINE,
    IMAGE,
    LINK
} SpanType;

/// span
typedef struct Span {
    // span type
    SpanType type;
    
    // close tag?
    bool close;
    
	// pos in plain text
    // for close tag, it is exclusive, i.e., it is next char
    int pos;
	
	// only used for color
    int color;
    
    // only used for size
    int fontSize;
    
    // only used for font
    char* fontName;
    
    // only used for image
    // offsetY follow opengl rule
    char* imageName;
	char* plist;
	char* atlas;
    float scaleX;
	float scaleY;
	float width;
	float height;
    float offsetY;
	CCSpriteFrame* frame; // only used when image is in a atlas
    
    // for link tag
    int normalBgColor;
    int selectedBgColor;
} Span;
typedef vector<Span> SpanList;

typedef struct {
    unsigned int height;
    unsigned int width;
    int          bitsPerComponent;
    bool         hasAlpha;
    bool         isPremultipliedAlpha;
    bool         hasShadow;
    CGSize       shadowOffset;
    float        shadowBlur;
    int          shadowColor;
    bool         hasStroke;
    float        strokeColorR;
    float        strokeColorG;
    float        strokeColorB;
    float        strokeSize;
    float        tintColorR;
    float        tintColorG;
    float        tintColorB;
    unsigned char*  data;
    
    // true indicating only doing measurement
    bool sizeOnly;
} tImageInfo;

static bool _initWithString(const char * pText, CCImage::ETextAlign eAlign, const char * pFontName, int nSize, tImageInfo* pInfo, CCPoint* outShadowStrokePadding, LinkMetaList* linkMetas, vector<CCRect>* imageRects, CC_DECRYPT_FUNC decryptFunc) {
    // TODO: not implemented
    return false;
}

NS_CC_BEGIN

CCImage_richlabel::CCImage_richlabel() :
m_shadowStrokePadding(CCPointZero) {
}

CCImage_richlabel::~CCImage_richlabel() {
}

bool CCImage_richlabel::initWithRichStringShadowStroke(const char * pText,
														int         nWidth,
														int         nHeight,
														ETextAlign eAlignMask,
														const char * pFontName,
														int         nSize,
														float       textTintR,
														float       textTintG,
														float       textTintB,
														bool shadow,
														float shadowOffsetX,
														float shadowOffsetY,
                                                        int   shadowColor,
														float shadowBlur,
														bool  stroke,
														float strokeR,
														float strokeG,
														float strokeB,
														float strokeSize,
														CC_DECRYPT_FUNC decryptFunc) {
    tImageInfo info = {0};
    info.width                  = nWidth;
    info.height                 = nHeight;
    info.hasShadow              = shadow;
    info.shadowOffset.width     = shadowOffsetX;
    info.shadowOffset.height    = shadowOffsetY;
    info.shadowBlur             = shadowBlur;
    info.shadowColor            = shadowColor;
    info.hasStroke              =  stroke;
    info.strokeColorR           =  strokeR;
    info.strokeColorG           = strokeG;
    info.strokeColorB           = strokeB;
    info.strokeSize             = strokeSize;
    info.tintColorR             = textTintR;
    info.tintColorG             = textTintG;
    info.tintColorB             = textTintB;
    info.sizeOnly = false;
    
    if (!_initWithString(pText, eAlignMask, pFontName, nSize, &info, &m_shadowStrokePadding, &m_linkMetas, &m_imageRects, decryptFunc))
    {
        return false;
    }
    m_nHeight = (short)info.height;
    m_nWidth = (short)info.width;
    m_nBitsPerComponent = info.bitsPerComponent;
    m_bHasAlpha = info.hasAlpha;
    m_bPreMulti = info.isPremultipliedAlpha;
    m_pData = info.data;
    
    return true;
}

CCSize CCImage_richlabel::measureRichString(const char* pText,
                                            const char* pFontName,
                                            int nSize,
                                            int maxWidth,
                                            float shadowOffsetX,
                                            float shadowOffsetY,
                                            float strokeSize,
											CC_DECRYPT_FUNC decryptFunc) {
    tImageInfo info = {0};
    info.width                  = maxWidth;
    info.height                 = 0;
    info.hasShadow              = shadowOffsetX != 0 || shadowOffsetY != 0;
    info.shadowOffset.width     = shadowOffsetX;
    info.shadowOffset.height    = shadowOffsetY;
    info.shadowBlur             = 0;
    info.shadowColor            = 0;
    info.hasStroke              = strokeSize != 0;
    info.strokeColorR           = 0;
    info.strokeColorG           = 0;
    info.strokeColorB           = 0;
    info.strokeSize             = strokeSize;
    info.tintColorR             = 0;
    info.tintColorG             = 0;
    info.tintColorB             = 0;
    info.sizeOnly = true;
    
    if (!_initWithString(pText, kAlignCenter, pFontName, nSize, &info, NULL, NULL, NULL, decryptFunc)) {
        return CCSizeZero;
    }
    return CCSizeMake(info.width, info.height);
}

NS_CC_END

#endif // #if CC_TARGET_PLATFORM == CC_PLATFORM_MAC