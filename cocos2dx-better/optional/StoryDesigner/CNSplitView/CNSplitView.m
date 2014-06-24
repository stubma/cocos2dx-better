//
//  CNSplitView.m
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

#import <QuartzCore/QuartzCore.h>
#import "CNSplitView.h"


static NSColor *kDefaultDeviderColor;
static CGFloat kDefaultAnimationDuration;

NSString *CNSplitViewInjectReferenceNotification    = @"InjectReference";
NSString *CNSplitViewWillShowToolbarNotification    = @"SplitViewWillShowToolbar";
NSString *CNSplitViewDidShowToolbarNotification     = @"SplitViewDidShowToolbar";
NSString *CNSplitViewWillHideToolbarNotification    = @"SplitViewWillHideToolbar";
NSString *CNSplitViewDidHideToolbarNotification     = @"SplitViewDidHideToolbar";

NSString *CNUserInfoToolbarKey = @"toolbar";
NSString *CNUserInfoEdgeKey = @"edge";


@interface CNSplitView () {
	NSColor *_dividerColor;
	NSView *_toolbarContainer;
	NSView *_anchoredView;
	CNSplitViewToolbar *_toolbar;
	BOOL _toolbarIsVisible;
	BOOL _animationIsRunning;
}
@property (assign, nonatomic) id <NSSplitViewDelegate> secondaryDelegate;
@end

@implementation CNSplitView

#pragma mark - Initialization

+ (void)initialize {
	kDefaultDeviderColor = [NSColor colorWithCalibratedRed:0.50 green:0.50 blue:0.50 alpha:1.0];
	kDefaultAnimationDuration = 0.10;
}

- (id)init {
	self = [super init];
	if (self) {
		[self commonConfiguration];
	}
	return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {
	self = [super initWithCoder:aDecoder];
	if (self) {
		[self commonConfiguration];
	}
	return self;
}

- (void)commonConfiguration {
	_dividerColor = kDefaultDeviderColor;
	_draggingHandleEnabled = NO;
	_toolbarContainer = nil;
	_anchoredView = nil;
	_toolbar = nil;
	_toolbarIsVisible = NO;
	_animationIsRunning = NO;
	_toolbarDelegate = nil;
	_secondaryDelegate = nil;
	_attachedSubviewIndex = NSNotFound;
}

#pragma mark - Configuring & Handling Toolbars

- (void)attachToolbar:(CNSplitViewToolbar *)theToolbar toSubViewAtIndex:(NSUInteger)index onEdge:(CNSplitViewToolbarEdge)anchorEdge {
	/// via notification we inject a refernce to ourself into the toolbar
	[[NSNotificationCenter defaultCenter] postNotificationName:CNSplitViewInjectReferenceNotification object:self];

	_attachedSubviewIndex = index;
	_anchoredView = [[self subviews] objectAtIndex:_attachedSubviewIndex];
	[_anchoredView setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];

	/// we need a new container view for our toolbar + anchoredView
	_toolbarContainer = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, NSWidth(_anchoredView.frame), NSHeight(_anchoredView.frame))];
	[_toolbarContainer setWantsLayer:YES];
	[_toolbarContainer setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];

	[self replaceSubview:_anchoredView with:_toolbarContainer];

	_toolbar = theToolbar;
	_toolbar.delegate = self;
	_toolbar.anchorEdge = anchorEdge;
	CGFloat posY = (_toolbar.anchorEdge == CNSplitViewToolbarEdgeBottom ? NSMinY(_anchoredView.frame) - _toolbar.height : NSHeight(_anchoredView.frame));
	[_toolbar setFrame:NSMakeRect(0, posY, NSWidth(_anchoredView.frame), _toolbar.height)];

	NSRect anchoredViewRect = NSMakeRect(0, 0, NSWidth(_anchoredView.frame), NSHeight(_anchoredView.frame));
	_anchoredView.frame = anchoredViewRect;

	[_toolbarContainer addSubview:_toolbar];
	[_toolbarContainer addSubview:_anchoredView];
}

