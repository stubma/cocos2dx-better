//
//  CNSplitViewDelegate.h
//
//  Created by Frank Gregor on 02.02.13.
//  Copyright (c) 2013 cocoa:naut. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CNSplitViewDefinitions.h"


/**
 Each of these delegate methods will also perform a notification post with the sender (`CNSplitView` instance) as notification object and a userInfo dictionary (except `toolbarAttachedSubviewIndex:`).
 The userInfo dictionary contains two elements that are accessible by these constants:
 
 `CNUserInfoToolbarKey`<br />
 The value of the object represented by this key holds the attached `CNSplitViewToolbar`.

 `CNUserInfoEdgeKey`<br />
 This object is a `NSNumber` containing the `CNSplitViewToolbarEdge` value the toolbar is attached to.
 */

@class CNSplitView, CNSplitViewToolbar;

@protocol CNSplitViewToolbarDelegate <NSSplitViewDelegate>
@required
/**
 Asks the delegate about the index of the subview where the toolbar is attached to.
 */
- (NSUInteger)toolbarAttachedSubviewIndex:(CNSplitViewToolbar *)theToolbar;


@optional

/**
 Informs the delegate that the splitview will show the specified toolbar on the given edge.
 
 @param theSplitView    The splitview that sends the message.
 @param theToolbar      The toolbar that will be visible.
 @param theEdge         The edge the toolbar is attached to.
 */
- (void)splitView:(CNSplitView *)theSplitView willShowToolbar:(CNSplitViewToolbar *)theToolbar onEdge:(CNSplitViewToolbarEdge)theEdge;

/**
 Informs the delegate that the splitview did show the specified toolbar on the given edge.

 @param theSplitView    The splitview that sends the message.
 @param theToolbar      The toolbar that is now visible.
 @param theEdge         The edge the toolbar is attached to.
 */
- (void)splitView:(CNSplitView *)theSplitView didShowToolbar:(CNSplitViewToolbar *)theToolbar onEdge:(CNSplitViewToolbarEdge)theEdge;

/**
 Informs the delegate that the splitview will hide the specified toolbar on the given edge.

 @param theSplitView    The splitview that sends the message.
 @param theToolbar      The toolbar that will be hidden.
 @param theEdge         The edge the toolbar is attached to.
 */
- (void)splitView:(CNSplitView *)theSplitView willHideToolbar:(CNSplitViewToolbar *)theToolbar onEdge:(CNSplitViewToolbarEdge)theEdge;

/**
 Informs the delegate that the splitview did hide the specified toolbar on the given edge.

 @param theSplitView    The splitview that sends the message.
 @param theToolbar      The toolbar that is now hidden.
 @param theEdge         The edge the toolbar is attached to.
 */
- (void)splitView:(CNSplitView *)theSplitView didHideToolbar:(CNSplitViewToolbar *)theToolbar onEdge:(CNSplitViewToolbarEdge)theEdge;

- (NSRect)splitView:(NSSplitView *)splitView additionalEffectiveRectOfDividerAtIndex:(NSInteger)dividerIndex;

@end
