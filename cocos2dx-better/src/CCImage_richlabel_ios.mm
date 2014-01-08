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
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#include "CCImage_richlabel.h"
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <CoreText/CoreText.h>
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
    char* imageName;
    float scaleX;
	float scaleY;
	float width;
	float height;
    
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

////////////////////////////////////////
// run delegate callback

// key is NSString, value is UIImage
static NSMutableDictionary* s_imageMap = [[NSMutableDictionary alloc] init];

static CGFloat getAscent(void* refCon) {
    Span& span = *(Span*)refCon;
    NSString* name = [NSString stringWithCString:span.imageName
                                        encoding:NSUTF8StringEncoding];
    UIImage* image = [s_imageMap objectForKey:name];
    return span.height != 0 ? span.height : (image.size.height * span.scaleY);
}

static CGFloat getDescent(void* refCon) {
    return 0;
}

static CGFloat getWidth(void* refCon) {
    Span& span = *(Span*)refCon;
    NSString* name = [NSString stringWithCString:span.imageName
                                        encoding:NSUTF8StringEncoding];
    UIImage* image = [s_imageMap objectForKey:name];
    return span.width != 0 ? span.width : (image.size.width * span.scaleX);
}

static CTRunDelegateCallbacks s_runDelegateCallbacks = {
    kCTRunDelegateVersion1,
    NULL,
    getAscent,
    getDescent,
    getWidth
};

///////////////////////////////////////

// alignment
#define ALIGN_TOP    1
#define ALIGN_CENTER 3
#define ALIGN_BOTTOM 2

// tag parsing state
typedef enum {
    READY,
    START_TAG,
    CLOSE_TAG,
    EQUAL,
    SUCCESS,
    FAIL
} TagParseState;

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

static SpanType checkTagName(unichar* p, int start, int end) {
    int len = end - start;
    switch(len) {
        case 1:
            if(p[start] == 'b') {
                return BOLD;
            } else if(p[start] == 'i') {
                return ITALIC;
            } else if(p[start] == 'u') {
                return UNDERLINE;
            }
            break;
        case 4:
            if(p[start] == 'f' &&
               p[start + 1] == 'o' &&
               p[start + 2] == 'n' &&
               p[start + 3] == 't') {
                return FONT;
            } else if(p[start] == 's' &&
                      p[start + 1] == 'i' &&
                      p[start + 2] == 'z' &&
                      p[start + 3] == 'e') {
                return SIZE;
            } else if(p[start] == 'l' &&
                      p[start + 1] == 'i' &&
                      p[start + 2] == 'n' &&
                      p[start + 3] == 'k') {
                return LINK;
            }
            break;
        case 5:
            if(p[start] == 'c' &&
               p[start + 1] == 'o' &&
               p[start + 2] == 'l' &&
               p[start + 3] == 'o' &&
               p[start + 4] == 'r') {
                return COLOR;
            } else if(p[start] == 'i' &&
                      p[start + 1] == 'm' &&
                      p[start + 2] == 'a' &&
                      p[start + 3] == 'g' &&
                      p[start + 4] == 'e') {
                return IMAGE;
            }
            break;
            
    }
    
    return UNKNOWN;
}

