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

#include "CCImage_colorlabel.h"
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
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

static CGSize _calculateStringSize(NSString *str, id font, CGSize *constrainSize) {
    NSArray *listItems = [str componentsSeparatedByString: @"\n"];
    CGSize dim = CGSizeZero;
    CGSize textRect = CGSizeZero;
    textRect.width = constrainSize->width > 0 ? constrainSize->width : 0x7fffffff;
    textRect.height = constrainSize->height > 0 ? constrainSize->height : 0x7fffffff;
    
    for (NSString *s in listItems) {
        CGSize tmp = [s sizeWithFont:font constrainedToSize:textRect];
        
        if (tmp.width > dim.width) {
			dim.width = tmp.width;
        }
        
        dim.height += tmp.height;
    }
    
    return dim;
}

static int parseColor(const char* p, int len) {
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

static char* buildSpan(const char* pText, SpanList& spans) {
	bool inSpan = false;
	int spanStart = 0;
	int plainLen = 0;
	int richLen = strlen(pText);
	char* plain = (char*)malloc(sizeof(char) * (richLen + 1));
	for(int i = 0; i < richLen; i++) {
		char c = pText[i];
		switch(c) {
			case '\\':
				if(i < richLen - 1) {
					char cc = pText[i + 1];
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
				for(j = i + 1; j < richLen; j++) {
					char cc = pText[j];
					if(cc == ']') {
						break;
					}
				}
				
				// if no close bracket, discard those content
				if(j >= richLen) {
					i = j;
					break;
				}
				
				// if find, we need check whether we are in a span
				if(inSpan) {
					if(CCUtils::startsWith(pText + i, "[/color]")) {
						inSpan = false;
						spanStart = plainLen;
					}
					
					// reset i to skip this part
					i = j;
				} else {
					// parse this span, if not recognized, need ignore
					if(CCUtils::startsWith(pText + i + 1, "color=")) {
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
						int color = parseColor(pText + i + 7, j - i - 7);
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
	if(spanStart < richLen && plainLen > spanStart) {
		Span span = {
			spanStart,
			plainLen,
			0xffffffff
		};
		spans.push_back(span);
	}
	
	// end of plain
	plain[plainLen] = 0;
	
#ifdef COCOS2D_DEBUG
	// debug output span info
	int i = 0;
	for(SpanList::iterator iter = spans.begin(); iter != spans.end(); iter++) {
		Span& span = *iter;
		CCLOG("span %d: %d - %d, color: 0x%08x", i++, span.start, span.end, span.color);
	}
#endif
	
	// return plain str
	return plain;
}

static bool _initWithString(const char * pText, CCImage::ETextAlign eAlign, const char * pFontName, int nSize, tImageInfo* pInfo) {
    bool bRet = false;
    do {
        CC_BREAK_IF(!pText || !pInfo);
        
		SpanList spans;
        char* plain = buildSpan(pText, spans);
        NSString* plainStr = [NSString stringWithUTF8String:plain];
        NSString* fntName = [NSString stringWithUTF8String:pFontName];
        
        CGSize dim, constrainSize;
        
        constrainSize.width     = pInfo->width;
        constrainSize.height    = pInfo->height;
        
        // On iOS custom fonts must be listed beforehand in the App info.plist (in order to be usable) and referenced only the by the font family name itself when
        // calling [UIFont fontWithName]. Therefore even if the developer adds 'SomeFont.ttf' or 'fonts/SomeFont.ttf' to the App .plist, the font must
        // be referenced as 'SomeFont' when calling [UIFont fontWithName]. Hence we strip out the folder path components and the extension here in order to get just
        // the font family name itself. This stripping step is required especially for references to user fonts stored in CCB files; CCB files appear to store
        // the '.ttf' extensions when referring to custom fonts.
        fntName = [[fntName lastPathComponent] stringByDeletingPathExtension];
        
        // create the font
        id font = [UIFont fontWithName:fntName size:nSize];
        
        if (font) {
            dim = _calculateStringSize(plainStr, font, &constrainSize);
        } else {
            if (!font) {
                font = [UIFont systemFontOfSize:nSize];
            }
			
            if (font) {
                dim = _calculateStringSize(plainStr, font, &constrainSize);
            }
        }
		
        CC_BREAK_IF(!font);
        
        // compute start point
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
        
        // draw text
        CGColorSpaceRef colorSpace  = CGColorSpaceCreateDeviceRGB();
        CGContextRef context        = CGBitmapContextCreate(data,
                                                            dim.width,
                                                            dim.height,
                                                            8,
                                                            dim.width * 4,
                                                            colorSpace,
                                                            kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
        
        CGColorSpaceRelease(colorSpace);
        
        if (!context) {
            delete[] data;
            break;
        }
		
        // move Y rendering to the top of the image
        CGContextTranslateCTM(context, 0.0f, (dim.height - shadowStrokePaddingY) );
        CGContextScaleCTM(context, 1.0f, -1.0f); //NOTE: NSString draws in UIKit referential i.e. renders upside-down compared to CGBitmapContext referential
        
        // store the current context
        UIGraphicsPushContext(context);
        
        // measure text size with specified font and determine the rectangle to draw text in
        unsigned uHoriFlag = eAlign & 0x0f;
		UITextAlignment align = UITextAlignmentLeft;
		switch(uHoriFlag) {
			case 2:
				align = UITextAlignmentRight;
				break;
			case 3:
				align = UITextAlignmentCenter;
				break;
			default:
				align = UITextAlignmentLeft;
				break;
		}
		
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
        
        // compute the rect used for rendering the text
        // based on wether shadows or stroke are enabled
        float textOriginX  = 0.0;
        float textOriginY = 0.0;
        float textWidth    = dim.width  - shadowStrokePaddingX;
        float textHeight   = dim.height - shadowStrokePaddingY;
        if (pInfo->shadowOffset.width < 0) {
            textOriginX = shadowStrokePaddingX;
        } else {
            textOriginX = 0.0;
        }
        if (pInfo->shadowOffset.height > 0) {
            textOriginY = startH;
        } else {
            textOriginY = startH - shadowStrokePaddingY;
        }
		
		// actually draw the text in the context
		// it depends on how many spans are there
		if(spans.size() == 1) {
			// set color
			Span& span = *spans.begin();
			if(span.color == 0xffffffff) {
				CGContextSetRGBFillColor(context, pInfo->tintColorR, pInfo->tintColorG, pInfo->tintColorB, 1);
			} else {
				CGContextSetRGBFillColor(context, ((span.color >> 16) & 0xff) / 255,
										 ((span.color >> 8) & 0xff) / 255,
										 (span.color & 0xff) / 255,
										 ((span.color >> 24) & 0xff) / 255);
			}
			
			[plainStr drawInRect:CGRectMake(textOriginX, textOriginY, textWidth, textHeight)
                        withFont:font
                   lineBreakMode:(UILineBreakMode)UILineBreakModeWordWrap
                       alignment:align];
		} else {
			float x = textOriginX;
			float y = textOriginY;
			for(SpanList::iterator iter = spans.begin(); iter != spans.end(); iter++) {
				Span& span = *iter;
				
				// set color
				if(span.color == 0xffffffff) {
					CGContextSetRGBFillColor(context, pInfo->tintColorR, pInfo->tintColorG, pInfo->tintColorB, 1);
				} else {
					CGContextSetRGBFillColor(context, ((span.color >> 16) & 0xff) / 255,
											 ((span.color >> 8) & 0xff) / 255,
											 (span.color & 0xff) / 255,
											 ((span.color >> 24) & 0xff) / 255);
				}
				
				// sub string
                NSString* sub = [[[NSString alloc] initWithBytes:plain + span.start
                                                          length:span.end - span.start
                                                        encoding:NSUTF8StringEncoding] autorelease];
				
				// get sub string size
				CGSize size = _calculateStringSize(sub, font, &constrainSize);
				
				// draw sub string
				[sub drawInRect:CGRectMake(x, y, size.width, size.height)
					   withFont:font
				  lineBreakMode:(UILineBreakMode)UILineBreakModeWordWrap
					  alignment:align];
				
				// increase
				x += size.width;
			}
		}
        
        // pop the context
        UIGraphicsPopContext();
        
        // release the context
        CGContextRelease(context);
		
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

CCImage_colorlabel::CCImage_colorlabel() {
}

CCImage_colorlabel::~CCImage_colorlabel() {
}

bool CCImage_colorlabel::initWithRichStringShadowStroke(const char * pText,
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