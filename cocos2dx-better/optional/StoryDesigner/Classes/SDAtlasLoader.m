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
#import "SDAtlasLoader.h"
#include <string>
#include <vector>
#include "zip.h"
#include "PVRTextureUtilities.h"
#import <AppKit/AppKit.h>
#include <unistd.h>

using namespace std;
using namespace pvrtexture;

@implementation SDAtlasLoader

+ (NSImage*)loadAtlas:(NSString*)texFilePath {
    // get extension
    NSString* texFilename = [texFilePath lastPathComponent];
    NSString* ext = [texFilename pathExtension];
    NSString* zip = nil;
    if([@"ccz" isEqualToString:ext] || [@"gz" isEqualToString:ext]) {
        zip = ext;
        ext = [[texFilename stringByDeletingPathExtension] pathExtension];
    }
    
    // get texture file data
    NSData* texRawData = [NSData dataWithContentsOfFile:texFilePath];
    
    // ns image of atlas
    NSImage* image = nil;
    
    // load image
    do {
        // if is pvr
        if([@"pvr" isEqualToString:ext]) {
            unsigned char* pvrdata = NULL;
            int pvrlen = 0;
            
            // inflate pvr if it is zipped
            if([@"ccz" isEqualToString:zip]) {
                pvrlen = ccInflateCCZFile((char*)[texRawData bytes], [texRawData length], &pvrdata);
            } else if([@"gz" isEqualToString:zip]) {
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
            BOOL png = [@"png" isEqualToString:ext];
            CGDataProviderRef imgDataProvider = CGDataProviderCreateWithCFData((CFDataRef)texRawData);
            CGImageRef cgImage = NULL;
            if(png) {
                cgImage = CGImageCreateWithPNGDataProvider(imgDataProvider,
                                                           NULL,
                                                           true,
                                                           kCGRenderingIntentDefault);
            } else {
                cgImage = CGImageCreateWithJPEGDataProvider(imgDataProvider,
                                                            NULL,
                                                            true,
                                                            kCGRenderingIntentDefault);
            }
            image = [[NSImage alloc] initWithCGImage:cgImage size:NSZeroSize];
            CGDataProviderRelease(imgDataProvider);
            CGImageRelease(cgImage);
        }
        
        // check image
        if(!image) {
            NSLog(@"can't load texture file, abort");
            break;
        }
    } while(0);

    return image;
}

+ (NSImage*)getFrameImage:(SDFrame*)frame fromAtlas:(NSImage*)image {
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
    rect.origin.y = image.size.height - rect.origin.y - rect.size.height;
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
    if(isJPG) {
        fileData = [bitmapRep representationUsingType:NSJPEGFileType properties:nil];
        fImage = [[NSImage alloc] initWithData:fileData];
    } else {
        fileData = [bitmapRep representationUsingType:NSPNGFileType properties:nil];
        fImage = [[NSImage alloc] initWithData:fileData];
    }
    
    // return image
    return fImage;
}

@end
