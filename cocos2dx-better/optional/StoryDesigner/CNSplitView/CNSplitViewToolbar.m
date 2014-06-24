//
//  CNSplitViewToolBar.m
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


#import "CNSplitView.h"
#import "CNSplitViewToolbar.h"
#import "CNSplitViewToolbarButton.h"
#import "CNSplitViewToolbarButtonCell.h"
#import "CNSplitViewDraggingHandle.h"


CGFloat kDefaultVerticalDraggingHandleWidth, kDefaultHorizontalDraggingHandleWidth;
static CGFloat kDefaultToolbarHeight, kDefaultItemDelimiterWidth;
static NSColor *kDefaultBorderColor, *kDefaultGradientStartColor, *kDefaultGradientEndColor;
static NSColor *delimiterGradientEndColor, *delimiterGradientMiddleColor, *delimiterGradientCenterColor;

NSString *CNSplitViewToolbarItemsEnableNotification = @"ToolbarItemsEnableNotification";
NSString *CNSplitViewToolbarItemsDisableNotification = @"ToolbarItemsDisableNotification";
NSString *CNSplitViewDraggingHandleEnableDisableNotification = @"DraggingHandleEnableDisableNotification";



@interface CNSplitViewToolbar () {
	NSRect _previousToolbarRect;
	NSMutableArray *_delimiterOffsets;
	CNSplitView *_enclosingSplitView;
	NSMutableArray *_toolbarItems;
	CNSplitViewDraggingHandle *_draggingHandle;
}

- (NSSize)sizeForToolbarButton:(CNSplitViewToolbarButton *)theButton;
- (NSSize)sizeForToolbarItem:(NSView *)theItem;
- (void)drawItemDelimiter;
- (void)recalculateItemPositions;
- (BOOL)containsSubView:(NSView *)subview;
- (BOOL)isDraggingHandleEnabled;
@end


@implementation CNSplitViewToolbar

#pragma mark - Initialzation

+ (void)initialize {
	kDefaultBorderColor             = [NSColor colorWithCalibratedRed:0.50 green:0.50 blue:0.50 alpha:1.0];
	kDefaultGradientStartColor      = [NSColor colorWithCalibratedRed:0.75 green:0.75 blue:0.75 alpha:1.0];
	kDefaultGradientEndColor        = [NSColor colorWithCalibratedRed:0.95 green:0.95 blue:0.95 alpha:1.0];
	delimiterGradientEndColor       = [NSColor colorWithCalibratedRed:0.75 green:0.75 blue:0.75 alpha:0.1];
	delimiterGradientMiddleColor    = [NSColor colorWithCalibratedRed:0.78 green:0.78 blue:0.78 alpha:0.5];
	delimiterGradientCenterColor    = [NSColor colorWithCalibratedRed:0.42 green:0.42 blue:0.42 alpha:1];

	kDefaultToolbarHeight = 24.0;
	kDefaultItemDelimiterWidth = 1.0;
	kDefaultVerticalDraggingHandleWidth = 17.0;
	kDefaultHorizontalDraggingHandleWidth = 25.0;
}

- (id)init {
	self = [super init];
	if (self) {
		[self commonConfiguration];
	}
	return self;
}

- (void)commonConfiguration {
	[self setFrame:NSZeroRect];
	_height = kDefaultToolbarHeight;
	_itemDelimiterEnabled = YES;
	_contentAlign = CNSplitViewToolbarContentAlignItemDirected;

	_toolbarItems = [[NSMutableArray alloc] init];
	_previousToolbarRect = NSZeroRect;
	_delimiterOffsets = [NSMutableArray array];
	_enclosingSplitView = nil;
	_draggingHandle = nil;
	_anchorEdge = CNSplitViewToolbarEdgeUndefined;
	[self setAnchorEdge:CNSplitViewToolbarEdgeBottom];

	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(windowStatusChanged) name:NSWindowDidBecomeKeyNotification object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(windowStatusChanged) name:NSWindowDidResignKeyNotification object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(injectEnclosingSplitView:) name:CNSplitViewInjectReferenceNotification object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(draggingHandleEnableDisable:) name:CNSplitViewDraggingHandleEnableDisableNotification object:nil];
}

