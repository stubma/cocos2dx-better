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
#import "SDPlistParser.h"
#import "SDAtlasLoader.h"

static AppDelegate s_sharedApplication;

@interface SDAppDelegate () <NSSplitViewDelegate, ACEViewDelegate, NSOutlineViewDataSource, NSOutlineViewDelegate>

@property (weak) IBOutlet MyEAGLView* glView;
@property (weak) IBOutlet NSWindow* window;
@property (weak) IBOutlet CNSplitView* hSplitView;
@property (weak) IBOutlet CNSplitView* vSplitView;
@property (weak) IBOutlet ACEView* aceView;
@property (weak) IBOutlet NSOutlineView *atlasOutline;
@property (weak) IBOutlet NSScrollView *propertyScrollView;
@property (assign) BOOL dirty;
@property (strong) NSString* file;
@property (assign) BOOL firstTime;
@property (assign) BOOL firstTextChange;
@property (strong) NSMutableArray* atlasPlists;
@property (strong) NSMutableDictionary* atlasFrameMap;
@property (strong) NSMutableDictionary* atlasImageMap;
@property (strong) NSMutableDictionary* atlasPathMap;
@property (strong) NSTextField* errorLabel;
@property (strong) CNSplitViewToolbar* toolbar;

- (void)initSplitViews;
- (void)initACEView;
- (void)onSave:(id)sender;
- (void)onLoad:(id)sender;
- (void)onNew:(id)sender;
- (void)onReplay:(id)sender;
- (void)onAddImages:(id)sender;
- (void)confirmSave:(BOOL)quit open:(BOOL)open;
- (void)showOpenPanel;
- (void)showSavePanel:(BOOL)quit;
- (void)save;
- (void)loadAtlases:(NSArray*)plists;
- (void)onDoubleClickOutline:(id)sender;
- (void)onStoryPlayerError:(NSNotification*)n;

@end

@implementation SDAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // init
    self.firstTime = YES;
    self.firstTextChange = YES;
    self.atlasPlists = [NSMutableArray array];
    self.atlasFrameMap = [NSMutableDictionary dictionary];
    self.atlasImageMap = [NSMutableDictionary dictionary];
    self.atlasPathMap = [NSMutableDictionary dictionary];
    
    // init split views
    [self initSplitViews];
    
    // init ace view
    [self initACEView];
    
    // reload outline
    [self.atlasOutline setDoubleAction:@selector(onDoubleClickOutline:)];
    [self.atlasOutline setTarget:self];
    [self.atlasOutline reloadData];
    
    // declare pasteboard type
    [[NSPasteboard generalPasteboard] declareTypes:@[NSStringPboardType] owner:self];
    
    // listener story player error
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(onStoryPlayerError:)
                                                 name:@"NSNotificationStoryPlayerError"
                                               object:nil];
    
    // start cocos2d-x loop
    CCApplication::sharedApplication()->run();
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender {
    if(self.dirty) {
        [self confirmSave:YES open:NO];
        return NSTerminateCancel;
    } else {
        return NSTerminateNow;
    }
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    if(self.file) {
        NSUserDefaults* d = [NSUserDefaults standardUserDefaults];
        [d setObject:self.file forKey:@"last_file"];
        [d setObject:self.atlasPlists forKey:self.file];
    }
    
    // remove observer
    [[NSNotificationCenter defaultCenter] removeObserver:self
                                                    name:@"NSNotificationStoryPlayerError"
                                                  object:nil];
}

- (void)applicationDidBecomeActive:(NSNotification *)notification {
    if(self.firstTime) {
        // preload atlas if has last file
        NSUserDefaults* d = [NSUserDefaults standardUserDefaults];
        NSString* lastFile = [d objectForKey:@"last_file"];
        if([lastFile length] > 0) {
            NSArray* plists = [d objectForKey:lastFile];
            [self loadAtlases:plists];
        }
        [self.atlasOutline reloadData];
        
        // reset flag
        self.firstTime = NO;
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
    // toolbar - new button
    CNSplitViewToolbarButton* newItem = [[CNSplitViewToolbarButton alloc] init];
    newItem.keyEquivalent = @"n";
    newItem.keyEquivalentModifierMask = NSCommandKeyMask;
    newItem.imageTemplate = CNSplitViewToolbarButtonImageTemplateAdd;
    newItem.toolTip = @"New";
    [newItem setTarget:self];
    [newItem setAction:@selector(onNew:)];
    
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
    
    // toolbar - error hint
    self.errorLabel = [[NSTextField alloc] init];
    [self.errorLabel setToolbarItemWidth:600];
    self.errorLabel.textColor = [NSColor redColor];
    [self.errorLabel setEditable:NO];
    [self.errorLabel setBordered:NO];
    [self.errorLabel setHidden:YES];
    self.errorLabel.alignment = NSRightTextAlignment;
    self.errorLabel.backgroundColor = [NSColor colorWithRed:0 green:0 blue:0 alpha:0];
    
    // create toolbar and add it
    CNSplitViewToolbar* toolbar = [[CNSplitViewToolbar alloc] init];
    [toolbar addItem:newItem align:CNSplitViewToolbarItemAlignLeft];
    [toolbar addItem:load align:CNSplitViewToolbarItemAlignLeft];
    [toolbar addItem:save align:CNSplitViewToolbarItemAlignLeft];
    [toolbar addItem:selectImage align:CNSplitViewToolbarItemAlignLeft];
    [toolbar addItem:replay align:CNSplitViewToolbarItemAlignLeft];
    [toolbar addItem:self.errorLabel align:CNSplitViewToolbarItemAlignRight];
    [self.hSplitView attachToolbar:toolbar
                  toSubViewAtIndex:1
                            onEdge:CNSplitViewToolbarEdgeBottom];
    self.toolbar = toolbar;
    
    // show toolbar
    [self.hSplitView showToolbarAnimated:NO];
}

- (void)confirmSave:(BOOL)quit open:(BOOL)open {
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
        } else if(open) {
            [self showOpenPanel];
        }
    }
}

