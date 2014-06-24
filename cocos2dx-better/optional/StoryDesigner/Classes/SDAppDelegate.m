/****************************************************************************
 Author: Luma (stubma@gmail.com)
 
 https://github.com/stubma/cocos2dx-better
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#import "SDAppDelegate.h"
#include "AppDelegate.h"
#import "MyEAGLView.h"
#import "CNSplitView.h"
#import <ACEView/ACEView.h>

static AppDelegate s_sharedApplication;

@interface SDAppDelegate () <NSSplitViewDelegate, ACEViewDelegate>

@property (weak) IBOutlet MyEAGLView* glView;
@property (weak) IBOutlet NSWindow* window;
@property (weak) IBOutlet CNSplitView* hSplitView;
@property (weak) IBOutlet CNSplitView* vSplitView;
@property (weak) IBOutlet ACEView* aceView;

- (void)initSplitViews;
- (void)onSave:(id)sender;
- (void)onLoad:(id)sender;

@end

@implementation SDAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // init split views
    [self initSplitViews];
    
    // init ace view
    [self.aceView setString:@"if x > 3 then"];
    [self.aceView setDelegate:self];
    [self.aceView setMode:ACEModeLua];
    [self.aceView setTheme:ACEThemeXcode];
    [self.aceView setShowInvisibles:NO];
    
    // start cocos2d-x loop
    CCApplication::sharedApplication()->run();
}

- (void)initSplitViews {
    // toolbar - save button
    CNSplitViewToolbarButton* save = [[CNSplitViewToolbarButton alloc] init];
    save.keyEquivalent = @"s";
    save.keyEquivalentModifierMask = NSCommandKeyMask;
    save.image = [NSImage imageNamed:@"save.png"];
    save.toolTip = @"Save";
    [save setTarget:self];
    [save setAction:@selector(onSave:)];
    
    // toolbar - load button
    CNSplitViewToolbarButton* load = [[CNSplitViewToolbarButton alloc] init];
    load.keyEquivalent = @"l";
    load.keyEquivalentModifierMask = NSCommandKeyMask;
    load.image = [NSImage imageNamed:@"load.png"];
    load.toolTip = @"Load";
    [load setTarget:self];
    [load setAction:@selector(onLoad:)];
    
    // create toolbar and add it
    CNSplitViewToolbar* toolbar = [[CNSplitViewToolbar alloc] init];
    [toolbar addItem:load align:CNSplitViewToolbarItemAlignLeft];
    [toolbar addItem:save align:CNSplitViewToolbarItemAlignLeft];
    [self.hSplitView attachToolbar:toolbar
                  toSubViewAtIndex:1
                            onEdge:CNSplitViewToolbarEdgeBottom];
    
    // show toolbar
    [self.hSplitView showToolbarAnimated:NO];
}

- (void)onSave:(id)sender {
    
}

- (void)onLoad:(id)sender {
    
}

#pragma mark -
#pragma mark splitview delegate

- (CGFloat)splitView:(NSSplitView *)splitView constrainMinCoordinate:(CGFloat)proposedMin ofSubviewAt:(NSInteger)dividerIndex {
    if(splitView == self.hSplitView) {
        return self.hSplitView.frame.size.height * 0.2f;
    } else {
        return self.vSplitView.frame.size.width * 0.1f;
    }
}

- (CGFloat)splitView:(NSSplitView *)splitView constrainMaxCoordinate:(CGFloat)proposedMax ofSubviewAt:(NSInteger)dividerIndex {
    if(splitView == self.hSplitView) {
        return self.hSplitView.frame.size.height * 0.9f;
    } else {
        return self.vSplitView.frame.size.width * 0.9f;
    }
}

#pragma mark -
#pragma mark ace view delegate

- (void) textDidChange:(NSNotification *)notification {
    
}

@end