- (void)showToolbarAnimated:(BOOL)animated {
	NSAssert(_toolbar != nil, @"** Note: Before you show a toolbar you should make use of 'addToolbar:besidesSubview:onEdge:'.");

	if (_animationIsRunning)
		return;

	__block CGFloat posY;
	_animationIsRunning = YES;

	/// inform the delegate
	[self splitView:self willShowToolbar:_toolbar onEdge:_toolbar.anchorEdge];

	[NSAnimationContext runAnimationGroup: ^(NSAnimationContext *context) {
	    context.duration = (animated ? kDefaultAnimationDuration : 0.0);
	    context.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionLinear];

	    posY = (_toolbar.anchorEdge == CNSplitViewToolbarEdgeBottom ? NSMinY(_anchoredView.frame) + _toolbar.height : 0);
	    NSRect adjustedAnchoredViewRect = NSMakeRect(NSMinX(_anchoredView.frame),
	                                                 posY,
	                                                 NSWidth(_anchoredView.frame),
	                                                 NSHeight(_anchoredView.frame) - _toolbar.height);
	    [[_anchoredView animator] setFrame:adjustedAnchoredViewRect];

	    /// place the toolbar
	    posY = (_toolbar.anchorEdge == CNSplitViewToolbarEdgeBottom ? 0 : NSHeight(_anchoredView.frame) - _toolbar.height);
	    NSPoint adjustedToolbarOrigin = NSMakePoint(NSMinX(_toolbar.frame), posY);
	    [[_toolbar animator] setFrameOrigin:adjustedToolbarOrigin];
        
	} completionHandler: ^{
	    _toolbarIsVisible = YES;
	    _animationIsRunning = NO;

	    /// inform the delegate
	    [self splitView:self didShowToolbar:_toolbar onEdge:_toolbar.anchorEdge];
	}];
}

- (void)hideToolbarAnimated:(BOOL)animated {
	if (_animationIsRunning)
		return;

	__block CGFloat posY;
	_animationIsRunning = YES;

	/// inform the delegate
	[self splitView:self willHideToolbar:_toolbar onEdge:_toolbar.anchorEdge];

	[NSAnimationContext runAnimationGroup: ^(NSAnimationContext *context) {
	    context.duration = (animated ? kDefaultAnimationDuration : 0.0);
	    context.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionLinear];

	    posY = (_toolbar.anchorEdge == CNSplitViewToolbarEdgeBottom ? NSMinY(_anchoredView.frame) - _toolbar.height : 0);
	    NSRect adjustedAnchoredViewRect = NSMakeRect(NSMinX(_anchoredView.frame),
	                                                 posY,
	                                                 NSWidth(_anchoredView.frame),
	                                                 NSHeight(_anchoredView.frame) + _toolbar.height);
	    [[_anchoredView animator] setFrame:adjustedAnchoredViewRect];

	    /// place the toolbar
	    posY = (_toolbar.anchorEdge == CNSplitViewToolbarEdgeBottom ? -_toolbar.height : NSHeight(_anchoredView.frame) + _toolbar.height);
	    NSPoint adjustedToolbarOrigin = NSMakePoint(NSMinX(_toolbar.frame), posY);
	    [[_toolbar animator] setFrameOrigin:adjustedToolbarOrigin];

	} completionHandler: ^{
	    _toolbarIsVisible = NO;
	    _animationIsRunning = NO;

	    /// inform the delegate
	    [self splitView:self didHideToolbar:_toolbar onEdge:_toolbar.anchorEdge];
	}];
}

- (void)toggleToolbarAnimated:(BOOL)animated {
	if (_toolbarIsVisible) {
		[self hideToolbarAnimated:animated];
	}
	else {
		[self showToolbarAnimated:animated];
	}
}

#pragma mark - Accessors

- (void)setDeviderColor:(NSColor *)theColor {
	_dividerColor = theColor;
}

- (void)setDraggingHandleEnabled:(BOOL)draggingHandleEnabled {
	_draggingHandleEnabled = draggingHandleEnabled;
	[[NSNotificationCenter defaultCenter] postNotificationName:CNSplitViewDraggingHandleEnableDisableNotification object:[NSNumber numberWithBool:_draggingHandleEnabled]];
}