#pragma mark - API

- (void)addItem:(id)theItem align:(CNSplitViewToolbarItemAlign)align {
	[theItem setToolbarItemAlign:align];

	[_toolbarItems addObject:theItem];
	[self addSubview:theItem];

	NSSize itemSize;
	if ([theItem isKindOfClass:[CNSplitViewToolbarButton class]]) {
		itemSize = [self sizeForToolbarButton:theItem];
		[(CNSplitViewToolbarButton *)theItem setFrame : NSMakeRect(0, 0, itemSize.width, itemSize.height)];
	}
	else {
		itemSize = [self sizeForToolbarItem:theItem];
		[(NSView *)theItem setFrame : NSMakeRect(0, 0, itemSize.width, itemSize.height)];
	}

	/// set the correct button alignment
	switch ([theItem toolbarItemAlign]) {
		case CNSplitViewToolbarItemAlignLeft:[(NSView *)theItem setAutoresizingMask : NSViewMaxXMargin]; break;

		case CNSplitViewToolbarItemAlignRight:[(NSView *)theItem setAutoresizingMask : NSViewMinXMargin]; break;
	}
}

- (void)removeItem:(id)theItem {
	[_toolbarItems removeObject:theItem];
	[theItem removeFromSuperview];
}

- (void)removeAllItems {
	self.subviews = [NSArray array];
	[_toolbarItems removeAllObjects];
	[self setNeedsDisplay:YES];
}

- (void)enable {
	[[NSNotificationCenter defaultCenter] postNotificationName:CNSplitViewToolbarItemsEnableNotification object:nil];
}

- (void)disable {
	[[NSNotificationCenter defaultCenter] postNotificationName:CNSplitViewToolbarItemsDisableNotification object:nil];
}

#pragma mark - Private Helper

- (NSSize)sizeForToolbarButton:(CNSplitViewToolbarButton *)theButton {
	/// calculate or set the correct button width
	NSSize buttonSize = NSZeroSize;
	NSSize imageSize = (theButton.image ? theButton.image.size : NSMakeSize(0, 0));
	NSSize textSize = (theButton.attributedTitle ? theButton.attributedTitle.size : NSMakeSize(0, 0));

	CGFloat buttonWidth = theButton.toolbarItemWidth;
	/// text + image
	if (textSize.width > 0 && imageSize.width > 0) {
		buttonWidth = kCNSplitViewToolbarButtonImageInset + imageSize.width + kCNSplitViewToolbarButtonImageToTextDistance + textSize.width + kCNSplitViewToolbarButtonTextInset;
	}
	/// image only
	else if (textSize.width == 0 && imageSize.width > 0) {
		CGFloat width = (kCNSplitViewToolbarButtonImageInset + imageSize.width + kCNSplitViewToolbarButtonImageInset);
		buttonWidth = (theButton.toolbarItemWidth > width ? theButton.toolbarItemWidth : width);
	}
	/// text only
	else if (textSize.width > 0 && imageSize.width == 0) {
		buttonWidth = kCNSplitViewToolbarButtonTextInset + textSize.width + kCNSplitViewToolbarButtonTextInset;
	}
	buttonWidth = (buttonWidth < theButton.toolbarItemWidth ? theButton.toolbarItemWidth : buttonWidth);
	buttonSize = NSMakeSize(buttonWidth, self.height - 1);

	return buttonSize;
}

- (NSSize)sizeForToolbarItem:(NSView *)theItem {
	NSSize itemSize = NSZeroSize;
	CGFloat itemWidth = theItem.toolbarItemWidth;

	itemWidth = (itemWidth < theItem.toolbarItemWidth ? theItem.toolbarItemWidth : itemWidth);
	itemSize = NSMakeSize(itemWidth, self.height - 1);

	return itemSize;
}

