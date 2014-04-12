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

#include "CCImagePicker.h"
#import <UIKit/UIKit.h>
#import "EAGLView.h"

@interface CCImagePickerDelegate : NSObject <UINavigationControllerDelegate, UIImagePickerControllerDelegate>

@property (nonatomic, assign) CCImagePickerCallback* callback;
@property (nonatomic, assign) int expectedWidth;
@property (nonatomic, assign) int expectedHeight;
@property (nonatomic, assign) UIViewController* parent;
@property (nonatomic, assign) BOOL keepRatio;
@property (nonatomic, assign) string path;

@end

@implementation CCImagePickerDelegate

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info {
	// if no callback set, no need proceed
	if(!self.callback)
		return;
	
	// get image after edited
	UIImage* image = [info objectForKey:UIImagePickerControllerEditedImage];
	
	// get CGImage info
	CGImageRef CGImage = image.CGImage;
	CGImageAlphaInfo alphaInfo = CGImageGetAlphaInfo(CGImage);
	BOOL hasAlpha = (alphaInfo == kCGImageAlphaPremultipliedLast ||
					 alphaInfo == kCGImageAlphaPremultipliedFirst ||
					 alphaInfo == kCGImageAlphaLast ||
					 alphaInfo == kCGImageAlphaFirst) ? YES : NO;
	
	/*
	 * WORKAROUND
	 * set hasAlpha to YES. It is a workaround for iPad. JPG image is abnormal after loading in
	 * iPad, but not in iPhone. Maybe a bug of iPad and force setting hasAlpha to YES solve this.
	 */
	hasAlpha = YES;
	
	// get image original size
	int bW = CGImageGetWidth(CGImage);
	int bH = CGImageGetHeight(CGImage);
	
	// need swap width and height for left/right orientation
	UIImageOrientation orientation = image.imageOrientation;
	BOOL swapWH = (orientation == UIImageOrientationLeft ||
				   orientation == UIImageOrientationRight ||
				   orientation == UIImageOrientationLeftMirrored ||
				   orientation == UIImageOrientationRightMirrored);
	
	// calculate real context size
	int w, h;
	if(self.keepRatio) {
		float scaleW = self.expectedWidth / (float)(swapWH ? bH : bW);
		float scaleH = self.expectedHeight / (float)(swapWH ? bW : bH);
		float scale = MIN(scaleW, scaleH);
		w = (swapWH ? bH : bW) * scale;
		h = (swapWH ? bW : bH) * scale;
	} else {
		w = self.expectedWidth;
		h = self.expectedHeight;
	}
	
	// now build transform for orientation
	float scaleX = 1;
	float scaleY = 1;
	CGAffineTransform transform = CGAffineTransformIdentity;
	switch(orientation) {
		case UIImageOrientationDown:
			transform = CGAffineTransformTranslate(transform, w, h);
			transform = CGAffineTransformRotate(transform, -M_PI);
			break;
		case UIImageOrientationLeft:
			transform = CGAffineTransformTranslate(transform, w, 0);
			transform = CGAffineTransformRotate(transform, M_PI_2);
			break;
		case UIImageOrientationRight:
			transform = CGAffineTransformTranslate(transform, 0, h);
			transform = CGAffineTransformRotate(transform, -M_PI_2);
			break;
		default:
			// do nothing
			break;
	}
	
	// calculate real drawing dest rect
	CGRect imageRect = CGRectMake(0,
								  0,
								  swapWH ? h : w,
								  swapWH ? w : h);
	
	// create a bitmap
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
	void* data = malloc(w * h * 4 * sizeof(char));
	alphaInfo = hasAlpha ? kCGImageAlphaPremultipliedLast : kCGImageAlphaNoneSkipLast;
	CGContextRef context = CGBitmapContextCreate(data, w, h, 8, 4 * w, colorSpace, alphaInfo | kCGBitmapByteOrder32Big);
	CGContextScaleCTM(context, scaleX, scaleY);
	CGContextTranslateCTM(context, scaleX == -1 ? (swapWH ? h : w) : 0, scaleY == -1 ? (swapWH ? w : h) : 0);
	CGContextConcatCTM(context, transform);
	
	// draw original image to bitmap, and create a new cg image
	CGContextClearRect(context, imageRect);
	CGContextDrawImage(context, imageRect, CGImage);
	CGImageRef cropImage = CGBitmapContextCreateImage(context);
	
	// check extension
	string ext = CCUtils::getPathExtension(self.path);
	bool png = ext == ".png";
	bool jpg = ext == ".jpg" || ext == ".jpeg";
	if(!png && !jpg)
		jpg = true;
	
	// ensure parent folder is all created
	string fullPath = CCFileUtils::sharedFileUtils()->getWritablePath() + self.path;
	NSString* nsPath = [NSString stringWithUTF8String:fullPath.c_str()];
	NSString* parentPath = [nsPath stringByDeletingLastPathComponent];
	NSFileManager* fm = [NSFileManager defaultManager];
	[fm createDirectoryAtPath:parentPath withIntermediateDirectories:YES attributes:Nil error:nil];
	
	// save cg image to file
	UIImage* uiImage = [UIImage imageWithCGImage:cropImage];
	if(png) {
		NSData* pngData = UIImagePNGRepresentation(uiImage);
		[pngData writeToFile:[NSString stringWithCString:fullPath.c_str() encoding:NSUTF8StringEncoding]
				  atomically:NO];
	} else {
		NSData* jpgData = UIImageJPEGRepresentation(uiImage, 1.0);
		[jpgData writeToFile:[NSString stringWithCString:fullPath.c_str() encoding:NSUTF8StringEncoding]
				  atomically:NO];
	}
	
	// release
	CGContextRelease(context);
	CGColorSpaceRelease(colorSpace);
	CGImageRelease(cropImage);
	free(data);
	
	// hide picker view
	[self.parent dismissModalViewControllerAnimated:YES];
	
	// release self
#if !__has_feature(objc_arc)
	[self autorelease];
#endif
	
	// callback
	if(self.callback) {
		self.callback->onImagePicked(fullPath, w, h);
	}
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker {
	// hide picker view
	[self.parent dismissModalViewControllerAnimated:YES];
	
	// notify callback
	if(self.callback) {
		self.callback->onImagePickingCancelled();
	}
	
	// release self
#if !__has_feature(objc_arc)
	[self autorelease];
#endif
}

@end

NS_CC_BEGIN

bool CCImagePicker::hasCamera() {
	return [UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera];
}

bool CCImagePicker::hasFrontCamera() {
	if([[UIDevice currentDevice].systemVersion floatValue] >= 4)
		return [UIImagePickerController isCameraDeviceAvailable:UIImagePickerControllerCameraDeviceFront];
	else
		return false;
}

void CCImagePicker::pickFromCamera(const string& path, CCImagePickerCallback* callback, int w, int h, bool front, bool keepRatio) {
	if(front && !hasFrontCamera()) {
		if(callback)
			callback->onImagePickingCancelled();
		return;
	}
	if(!front && !hasCamera()) {
		if(callback)
			callback->onImagePickingCancelled();
		return;
	}
	
	// acquire a view controller
	UIView* view = [EAGLView sharedEGLView];
	UIViewController* vc = CCUtils::findViewController(view);
	if(!vc) {
		CCLOGWARN("Can't find a view controller, camera picker can't be started");
		if(callback)
			callback->onImagePickingCancelled();
		return;
	}
	
	// create image picker
	UIImagePickerController* picker = [[UIImagePickerController alloc] init];
    picker.sourceType = UIImagePickerControllerSourceTypeCamera;
    if(front)
    	picker.cameraDevice = UIImagePickerControllerCameraDeviceFront;
	picker.allowsEditing = YES;
	
	// set delegate
	CCImagePickerDelegate* d = [[CCImagePickerDelegate alloc] init];
	d.path = path;
	d.parent = vc;
	d.callback = callback;
	d.expectedWidth = w;
	d.expectedHeight = h;
	d.keepRatio = keepRatio;
	picker.delegate = d;
	
	// show picker
	[vc presentModalViewController:picker animated:YES];
	
	// release
#if !__has_feature(objc_arc)
	[picker release];
#endif
}

void CCImagePicker::pickFromAlbum(const string& path, CCImagePickerCallback* callback, int w, int h, bool keepRatio) {
	// acquire a view controller
	UIView* view = [EAGLView sharedEGLView];
	UIViewController* vc = CCUtils::findViewController(view);
	if(!vc) {
		CCLOGWARN("Can't find a view controller, camera picker can't be started");
		if(callback)
			callback->onImagePickingCancelled();
		return;
	}
	
	// create image picker
	UIImagePickerController* picker = [[UIImagePickerController alloc] init];
    picker.sourceType = UIImagePickerControllerSourceTypeSavedPhotosAlbum;
	picker.allowsEditing = YES;
	
	// set delegate
	CCImagePickerDelegate* d = [[CCImagePickerDelegate alloc] init];
	d.path = path;
	d.parent = vc;
	d.callback = callback;
	d.expectedWidth = w;
	d.expectedHeight = h;
	d.keepRatio = keepRatio;
	picker.delegate = d;
	
	// show picker
	[vc presentModalViewController:picker animated:YES];
	
	// release
#if !__has_feature(objc_arc)
	[picker release];
#endif
}

NS_CC_END

#endif // #if CC_TARGET_PLATFORM == CC_PLATFORM_IOS