// if parse failed, endTagPos will be len, otherwise it is end tag position
static SpanType checkTag(unichar* p, int start, int len, int* endTagPos, int* dataStart, int* dataEnd, bool* close) {
    SpanType type = UNKNOWN;
    TagParseState state = READY;
    int tagNameStart, tagNameEnd;
    *close = false;
    *endTagPos = len;
    *dataStart = -1;
    int i = start;
    while(i < len) {
        switch (state) {
            case READY:
                if(p[i] == TAG_START) {
                    state = START_TAG;
                    tagNameStart = ++i;
                } else {
                    state = FAIL;
                }
                break;
            case START_TAG:
                if((i == start + 1) && p[i] == '/') {
                    state = CLOSE_TAG;
                    *close = true;
                    tagNameStart = ++i;
                } else if(p[i] == '=') {
                    state = EQUAL;
                    tagNameEnd = i++;
                    type = checkTagName(p, tagNameStart, tagNameEnd);
                    *dataStart = i;
                } else if(p[i] == TAG_END) {
                    state = SUCCESS;
                    *endTagPos = i;
                    *dataEnd = i;
                    tagNameEnd = i;
                    if(type == UNKNOWN) {
                        type = checkTagName(p, tagNameStart, tagNameEnd);
                    }
                } else if(p[i] == ' ') {
                    state = EQUAL;
                    tagNameEnd = i++;
                    type = checkTagName(p, tagNameStart, tagNameEnd);
                    *dataStart = i;
                } else {
                    i++;
                }
                break;
            case CLOSE_TAG:
                if(p[i] == TAG_END) {
                    state = SUCCESS;
                    *endTagPos = i;
                    tagNameEnd = i;
                    type = checkTagName(p, tagNameStart, tagNameEnd);
                } else {
                    i++;
                }
                break;
            case EQUAL:
                if(p[i] == TAG_END) {
                    state = SUCCESS;
                    *endTagPos = i;
                    *dataEnd = i;
                } else {
                    i++;
                }
                break;
            default:
                break;
        }
        
        if(state == FAIL || state == SUCCESS)
            break;
    }
    
    if(state != SUCCESS)
        type = UNKNOWN;
    
    return type;
}

