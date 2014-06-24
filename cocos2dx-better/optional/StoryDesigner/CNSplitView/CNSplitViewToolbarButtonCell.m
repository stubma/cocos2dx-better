//
//  CNBarButtonCell.m
//  SieveMail
//
//  Created by cocoa:naut on 14.08.12.
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


#import "CNSplitViewToolbarButtonCell.h"


static NSGradient *btnGradient, *btnHighlightGradient;
static NSColor *gradientStartColor, *gradientEndColor;
static CGFloat kDefaultImageFraction, kDefaultImageEnabledFraction, kDefaultImageDisabledFraction;

@implementation CNSplitViewToolbarButtonCell

#pragma mark - Initialization

+ (void)initialize {
	gradientStartColor = [NSColor colorWithCalibratedRed:0.95 green:0.95 blue:0.95 alpha:1.0];
	gradientEndColor = [NSColor colorWithCalibratedRed:0.75 green:0.75 blue:0.75 alpha:1.0];
	btnHighlightGradient = [[NSGradient alloc] initWithStartingColor:gradientStartColor
	                                                     endingColor:[NSColor colorWithCalibratedWhite:0.664 alpha:1.000]];
	kDefaultImageFraction = 0.0;
	kDefaultImageEnabledFraction = 1.0;
	kDefaultImageDisabledFraction = 0.42;
}

- (id)init {
	self = [super init];
	if (self) {
		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
		[nc addObserver:self selector:@selector(parentWindowDidBecomeKeyWindow) name:NSWindowDidBecomeKeyNotification object:nil];
		[nc addObserver:self selector:@selector(parentWindowDidResignKeyWindow) name:NSWindowDidResignKeyNotification object:nil];
	}
	return self;
}

#pragma mark - Notifications

- (void)parentWindowDidBecomeKeyWindow {
	btnGradient = [[NSGradient alloc] initWithStartingColor:gradientStartColor
	                                            endingColor:gradientEndColor];
	kDefaultImageFraction = kDefaultImageEnabledFraction;
}

- (void)parentWindowDidResignKeyWindow {
	btnGradient = [[NSGradient alloc] initWithStartingColor:[gradientStartColor highlightWithLevel:kDefaultColorHighlightLevel]
	                                            endingColor:[gradientEndColor highlightWithLevel:kDefaultColorHighlightLevel]];
	kDefaultImageFraction = kDefaultImageDisabledFraction;
}

#pragma mark - Drawing

- (void)drawBezelWithFrame:(NSRect)cellFrame inView:(NSView *)controlView {
	NSBezierPath *buttonPath = [NSBezierPath bezierPathWithRect:cellFrame];
	switch (self.isHighlighted) {
		case YES: {
            [btnHighlightGradient drawInBezierPath:buttonPath angle:90];
            break;
        }

		case NO:
            [btnGradient drawInBezierPath:buttonPath angle:90];
            break;
	}
}

- (void)drawImage:(NSImage *)image withFrame:(NSRect)frame inView:(NSView *)controlView {
	NSSize imageSize = image.size;
	NSRect imageRect = NSZeroRect;

	if (![self.attributedTitle.string isEqualToString:@""]) {
		switch (self.imagePosition) {
			case NSImageRight: {
				imageRect = NSMakeRect(NSWidth(controlView.frame) - imageSize.width - kCNSplitViewToolbarButtonImageInset,
				                       (NSHeight(controlView.frame) - imageSize.height) / 2,
				                       imageSize.width,
				                       imageSize.height);
				break;
			}

			case NSImageLeft:
			default: {
				imageRect = NSMakeRect(kCNSplitViewToolbarButtonImageInset,
				                       (NSHeight(controlView.frame) - imageSize.height) / 2,
				                       imageSize.width,
				                       imageSize.height);
				break;
			}
		}
	}
	else {
		imageRect = NSMakeRect((NSWidth(controlView.frame) - imageSize.width) / 2,
		                       (NSHeight(controlView.frame) - imageSize.height) / 2,
		                       imageSize.width,
		                       imageSize.height);
	}


	if (self.isEnabled) {
		if (self.isHighlighted) {
			imageRect.origin = NSMakePoint(NSMinX(imageRect), NSMinY(imageRect) + 1);
        }
		[image drawInRect:imageRect fromRect:NSZeroRect operation:NSCompositeSourceOver fraction:kDefaultImageFraction respectFlipped:YES hints:nil];
	}
	else {
		CGFloat fraction = (self.imageDimsWhenDisabled ? kDefaultImageDisabledFraction : kDefaultImageEnabledFraction);
		[image drawInRect:imageRect fromRect:NSZeroRect operation:NSCompositeSourceOver fraction:fraction respectFlipped:YES hints:nil];
	}
}

- (NSRect)drawTitle:(NSAttributedString *)title withFrame:(NSRect)frame inView:(NSView *)controlView {
	if (self.isHighlighted)
		frame.origin = NSMakePoint(NSMinX(frame), NSMinY(frame) + 1);
	[title drawWithRect:frame options:NSStringDrawingUsesLineFragmentOrigin | NSStringDrawingUsesFontLeading];
	return frame;
}

@end