- (void)setVertical:(BOOL)flag {
	[super setVertical:flag];
	[self adjustSubviews];
	[[NSNotificationCenter defaultCenter] postNotificationName:CNSplitViewDraggingHandleEnableDisableNotification object:[NSNumber numberWithBool:self.draggingHandleEnabled]];
}

- (void)setDelegate:(id <NSSplitViewDelegate> )delegate {
	_secondaryDelegate = delegate;
	[super setDelegate:self];
}

#pragma mark - Private Helper

- (void)adjustRectForNeighbourView:(id)neighbourView withButtonBarHeight:(CGFloat)barHeight onAnchoredEdge:(CNSplitViewToolbarEdge)anchoredEdge {
	NSRect neighbourViewRect = [neighbourView frame];
	NSRect adjustedRect = NSMakeRect(neighbourViewRect.origin.x,
	                                 (anchoredEdge == CNSplitViewToolbarEdgeBottom ? neighbourViewRect.origin.y + barHeight : neighbourViewRect.origin.y),
	                                 neighbourViewRect.size.width,
	                                 neighbourViewRect.size.height - barHeight);
	[neighbourView setFrame:adjustedRect];
}

#pragma mark - NSSplitView

- (NSColor *)dividerColor {
	return _dividerColor;
}

#pragma mark - NSSplitView Delegate

- (NSRect)splitView:(NSSplitView *)splitView additionalEffectiveRectOfDividerAtIndex:(NSInteger)dividerIndex {
	return [_toolbar splitView:splitView additionalEffectiveRectOfDividerAtIndex:dividerIndex];
}

- (CGFloat)splitView:(NSSplitView *)splitView constrainMinCoordinate:(CGFloat)proposedMin ofSubviewAt:(NSInteger)dividerIndex {
	if ([self.secondaryDelegate respondsToSelector:_cmd]) {
		return [self.secondaryDelegate splitView:splitView constrainMinCoordinate:proposedMin ofSubviewAt:dividerIndex];
	}
	return proposedMin;
}

- (CGFloat)splitView:(NSSplitView *)splitView constrainMaxCoordinate:(CGFloat)proposedMax ofSubviewAt:(NSInteger)dividerIndex {
	if ([self.secondaryDelegate respondsToSelector:_cmd]) {
		return [self.secondaryDelegate splitView:splitView constrainMaxCoordinate:proposedMax ofSubviewAt:dividerIndex];
	}
	return proposedMax;
}

- (BOOL)splitView:(NSSplitView *)splitView canCollapseSubview:(NSView *)subview {
	if ([self.secondaryDelegate respondsToSelector:_cmd]) {
		return [self.secondaryDelegate splitView:splitView canCollapseSubview:subview];
	}
	return NO;
}

- (BOOL)splitView:(NSSplitView *)splitView shouldCollapseSubview:(NSView *)subview forDoubleClickOnDividerAtIndex:(NSInteger)dividerIndex {
	if ([self.secondaryDelegate respondsToSelector:_cmd]) {
		return [self.secondaryDelegate splitView:splitView shouldCollapseSubview:subview forDoubleClickOnDividerAtIndex:dividerIndex];
	}
	return NO;
}

- (CGFloat)splitView:(NSSplitView *)splitView constrainSplitPosition:(CGFloat)proposedPosition ofSubviewAt:(NSInteger)dividerIndex {
	if ([self.secondaryDelegate respondsToSelector:_cmd]) {
		return [self.secondaryDelegate splitView:splitView constrainSplitPosition:proposedPosition ofSubviewAt:dividerIndex];
	}
	return proposedPosition;
}

- (void)splitView:(NSSplitView *)splitView resizeSubviewsWithOldSize:(NSSize)oldSize {
	if ([self.secondaryDelegate respondsToSelector:_cmd]) {
		[self.secondaryDelegate splitView:splitView resizeSubviewsWithOldSize:oldSize];
	}
	else {
		[self adjustSubviews];
	}
}

- (BOOL)splitView:(NSSplitView *)splitView shouldAdjustSizeOfSubview:(NSView *)view {
	if ([self.secondaryDelegate respondsToSelector:_cmd]) {
		return [self.secondaryDelegate splitView:splitView shouldAdjustSizeOfSubview:view];
	}
	return YES;
}

