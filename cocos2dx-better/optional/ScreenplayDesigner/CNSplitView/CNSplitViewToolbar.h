//
//  CNSplitViewToolBar.h
//
//  Created by cocoa:naut on 27.07.12.
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


@class CNSplitViewToolbar;
@protocol CNSplitViewToolbarDelegate;


/**
 `CNSplitViewToolbar` is a flexible and easy to use anchored button bar that will be placed on the
 bottom or top edge of a spcified split view divider.
 */


@interface CNSplitViewToolbar : NSView <NSSplitViewDelegate>

@property id<CNSplitViewToolbarDelegate> delegate;

#pragma mark - Configuring the toolbar behavior
/** @name Configuring the toolbar behavior */

/**
 Property that specifies the height of the toolbar.
 */
@property (nonatomic, assign) CGFloat height;

/**
 Property that specifies the edge the toolbar will be attached to.

 The value of this property is passed through from `CNSplitView` and should **never** called manually.
 */
@property (nonatomic, assign) CNSplitViewToolbarEdge anchorEdge;

/**
 Property that indicates wheter a delimiter between toolbar items should be displayed.

 @param itemDelimiterEnabled    `YES` the item delimiter should be visible, otherwise `NO`.
 */
@property (assign, nonatomic, getter = isItemDelimiterEnabled) BOOL itemDelimiterEnabled;

/**
 Property that specifies the alignment of all toolbar items.

 There are two possible values specified in an `enum` for this property.

    typedef enum {
        CNSplitViewToolbarContentAlignItemDirected = 0,
        CNSplitViewToolbarContentAlignCentered
    } CNSplitViewToolbarContentAlign;

 `CNSplitViewToolbarContentAlignItemDirected`<br />
 The item alignment follows the value of the [CNSplitViewToolbarButton toolbarButtonAlign] of each item.

 `CNSplitViewToolbarContentAlignCentered`<br />
 The value of the [CNSplitViewToolbarButton toolbarButtonAlign] of each item will be ignored and all items are centered.
 */
@property (assign, nonatomic) CNSplitViewToolbarContentAlign contentAlign;



#pragma mark - Toolbar and Item handling
/** @name Toolbar and Item handling */

/**
 Adds an item to the `CNSplitViewToolbar` with the specified *itemAlign*.
 
 An item sould be any subclass of `NSControl` (buttons, text fields, slider, popup buttons etc.). You can specify
 the item alignment by using values of this enum:
 
    typedef enum {
        CNSplitViewToolbarItemAlignLeft = 0,
        CNSplitViewToolbarItemAlignRight
    } CNSplitViewToolbarItemAlign;

 `CNSplitViewToolbarItemAlignLeft`<br />
 The item will be left aligned.

 `CNSplitViewToolbarItemAlignRight`<br />
 The item will be right aligned.
 
 @note Dependent of the contentAlign property the item alignment may be ignored. It only take effect if contentAlign is set to `CNSplitViewToolbarContentAlignItemDirected`.
 Otherwise the items will be centered.

 @param theItem     A `NSControl` subclass eg. `NSButton`, `NSSlider` etc.
 @param itemAlign   The horizontal alignment of the item.
 */
- (void)addItem:(id)theItem align:(CNSplitViewToolbarItemAlign)itemAlign;

/**
 Removes a given `CNAnchoredButton`.

 @param button    A present `CNAnchoredButton` object that should be removed.
 */
- (void)removeItem:(id)theItem;

/**
 Removes all placed items.
 */
- (void)removeAllItems;

/**
 Disable all receivers items.
 */
- (void)disable;

/**
 Enable all receivers items.
 */
- (void)enable;

@end
