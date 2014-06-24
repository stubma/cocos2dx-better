//
//  CNSplitViewToolBarButton.m
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


#import "CNSplitViewToolbarButton.h"
#import "CNSplitViewToolbarButtonCell.h"


@interface CNSplitViewToolbarButton () {}
@property (strong) NSMenu *itemContextMenu;
@end

@implementation CNSplitViewToolbarButton

#pragma mark - Initialzation
+ (Class)cellClass {
	return [CNSplitViewToolbarButtonCell class];
}

- (instancetype)initWithContextMenu:(NSMenu *)theContextMenu {
	self = [self init];
	if (self) {
		_itemContextMenu = theContextMenu;
	}
	return self;
}

- (instancetype)init {
	self = [super init];
	if (self) {
		[self commonConfiguration];
	}
	return self;
}

- (void)commonConfiguration {
	_imageTemplate = CNSplitViewToolbarButtonImageTemplatePlain;
	_itemContextMenu = nil;

	[self setAutoresizingMask:NSViewNotSizable];
	[self setImagePosition:NSImageLeft];
	[self setButtonType:NSMomentaryPushInButton];
	[self setBezelStyle:NSSmallSquareBezelStyle];
	[self setTitle:@""];

    NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
	[nc addObserver:self selector:@selector(enableMe) name:CNSplitViewToolbarItemsEnableNotification object:nil];
	[nc addObserver:self selector:@selector(disableMe) name:CNSplitViewToolbarItemsDisableNotification object:nil];
	[nc addObserver:self selector:@selector(windowStatusChanged) name:NSWindowDidBecomeKeyNotification object:nil];
	[nc addObserver:self selector:@selector(windowStatusChanged) name:NSWindowDidResignKeyNotification object:nil];
}

#pragma mark - Notifications

- (void)enableMe {
	[self setEnabled:YES];
}

- (void)disableMe {
	[self setEnabled:NO];
}

- (void)windowStatusChanged {
	[self setNeedsDisplay:YES];
}

#pragma mark - Handling Context Menus

- (void)mouseDown:(NSEvent *)theEvent {
	if (self.itemContextMenu) {
		NSEvent *leftMouseDown = [NSEvent mouseEventWithType:NSLeftMouseDown
		                                            location:theEvent.locationInWindow
		                                       modifierFlags:0
		                                           timestamp:0
		                                        windowNumber:[self.window windowNumber]
		                                             context:nil
		                                         eventNumber:0
		                                          clickCount:theEvent.clickCount
		                                            pressure:0];
		[NSMenu popUpContextMenu:self.itemContextMenu withEvent:leftMouseDown forView:self];
	}

	else {
		[super mouseDown:theEvent];
	}
}

#pragma mark - Accessors

- (void)setTitle:(NSString *)aString {
	NSMutableParagraphStyle *textStyle = [[NSMutableParagraphStyle defaultParagraphStyle] mutableCopy];
	[textStyle setAlignment:NSCenterTextAlignment];

	NSFont *font = [NSFont fontWithName:@"Helvetiva Neue" size:11.0];

	NSColor *textColor = [NSColor controlTextColor];
	NSShadow *textShadow = [[NSShadow alloc] init];
	[textShadow setShadowColor:[NSColor whiteColor]];
	[textShadow setShadowOffset:NSMakeSize(0, -1)];
	[textShadow setShadowBlurRadius:0];

	NSDictionary *attributes = [NSDictionary dictionaryWithObjectsAndKeys:
	                            textShadow, NSShadowAttributeName,
	                            textColor,  NSForegroundColorAttributeName,
	                            textStyle,  NSParagraphStyleAttributeName,
	                            font,       NSFontAttributeName,
	                            nil];
	NSAttributedString *attributedTitle = [[NSAttributedString alloc] initWithString:aString attributes:attributes];
	[self setAttributedTitle:attributedTitle];
}

- (void)setImagePosition:(NSCellImagePosition)aPosition {
	[(CNSplitViewToolbarButtonCell *)[self cell] setImagePosition : aPosition];
}

- (void)setImageTemplate:(CNSplitViewToolbarButtonImageTemplate)theImageTemplate {
	_imageTemplate = theImageTemplate;
	switch (_imageTemplate) {
		case CNSplitViewToolbarButtonImageTemplateAdd:           self.image = [NSImage imageNamed:NSImageNameAddTemplate]; break;
		case CNSplitViewToolbarButtonImageTemplateRemove:        self.image = [NSImage imageNamed:NSImageNameRemoveTemplate]; break;
		case CNSplitViewToolbarButtonImageTemplateQuickLook:     self.image = [NSImage imageNamed:NSImageNameQuickLookTemplate]; break;
		case CNSplitViewToolbarButtonImageTemplateAction:        self.image = [NSImage imageNamed:NSImageNameActionTemplate]; break;
		case CNSplitViewToolbarButtonImageTemplateShare:         self.image = [NSImage imageNamed:NSImageNameShareTemplate]; break;
		case CNSplitViewToolbarButtonImageTemplateIconView:      self.image = [NSImage imageNamed:NSImageNameIconViewTemplate]; break;
		case CNSplitViewToolbarButtonImageTemplateListView:      self.image = [NSImage imageNamed:NSImageNameListViewTemplate]; break;
		case CNSplitViewToolbarButtonImageTemplateLockLocked:    self.image = [NSImage imageNamed:NSImageNameLockLockedTemplate]; break;
		case CNSplitViewToolbarButtonImageTemplateLockUnlocked:  self.image = [NSImage imageNamed:NSImageNameLockUnlockedTemplate]; break;
		case CNSplitViewToolbarButtonImageTemplateGoRight:       self.image = [NSImage imageNamed:NSImageNameGoRightTemplate]; break;
		case CNSplitViewToolbarButtonImageTemplateGoLeft:        self.image = [NSImage imageNamed:NSImageNameGoLeftTemplate]; break;
		case CNSplitViewToolbarButtonImageTemplateStopProgress:  self.image = [NSImage imageNamed:NSImageNameStopProgressTemplate]; break;
		case CNSplitViewToolbarButtonImageTemplateRefresh:       self.image = [NSImage imageNamed:NSImageNameRefreshTemplate]; break;
		default: break;
	}
}

@end
