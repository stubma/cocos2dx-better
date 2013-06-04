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
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#include "CCImage_richlabel.h"
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <CoreText/CoreText.h>
#include <math.h>

/// span
typedef struct Span {
	// start char
	int start;
	
	// end char, exclusive
	int end;
	
	// color
	int color;
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
    float        shadowOpacity;
    bool         hasStroke;
    float        strokeColorR;
    float        strokeColorG;
    float        strokeColorB;
    float        strokeSize;
    float        tintColorR;
    float        tintColorG;
    float        tintColorB;
    unsigned char*  data;
} tImageInfo;

// alignment
#define ALIGN_TOP    1
#define ALIGN_CENTER 3
#define ALIGN_BOTTOM 2

static int parseColor(const unichar* p, int len) {
	int color = 0;
	for(int i = 0; i < len; i++) {
		color <<= 4;
		char c = p[i];
		if(c >= '0' && c <= '9') {
			color |= c - '0';
		} else if(c >= 'a' && c <= 'f') {
			color |= c - 'a' + 10;
		} else if(c >= 'A' && c <= 'F') {
			color |= c - 'A' + 10;
		}
	}
	
	return color;
}

static unichar* buildSpan(const char* pText, SpanList& spans, int* outLen) {
    // get unichar of string
    NSString* ns = [NSString stringWithUTF8String:pText];
    int uniLen = [ns length];
    unichar* uniText = (unichar*)malloc(uniLen * sizeof(unichar));
    [ns getCharacters:uniText];
    
	bool inSpan = false;
	int spanStart = 0;
	int plainLen = 0;
	unichar* plain = (unichar*)malloc(sizeof(unichar) * uniLen);
	for(int i = 0; i < uniLen; i++) {
		unichar c = uniText[i];
		switch(c) {
			case '\\':
				if(i < uniLen - 1) {
					unichar cc = uniText[i + 1];
					if(cc == '[' || cc == ']') {
						plain[plainLen++] = cc;
						i++;
					}
				} else {
					plain[plainLen++] = c;
				}
				break;
			case '[':
			{
				// find close bracket
				int j;
				for(j = i + 1; j < uniLen; j++) {
					unichar cc = uniText[j];
					if(cc == ']') {
						break;
					}
				}
				
				// if no close bracket, discard those content
				if(j >= uniLen) {
					i = j;
					break;
				}
				
				// if find, we need check whether we are in a span
				if(inSpan) {
					if(uniText[i] == '['
                       && uniText[i + 1] == '/'
                       && uniText[i + 2] == 'c'
                       && uniText[i + 3] == 'o'
                       && uniText[i + 4] == 'l'
                       && uniText[i + 5] == 'o'
                       && uniText[i + 6] == 'r'
                       && uniText[i + 7] == ']') {
						inSpan = false;
						spanStart = plainLen;
					}
					
					// reset i to skip this part
					i = j;
				} else {
					// parse this span, if not recognized, need ignore
					if(uniText[i + 1] == 'c'
                       && uniText[i + 2] == 'o'
                       && uniText[i + 3] == 'l'
                       && uniText[i + 4] == 'o'
                       && uniText[i + 5] == 'r'
                       && uniText[i + 6] == '=') {
						// add previous span
						Span span;
						if(plainLen > spanStart) {
							span.start = spanStart;
							span.end = plainLen;
							span.color = 0xffffffff;
							spans.push_back(span);
						}
						spanStart = plainLen;
						
						// add color span
						int color = parseColor(uniText + i + 7, j - i - 7);
						span.start = spanStart;
						span.end = spanStart;
						span.color = color;
						spans.push_back(span);
						
						// set flag
						inSpan = true;
					}
					
					// reset i to skip this part
					i = j;
				}
				
				break;
			}
			case ']':
				// just discard it
				break;
			default:
				plain[plainLen++] = c;
				
				// if in span, update last span end position
				if(inSpan) {
					Span& span = *spans.rbegin();
					span.end++;
				}
				break;
		}
	}
	
	// last span
	if(spanStart < uniLen && plainLen > spanStart) {
		Span span = {
			spanStart,
			plainLen,
			0xffffffff
		};
		spans.push_back(span);
	}
	
	// end of plain
	plain[plainLen] = 0;
    if(outLen)
        *outLen = plainLen;
	
#ifdef COCOS2D_DEBUG
	// debug output span info
	int i = 0;
	for(SpanList::iterator iter = spans.begin(); iter != spans.end(); iter++) {
		Span& span = *iter;
		CCLOG("span %d: %d - %d, color: 0x%08x", i++, span.start, span.end, span.color);
	}
#endif
    
    // release
    free(uniText);
	
	// return plain str
	return plain;
}

