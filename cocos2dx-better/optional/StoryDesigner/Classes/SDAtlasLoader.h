//
//  SDAtlasLoader.h
//  StoryDesigner
//
//  Created by maruojie on 14-6-26.
//  Copyright (c) 2014年 luma. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SDPlistParser.h"

@interface SDAtlasLoader : NSObject

+ (NSImage*)loadAtlas:(NSString*)texFilename;
+ (NSImage*)getFrameImage:(const Frame&)frame fromAtlas:(NSImage*)image;

@end
