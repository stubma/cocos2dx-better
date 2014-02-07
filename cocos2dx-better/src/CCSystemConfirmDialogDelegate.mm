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
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#import "CCSystemConfirmDialogDelegate.h"

@implementation CCSystemConfirmDialogDelegate

- (id)initWithOK:(CCCallFunc*)onOK cancel:(CCCallFunc*)onCancel {
	if(self = [super init]) {
		m_onOK = onOK;
		m_onCancel = onCancel;
        CC_SAFE_RETAIN(m_onOK);
        CC_SAFE_RETAIN(m_onCancel);
		return self;
	}
	return nil;
}

- (void)dealloc {
    CC_SAFE_RELEASE(m_onOK);
    CC_SAFE_RELEASE(m_onCancel);
#if !__has_feature(objc_arc)
    [super dealloc];
#endif
}

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex {
	if(buttonIndex == alertView.cancelButtonIndex) {
		if(m_onCancel)
			m_onCancel->execute();
	} else if(buttonIndex == alertView.firstOtherButtonIndex) {
		if(m_onOK)
			m_onOK->execute();
	}
	
    // release
#if !__has_feature(objc_arc)
	[self autorelease];
#endif
}

@end

#endif // #if CC_TARGET_PLATFORM == CC_PLATFORM_IOS