- (BOOL)splitView:(NSSplitView *)splitView shouldHideDividerAtIndex:(NSInteger)dividerIndex {
	if ([self.secondaryDelegate respondsToSelector:_cmd]) {
		return [self.secondaryDelegate splitView:splitView shouldHideDividerAtIndex:dividerIndex];
	}
	return NO;
}

- (NSRect)splitView:(NSSplitView *)splitView effectiveRect:(NSRect)proposedEffectiveRect forDrawnRect:(NSRect)drawnRect ofDividerAtIndex:(NSInteger)dividerIndex {
	if ([self.secondaryDelegate respondsToSelector:_cmd]) {
		return [self.secondaryDelegate splitView:splitView effectiveRect:proposedEffectiveRect forDrawnRect:drawnRect ofDividerAtIndex:dividerIndex];
	}
	return proposedEffectiveRect;
}

#pragma mark - CNSplitView Delegate

- (void)splitView:(CNSplitView *)theSplitView willShowToolbar:(CNSplitViewToolbar *)theToolbar onEdge:(CNSplitViewToolbarEdge)theEdge {
	NSDictionary *userInfo = @{ CNUserInfoToolbarKey: theToolbar, CNUserInfoEdgeKey: [NSNumber numberWithInteger:theEdge] };
	[[NSNotificationCenter defaultCenter] postNotificationName:CNSplitViewWillShowToolbarNotification object:self userInfo:userInfo];

	if ([self.toolbarDelegate respondsToSelector:_cmd]) {
		[self.toolbarDelegate splitView:theSplitView willShowToolbar:theToolbar onEdge:theEdge];
	}
}

- (void)splitView:(CNSplitView *)theSplitView didShowToolbar:(CNSplitViewToolbar *)theToolbar onEdge:(CNSplitViewToolbarEdge)theEdge {
	NSDictionary *userInfo = @{ CNUserInfoToolbarKey: theToolbar, CNUserInfoEdgeKey: [NSNumber numberWithInteger:theEdge] };
	[[NSNotificationCenter defaultCenter] postNotificationName:CNSplitViewDidShowToolbarNotification object:self userInfo:userInfo];

	if ([self.toolbarDelegate respondsToSelector:_cmd]) {
		[self.toolbarDelegate splitView:theSplitView didShowToolbar:theToolbar onEdge:theEdge];
	}
}

- (void)splitView:(CNSplitView *)theSplitView willHideToolbar:(CNSplitViewToolbar *)theToolbar onEdge:(CNSplitViewToolbarEdge)theEdge {
	NSDictionary *userInfo = @{ CNUserInfoToolbarKey: theToolbar, CNUserInfoEdgeKey: [NSNumber numberWithInteger:theEdge] };
	[[NSNotificationCenter defaultCenter] postNotificationName:CNSplitViewWillHideToolbarNotification object:self userInfo:userInfo];

	if ([self.toolbarDelegate respondsToSelector:_cmd]) {
		[self.toolbarDelegate splitView:theSplitView willHideToolbar:theToolbar onEdge:theEdge];
	}
}

- (void)splitView:(CNSplitView *)theSplitView didHideToolbar:(CNSplitViewToolbar *)theToolbar onEdge:(CNSplitViewToolbarEdge)theEdge {
	NSDictionary *userInfo = @{ CNUserInfoToolbarKey: theToolbar, CNUserInfoEdgeKey: [NSNumber numberWithInteger:theEdge] };
	[[NSNotificationCenter defaultCenter] postNotificationName:CNSplitViewDidHideToolbarNotification object:self userInfo:userInfo];

	if ([self.toolbarDelegate respondsToSelector:_cmd]) {
		[self.toolbarDelegate splitView:theSplitView didHideToolbar:theToolbar onEdge:theEdge];
	}
}

#pragma mark - CNToolbar Delegate

- (NSUInteger)toolbarAttachedSubviewIndex:(CNSplitViewToolbar *)theToolbar {
	return _attachedSubviewIndex;
}

@end