- (void)drawItemDelimiter {
	if (!self.itemDelimiterEnabled)
		return;

	BOOL isKeyWindow = [[self window] isKeyWindow];
	NSGradient *gradient = [[NSGradient alloc] initWithColorsAndLocations:
	                        (isKeyWindow ? delimiterGradientEndColor : [delimiterGradientEndColor highlightWithLevel:kDefaultColorHighlightLevel]), 0.0,
	                        (isKeyWindow ? delimiterGradientMiddleColor : [delimiterGradientMiddleColor highlightWithLevel:kDefaultColorHighlightLevel]), 0.10,
	                        (isKeyWindow ? delimiterGradientCenterColor : [delimiterGradientCenterColor highlightWithLevel:kDefaultColorHighlightLevel]), 0.50,
	                        (isKeyWindow ? delimiterGradientMiddleColor : [delimiterGradientMiddleColor highlightWithLevel:kDefaultColorHighlightLevel]), 0.90,
	                        (isKeyWindow ? delimiterGradientEndColor : [delimiterGradientEndColor highlightWithLevel:kDefaultColorHighlightLevel]), 1.0,
	                        nil];

	[_delimiterOffsets enumerateObjectsUsingBlock: ^(id obj, NSUInteger idx, BOOL *stop) {
	    CGFloat posX = [(NSNumber *)obj doubleValue];
	    NSRect delimiterRect = NSMakeRect(posX, (self.anchorEdge == CNSplitViewToolbarEdgeBottom ? 0 : 1), kDefaultItemDelimiterWidth, NSHeight(self.bounds) - 1);
	    NSBezierPath *delimiterLine = [NSBezierPath bezierPathWithRect:delimiterRect];
	    [gradient drawInBezierPath:delimiterLine angle:90];
	}];
}

- (void)recalculateItemPositions {
	CGFloat draggingHandleWidth = ([_enclosingSplitView isVertical] ? kDefaultVerticalDraggingHandleWidth : kDefaultHorizontalDraggingHandleWidth);
	__block CGFloat leftOffset = 0;
	__block CGFloat rightOffset = ([self isDraggingHandleEnabled] ? NSWidth(self.frame) - draggingHandleWidth : NSWidth(self.frame));

	[_delimiterOffsets removeAllObjects];

	switch (self.contentAlign) {
		case CNSplitViewToolbarContentAlignItemDirected: {
			[_toolbarItems enumerateObjectsUsingBlock: ^(id obj, NSUInteger idx, BOOL *stop) {
			    if ([obj isKindOfClass:[NSView class]]) {
			        NSView *item = (NSView *)obj;

			        CGFloat originY = ([self bounds].size.height - [item bounds].size.height - 1) / 2;
			        if (self.anchorEdge == CNSplitViewToolbarEdgeTop) {
						originY++;
                    }

			        if (item.toolbarItemAlign == CNSplitViewToolbarItemAlignLeft) {
			            item.autoresizingMask |= NSViewMaxXMargin;
			            [item setFrameOrigin:NSMakePoint(leftOffset, originY)];
			            leftOffset += NSWidth(item.frame) + (self.isItemDelimiterEnabled ? kDefaultItemDelimiterWidth : 0);
			            [_delimiterOffsets setObject:@(ceil(NSMaxX(item.frame))) atIndexedSubscript:idx];
					}
			        else {
			            item.autoresizingMask |= NSViewMinXMargin;
			            [item setFrameOrigin:NSMakePoint(rightOffset - NSWidth(item.frame), originY)];
			            rightOffset -= (NSWidth(item.frame) + (self.isItemDelimiterEnabled ? kDefaultItemDelimiterWidth : 0));
			            [_delimiterOffsets setObject:@(ceil(NSMinX(item.frame)) - 1) atIndexedSubscript:idx];
					}
				}
			}];
			break;
		}

		case CNSplitViewToolbarContentAlignCentered: {
			__block CGFloat allButtonsWidth = 0;

			/// calculate the left offset related to all button widths
			[_toolbarItems enumerateObjectsUsingBlock: ^(id obj, NSUInteger idx, BOOL *stop) {
			    if ([obj isKindOfClass:[NSView class]]) {
			        NSView *item = (NSView *)obj;
			        allButtonsWidth += NSWidth(item.frame) + (self.isItemDelimiterEnabled ? kDefaultItemDelimiterWidth : 0);
				}
			}];
			leftOffset = ceil((NSWidth(self.bounds) - allButtonsWidth) / 2);

			/// calculate the button positions
			[_toolbarItems enumerateObjectsUsingBlock: ^(id obj, NSUInteger idx, BOOL *stop) {
			    if ([obj isKindOfClass:[NSView class]]) {
			        NSView *item = (NSView *)obj;

			        CGFloat originY = ([self bounds].size.height - [item bounds].size.height - 1) / 2;
			        if (self.anchorEdge == CNSplitViewToolbarEdgeTop) {
						originY++;
                    }

			        item.autoresizingMask = NSViewNotSizable;
			        [item setFrameOrigin:NSMakePoint(leftOffset, originY)];
			        leftOffset += ceil(NSWidth(item.frame)) + (self.isItemDelimiterEnabled ? kDefaultItemDelimiterWidth : 0);

			        /// As we draw the delimiter always on the right side of an item,
			        /// the first item needs to get space for an additional delimiter on its left side.
			        if (idx == 0) {
                     [_delimiterOffsets setObject:@(ceil(NSMinX(item.frame)) - 1) atIndexedSubscript:idx];
                    }

			        [_delimiterOffsets setObject:@(ceil(NSMaxX(item.frame))) atIndexedSubscript:idx + 1];
				}
			}];
			break;
		}
	}
}

