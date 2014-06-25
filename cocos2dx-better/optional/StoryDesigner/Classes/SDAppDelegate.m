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
#include "PreviewScene.h"

static AppDelegate s_sharedApplication;

@interface SDAppDelegate () <NSSplitViewDelegate, ACEViewDelegate>

@property (weak) IBOutlet MyEAGLView* glView;
@property (weak) IBOutlet NSWindow* window;
@property (weak) IBOutlet CNSplitView* hSplitView;
@property (weak) IBOutlet CNSplitView* vSplitView;
@property (weak) IBOutlet ACEView* aceView;
@property (assign) BOOL dirty;
@property (strong) NSString* file;
@property (assign) BOOL firstTime;

- (void)initSplitViews;
- (void)initACEView;
- (void)onSave:(id)sender;
- (void)onLoad:(id)sender;
- (void)onReplay:(id)sender;
- (void)onAddImages:(id)sender;
- (void)confirmSave:(BOOL)quit;
- (void)showOpenPanel;
- (void)showSavePanel:(BOOL)quit;
- (void)save;

@end

@implementation SDAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // init
    self.firstTime = YES;
    
    // init split views
    [self initSplitViews];
    
    // init ace view
    [self initACEView];
    
    // start cocos2d-x loop
    CCApplication::sharedApplication()->run();
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender {
    if(self.dirty) {
        [self confirmSave:YES];
        return NSTerminateCancel;
    } else {
        return NSTerminateNow;
    }
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    if(self.file) {
        NSUserDefaults* d = [NSUserDefaults standardUserDefaults];
        [d setObject:self.file forKey:@"last_file"];
    }
}

- (void)initACEView {
    // init
    [self.aceView setDelegate:self];
    [self.aceView setMode:ACEModeLua];
    [self.aceView setTheme:ACEThemeXcode];
    [self.aceView setShowInvisibles:NO];
    
    // load content from last file
    NSUserDefaults* d = [NSUserDefaults standardUserDefaults];
    NSString* lastFile = [d objectForKey:@"last_file"];
    if([lastFile length] > 0) {
        NSFileManager* fm = [NSFileManager defaultManager];
        if([fm fileExistsAtPath:lastFile]) {
            self.aceView.string = [NSString stringWithContentsOfFile:lastFile
                                                            encoding:NSUTF8StringEncoding
                                                               error:nil];
            self.file = lastFile;
            self.dirty = NO;
        }
    }
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
    
    // toolbar - select images
    NSButton* selectImage = [[NSButton alloc] init];
    [selectImage setButtonType:NSMomentaryPushInButton];
    [selectImage setTitle:@"Add Images"];
    [selectImage setToolbarItemWidth:100];
    [selectImage setBezelStyle:NSRoundedBezelStyle];
    [selectImage setFont:[NSFont systemFontOfSize:[NSFont systemFontSizeForControlSize:NSSmallControlSize]]];
    [[selectImage cell] setControlSize:NSSmallControlSize];
    [selectImage setTarget:self];
    [selectImage setAction:@selector(onAddImages:)];
    
    // toolbar - replay
    NSButton* replay = [[NSButton alloc] init];
    [replay setButtonType:NSMomentaryPushInButton];
    [replay setTitle:@"Replay"];
    [replay setToolbarItemWidth:100];
    [replay setBezelStyle:NSRoundedBezelStyle];
    [replay setFont:[NSFont systemFontOfSize:[NSFont systemFontSizeForControlSize:NSSmallControlSize]]];
    [[replay cell] setControlSize:NSSmallControlSize];
    [replay setTarget:self];
    [replay setAction:@selector(onReplay:)];
    
    // create toolbar and add it
    CNSplitViewToolbar* toolbar = [[CNSplitViewToolbar alloc] init];
    [toolbar addItem:load align:CNSplitViewToolbarItemAlignLeft];
    [toolbar addItem:save align:CNSplitViewToolbarItemAlignLeft];
    [toolbar addItem:selectImage align:CNSplitViewToolbarItemAlignLeft];
    [toolbar addItem:replay align:CNSplitViewToolbarItemAlignLeft];
    [self.hSplitView attachToolbar:toolbar
                  toSubViewAtIndex:1
                            onEdge:CNSplitViewToolbarEdgeBottom];
    
    // show toolbar
    [self.hSplitView showToolbarAnimated:NO];
}

