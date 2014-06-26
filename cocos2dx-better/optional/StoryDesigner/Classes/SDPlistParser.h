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

typedef struct {
	NSString* key;
	CGPoint offset;
    CGSize originalSize;
    CGRect texRect;
	CGRect sourceColorRect;
    BOOL rotated;
} Frame;
extern vector<Frame> gCurrentAtlasFrameList;

@interface SDPlistParser : NSObject

+ (NSString*)parse:(NSString*)plist;

@end