static unichar* buildSpan(const char* pText, SpanList& spans, int* outLen) {
    // get unichar of string
    NSString* ns = [NSString stringWithUTF8String:pText];
    int uniLen = [ns length];
    unichar* uniText = (unichar*)malloc(uniLen * sizeof(unichar));
    [ns getCharacters:uniText range:NSMakeRange(0, uniLen)];
    
    bool inImageTag = false;
	int plainLen = 0;
	unichar* plain = (unichar*)malloc(sizeof(unichar) * uniLen);
	for(int i = 0; i < uniLen; i++) {
		unichar c = uniText[i];
		switch(c) {
			case '\\':
				if(i < uniLen - 1) {
					unichar cc = uniText[i + 1];
					if(cc == TAG_START || cc == TAG_END) {
						plain[plainLen++] = cc;
						i++;
					}
				} else {
					plain[plainLen++] = c;
				}
				break;
			case TAG_START:
			{
                // parse the tag
                Span span;
                int endTagPos, dataStart, dataEnd;
                SpanType type = checkTag(uniText, i, uniLen, &endTagPos, &dataStart, &dataEnd, &span.close);
                
                // fill span info
                do {
                    // if type is unknown, discard
                    CC_BREAK_IF(type == UNKNOWN);
                    
                    // you can't embed other tag in image tag, discard
                    CC_BREAK_IF(inImageTag && (!span.close || type != IMAGE));
                    
                    // parse span data
                    span.type = type;
                    span.pos = plainLen;
                    if(span.close) {
                        // special check for image end, use a placeholder for image tag
                        if(inImageTag) {
                            plain[plainLen++] = 0xfffc;
                            span.pos++;
                            inImageTag = false;
                        }
                    } else {
                        switch(span.type) {
                            case COLOR:
                                span.color = parseColor(uniText + dataStart, dataEnd - dataStart);
                                break;
                            case FONT:
                            {
                                NSString* font = [NSString stringWithCharacters:uniText + dataStart
                                                                         length:dataEnd - dataStart];
                                const char* cFont = [font cStringUsingEncoding:NSUTF8StringEncoding];
                                int len = strlen(cFont);
                                span.fontName = (char*)calloc(sizeof(char), len + 1);
                                strcpy(span.fontName, cFont);
                                break;
                            }
                            case SIZE:
                            {
                                NSString* size = [NSString stringWithCharacters:uniText + dataStart
                                                                         length:dataEnd - dataStart];
                                span.fontSize = [size intValue];
                                break;
                            }
                            case IMAGE:
                            {
                                NSString* name = [NSString stringWithCharacters:uniText + dataStart
                                                                         length:dataEnd - dataStart];
                                NSArray* parts = [name componentsSeparatedByString:@" "];
                                name = [parts objectAtIndex:0];
                                const char* cName = [name cStringUsingEncoding:NSUTF8StringEncoding];
                                int len = strlen(cName);
                                span.imageName = (char*)calloc(sizeof(char), len + 1);
                                strcpy(span.imageName, cName);
                                
                                // set default
                                span.scaleX = span.scaleY = 1;
								span.width = span.height = 0;
                                
                                // if has other parts, check
                                if([parts count] > 1) {
                                    int count = [parts count];
                                    for(int i = 1; i < count; i++) {
                                        NSString* part = [parts objectAtIndex:i];
                                        NSArray* pair = [part componentsSeparatedByString:@"="];
                                        if([pair count] == 2) {
                                            NSString* key = [pair objectAtIndex:0];
                                            NSString* value = [pair objectAtIndex:1];
                                            if([@"scale" isEqualToString:key]) {
                                                span.scaleX = span.scaleY = [value floatValue];
                                            } else if([@"scalex" isEqualToString:key]) {
												span.scaleX = [value floatValue];
											} else if([@"scaley" isEqualToString:key]) {
												span.scaleY = [value floatValue];
											} else if([@"w" isEqualToString:key]) {
												span.width = [value floatValue];
											} else if([@"h" isEqualToString:key]) {
												span.height = [value floatValue];
											}
                                        }
                                    }
                                }
                                
                                // flag
                                inImageTag = true;
                                
                                break;
                            }
                            case LINK:
                            {
                                NSString* v = [NSString stringWithCharacters:uniText + dataStart
                                                                         length:dataEnd - dataStart];
                                NSArray* parts = [v componentsSeparatedByString:@" "];
                                for(NSString* part in parts) {
                                    NSArray* pair = [part componentsSeparatedByString:@"="];
                                    if([pair count] == 2) {
                                        unichar tmp[32];
                                        NSString* key = [pair objectAtIndex:0];
                                        NSString* value = [pair objectAtIndex:1];
                                        if([@"bg" isEqualToString:key]) {
                                            [value getCharacters:tmp];
                                            span.normalBgColor = parseColor(tmp, [value length]);
                                        } else if([@"bg_click" isEqualToString:key]) {
                                            [value getCharacters:tmp];
                                            span.selectedBgColor = parseColor(tmp, [value length]);
                                        }
                                    }
                                }
                                break;
                            }
                            default:
                                break;
                        }
                    }
                    
                    // add span
                    spans.push_back(span);
                    
                    // set i pos
                    i = endTagPos;
                } while(0);
				
				break;
			}
			case TAG_END:
				// just discard it
				break;
			default:
                // ignore text between image tag
                if(!inImageTag) {
                    plain[plainLen++] = c;
                }
				break;
		}
	}
	
	// return length
    if(outLen)
        *outLen = plainLen;
	
#ifdef COCOS2D_DEBUG
//	// debug output span info
//	int i = 0;
//	for(SpanList::iterator iter = spans.begin(); iter != spans.end(); iter++) {
//		Span& span = *iter;
//		CCLOG("span %d: %d, %d, %d", i++, span.type, span.close, span.pos);
//	}
#endif
    
    // release
    free(uniText);
	
	// return plain str
	return plain;
}