- (void)onStoryPlayerError:(NSNotification *)n {
    [self.errorLabel setHidden:NO];
    self.errorLabel.stringValue = [n object];
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
    storyLayer->stopPlay();
    if(storyLayer->preloadStoryString(script)) {
        [self.errorLabel setHidden:YES];
        storyLayer->playStory();
    } else {
        self.errorLabel.stringValue = @"script has error, please check";
        [self.errorLabel setHidden:NO];
    }
}

- (void)onAddImages:(id)sender {
    NSOpenPanel* openDlg = [NSOpenPanel openPanel];
    [openDlg setCanChooseFiles:YES];
    [openDlg setAllowsMultipleSelection:YES];
    [openDlg setAllowedFileTypes:@[@"plist"]];
    [openDlg setPrompt:@"Load"];
    if ([openDlg runModal] == NSOKButton) {
        NSArray* files = [openDlg filenames];
        [self loadAtlases:files];
        
        // reload outline
        [self.atlasOutline reloadData];
    }
}

- (void)loadAtlases:(NSArray*)plists {
    for(NSString* file in plists) {
        // parse plist
        NSString* atlasPath = [SDPlistParser parse:file];
        
        // load atlas
        NSImage* atlasImage = [SDAtlasLoader loadAtlas:atlasPath];
        
        // save
        [self.atlasPlists addObject:file];
        [self.atlasPathMap setObject:atlasPath forKey:file];
        [self.atlasImageMap setObject:atlasImage forKey:file];
        [self.atlasFrameMap setObject:[NSArray arrayWithArray:gCurrentAtlasFrameList]
                               forKey:file];
        
        // load into sprite frame cache
        CCResourceLoader::loadZwoptex([file cStringUsingEncoding:NSUTF8StringEncoding],
                                      [atlasPath cStringUsingEncoding:NSUTF8StringEncoding],
                                      NULL);
    }
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
        [self confirmSave:NO open:YES];
    } else {
        [self showOpenPanel];
    }
}

- (void)onNew:(id)sender {
    if(self.dirty) {
        [self confirmSave:NO open:NO];
    }
    self.file = nil;
    self.aceView.string = @"";
    self.dirty = NO;
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
    if(!self.firstTextChange)
        self.dirty = YES;
    self.firstTextChange = NO;
}

#pragma mark -
#pragma mark outline view delegate

- (id)outlineView:(NSOutlineView *)outlineView child:(NSInteger)index ofItem:(id)item {
    if(item) {
        NSArray* frames = [self.atlasFrameMap objectForKey:item];
        return [frames objectAtIndex:index];
    } else {
        return [self.atlasPlists objectAtIndex:index];
    }
}

- (BOOL)outlineView:(NSOutlineView *)outlineView isItemExpandable:(id)item {
    if([self.atlasPlists containsObject:item])
        return YES;
    else
        return NO;
}

- (NSInteger)outlineView:(NSOutlineView *)outlineView numberOfChildrenOfItem:(id)item {
    if(item) {
        NSArray* frames = [self.atlasFrameMap objectForKey:item];
        return [frames count];
    } else {
        return [self.atlasPlists count];
    }
}

- (id)outlineView:(NSOutlineView *)outlineView objectValueForTableColumn:(NSTableColumn *)tableColumn byItem:(id)item {
    if([item isKindOfClass:[NSString class]]) {
        NSString* itemStr = (NSString*)item;
        return [itemStr lastPathComponent];
    } else {
        SDFrame* frame = (SDFrame*)item;
        return frame.key;
    }
}

- (void)outlineViewSelectionDidChange:(NSNotification *)notification {
    // if item is nil, return
    id item = [self.atlasOutline itemAtRow:self.atlasOutline.selectedRow];
    if(!item)
        return;
    
    // load image from atlas or a frame
    NSImageView* imageView = [[NSImageView alloc] init];
    if([item isKindOfClass:[NSString class]]) {
        imageView.image = [self.atlasImageMap objectForKey:item];
    } else {
        NSImage* atlas = [self.atlasImageMap objectForKey:[self.atlasOutline parentForItem:item]];
        imageView.image = [SDAtlasLoader getFrameImage:item fromAtlas:atlas];
    }
    imageView.frame = NSMakeRect(0, 0, imageView.image.size.width, imageView.image.size.height);
    self.propertyScrollView.documentView = imageView;
    [self.propertyScrollView.documentView scrollPoint:NSMakePoint(0, imageView.bounds.size.height)];
}

- (BOOL)outlineView:(NSOutlineView *)outlineView shouldEditTableColumn:(NSTableColumn *)tableColumn item:(id)item {
    return NO;
}

- (void)onDoubleClickOutline:(id)sender {
    id item = [self.atlasOutline itemAtRow:self.atlasOutline.selectedRow];
    if(![item isKindOfClass:[NSString class]]) {
        SDFrame* frame = (SDFrame*)item;
        NSString* s = [NSString stringWithFormat:@"\"%@\"", frame.key];
        [[NSPasteboard generalPasteboard] setString:s forType:NSStringPboardType];
    }
}

@end
