//
//  CNSplitViewToolBarButton.h
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


@interface CNSplitViewToolbarButton : NSButton

/** @name Toolbar Button Creation */

/**
 Creates and returns an initialized `CNSplitViewToolbarButton` object using the given menu on mouse click.
 
 This initializer takes a fully configured `NSMenu` object including all the needed `NSMenuItem` items. If a valid menu is 
 present it will be displayed via left mouse click on the toolbar button. In this case the button won't show a highlight effect.
 
 @param theContextMenu  The context menu.
 */
- (instancetype)initWithContextMenu:(NSMenu *)theContextMenu;



/** @name Toolbar Button Image */

/**
 Sets the image template for the button item.
 
 Instead of using the receivers image property you can use this one to get a quick access to the system provided icons. For more
 more detailed informations to these icons please take a look at the [developer documentation by Apple](http://developer.apple.com/library/mac/#documentation/userexperience/conceptual/applehiguidelines/SystemProvidedIcons/SystemProvidedIcons.html)
 
 The value of this property must be one of these enum items:
 
    typedef enum {
        CNSplitViewToolbarButtonImageTemplatePlain = 0,
        CNSplitViewToolbarButtonImageTemplateAdd,
        CNSplitViewToolbarButtonImageTemplateRemove,
        CNSplitViewToolbarButtonImageTemplateQuickLook,
        CNSplitViewToolbarButtonImageTemplateAction,
        CNSplitViewToolbarButtonImageTemplateShare,
        CNSplitViewToolbarButtonImageTemplateIconView,
        CNSplitViewToolbarButtonImageTemplateListView,
        CNSplitViewToolbarButtonImageTemplateLockLocked,
        CNSplitViewToolbarButtonImageTemplateLockUnlocked,
        CNSplitViewToolbarButtonImageTemplateGoRight,
        CNSplitViewToolbarButtonImageTemplateGoLeft,
        CNSplitViewToolbarButtonImageTemplateStopProgress,
        CNSplitViewToolbarButtonImageTemplateRefresh,
    } CNSplitViewToolbarButtonImageTemplate;

 */
@property (nonatomic, assign) CNSplitViewToolbarButtonImageTemplate imageTemplate;

@end
