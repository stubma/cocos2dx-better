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
#import "MyEAGLView.h"
#import <objc/runtime.h>
#import <objc/message.h>

static MyEAGLView* view;

@implementation MyEAGLView

+ (id)sharedEGLView {
	return view;
}

- (id)initWithCoder:(NSCoder *)aDecoder {
    if(self = [super initWithCoder:aDecoder]) {
        CCEGLView::sharedOpenGLView()->setFrameSize(400, 800);
        frameZoomFactor_ = 1.0f;
        view = self;
        
        // XXX: method swizzle, to change EAGLView sharedEGLView
        Method origMethod = class_getClassMethod(self.superclass, @selector(sharedEGLView));
        Method newMethod = class_getClassMethod(self.class, @selector(sharedEGLView));
        method_exchangeImplementations(origMethod, newMethod);
        
        return self;
    }
    return nil;
}

- (void)setFrame:(NSRect)frameRect {
    if(NSEqualRects(frameRect, self.frame))
        return;
    [super setFrame:frameRect];
    
    CCDirector* d = CCDirector::sharedDirector();
    CCSize size = CCEGLView::sharedOpenGLView()->getDesignResolutionSize();
    CCEGLView::sharedOpenGLView()->setFrameSize(frameRect.size.width, frameRect.size.height);
    if(d->getRunningScene()) {
        CCEGLView::sharedOpenGLView()->setDesignResolutionSize(size.width, size.height, kResolutionShowAll);
        CCNotificationCenter::sharedNotificationCenter()->postNotification(kCCNotificationFrameSizeChanged);
    }
}

@end
