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
#import <Foundation/Foundation.h>
#import "SDPlistParser.h"
#import <AppKit/AppKit.h>
#include <unistd.h>

@implementation SDFrame
@end

typedef enum {
	TAG_UNKNOWN,
	TAG_PLIST,
	TAG_DICT,
	TAG_KEY,
	TAG_REAL,
	TAG_STRING,
	TAG_INTEGER,
	TAG_TRUE,
	TAG_FALSE
} PListTag;

// parsing state
typedef enum {
	READY,
	PARSING_FRAMES,
	PARSING_FRAME,
	PARSING_METADATA,
	PARSING_TEXTURE
} ParseState;

typedef struct {
	int format;
	CGSize size;
	NSString* texFilename;
} AtlasFile;

// atlas and frame list
NSMutableArray* gCurrentAtlasFrameList = [[NSMutableArray alloc] init];
AtlasFile atlas;

// for parsing state maintenance
NSString* lastKey;
vector<PListTag> tags;
ParseState state = READY;
SDFrame* lastFrame = nil;

static CGPoint parsePoint(NSString* v) {
	NSUInteger len = [v length];
	char* tmp = (char*)calloc(len + 1, sizeof(char));
	for(int i = 0, j = 0; i < len; i++) {
		if(!isspace([v characterAtIndex:i])) {
			tmp[j++] = [v characterAtIndex:i];
		}
	}
	
	float w, h;
	sscanf(tmp, "{%f,%f}", &w, &h);
	free(tmp);
	return CGPointMake(w, h);
}

static CGSize parseSize(NSString* v) {
	NSUInteger len = [v length];
	char* tmp = (char*)calloc(len + 1, sizeof(char));
	for(int i = 0, j = 0; i < len; i++) {
		if(!isspace([v characterAtIndex:i])) {
			tmp[j++] = [v characterAtIndex:i];
		}
	}
	
	float w, h;
	sscanf(tmp, "{%f,%f}", &w, &h);
	free(tmp);
	return CGSizeMake(w, h);
}

static CGRect parseRect(NSString* v) {
	NSUInteger len = [v length];
	char* tmp = (char*)calloc(len + 1, sizeof(char));
	for(int i = 0, j = 0; i < len; i++) {
		if(!isspace([v characterAtIndex:i])) {
			tmp[j++] = [v characterAtIndex:i];
		}
	}
	
	float x, y, w, h;
	sscanf(tmp, "{{%f,%f},{%f,%f}}", &x, &y, &w, &h);
	free(tmp);
	return CGRectMake(x, y, w, h);
}

static PListTag topTag() {
	return *tags.rbegin();
}

static void pushTag(PListTag tag) {
	tags.push_back(tag);
}

static void popTag() {
	tags.pop_back();
}

static PListTag parentTag() {
	return *(tags.rbegin() + 1);
}

static PListTag grandpaTag() {
	return *(tags.rbegin() + 2);
}

static PListTag getPListTag(NSString* name) {
	if([@"plist" isEqualToString:name])
		return TAG_PLIST;
	else if([@"dict" isEqualToString:name])
		return TAG_DICT;
	else if([@"key" isEqualToString:name])
		return TAG_KEY;
	else if([@"real" isEqualToString:name])
		return TAG_REAL;
	else if([@"string" isEqualToString:name])
		return TAG_STRING;
	else if([@"integer" isEqualToString:name])
		return TAG_INTEGER;
	else if([@"true" isEqualToString:name])
		return TAG_TRUE;
	else if([@"false" isEqualToString:name])
		return TAG_FALSE;
	else
		return TAG_UNKNOWN;
}

@interface PlistDelegate : NSObject <NSXMLParserDelegate>

@end

@implementation PlistDelegate

- (void)parser:(NSXMLParser *)parser didStartElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI qualifiedName:(NSString *)qualifiedName attributes:(NSDictionary *)attributeDict {
	PListTag tag = getPListTag(elementName);
	pushTag(tag);
	
	switch(topTag()) {
		case TAG_DICT:
			if(!lastKey)
				break;
			
			switch(state) {
				case READY:
					if([@"frames" isEqualToString:lastKey])
						state = PARSING_FRAMES;
					else if([@"metadata" isEqualToString:lastKey]) // XML generic or XML cocos2d format
						state = PARSING_METADATA;
					else if([@"texture" isEqualToString:lastKey]) { // XML Legacy format
						state = PARSING_TEXTURE;
						
						// XML Legacy format doesn't have format attribute, we just set it to 1
						atlas.format = 1;
					}
					break;
				case PARSING_FRAMES:
					state = PARSING_FRAME;
                    lastFrame = [[SDFrame alloc] init];
					lastFrame.key = lastKey;
					lastKey = nil;
					break;
			}
			break;
		case TAG_TRUE:
			if(state == PARSING_FRAME) {
				if([@"rotated" isEqualToString:lastKey] || [@"textureRotated" isEqualToString:lastKey]) {
					lastFrame.rotated = true;
				}
			}
			break;
		case TAG_FALSE:
			if(state == PARSING_FRAME) {
				if([@"rotated" isEqualToString:lastKey] || [@"textureRotated" isEqualToString:lastKey]) {
					lastFrame.rotated = false;
				}
			}
			break;
		default:
			break;
	}
}

