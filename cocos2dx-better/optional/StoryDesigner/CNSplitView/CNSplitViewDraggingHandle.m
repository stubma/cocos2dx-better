//
//  CNSplitViewDraggingHandle.m
//
//  Created by Frank Gregor on 01.02.13.
//  Copyright (c) 2013 cocoa:naut. All rights reserved.
//

/*
 The MIT License (MIT)
 Copyright © 2013 Frank Gregor, <phranck@cocoanaut.com>

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

#import "CNSplitViewDraggingHandle.h"
#import "CNSplitViewDefinitions.h"

static NSColor *delimiterGradientEndColor, *delimiterGradientCenterColor;
static float lineWidth, lineDistance, hInset, vInset;
static int numberOfLines;

@implementation CNSplitViewDraggingHandle

+ (void)initialize {
	delimiterGradientEndColor = [NSColor colorWithCalibratedRed:0.93 green:0.93 blue:0.93 alpha:1.0];
	delimiterGradientCenterColor = [NSColor colorWithCalibratedRed:0.42 green:0.42 blue:0.42 alpha:1];
	lineDistance = 4.0;
	lineWidth = 1.0;
	numberOfLines = 3;
}

- (void)drawRect:(NSRect)dirtyRect {
	BOOL isKeyWindow = [[self window] isKeyWindow];
	NSGradient *lineGradient = [[NSGradient alloc] initWithColorsAndLocations:
	                            (isKeyWindow ? delimiterGradientEndColor : [delimiterGradientEndColor highlightWithLevel:kDefaultColorHighlightLevel]), 0.0,
	                            (isKeyWindow ? delimiterGradientCenterColor : [delimiterGradientCenterColor highlightWithLevel:kDefaultColorHighlightLevel]), 0.40,
	                            (isKeyWindow ? delimiterGradientEndColor : [delimiterGradientEndColor highlightWithLevel:kDefaultColorHighlightLevel]), 1.0,
	                            nil];
	NSBezierPath *linePath = nil;
	NSRect lineRect = NSZeroRect;

	if (self.isVertical) {
		hInset = 5.0;
		vInset = 3.0;
		for (int lineOffset = 0; lineOffset < numberOfLines; lineOffset++) {
			lineRect = NSMakeRect(NSMaxX(self.bounds) - (hInset + lineOffset * lineDistance),
			                      NSMinY(self.bounds) + vInset,
			                      lineWidth,
			                      NSHeight(self.bounds) - vInset * 2);
			linePath = [NSBezierPath bezierPathWithRect:lineRect];
			[lineGradient drawInBezierPath:linePath angle:90];
		}
	}

	else {
		hInset = 3.0;
		vInset = ceil((NSHeight(self.bounds) + 1 - (numberOfLines * lineWidth + (numberOfLines - 1) * lineDistance)) / 2);
		for (int lineOffset = 0; lineOffset < numberOfLines; lineOffset++) {
			lineRect = NSMakeRect(NSMinX(self.bounds) + hInset,
			                      NSMinY(self.bounds) + (vInset + lineOffset * lineDistance),
			                      NSWidth(self.bounds) - hInset * 2,
			                      lineWidth);
			linePath = [NSBezierPath bezierPathWithRect:lineRect];
			[lineGradient drawInBezierPath:linePath angle:135];
		}
	}
}

@end