- (void)confirmSave:(BOOL)quit {
    NSAlert *alert = [[NSAlert alloc] init];
    [alert addButtonWithTitle:@"Save"];
    [alert addButtonWithTitle:@"Discard"];
    [alert addButtonWithTitle:@"Cancel"];
    [alert setMessageText:@"Save current story?"];
    [alert setAlertStyle:NSWarningAlertStyle];
    NSInteger result = [alert runModal];
    if (result == NSAlertFirstButtonReturn) { // save
        if(self.file) {
            [self save];
            [[NSApplication sharedApplication] terminate:self];
        } else {
            [self showSavePanel:quit];
        }
    } else if(result == NSAlertSecondButtonReturn) { // discard
        if(quit) {
            self.dirty = NO;
            [[NSApplication sharedApplication] terminate:self];
        } else {
            [self showOpenPanel];
        }
    }
}

- (void)showSavePanel:(BOOL)quit {
    NSSavePanel* saveDlg = [NSSavePanel savePanel];
    [saveDlg setCanCreateDirectories:YES];
    [saveDlg setShowsHiddenFiles:NO];
    [saveDlg setExtensionHidden:NO];
    if ([saveDlg runModal] == NSOKButton) {
        self.file = [saveDlg filename];
        [self save];
        self.dirty = NO;
        
        if(quit) {
            [[NSApplication sharedApplication] terminate:self];
        }
    }
}

- (void)showOpenPanel {
    NSOpenPanel* openDlg = [NSOpenPanel openPanel];
    [openDlg setCanChooseFiles:YES];
    [openDlg setAllowsMultipleSelection:NO];
    [openDlg setPrompt:@"Load"];
    if ([openDlg runModal] == NSOKButton) {
        NSArray* files = [openDlg filenames];
        if([files count] > 0) {
            self.file = [files objectAtIndex:0];
            self.aceView.string = [NSString stringWithContentsOfFile:self.file
                                                            encoding:NSUTF8StringEncoding
                                                               error:nil];
            self.dirty = NO;
        }
    }
}

- (void)save {
    // save to file
    NSFileManager* fm = [NSFileManager defaultManager];
    if(![fm fileExistsAtPath:self.file]) {
        [fm createFileAtPath:self.file contents:nil attributes:nil];
    }
    NSFileHandle* f = [NSFileHandle fileHandleForWritingAtPath:self.file];
    NSMutableData* data = [NSMutableData data];
    [data appendBytes:[self.aceView.string cStringUsingEncoding:NSUTF8StringEncoding]
               length:[self.aceView.string lengthOfBytesUsingEncoding:NSUTF8StringEncoding]];
    [f writeData:data];
    [f truncateFileAtOffset:[f offsetInFile]];
    [f synchronizeFile];
    [f closeFile];
    
    // clear flag
    self.dirty = NO;
}

- (void)onReplay:(id)sender {
    Preview* scene = (Preview*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0);
    CCStoryLayer* storyLayer = scene->getStoryLayer();
    string script = [self.aceView.string cStringUsingEncoding:NSUTF8StringEncoding];
    storyLayer->reset();
    storyLayer->preloadStoryString(script);
    storyLayer->playStory();
}

- (void)onAddImages:(id)sender {
    
}

- (void)onSave:(id)sender {
    if(self.file) {
        if(self.dirty) {
            [self save];
        }
    } else {
        [self showSavePanel:NO];
    }
}

- (void)onLoad:(id)sender {
    if(self.dirty) {
        [self confirmSave:NO];
    } else {
        [self showOpenPanel];
    }
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
    if(!self.firstTime)
        self.dirty = YES;
    self.firstTime = NO;
}

@end
