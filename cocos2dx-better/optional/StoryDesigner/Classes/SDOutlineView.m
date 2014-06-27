//
//  SDOutlineView.m
//  StoryDesigner
//
//  Created by maruojie on 14-6-27.
//  Copyright (c) 2014年 luma. All rights reserved.
//

#import "SDOutlineView.h"
#import "SDAppDelegate.h"

@interface SDOutlineView ()

@property (weak) IBOutlet NSMenu* contextMenu;

@end

@implementation SDOutlineView

- (NSMenu*)menuForEvent:(NSEvent *)event {
    NSPoint pt = [self convertPoint:[event locationInWindow] fromView:nil];
    NSInteger row = [self rowAtPoint:pt];
    id item = [self itemAtRow:row];
    if([item isKindOfClass:[NSString class]]) {
        [self selectRowIndexes:[NSIndexSet indexSetWithIndex:row] byExtendingSelection:NO];
        return self.contextMenu;
    } else {
        return nil;
    }
}

@end
