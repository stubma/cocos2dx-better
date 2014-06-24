//
//  CNSplitView.h
//
//  Created by cocoa:naut on 29.07.12.
//  Copyright (c) 2012 cocoa:naut. All rights reserved.
//

/*
 The MIT License (MIT)
 Copyright © 2012 Frank Gregor, <phranck@cocoanaut.com>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the “Software”), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
*/

#import <Cocoa/Cocoa.h>
#import "CNSplitViewDefinitions.h"
#import "CNSplitViewToolbar.h"
#import "CNSplitViewToolbarButton.h"
#import "CNSplitViewToolbarDelegate.h"
#import "NSView+CNSplitViewToolbar.h"


/**
 The intention of `CNSplitView` was to create an extended version of the standard `NSSplitView` to implement the so beloved
 feature of Brandon Walkin's [BWToolkit](http://brandonwalkin.com/bwtoolkit/) that has support for an anchored toolbar. Since
 the release of Xcode 4 Apple broke with the less or more well known plugin structure of Xcode 3 and BWToolkit is no longer supported.
 
 Well, `CNSplitView` tries to fill the hole and provides a feature for adding such a toolbar.
 */



@interface CNSplitView : NSSplitView <NSSplitViewDelegate, CNSplitViewToolbarDelegate>

#pragma mark - Initializing a CNSplitView Object
/** @name Initializing a CNSplitView Object */

@property (nonatomic) id<CNSplitViewToolbarDelegate> toolbarDelegate;

#pragma mark - Configuring & Handling Toolbars
/** @name Configuring & Handling Toolbars */

/**
 Adds a toolbar besides a `CNSplitView`'s subview.
 
 This method attaches an instance of a `CNSplitViewToolbar` to a receivers subview specified by its index on the top or bottom edge of it.
 The edge the toolbar should be attached to is a value of the `CNSplitViewToolbarEdge` enum:

    typedef enum {
        CNSplitViewToolbarEdgeUndefined = 0,
        CNSplitViewToolbarEdgeBottom,
        CNSplitViewToolbarEdgeTop
    } CNSplitViewToolbarEdge;

 @note Attaching a toolbar to the split view doesn't imply that it will be visible. You have to call showToolbarAnimated: or toggleToolbarAnimated: explicitly.

 @param theToolbar      An instance of `CNSplitViewToolbar`.
 @param theSubviewIndex The subview's index the toolbar will be attached to. The first index `0` is the leftmost (on vertical orientation) or the topmost (on horizontal orientation) subview.
 @param theEdge         A value provided by the `CNSplitViewToolbarEdge` enum.
 */
- (void)attachToolbar:(CNSplitViewToolbar *)theToolbar toSubViewAtIndex:(NSUInteger)dividerIndex onEdge:(CNSplitViewToolbarEdge)anchorEdge;

@property (assign, readonly) NSUInteger attachedSubviewIndex;

/**
 Shows an attached toolbar.
 
 Before you can use this function you have to add a toolbar to the split view, of course.
 
 @param animated    `YES` the toolbar will be shown using a slide in animation, otherwise `NO` for no animation.
 */
- (void)showToolbarAnimated:(BOOL)animated;

/**
 Hides an already visible toolbar.

 @param animated    `YES` the toolbar will be hidden using a slide out animation, otherwise `NO` for no animation.
 */
- (void)hideToolbarAnimated:(BOOL)animated;

/**
 Toggles the visibility of an attached toolbar.

 @param animated    `YES` for an animated toggle effect, otherwise `NO`.
 */
- (void)toggleToolbarAnimated:(BOOL)animated;


#pragma mark - Managing the Delegate and the Data Source
/** @name Managing the Delegate and the Data Source */

/**
 Sets the color of the split view divider.
 
 You can make use of any `NSColor` instance even with pattern images.
 
 @param theColor    An instance of `NSColor`.
 */
- (void)setDeviderColor:(NSColor *)theColor;

/**
 Property that indicates whether an assistive split view dragging handle is enabled or not.
 */
@property (assign, nonatomic, getter = isDraggingHandleEnabled) BOOL draggingHandleEnabled;


@end