static bool _initWithString(const char * pText, CCImage::ETextAlign eAlign, const char * pFontName, int nSize, tImageInfo* pInfo) {
    bool bRet = false;
    do {
        CC_BREAK_IF(!pText || !pInfo);
        
        // On iOS custom fonts must be listed beforehand in the App info.plist (in order to be usable) and referenced only the by the font family name itself when
        // calling [UIFont fontWithName]. Therefore even if the developer adds 'SomeFont.ttf' or 'fonts/SomeFont.ttf' to the App .plist, the font must
        // be referenced as 'SomeFont' when calling [UIFont fontWithName]. Hence we strip out the folder path components and the extension here in order to get just
        // the font family name itself. This stripping step is required especially for references to user fonts stored in CCB files; CCB files appear to store
        // the '.ttf' extensions when referring to custom fonts.
        NSString* fntName = [NSString stringWithUTF8String:pFontName];
        fntName = [[fntName lastPathComponent] stringByDeletingPathExtension];
        
        // create the font
        UIFont* font = [UIFont fontWithName:fntName size:nSize];
        CC_BREAK_IF(!font);
        
        // create ct font
        CTFontRef ctFont = CTFontCreateWithName((CFStringRef)font.fontName, nSize, NULL);

        // get plain text and extract span list
		SpanList spans;
        int uniLen;
        unichar* plain = buildSpan(pText, spans, &uniLen);
        
        // create attributed string
        CFStringRef plainCFStr = CFStringCreateWithCharacters(kCFAllocatorDefault,
                                                              (const UniChar*)plain,
                                                              uniLen);
        CFMutableAttributedStringRef plainCFAStr = CFAttributedStringCreateMutable(kCFAllocatorDefault, 0);
        CFAttributedStringReplaceString(plainCFAStr, CFRangeMake(0, 0), plainCFStr);
        
        // set font
        CFAttributedStringSetAttribute(plainCFAStr,
                                       CFRangeMake(0, CFAttributedStringGetLength(plainCFAStr)),
                                       kCTFontAttributeName,
                                       ctFont);
        
        // set default color
        CGColorSpaceRef colorSpace  = CGColorSpaceCreateDeviceRGB();
        CGFloat defaultColorComp[] = {
            pInfo->tintColorR, pInfo->tintColorG, pInfo->tintColorB, 1
        };
        CGColorRef defaultColor = CGColorCreate(colorSpace, defaultColorComp);
        CFAttributedStringSetAttribute(plainCFAStr,
                                       CFRangeMake(0, CFAttributedStringGetLength(plainCFAStr)),
                                       kCTForegroundColorAttributeName,
                                       defaultColor);
        CGColorRelease(defaultColor);
        
        // set span color
        for(SpanList::iterator iter = spans.begin(); iter != spans.end(); iter++) {
            Span& span = *iter;
            if(span.color != 0xffffffff) {
                CGFloat comp[] = {
                    ((span.color >> 16) & 0xff) / 255,
                    ((span.color >> 8) & 0xff) / 255,
                    (span.color & 0xff) / 255,
                    ((span.color >> 24) & 0xff) / 255
                };
                CGColorRef fc = CGColorCreate(colorSpace, comp);
                CFAttributedStringSetAttribute(plainCFAStr,
                                               CFRangeMake(span.start, span.end - span.start),
                                               kCTForegroundColorAttributeName,
                                               fc);
                CGColorRelease(fc);
            }
        }
        
        // set paragraph style, including line spacing and alignment
        CTTextAlignment alignment = kCTLeftTextAlignment;
        switch(eAlign & 0x0f) {
            case 2:
                alignment = kCTRightTextAlignment;
                break;
            case 3:
                alignment = kCTCenterTextAlignment;
                break;
            default:
                break;
        }
        float space = 0;
        CTParagraphStyleSetting paraSettings[] = {
            { kCTParagraphStyleSpecifierAlignment, sizeof(alignment), &alignment},
            { kCTParagraphStyleSpecifierLineSpacingAdjustment, sizeof(CGFloat), &space },
            { kCTParagraphStyleSpecifierMaximumLineSpacing, sizeof(CGFloat), &space },
            { kCTParagraphStyleSpecifierMinimumLineSpacing, sizeof(CGFloat), &space }
        };
        CTParagraphStyleRef paraStyle = CTParagraphStyleCreate(paraSettings,
                                                               sizeof(paraSettings) / sizeof(paraSettings[0]));
        CFAttributedStringSetAttribute(plainCFAStr,
                                       CFRangeMake(0, CFAttributedStringGetLength(plainCFAStr)),
                                       kCTParagraphStyleAttributeName,
                                       paraStyle);
        
        // create frame setter
        CTFramesetterRef fs = CTFramesetterCreateWithAttributedString(plainCFAStr);
        
        // constrain size
        CFRange range;
        CGSize constrainSize = CGSizeMake(pInfo->width, pInfo->height);
        CGSize ctConstrainSize = constrainSize;
        if(ctConstrainSize.width <= 0) {
            ctConstrainSize.width = CGFLOAT_MAX;
        }
        if(ctConstrainSize.height <= 0) {
            ctConstrainSize.height = CGFLOAT_MAX;
        }
        CGSize dim = CTFramesetterSuggestFrameSizeWithConstraints(fs,
                                                                  CFRangeMake(0, 0),
                                                                  NULL,
                                                                  ctConstrainSize,
                                                                  &range);
        dim.width = ceilf(dim.width);
        dim.height = ceilf(dim.height);
        
        // vertical alignment
        int startH = 0;
        if (constrainSize.height > dim.height) {
            // vertical alignment
            unsigned int vAlignment = (eAlign >> 4) & 0x0F;
            if (vAlignment == ALIGN_TOP) {
                startH = 0;
            } else if (vAlignment == ALIGN_CENTER) {
                startH = (constrainSize.height - dim.height) / 2;
            } else {
                startH = constrainSize.height - dim.height;
            }
        }
        
        // adjust text rect
        if (constrainSize.width > 0 && constrainSize.width > dim.width) {
            dim.width = constrainSize.width;
        }
        if (constrainSize.height > 0 && constrainSize.height > dim.height) {
            dim.height = constrainSize.height;
        }
        
        // create frame
        CGMutablePathRef path = CGPathCreateMutable();
        CGPathAddRect(path, NULL, CGRectMake(0, 0, dim.width, dim.height));
        CTFrameRef frame = CTFramesetterCreateFrame(fs,
                                                    CFRangeMake(0, 0),
                                                    path,
                                                    NULL);
        
        // compute the padding needed by shadow and stroke
        float shadowStrokePaddingX = 0.0f;
        float shadowStrokePaddingY = 0.0f;
        if (pInfo->hasStroke) {
            shadowStrokePaddingX = ceilf(pInfo->strokeSize);
            shadowStrokePaddingY = ceilf(pInfo->strokeSize);
        }
        if (pInfo->hasShadow) {
            shadowStrokePaddingX = MAX(shadowStrokePaddingX, (float)fabs(pInfo->shadowOffset.width));
            shadowStrokePaddingY = MAX(shadowStrokePaddingY, (float)fabs(pInfo->shadowOffset.height));
        }
        
        // add the padding (this could be 0 if no shadow and no stroke)
        dim.width  += shadowStrokePaddingX;
        dim.height += shadowStrokePaddingY;
        
		// allocate data for bitmap
        unsigned char* data = new unsigned char[(int)(dim.width * dim.height * 4)];
        memset(data, 0, (int)(dim.width * dim.height * 4));
        
        // create context
        CGContextRef context = CGBitmapContextCreate(data,
                                                     dim.width,
                                                     dim.height,
                                                     8,
                                                     dim.width * 4,
                                                     colorSpace,
                                                     kCGImageAlphaPremultipliedLast |kCGBitmapByteOrder32Big);
        
        // if fail, release
        // if ok, draw text by core text
        if (!context) {
            delete[] data;
        } else {
            // store the current context
            UIGraphicsPushContext(context);
            
            // alow anti-aliasing
            CGContextSetAllowsAntialiasing(context, YES);
            
            // take care of stroke if needed
            if (pInfo->hasStroke) {
                CGContextSetTextDrawingMode(context, kCGTextFillStroke);
                CGContextSetRGBStrokeColor(context, pInfo->strokeColorR, pInfo->strokeColorG, pInfo->strokeColorB, 1);
                CGContextSetLineWidth(context, pInfo->strokeSize);
            }
            
            // take care of shadow if needed
            if (pInfo->hasShadow) {
                CGSize offset;
                offset.height = pInfo->shadowOffset.height;
                offset.width  = pInfo->shadowOffset.width;
                CGContextSetShadow(context, offset, pInfo->shadowBlur);
            }
            
            // set color
            CGContextSetRGBFillColor(context, pInfo->tintColorR, pInfo->tintColorG, pInfo->tintColorB, 1);
            
            // vertical alignment
            CGContextTranslateCTM(context, 0, -startH);
            
            // draw frame
            CTFrameDraw(frame, context);
                        
            // pop the context
            UIGraphicsPopContext();
            
            // release
            CGContextRelease(context);
        }
        
        // release
        CGColorSpaceRelease(colorSpace);
        CFRelease(plainCFStr);
        CFRelease(plainCFAStr);
        CFRelease(fs);
        CFRelease(ctFont);
        CFRelease(frame);
        CFRelease(path);
        CFRelease(paraStyle);
		
        // output params
        pInfo->data                 = data;
        pInfo->hasAlpha             = true;
        pInfo->isPremultipliedAlpha = true;
        pInfo->bitsPerComponent     = 8;
        pInfo->width                = dim.width;
        pInfo->height               = dim.height;
        bRet                        = true;
    } while(0);
	
    return bRet;
}

NS_CC_BEGIN

CCImage_richlabel::CCImage_richlabel() {
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
														float shadowOpacity,
														float shadowBlur,
														bool  stroke,
														float strokeR,
														float strokeG,
														float strokeB,
														float strokeSize) {
    tImageInfo info = {0};
    info.width                  = nWidth;
    info.height                 = nHeight;
    info.hasShadow              = shadow;
    info.shadowOffset.width     = shadowOffsetX;
    info.shadowOffset.height    = shadowOffsetY;
    info.shadowBlur             = shadowBlur;
    info.shadowOpacity          = shadowOpacity;
    info.hasStroke              =  stroke;
    info.strokeColorR           =  strokeR;
    info.strokeColorG           = strokeG;
    info.strokeColorB           = strokeB;
    info.strokeSize             = strokeSize;
    info.tintColorR             = textTintR;
    info.tintColorG             = textTintG;
    info.tintColorB             = textTintB;
    
    
    if (! _initWithString(pText, eAlignMask, pFontName, nSize, &info))
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

NS_CC_END

#endif // #if CC_TARGET_PLATFORM == CC_PLATFORM_IOS