- (void)parser:(NSXMLParser *)parser didEndElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI qualifiedName:(NSString *)qName {
	switch(topTag()) {
		case TAG_DICT:
			switch(state) {
				case PARSING_FRAME:
					// append frame
					[gCurrentAtlasFrameList addObject:lastFrame];
					state = PARSING_FRAMES;
					break;
				case PARSING_FRAMES:
				case PARSING_METADATA:
				case PARSING_TEXTURE:
					state = READY;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
	
	// pop
	popTag();
}

- (void)parser:(NSXMLParser *)parser foundCharacters:(NSString *)string {
	switch(topTag()) {
		case TAG_KEY:
		{
			NSString* key = [string stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
			
			// save the last key
			lastKey = key;
			break;
		}
		case TAG_INTEGER:
		{
			switch(state) {
				case PARSING_METADATA:
				{
					if([@"format" isEqualToString:lastKey]) {
						atlas.format = [string intValue];
					}
					break;
				}
				case PARSING_TEXTURE:
				{
					if([@"width" isEqualToString:lastKey]) {
						float width = [string floatValue];
						atlas.size.width = width;
					} else if([@"height" isEqualToString:lastKey]) {
						atlas.size.height = [string floatValue];
					}
					break;
				}
				case PARSING_FRAME:
				{
                    if([@"x" isEqualToString:lastKey]) {
                        CGRect r = lastFrame.texRect;
                        r.origin.x = [string floatValue];
                        lastFrame.texRect = r;
					} else if([@"y" isEqualToString:lastKey]) {
                        CGRect r = lastFrame.texRect;
                        r.origin.y = [string floatValue];
                        lastFrame.texRect = r;
					} else if([@"width" isEqualToString:lastKey]) {
                        CGRect r = lastFrame.texRect;
                        r.size.width = [string floatValue];
                        lastFrame.texRect = r;
					} else if([@"height" isEqualToString:lastKey]) {
                        CGRect r = lastFrame.texRect;
                        r.size.height = [string floatValue];
                        lastFrame.texRect = r;
					} else if([@"offsetX" isEqualToString:lastKey]) {
                        CGPoint offset = lastFrame.offset;
                        offset.x = [string floatValue];
                        lastFrame.offset = offset;
					} else if([@"offsetY" isEqualToString:lastKey]) {
                        CGPoint offset = lastFrame.offset;
                        offset.y = [string floatValue];
                        lastFrame.offset = offset;
					} else if([@"originalWidth" isEqualToString:lastKey]) {
                        CGSize s = lastFrame.originalSize;
                        s.width = [string floatValue];
						lastFrame.originalSize = s;
					} else if([@"originalHeight" isEqualToString:lastKey]) {
                        CGSize s = lastFrame.originalSize;
                        s.height = [string floatValue];
						lastFrame.originalSize = s;
					}
					break;
				}
			}
			break;
		}
		case TAG_REAL:
		{
			switch(state) {
				case PARSING_FRAME:
				{
					if([@"x" isEqualToString:lastKey]) {
                        CGRect r = lastFrame.texRect;
                        r.origin.x = [string floatValue];
                        lastFrame.texRect = r;
					} else if([@"y" isEqualToString:lastKey]) {
                        CGRect r = lastFrame.texRect;
                        r.origin.y = [string floatValue];
                        lastFrame.texRect = r;
					} else if([@"width" isEqualToString:lastKey]) {
                        CGRect r = lastFrame.texRect;
                        r.size.width = [string floatValue];
                        lastFrame.texRect = r;
					} else if([@"height" isEqualToString:lastKey]) {
                        CGRect r = lastFrame.texRect;
                        r.size.height = [string floatValue];
                        lastFrame.texRect = r;
					} else if([@"offsetX" isEqualToString:lastKey]) {
                        CGPoint offset = lastFrame.offset;
                        offset.x = [string floatValue];
                        lastFrame.offset = offset;
					} else if([@"offsetY" isEqualToString:lastKey]) {
                        CGPoint offset = lastFrame.offset;
                        offset.y = [string floatValue];
                        lastFrame.offset = offset;
					} else if([@"originalWidth" isEqualToString:lastKey]) {
                        CGSize s = lastFrame.originalSize;
                        s.width = [string floatValue];
						lastFrame.originalSize = s;
					} else if([@"originalHeight" isEqualToString:lastKey]) {
                        CGSize s = lastFrame.originalSize;
                        s.height = [string floatValue];
						lastFrame.originalSize = s;
					}
					break;
				}
			}
			break;
		}
		case TAG_STRING:
		{
			switch(state) {
				case PARSING_METADATA:
					if([@"size" isEqualToString:lastKey]) {
						atlas.size = parseSize(string);
					} else if([@"textureFileName" isEqualToString:lastKey]) {
						atlas.texFilename = string;
					}
					break;
				case PARSING_FRAME:
				{
					if([@"frame" isEqualToString:lastKey] || [@"textureRect" isEqualToString:lastKey]) {
						lastFrame.texRect = parseRect(string);
					} else if([@"offset" isEqualToString:lastKey] || [@"spriteOffset" isEqualToString:lastKey]) {
						lastFrame.offset = parsePoint(string);
					} else if([@"sourceSize" isEqualToString:lastKey] || [@"spriteSourceSize" isEqualToString:lastKey]) {
						lastFrame.originalSize = parseSize(string);
					} else if([@"sourceColorRect" isEqualToString:lastKey] || [@"spriteColorRect" isEqualToString:lastKey]) {
						lastFrame.sourceColorRect = parseRect(string);
                    } else if([@"x" isEqualToString:lastKey]) {
                        CGRect r = lastFrame.texRect;
                        r.origin.x = [string floatValue];
                        lastFrame.texRect = r;
					} else if([@"y" isEqualToString:lastKey]) {
                        CGRect r = lastFrame.texRect;
                        r.origin.y = [string floatValue];
                        lastFrame.texRect = r;
					} else if([@"width" isEqualToString:lastKey]) {
                        CGRect r = lastFrame.texRect;
                        r.size.width = [string floatValue];
                        lastFrame.texRect = r;
					} else if([@"height" isEqualToString:lastKey]) {
                        CGRect r = lastFrame.texRect;
                        r.size.height = [string floatValue];
                        lastFrame.texRect = r;
					} else if([@"offsetX" isEqualToString:lastKey]) {
                        CGPoint offset = lastFrame.offset;
                        offset.x = [string floatValue];
                        lastFrame.offset = offset;
					} else if([@"offsetY" isEqualToString:lastKey]) {
                        CGPoint offset = lastFrame.offset;
                        offset.y = [string floatValue];
                        lastFrame.offset = offset;
					} else if([@"originalWidth" isEqualToString:lastKey]) {
                        CGSize s = lastFrame.originalSize;
                        s.width = [string floatValue];
						lastFrame.originalSize = s;
					} else if([@"originalHeight" isEqualToString:lastKey]) {
                        CGSize s = lastFrame.originalSize;
                        s.height = [string floatValue];
						lastFrame.originalSize = s;
					}
					
					break;
				}
			}
		}
	}
}

@end

@implementation SDPlistParser

+ (NSString*)parse:(NSString*)plist {
    // full path
    plist = [plist stringByExpandingTildeInPath];
    
    // image path
    NSString* imagePath = nil;
    
    do {
        // check plist existence
        NSFileManager* fm = [NSFileManager defaultManager];
        if(![fm fileExistsAtPath:plist]) {
            NSLog(@"can't find plist file, abort");
            break;
        }
        
        // parse plist
        [gCurrentAtlasFrameList removeAllObjects];
        NSData* plistData = [NSData dataWithContentsOfFile:plist];
        NSXMLParser* parser = [[NSXMLParser alloc] initWithData:plistData];
        PlistDelegate* delegate = [[PlistDelegate alloc] init];
        parser.delegate = delegate;
        [parser parse];
        lastFrame = nil;
        
        // check texture file
        NSString* texFilePath = [[plist stringByDeletingLastPathComponent] stringByAppendingPathComponent:atlas.texFilename];
        NSLog(@"the texture file should be %@", texFilePath);
        if(![fm fileExistsAtPath:texFilePath]) {
            NSLog(@"can't find texture file, abort");
            break;
        }
        imagePath = texFilePath;
    } while(0);
    
    // return
    return imagePath;
}

@end