static void renderEmbededImages(CGContextRef context, CTFrameRef frame, unichar* plain, SpanList& spans) {
    if([s_imageMap count] > 0) {
        // get line count and their origin
        CFArrayRef linesArray = CTFrameGetLines(frame);
        int lineCount = CFArrayGetCount(linesArray);
        CGPoint* origin = (CGPoint*)malloc(sizeof(CGPoint) * lineCount);
        CTFrameGetLineOrigins(frame, CFRangeMake(0, 0), origin);
        
        // iterate every line
        for (CFIndex i = 0; i < lineCount; i++) {
            // get line character range
            CTLineRef line = (CTLineRef)CFArrayGetValueAtIndex(linesArray, i);
            CFRange range = CTLineGetStringRange(line);
            
            // find image placeholder
            int end = range.location + range.length;
            for(int j = range.location; j < end; j++) {
                // if placeholder matched, render this image
                if(plain[j] == 0xfffc) {
                    // get offset
                    CGFloat offsetX = CTLineGetOffsetForStringIndex(line, j, NULL);
                    
                    // get span, if one image span matched index, draw the image
                    for(SpanList::iterator iter = spans.begin(); iter != spans.end(); iter++) {
                        Span& span = *iter;
                        if(span.type == IMAGE && !span.close && span.pos == j) {
                            NSString* imageName = [NSString stringWithCString:span.imageName
                                                                     encoding:NSUTF8StringEncoding];
                            UIImage* image = [s_imageMap objectForKey:imageName];
                            CGRect rect = CGRectMake(offsetX + origin[i].x,
                                                     origin[i].y,
                                                     span.width != 0 ? span.width : (image.size.width * span.scaleX),
                                                     span.height != 0 ? span.height : (image.size.height * span.scaleY));
                            CGContextDrawImage(context, rect, image.CGImage);
                            break;
                        }
                    }
                }
            }
        }
        
        // free origin
        free(origin);
    }
}

static void extractLinkMeta(CTFrameRef frame, SpanList& spans, LinkMetaList& linkMetas) {
    // get line count and their origin
    CFArrayRef linesArray = CTFrameGetLines(frame);
    int lineCount = CFArrayGetCount(linesArray);
    CGPoint* origin = (CGPoint*)malloc(sizeof(CGPoint) * lineCount);
    CTFrameGetLineOrigins(frame, CFRangeMake(0, 0), origin);
    
    // divide scale factor for origin
    for (CFIndex i = 0; i < lineCount; i++) {
        origin[i].x /= CC_CONTENT_SCALE_FACTOR();
        origin[i].y /= CC_CONTENT_SCALE_FACTOR();
    }
    
    // get range of every line
    CFRange* range = (CFRange*)malloc(sizeof(CFRange) * lineCount);
    for (CFIndex i = 0; i < lineCount; i++) {
        CTLineRef line = (CTLineRef)CFArrayGetValueAtIndex(linesArray, i);
        range[i] = CTLineGetStringRange(line);
    }
    
    // find every link span
    LinkMeta meta;
	int tag = 0;
    int linkStart, linkEnd;
    int startLine, endLine;
    for(SpanList::iterator iter = spans.begin(); iter != spans.end(); iter++) {
        Span& span = *iter;
        if(span.type == LINK) {
            if(!span.close) {
                // get start pos
                linkStart = span.pos;
                
                // save color info
                meta.normalBgColor = span.normalBgColor;
                meta.selectedBgColor = span.selectedBgColor;
            } else {
                // remember end pos
                linkEnd = span.pos;
                
                // find out the line for start and end pos
                for (CFIndex i = 0; i < lineCount; i++) {
                    if(linkStart >= range[i].location &&
                       linkStart < range[i].location + range[i].length) {
                        startLine = i;
                    }
                    
                    if(linkEnd >= range[i].location &&
                       linkEnd < range[i].location + range[i].length) {
                        endLine = i;
                        break;
                    }
                }
                
                // get rect area
				while(startLine <= endLine) {
					CGFloat ascent;
                    CGFloat descent;
					int charEnd = linkEnd;
					if(startLine < endLine)
						charEnd = range[startLine].location + range[startLine].length;
                    CTLineRef line = (CTLineRef)CFArrayGetValueAtIndex(linesArray, startLine);
                    CGFloat startOffsetX = CTLineGetOffsetForStringIndex(line, linkStart, NULL) / CC_CONTENT_SCALE_FACTOR();
                    CGFloat endOffsetX = CTLineGetOffsetForStringIndex(line, charEnd, NULL) / CC_CONTENT_SCALE_FACTOR();
                    CTLineGetTypographicBounds(line, &ascent, &descent, NULL);
                    ascent /= CC_CONTENT_SCALE_FACTOR();
                    descent /= CC_CONTENT_SCALE_FACTOR();
                    meta.x = origin[startLine].x + startOffsetX;
                    meta.y = origin[startLine].y - descent;
                    meta.width = origin[startLine].x + endOffsetX - meta.x;
                    meta.height = descent + ascent;
					meta.tag = tag;
					
					// push meta
					linkMetas.push_back(meta);
					
					// move line
					startLine++;
					
					// move start
					linkStart = charEnd;
				}
				
				// increase tag
				tag++;
            }
        }
    }
    
    // free
    free(origin);
    free(range);
}