- (BOOL)containsSubView:(NSView *)subview {
	__block BOOL containsSubView = NO;
	[[self subviews] enumerateObjectsUsingBlock: ^(NSView *aView, NSUInteger idx, BOOL *stop) {
	    if ([subview isEqualTo:aView]) {
	        containsSubView = YES;
	        *stop = YES;
		}
	}];
	return containsSubView;
}

- (BOOL)isDraggingHandleEnabled {
	return ![self isAttachedToEnclosingSplitViewLastSubview]
    && ((_enclosingSplitView.isDraggingHandleEnabled && (![_enclosingSplitView isVertical] && self.anchorEdge == CNSplitViewToolbarEdgeBottom))
    || (_enclosingSplitView.isDraggingHandleEnabled && ([_enclosingSplitView isVertical] && _enclosingSplitView.attachedSubviewIndex == 0)));
}

- (BOOL)isAttachedToEnclosingSplitViewLastSubview {
	BOOL result = NO;
	if ([self.delegate respondsToSelector:@selector(toolbarAttachedSubviewIndex:)]) {
		NSUInteger toolbarAttachedSubviewIndex = [self.delegate toolbarAttachedSubviewIndex:self];
		result = (toolbarAttachedSubviewIndex == [[_enclosingSplitView subviews] count] - 1);
	}
	return result;
}

#pragma mark - Accessors

- (void)setAnchorEdge:(CNSplitViewToolbarEdge)anchorEdge {
	_anchorEdge = anchorEdge;
	[self setAutoresizingMask:NSViewWidthSizable | (_anchorEdge == CNSplitViewToolbarEdgeBottom ? NSViewMaxYMargin : NSViewMinYMargin)];

	/// In case of the toolbar is on top, we have to draw a 1 pixel wide separator line on the bottom side (origin.y == 0).
	/// For this reason all toolbar buttons have to placed this 1 pixel above that line for not overpainting it.
	if (_anchorEdge == CNSplitViewToolbarEdgeTop) {
		[self.subviews enumerateObjectsUsingBlock: ^(NSView *aView, NSUInteger idx, BOOL *stop) {
		    NSPoint adjustedOrigin = [aView frame].origin;
		    adjustedOrigin.y++;
		    [aView setFrameOrigin : adjustedOrigin];
		}];
	}
}

- (void)setItemDelimiterEnabled:(BOOL)itemDelimiterEnabled {
	_itemDelimiterEnabled = itemDelimiterEnabled;
	_previousToolbarRect = NSZeroRect;
	[self setNeedsDisplay:YES];
}

- (void)setContentAlign:(CNSplitViewToolbarContentAlign)contentAlign {
	_contentAlign = contentAlign;
	_previousToolbarRect = NSZeroRect;
	[self setNeedsDisplay:YES];
}

