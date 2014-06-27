#import <Foundation/Foundation.h>
#include <string>
#include <vector>
#include "zip.h"
#include "PVRTextureUtilities.h"
#import <AppKit/AppKit.h>
#include <unistd.h>

using namespace std;
using namespace pvrtexture;

typedef struct {
	NSString* key;
	CGPoint offset;
    CGSize originalSize;
    CGRect texRect;
	CGRect sourceColorRect;
    BOOL rotated;
} Frame;

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
} Atlas;

// atlas and frame list
vector<Frame> frameList;
Atlas atlas;

// for parsing state maintenance
NSString* lastKey;
vector<PListTag> tags;
ParseState state = READY;
Frame lastFrame;

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
					frameList.push_back(lastFrame);
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
						lastFrame.texRect.origin.x = [string floatValue];
					} else if([@"y" isEqualToString:lastKey]) {
						lastFrame.texRect.origin.y = [string floatValue];
					} else if([@"width" isEqualToString:lastKey]) {
						lastFrame.texRect.size.width = [string floatValue];
					} else if([@"height" isEqualToString:lastKey]) {
						lastFrame.texRect.size.height = [string floatValue];
					} else if([@"offsetX" isEqualToString:lastKey]) {
						lastFrame.offset.x = [string floatValue];
					} else if([@"offsetY" isEqualToString:lastKey]) {
						lastFrame.offset.y = [string floatValue];
					} else if([@"originalWidth" isEqualToString:lastKey]) {
						lastFrame.originalSize.width = [string floatValue];
					} else if([@"originalHeight" isEqualToString:lastKey]) {
						lastFrame.originalSize.height = [string floatValue];
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
						lastFrame.texRect.origin.x = [string floatValue];
					} else if([@"y" isEqualToString:lastKey]) {
						lastFrame.texRect.origin.y = [string floatValue];
					} else if([@"width" isEqualToString:lastKey]) {
						lastFrame.texRect.size.width = [string floatValue];
					} else if([@"height" isEqualToString:lastKey]) {
						lastFrame.texRect.size.height = [string floatValue];
					} else if([@"offsetX" isEqualToString:lastKey]) {
						lastFrame.offset.x = [string floatValue];
					} else if([@"offsetY" isEqualToString:lastKey]) {
						lastFrame.offset.y = [string floatValue];
					} else if([@"originalWidth" isEqualToString:lastKey]) {
						lastFrame.originalSize.width = [string floatValue];
					} else if([@"originalHeight" isEqualToString:lastKey]) {
						lastFrame.originalSize.height = [string floatValue];
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
						lastFrame.texRect.origin.x = [string floatValue];
					} else if([@"y" isEqualToString:lastKey]) {
						lastFrame.texRect.origin.y = [string floatValue];
					} else if([@"width" isEqualToString:lastKey]) {
						lastFrame.texRect.size.width = [string floatValue];
					} else if([@"height" isEqualToString:lastKey]) {
						lastFrame.texRect.size.height = [string floatValue];
					} else if([@"offsetX" isEqualToString:lastKey]) {
						lastFrame.offset.x = [string floatValue];
					} else if([@"offsetY" isEqualToString:lastKey]) {
						lastFrame.offset.y = [string floatValue];
					} else if([@"originalWidth" isEqualToString:lastKey]) {
						lastFrame.originalSize.width = [string floatValue];
					} else if([@"originalHeight" isEqualToString:lastKey]) {
						lastFrame.originalSize.height = [string floatValue];
					}
					
					break;
				}
			}
		}
	}
}

@end

