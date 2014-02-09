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
#import "CCUtils.h"
#import <StoreKit/StoreKit.h>
#import "EAGLView.h"

@interface SKStoreProductViewControllerDelegate_openAppInStore : NSObject <SKStoreProductViewControllerDelegate>
@end

@implementation SKStoreProductViewControllerDelegate_openAppInStore

- (void)productViewControllerDidFinish:(SKStoreProductViewController *)viewController {
    [viewController dismissViewControllerAnimated:YES completion:^{
#if !__has_feature(objc_arc)
        [viewController autorelease];
		[self autorelease];
#endif
    }];
}

@end

void CCUtils::openAppInStore(const string& appId) {
	if([[UIDevice currentDevice].systemVersion floatValue] < 6.0f) {
		NSString* urlStr = [NSString stringWithFormat:@"itms-apps://itunes.apple.com/us/app/id%s?mt=8", appId.c_str()];
		NSURL* url = [NSURL URLWithString:urlStr];
		[[UIApplication sharedApplication] openURL:url];
	} else {
		SKStoreProductViewController* storeProductVC = [[SKStoreProductViewController alloc] init];
		storeProductVC.delegate = [[SKStoreProductViewControllerDelegate_openAppInStore alloc] init];
		NSDictionary* dict = [NSDictionary dictionaryWithObject:[NSString stringWithUTF8String:appId.c_str()]
														 forKey:SKStoreProductParameterITunesItemIdentifier];
		[storeProductVC loadProductWithParameters:dict completionBlock:^(BOOL result, NSError* error) {
			if (result) {
				UIViewController* vc = CCUtils::findViewController([EAGLView sharedEGLView]);
				[vc presentViewController:storeProductVC animated:YES completion:nil];
			}
		}];
	}
}