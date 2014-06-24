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
    CCEGLView::sharedOpenGLView()->setFrameSize(frameRect.size.width, frameRect.size.height);
    if(d->getRunningScene()) {
        CCEGLView::sharedOpenGLView()->setDesignResolutionSize(frameRect.size.width, frameRect.size.height, kResolutionNoBorder);
        CCNotificationCenter::sharedNotificationCenter()->postNotification(kCCNotificationFrameSizeChanged);
    }
}

@end
