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
#ifndef __CCImagePicker__
#define __CCImagePicker__

#include "cocos2d.h"
#include "CCImagePickerCallback.h"

using namespace std;

NS_CC_BEGIN

/**
 * Image picker wrapper. Picked image will be saved into a file and caller should provide a callback
 * to get the file path.
 * 
 * \note
 * It supports Android 2.3+, but parameter keepRatio is not supported in Android.
 */
class CC_DLL CCImagePicker {
	/**
	 * pick a image from camera and return it as RGBA8888. You can specify an expected
	 * image size and image will be scaled if not matched. If device doesn't have a desired camera,
	 * callback onImagePickingCancelled will be invoked	 
	 *
	 * @param path the relative path of image file, it will be mapped to platform writable path. In iOS, it is ~/Documents,
	 *		in Android, it is cache dir
	 * @param callback callback
	 * @param w expected image width
	 * @param h expected image height
	 * @param front true means use front camera
	 * @param keepRatio True means if the image need to be scaled, it will keep the
	 *		width height ratio.
	 */
	static void pickFromCamera(const string& path, CCImagePickerCallback* callback, int w, int h, bool front, bool keepRatio);
	
public:
	/**
	 * does device have camera
	 *
	 * @return true means device has at least one camera
	 */
	static bool hasCamera();
	
	/**
	 * does device have front camera
	 *
	 * @return true means device has front camera
	 */
	static bool hasFrontCamera();
	
	/**
	 * pick a image from camera and save it to file. You can specify an expected
	 * image size and image will be scaled if not matched. If device doesn't have a camera,
	 * callback onImagePickingCancelled will be invoked
	 *
	 * @param path the relative path of image file, it will be mapped to platform writable path. In iOS, it is ~/Documents,
	 *		in Android, it is cache dir
	 * @param callback callback
	 * @param w expected image width, the final image width may not same as this
	 * @param h expected image height, the final image height may not same as this
	 * @param keepRatio by default is true. True means if the image need to be scaled, it will keep the
	 *		width height ratio.
	 */
	static void pickFromCamera(const string& path, CCImagePickerCallback* callback, int w, int h, bool keepRatio = true);
	
	/**
	 * pick a image from front camera and save it to a file. You can specify an expected
	 * image size and image will be scaled if not matched. If device doesn't have a front camera,
	 * callback onImagePickingCancelled will be invoked.
	 *
	 * @param path the relative path of image file, it will be mapped to platform writable path. In iOS, it is ~/Documents,
	 *		in Android, it is cache dir
	 * @param callback callback
	 * @param w expected image width
	 * @param h expected image height
	 * @param keepRatio by default is true. True means if the image need to be scaled, it will keep the
	 *		width height ratio.
	 */
	static void pickFromFrontCamera(const string& path, CCImagePickerCallback* callback, int w, int h, bool keepRatio = true);
	
	/**
	 * pick a image from album and save it to a file. You can specify an expected
	 * image size and image will be scaled if not matched.
	 *
	 * @param path the relative path of image file, it will be mapped to platform writable path. In iOS, it is ~/Documents,
	 *		in Android, it is cache dir
	 * @param callback callback
	 * @param w expected image width
	 * @param h expected image height
	 * @param keepRatio by default is true. True means if the image need to be scaled, it will keep the
	 *		width height ratio.
	 */
	static void pickFromAlbum(const string& path, CCImagePickerCallback* callback, int w, int h, bool keepRatio = true);
};

NS_CC_END

#endif // __CCImagePicker__
