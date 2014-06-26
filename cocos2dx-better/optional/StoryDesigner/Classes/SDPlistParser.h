//
//  SDPlistParser.h
//  StoryDesigner
//
//  Created by maruojie on 14-6-26.
//  Copyright (c) 2014年 luma. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <string>
#include <vector>

using namespace std;

@interface SDFrame : NSObject

@property (strong) NSString* key;
@property (assign) CGPoint offset;
@property (assign) CGSize originalSize;
@property (assign) CGRect texRect;
@property (assign) CGRect sourceColorRect;
@property (assign) BOOL rotated;

@end

extern NSMutableArray* gCurrentAtlasFrameList;

@interface SDPlistParser : NSObject

+ (NSString*)parse:(NSString*)plist;

@end