int main(int argc, char * argv[]) {
    @autoreleasepool {
        // parse options
        BOOL help = NO;
        BOOL compact = NO;
        BOOL toPNG = NO;
        float scale = 1;
        int ch;
        const char* optString = "s:chp";
        while((ch = getopt(argc, argv, optString)) != -1) {
            switch(ch) {
                case 's':
                    scale = atof(optarg);
                    break;
                case 'c':
                    compact = YES;
                    break;
                case 'p':
                    toPNG = YES;
                    break;
                case 'h':
                    help = YES;
                    break;
            }
        }
        
        // if help, print usage
        if(help) {
            NSLog(@"Usage: ae [options] plist outputdir");
            NSLog(@"options:");
            NSLog(@"-h: show usage");
            NSLog(@"-s num: scale every frame, if not set, no scale");
            NSLog(@"-p: if set, the atlas image will be converted to a PNG file and saved as \"test.png\", this option is ignored when source atlas image is a PNG file");
            NSLog(@"-c: if set, transparent part will be truncated from output");
            return 0;
        }
        
        // parse args
        NSString* plist = [NSString stringWithCString:argv[optind] encoding:NSUTF8StringEncoding];
        NSString* dir = [NSString stringWithCString:argv[optind + 1] encoding:NSUTF8StringEncoding];
        plist = [plist stringByExpandingTildeInPath];
        dir = [dir stringByExpandingTildeInPath];
        
        // info
        NSLog(@"output directory: %@", dir);
        NSLog(@"start extracting...");
        
        do {
            // check plist existence
            NSFileManager* fm = [NSFileManager defaultManager];
            if(![fm fileExistsAtPath:plist]) {
                NSLog(@"can't find plist file, abort");
                break;
            }
                
            // parse plist
            NSData* plistData = [NSData dataWithContentsOfFile:plist];
            NSXMLParser* parser = [[NSXMLParser alloc] initWithData:plistData];
            PlistDelegate* delegate = [[PlistDelegate alloc] init];
            parser.delegate = delegate;
            [parser parse];

            // check texture file
            NSString* texFilePath = [[plist stringByDeletingLastPathComponent] stringByAppendingPathComponent:atlas.texFilename];
            NSLog(@"the texture file should be %@", texFilePath);
            if(![fm fileExistsAtPath:texFilePath]) {
                NSLog(@"can't find texture file, abort");
                break;
            }
            
            // create output dir
            if(![fm createDirectoryAtPath:dir withIntermediateDirectories:YES attributes:nil error:nil]) {
                NSLog(@"can't create output dir, abort");
                break;
            }
            
            // get extension
            NSString* ext = [atlas.texFilename pathExtension];
            NSString* zip = nil;
            if([@"ccz" isEqualToString:ext] || [@"gz" isEqualToString:ext]) {
                zip = ext;
                ext = [[atlas.texFilename stringByDeletingPathExtension] pathExtension];
            }
            
            // is png?
            BOOL atlasIsPNG = [@"png" isEqualToString:ext];
            
            // get texture file data
            NSData* texRawData = [NSData dataWithContentsOfFile:texFilePath];
            
            // ns image of atlas
            NSImage* image = nil;
            
            // if is pvr
            if([@"pvr" isEqualToString:ext]) {
                unsigned char* pvrdata = NULL;
                int pvrlen = 0;
                
                // inflate pvr if it is zipped
                if([@".ccz" isEqualToString:zip]) {
                    pvrlen = ccInflateCCZFile((char*)[texRawData bytes], [texRawData length], &pvrdata);
                } else if([@".gz" isEqualToString:zip]) {
                    pvrlen = ccInflateGZipFile([texFilePath cStringUsingEncoding:NSUTF8StringEncoding], &pvrdata);
                } else {
                    pvrlen = (int)[texRawData length];
                    pvrdata = (unsigned char*)malloc(pvrlen * sizeof(unsigned char));
                    memcpy(pvrdata, [texRawData bytes], pvrlen * sizeof(unsigned char));
                }
                
                // create texture from pvr data
                CPVRTexture sTexture(pvrdata);
                const PixelType ptFormat              = sTexture.getPixelType();
                const EPVRTVariableType eChannelType  = ePVRTVarTypeUnsignedByteNorm;
                const ECompressorQuality eQuality     = ePVRTCNormal;
                const EPVRTColourSpace eColourspace   = ePVRTCSpacelRGB;
                const bool bDoDither                  = false;
                
                // trans
                bool result = Transcode(sTexture, ptFormat, eChannelType, eColourspace, eQuality, bDoDither);
                if(!result) {
                    NSLog(@"Failed to transcode pvr");
                    break;
                }

                CGColorSpaceRef space = CGColorSpaceCreateDeviceRGB();
                CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, sTexture.getDataPtr(), sTexture.getDataSize(), NULL);
                CGImageRef cgImage = NULL;
                
                // create cg image
                int width = sTexture.getWidth();
                int height = sTexture.getHeight();
                int bitsPerPixel = sTexture.getBitsPerPixel();
                int bitsPerComponent = bitsPerPixel / 4;
                int bytesPerRow = bitsPerPixel / 8 * width;
                CGBitmapInfo bitmapInfo = sTexture.isPreMultiplied() ? kCGImageAlphaPremultipliedLast : kCGImageAlphaLast;
                bitmapInfo |= (bitsPerPixel == 16) ? kCGBitmapByteOrder16Little : kCGBitmapByteOrder32Little;
                bool shouldInterpolate = NO;
                CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
                cgImage = CGImageCreate(width,
                                        height,
                                        bitsPerComponent,
                                        bitsPerPixel,
                                        bytesPerRow,
                                        space,
                                        bitmapInfo,
                                        provider,
                                        NULL,
                                        shouldInterpolate,
                                        renderingIntent);
                
                // release
                CGColorSpaceRelease(space);
                CGDataProviderRelease(provider);
                
                // check cg image
                if(!cgImage) {
                    NSLog(@"Failed to create NSImage from cgimage");
                    break;
                }
                
                // create ns image
                image = [[NSImage alloc] initWithCGImage:cgImage size:NSMakeSize(width, height)];
                
                // release cg image
                CGImageRelease(cgImage);
                
                // free
                free(pvrdata);
            } else if([@"png" isEqualToString:ext] || [@"jpg" isEqualToString:ext] || [@"jpeg" isEqualToString:ext]) {
                image = [[NSImage alloc] initWithContentsOfFile:texFilePath];
            }
            
            // convert atlas to png
            if(!atlasIsPNG && toPNG) {
                // get png data
                [image lockFocus];
                NSBitmapImageRep* bitmapRep = [[NSBitmapImageRep alloc] initWithFocusedViewRect:NSMakeRect(0, 0, image.size.width, image.size.height)];
                [image unlockFocus];
                NSData* pngData = [bitmapRep representationUsingType:NSPNGFileType properties:Nil];
                
                // save pngdata
                NSString* dst = [dir stringByAppendingPathComponent:@"test.png"];
                [pngData writeToFile:dst atomically:YES];
            }
            
            // check image
            if(!image) {
                NSLog(@"can't load texture file, abort");
                break;
            }
            
            // process every frame
            float atlasHeight = image.size.height;
            for(vector<Frame>::iterator iter = frameList.begin(); iter != frameList.end(); iter++) {
                Frame& frame = *iter;
                NSLog(@"process frame %@...", frame.key);
                
                // create image for frame
                NSImage* fImage = [[NSImage alloc] initWithSize:NSMakeSize(frame.originalSize.width, frame.originalSize.height)];
                [fImage lockFocus];
                NSBitmapImageRep* bitmapRep = [[NSBitmapImageRep alloc] initWithFocusedViewRect:NSMakeRect(0, 0, fImage.size.width, fImage.size.height)];
                [fImage unlockFocus];
                
                // set bitmap as draw context
                NSGraphicsContext* gc = [NSGraphicsContext graphicsContextWithBitmapImageRep:bitmapRep];
                [NSGraphicsContext setCurrentContext:gc];
                
                // set tranform
                NSAffineTransform* t = [NSAffineTransform transform];
                [t translateXBy:frame.originalSize.width / 2
                            yBy:frame.originalSize.height / 2];
                [t translateXBy:frame.offset.x
                            yBy:frame.offset.y];
                if(frame.rotated)
                    [t rotateByDegrees:90];
                [t concat];

                // draw frame
                CGRect rect = frame.texRect;
                if(frame.rotated)
                    swap(rect.size.width, rect.size.height);
                rect.origin.y = atlasHeight - rect.origin.y - rect.size.height;
                if(frame.rotated) {
                    [image drawAtPoint:NSMakePoint(-frame.texRect.size.height / 2, -frame.texRect.size.width / 2)
                              fromRect:rect
                             operation:NSCompositeSourceOver
                              fraction:1];
                } else {
                    [image drawAtPoint:NSMakePoint(-frame.texRect.size.width / 2, -frame.texRect.size.height / 2)
                              fromRect:rect
                             operation:NSCompositeSourceOver
                              fraction:1];
                }
                
                // get data based on extension
                NSString* ext = [frame.key pathExtension];
                BOOL isJPG = [@"jpg" isEqualToString:ext] || [@"jpeg" isEqualToString:ext];
                NSData* fileData = nil;
                if(isJPG)
                    fileData = [bitmapRep representationUsingType:NSJPEGFileType properties:nil];
                else
                    fileData = [bitmapRep representationUsingType:NSPNGFileType properties:nil];
                
                // save data
                NSString* dst = [dir stringByAppendingPathComponent:frame.key];
                [fileData writeToFile:dst atomically:NO];
                
                // should be compact or scaled?
                if(compact || scale != 1) {
                    // reload this image
                    fImage = [[NSImage alloc] initWithContentsOfFile:dst];
                    
                    // the new image size
                    CGSize size = frame.originalSize;
                    if(compact)
                        size = frame.sourceColorRect.size;
                    if(scale != 1) {
                        size.width *= scale;
                        size.height *= scale;
                    }
                    
                    // create new image
                    NSImage* scaledImage = [[NSImage alloc] initWithSize:size];
                    [scaledImage lockFocus];
                    NSBitmapImageRep* scaledRep = [[NSBitmapImageRep alloc] initWithFocusedViewRect:NSMakeRect(0, 0, scaledImage.size.width, scaledImage.size.height)];
                    [scaledImage unlockFocus];
                    
                    // set bitmap as draw context
                    NSGraphicsContext* gc = [NSGraphicsContext graphicsContextWithBitmapImageRep:scaledRep];
                    [NSGraphicsContext setCurrentContext:gc];
                    
                    // transform
                    NSAffineTransform* t = [NSAffineTransform transform];
                    [t scaleBy:scale];
                    [t concat];
                    
                    // draw
                    CGRect rect = frame.sourceColorRect;
                    rect.origin.y = frame.originalSize.height - rect.origin.y - rect.size.height;
                    [fImage drawAtPoint:compact ? NSZeroPoint : NSMakePoint(rect.origin.x, rect.origin.y)
                               fromRect:rect
                              operation:NSCompositeSourceOver
                               fraction:1];
                
                    // get data
                    NSData* fileData = nil;
                    if(isJPG)
                        fileData = [scaledRep representationUsingType:NSJPEGFileType properties:nil];
                    else
                        fileData = [scaledRep representationUsingType:NSPNGFileType properties:nil];
                    
                    // save data
                    NSString* dst = [dir stringByAppendingPathComponent:frame.key];
                    [fileData writeToFile:dst atomically:NO];
                }
                
                NSLog(@"done");
            }
        } while(0);
        
        // info
        NSLog(@"end extracting, done");
    }
    
    return 0;
}