#pragma mark - Notifications

- (void)windowStatusChanged {
	[self setNeedsDisplay:YES];
}

- (void)injectEnclosingSplitView:(NSNotification *)notification {
	_enclosingSplitView = [notification object];
}

- (void)draggingHandleEnableDisable:(NSNotification *)notification {
	[_draggingHandle removeFromSuperview];
	_draggingHandle = nil;

	BOOL draggingHandleEnabled = [(NSNumber *)[notification object] boolValue];
	if (draggingHandleEnabled) {
		if ([_enclosingSplitView isVertical] || (![_enclosingSplitView isVertical] && self.anchorEdge == CNSplitViewToolbarEdgeBottom && ![self isAttachedToEnclosingSplitViewLastSubview])) {
			_draggingHandle = [[CNSplitViewDraggingHandle alloc] init];
			_draggingHandle.vertical = _enclosingSplitView.isVertical;
			[self addSubview:_draggingHandle];
		}
	}
	[self recalculateItemPositions];
	[self setNeedsDisplay:YES];
}

#pragma mark - NSView

- (void)drawRect:(NSRect)dirtyRect {
	BOOL isKeyWindow = [[self window] isKeyWindow];

	NSColor *startColor = (isKeyWindow ? kDefaultGradientStartColor : [kDefaultGradientStartColor highlightWithLevel:kDefaultColorHighlightLevel]);
	NSColor *endColor = (isKeyWindow ? kDefaultGradientEndColor : [kDefaultGradientEndColor highlightWithLevel:kDefaultColorHighlightLevel]);
	NSGradient *toolbarKeyWindowGradient = [[NSGradient alloc] initWithStartingColor:startColor endingColor:endColor];
	NSBezierPath *buttonBarPath = [NSBezierPath bezierPathWithRect:dirtyRect];
	[toolbarKeyWindowGradient drawInBezierPath:buttonBarPath angle:90];

	CGFloat originY = (self.anchorEdge == CNSplitViewToolbarEdgeTop ? 0 : NSHeight(dirtyRect) - 1);
	NSRect borderLineRect = NSMakeRect(0, originY, NSWidth(dirtyRect), 1.0);
	NSBezierPath *borderLinePath = [NSBezierPath bezierPathWithRect:borderLineRect];
	NSColor *borderColor = (isKeyWindow ? kDefaultBorderColor : [kDefaultBorderColor highlightWithLevel:kDefaultColorHighlightLevel]);
	[borderColor setFill];
	[borderLinePath fill];

	if (!NSEqualRects(self.bounds, _previousToolbarRect))
		[self recalculateItemPositions];
	_previousToolbarRect = self.bounds;

	[self drawItemDelimiter];
}

#pragma mark - NSSplitView Delegate

- (NSRect)splitView:(NSSplitView *)splitView additionalEffectiveRectOfDividerAtIndex:(NSInteger)dividerIndex {
	NSRect draggingHandleRect = NSZeroRect;

	NSView *aSubView = [[splitView subviews] objectAtIndex:dividerIndex];
	if ([self isDraggingHandleEnabled]) {
		CGFloat originY = NSMinY(aSubView.frame) - (self.anchorEdge == CNSplitViewToolbarEdgeTop ? -1 : 1);
		if ([_enclosingSplitView isVertical]) {
			_draggingHandle.frame = NSMakeRect(NSMaxX(aSubView.frame) - kDefaultVerticalDraggingHandleWidth, originY, kDefaultVerticalDraggingHandleWidth, NSHeight(self.bounds));
		}
		else {
			_draggingHandle.frame = NSMakeRect(NSMaxX(aSubView.frame) - kDefaultHorizontalDraggingHandleWidth, originY, kDefaultHorizontalDraggingHandleWidth, NSHeight(self.bounds));
		}
		_draggingHandle.autoresizingMask = NSViewMinXMargin;
		draggingHandleRect = [_draggingHandle convertRect:[_draggingHandle bounds] toView:splitView];
	}

	return draggingHandleRect;
}

@end