static bool _initWithString(const char * pText, CCImage::ETextAlign eAlign, const char * pFontName, int nSize, tImageInfo* pInfo, CCPoint* outShadowStrokePadding, LinkMetaList* linkMetas) {
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
        if(nSize <= 0)
            nSize = (int)[UIFont systemFontSize];
        UIFont* uiDefaultFont = [UIFont fontWithName:fntName size:nSize];
        CC_BREAK_IF(!uiDefaultFont);
        CTFontRef defaultFont = CTFontCreateWithName((CFStringRef)uiDefaultFont.fontName, nSize, NULL);

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
        int plainLen = CFAttributedStringGetLength(plainCFAStr);
        
        // font and color stack
        int defaultColor = 0xff000000
            | ((int)(pInfo->tintColorR * 255) << 16)
            | ((int)(pInfo->tintColorG * 255) << 8)
            | (int)(pInfo->tintColorB * 255);
        vector<CTFontRef> fontStack;
        vector<int> colorStack;
        fontStack.push_back(defaultFont);
        colorStack.push_back(defaultColor);
        
        // iterate all spans, install attributes
        int colorStart = 0;
        int fontStart = 0;
        int underlineStart = -1;
        int imageStart = -1;
        CGColorSpaceRef colorSpace  = CGColorSpaceCreateDeviceRGB();
        for(SpanList::iterator iter = spans.begin(); iter != spans.end(); iter++) {
            Span& span = *iter;
            if(span.close) {
                switch(span.type) {
                    case COLOR:
                    {
                        if(span.pos > colorStart) {
                            int curColor = *colorStack.rbegin();
                            CGFloat comp[] = {
                                ((curColor >> 16) & 0xff) / 255.0f,
                                ((curColor >> 8) & 0xff) / 255.0f,
                                (curColor & 0xff) / 255.0f,
                                ((curColor >> 24) & 0xff) / 255.0f
                            };
                            CGColorRef fc = CGColorCreate(colorSpace, comp);
                            CFAttributedStringSetAttribute(plainCFAStr,
                                                           CFRangeMake(colorStart, span.pos - colorStart),
                                                           kCTForegroundColorAttributeName,
                                                           fc);
                            CGColorRelease(fc);
                            
                            // start need to be reset
                            colorStart = span.pos;
                        }
                        
                        // pop color
                        colorStack.pop_back();
                        
                        break;
                    }
                    case FONT:
                    case SIZE:
                    case BOLD:
                    case ITALIC:
                    {
                        // set previous span
                        CTFontRef font = *fontStack.rbegin();
                        if(span.pos > fontStart && font) {
                            CFAttributedStringSetAttribute(plainCFAStr,
                                                           CFRangeMake(fontStart, plainLen - fontStart),
                                                           kCTFontAttributeName,
                                                           font);
                            fontStart = span.pos;
                        }
                        
                        // pop font
                        fontStack.pop_back();
                        if(font)
                            CFRelease(font);
                        
                        break;
                    }
                    case UNDERLINE:
                    {
                        if(underlineStart > -1) {
                            CFIndex style = kCTUnderlineStyleSingle;
                            CFNumberRef styleNum = CFNumberCreate(kCFAllocatorDefault, kCFNumberCFIndexType, &style);
                            CFAttributedStringSetAttribute(plainCFAStr,
                                                           CFRangeMake(underlineStart, span.pos - underlineStart),
                                                           kCTUnderlineStyleAttributeName,
                                                           styleNum);
                            CFRelease(styleNum);
                            underlineStart = -1;
                        }
                        break;
                    }
                    case IMAGE:
                    {
                        if(imageStart > -1) {
                            CTRunDelegateRef delegate = CTRunDelegateCreate(&s_runDelegateCallbacks, &span);
                            CFAttributedStringSetAttribute(plainCFAStr,
                                                           CFRangeMake(imageStart, span.pos - imageStart),
                                                           kCTRunDelegateAttributeName,
                                                           delegate);
                            CFRelease(delegate);
                            
                            // register image, if it starts with '/', treat it as an external image
                            NSString* imageName = [NSString stringWithCString:span.imageName
                                                                     encoding:NSUTF8StringEncoding];
                            if([imageName characterAtIndex:0] == '/') {
                                // map image path to sandbox
                                NSString* path = [@"~/Documents" stringByExpandingTildeInPath];
                                path = [path stringByAppendingPathComponent:imageName];
                                
                                // check image extension
                                NSString* pathWithoutExt = [path stringByDeletingPathExtension];
                                NSString* ext = [path substringFromIndex:[pathWithoutExt length]];
                                bool png = [ext compare:@".png" options:NSCaseInsensitiveSearch] == NSOrderedSame;
                                bool jpg = [ext compare:@".jpg" options:NSCaseInsensitiveSearch] == NSOrderedSame ||
                                    [ext compare:@".jpeg" options:NSCaseInsensitiveSearch] == NSOrderedSame;
                                
                                // try to load its data
                                NSData* nsData = [NSData dataWithContentsOfFile:path];
                                if(!nsData)
                                    break;
                                
                                // create CGImage from data
                                CGDataProviderRef imgDataProvider = CGDataProviderCreateWithCFData((CFDataRef)nsData);
                                CGImageRef cgImage = NULL;
                                if(png) {
                                    cgImage = CGImageCreateWithPNGDataProvider(imgDataProvider,
                                                                               NULL,
                                                                               true,
                                                                               kCGRenderingIntentDefault);
                                } else if(jpg) {
                                    cgImage = CGImageCreateWithJPEGDataProvider(imgDataProvider,
                                                                                NULL,
                                                                                true,
                                                                                kCGRenderingIntentDefault);
                                } else {
                                    // unsupported image format
                                    break;
                                }
                                
                                // create UIImage and register it
                                if(cgImage) {
                                    UIImage* image = [UIImage imageWithCGImage:cgImage];
                                    CGImageRelease(cgImage);
                                    [s_imageMap setValue:image forKey:imageName];
                                }
                                
                                // release data provider
                                CGDataProviderRelease(imgDataProvider);
                            } else {
                                UIImage* image = [UIImage imageNamed:imageName];
                                [s_imageMap setValue:image forKey:imageName];
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }
            } else {
                switch(span.type) {
                    case COLOR:
                    {
                        // process previous run
                        if(span.pos > colorStart) {
                            int curColor = *colorStack.rbegin();
                            CGFloat comp[] = {
                                ((curColor >> 16) & 0xff) / 255.0f,
                                ((curColor >> 8) & 0xff) / 255.0f,
                                (curColor & 0xff) / 255.0f,
                                ((curColor >> 24) & 0xff) / 255.0f
                            };
                            CGColorRef fc = CGColorCreate(colorSpace, comp);
                            CFAttributedStringSetAttribute(plainCFAStr,
                                                           CFRangeMake(colorStart, span.pos - colorStart),
                                                           kCTForegroundColorAttributeName,
                                                           fc);
                            CGColorRelease(fc);
                            
                            // start need to be reset
                            colorStart = span.pos;
                        }
                        
                        // push color
                        colorStack.push_back(span.color);
                        
                        break;
                    }
                    case FONT:
                    {
                        // set previous span
                        CTFontRef curFont = *fontStack.rbegin();
                        if(span.pos > fontStart) {
                            CFAttributedStringSetAttribute(plainCFAStr,
                                                           CFRangeMake(fontStart, plainLen - fontStart),
                                                           kCTFontAttributeName,
                                                           curFont);
                            fontStart = span.pos;
                        }
                        
                        // create the font
                        NSString* fontName = [NSString stringWithCString:span.fontName
                                                                encoding:NSUTF8StringEncoding];
                        fontName = [[fontName lastPathComponent] stringByDeletingPathExtension];
                        UIFont* uiFont = [UIFont fontWithName:fontName
                                                         size:CTFontGetSize(curFont)];
                        CC_BREAK_IF(!uiFont);
                        CTFontRef font = CTFontCreateWithName((CFStringRef)uiFont.fontName,
                                                              CTFontGetSize(curFont),
                                                              NULL);
                        fontStack.push_back(font);
                        
                        break;
                    }
                    case SIZE:
                    {
                        // set previous span
                        CTFontRef curFont = *fontStack.rbegin();
                        if(span.pos > fontStart) {
                            CFAttributedStringSetAttribute(plainCFAStr,
                                                           CFRangeMake(fontStart, plainLen - fontStart),
                                                           kCTFontAttributeName,
                                                           curFont);
                            fontStart = span.pos;
                        }
                        
                        // push new font
                        CTFontDescriptorRef fd = CTFontCopyFontDescriptor(curFont);
                        CTFontRef font = CTFontCreateCopyWithAttributes(curFont,
                                                                        span.fontSize * CC_CONTENT_SCALE_FACTOR(),
                                                                        NULL,
                                                                        fd);
                        fontStack.push_back(font);
                        CFRelease(fd);
                        
                        break;
                    }
                    case BOLD:
                    {
                        // set previous span
                        CTFontRef curFont = *fontStack.rbegin();
                        if(span.pos > fontStart) {
                            CFAttributedStringSetAttribute(plainCFAStr,
                                                           CFRangeMake(fontStart, plainLen - fontStart),
                                                           kCTFontAttributeName,
                                                           curFont);
                            fontStart = span.pos;
                        }
                        
                        // create new font
                        CTFontRef font = CTFontCreateCopyWithSymbolicTraits(curFont,
                                                                            CTFontGetSize(curFont),
                                                                            NULL,
                                                                            kCTFontBoldTrait,
                                                                            kCTFontBoldTrait);
                        fontStack.push_back(font);
                        
                        break;
                    }
                    case ITALIC:
                    {
                        // set previous span
                        CTFontRef curFont = *fontStack.rbegin();
                        if(span.pos > fontStart) {
                            CFAttributedStringSetAttribute(plainCFAStr,
                                                           CFRangeMake(fontStart, plainLen - fontStart),
                                                           kCTFontAttributeName,
                                                           curFont);
                            fontStart = span.pos;
                        }
                   
                        // create new font
                        CTFontRef font = CTFontCreateCopyWithSymbolicTraits(curFont,
                                                                            CTFontGetSize(curFont),
                                                                            NULL,
                                                                            kCTFontItalicTrait,
                                                                            kCTFontItalicTrait);
                        fontStack.push_back(font);
                        
                        break;
                    }
                    case UNDERLINE:
                    {
                        underlineStart = span.pos;
                        break;
                    }
                    case IMAGE:
                    {
                        imageStart = span.pos;
                        break;
                    }
                    default:
                        break;
                }
            }
        }
        
        // last segment
        if(plainLen > colorStart) {
            int curColor = *colorStack.rbegin();
            CGFloat comp[] = {
                ((curColor >> 16) & 0xff) / 255.0f,
                ((curColor >> 8) & 0xff) / 255.0f,
                (curColor & 0xff) / 255.0f,
                ((curColor >> 24) & 0xff) / 255.0f
            };
            CGColorRef fc = CGColorCreate(colorSpace, comp);
            CFAttributedStringSetAttribute(plainCFAStr,
                                           CFRangeMake(colorStart, plainLen - colorStart),
                                           kCTForegroundColorAttributeName,
                                           fc);
            CGColorRelease(fc);
        }
        if(plainLen > fontStart) {
            CTFontRef font = *fontStack.rbegin();
            CFAttributedStringSetAttribute(plainCFAStr,
                                           CFRangeMake(fontStart, plainLen - fontStart),
                                           kCTFontAttributeName,
                                           font);
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
        
        // text origin offset
        int startY = 0;
        int startX = 0;
        if (constrainSize.height > dim.height) {
            // vertical alignment
            unsigned int vAlignment = (eAlign >> 4) & 0x0F;
            if (vAlignment == ALIGN_TOP) {
                startY = 0;
            } else if (vAlignment == ALIGN_CENTER) {
                startY = (constrainSize.height - dim.height) / 2;
            } else {
                startY = constrainSize.height - dim.height;
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
            if(pInfo->shadowOffset.height < 0) {
                startY += pInfo->shadowOffset.height;
            }
            if(pInfo->shadowOffset.width < 0) {
                startX += pInfo->shadowOffset.width;
            }
        }
        
        // add the padding (this could be 0 if no shadow and no stroke)
        dim.width  += shadowStrokePaddingX;
        dim.height += shadowStrokePaddingY;
		
		// save shadow stroke padding
		if(outShadowStrokePadding) {
			outShadowStrokePadding->x = -startX;
			outShadowStrokePadding->y = -startY;
		}
        
		// allocate data for bitmap
        // but don't do that if only measure
        unsigned char* data = NULL;
        CGContextRef context = NULL;
        if(!pInfo->sizeOnly) {
            data = new unsigned char[(int)(dim.width * dim.height * 4)];
            memset(data, 0, (int)(dim.width * dim.height * 4));
            
            // create context
            context = CGBitmapContextCreate(data,
                                            dim.width,
                                            dim.height,
                                            8,
                                            dim.width * 4,
                                            colorSpace,
                                            kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
        }
        
        // if fail to create context, break
        // if ok, draw text by core text
		do {
			CC_BREAK_IF(!context);
			
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
                CGFloat comp[] = {
                    ((pInfo->shadowColor >> 16) & 0xff) / 255.0f,
                    ((pInfo->shadowColor >> 8) & 0xff) / 255.0f,
                    (pInfo->shadowColor & 0xff) / 255.0f,
                    ((pInfo->shadowColor >> 24) & 0xff) / 255.0f
                };
                CGColorRef shadowColor = CGColorCreate(colorSpace, comp);
                CGContextSetShadowWithColor(context, offset, pInfo->shadowBlur, shadowColor);
                CGColorRelease(shadowColor);
            }
            
            // vertical alignment
            CGContextTranslateCTM(context, -startX, -startY);
            
            // draw frame
            CTFrameDraw(frame, context);
			
            // if has cached images, try render those images
            renderEmbededImages(context, frame, plain, spans);
            
            // if has link tag, build link info
            if(linkMetas)
                extractLinkMeta(frame, spans, *linkMetas);
			
            // pop the context
            UIGraphicsPopContext();
		} while(0);
        
        // release
		if(context)
			CGContextRelease(context);
        CGColorSpaceRelease(colorSpace);
        CFRelease(plainCFStr);
        CFRelease(plainCFAStr);
        CFRelease(fs);
        CFRelease(defaultFont);
        CFRelease(frame);
        CFRelease(path);
        CFRelease(paraStyle);
		free(plain);
        for(SpanList::iterator iter = spans.begin(); iter != spans.end(); iter++) {
            Span& span = *iter;
            if(span.type == FONT && !span.close) {
                free(span.fontName);
            } else if(span.type == IMAGE && !span.close) {
                free(span.imageName);
            }
        }
        [s_imageMap removeAllObjects];
		
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
														float strokeSize) {
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
    
    if (!_initWithString(pText, eAlignMask, pFontName, nSize, &info, &m_shadowStrokePadding, &m_linkMetas))
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
                                            float strokeSize) {
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
    
    if (!_initWithString(pText, kAlignCenter, pFontName, nSize, &info, NULL, NULL)) {
        return CCSizeZero;
    }
    return CCSizeMake(info.width, info.height);
}

NS_CC_END

#endif // #if CC_TARGET_PLATFORM == CC_PLATFORM_IOS