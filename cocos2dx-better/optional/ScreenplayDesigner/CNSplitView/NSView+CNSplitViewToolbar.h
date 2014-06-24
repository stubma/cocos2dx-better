//
//  NSView+CNSplitViewToolbar.h
//  CNSplitView Example
//
//  Created by Frank Gregor on 04.02.13.
//  Copyright (c) 2013 cocoa:naut. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "CNSplitViewDefinitions.h"

@interface NSView (CNSplitViewToolbar)

/**
 Returns the receivers horizontal alignment inside the `CNSplitViewToolbar`.
 
 @return    A value of type `CNSplitViewToolbarItemAlign`.
 */
- (CNSplitViewToolbarItemAlign)toolbarItemAlign;

/**
 Set the receivers horizontal alignment inside the `CNSplitViewToolbar`.
 
 @param     theAlign    A value of type `CNSplitViewToolbarItemAlign`.
 */
- (void)setToolbarItemAlign:(CNSplitViewToolbarItemAlign)theAlign;


/**
 Returns the receivers width.

 @return    The width of the receiver in points.
 */
- (CGFloat)toolbarItemWidth;

/**
 Set the receivers width.

 @param     theItemWidth    The width of the receiver.
 */
- (void)setToolbarItemWidth:(CGFloat)theItemWidth;

@end
