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
package org.cocos2dx.lib;

import org.cocos2dx.lib.cropimage.CropImageIntentBuilder;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap.CompressFormat;
import android.hardware.Camera.CameraInfo;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;

public class ImagePickerActivity extends Activity {
	private static final int REQ_CAPTURE_IMAGE = 100; 
	private static final int REQ_CROP_IMAGE = 101; 
	private static final int REQ_SELECT_PHOTO = 102; 
	
	@Override
	protected void onPostCreate(Bundle savedInstanceState) {
		super.onPostCreate(savedInstanceState);
		
		if(CCImagePicker.sFromAlbum) {
			Intent intent = new Intent(Intent.ACTION_PICK);
			intent.setType("image/*");
			startActivityForResult(intent, REQ_SELECT_PHOTO);
		} else {
			Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
			intent.putExtra(MediaStore.EXTRA_OUTPUT, Uri.fromFile(CCImagePicker.sDestFile));
			if(CCImagePicker.sFront) {
				intent.putExtra("android.intent.extras.CAMERA_FACING", CameraInfo.CAMERA_FACING_FRONT);
			}
			startActivityForResult(intent, REQ_CAPTURE_IMAGE); 
		}
	}
	
	private boolean isPNG() {
		String path = CCImagePicker.sDestFile.getAbsolutePath();
		int lastDot = path.lastIndexOf('.');
		if(lastDot == -1) {
			return false;
		} else {
			return path.substring(lastDot + 1).equalsIgnoreCase("png");
		}
	}
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		switch(requestCode) {
			case REQ_SELECT_PHOTO:
				if(resultCode == RESULT_OK) {
					Uri selectedImage = data.getData();
					Uri uri = Uri.fromFile(CCImagePicker.sDestFile);
		            CropImageIntentBuilder cropImage = new CropImageIntentBuilder(CCImagePicker.sExpectedWidth, 
		            		CCImagePicker.sExpectedHeight, uri);
		            cropImage.setSourceImage(selectedImage);
		            if(isPNG())
		            	cropImage.setOutputFormat(CompressFormat.PNG.toString());
		            startActivityForResult(cropImage.getIntent(this), REQ_CROP_IMAGE);
				} else {
					// callback
					CCImagePicker.onImagePickingCancelled();
					
					// finish self
					finish();
				}
				break;
			case REQ_CAPTURE_IMAGE:
				if(resultCode == RESULT_OK) {
		            Uri uri = Uri.fromFile(CCImagePicker.sDestFile);
		            CropImageIntentBuilder cropImage = new CropImageIntentBuilder(CCImagePicker.sExpectedWidth, 
		            		CCImagePicker.sExpectedHeight, uri);
		            cropImage.setSourceImage(uri);
		            if(isPNG())
		            	cropImage.setOutputFormat(CompressFormat.PNG.toString());
		            startActivityForResult(cropImage.getIntent(this), REQ_CROP_IMAGE);
				} else {
					// callback
					CCImagePicker.onImagePickingCancelled();
					
					// finish self
					finish();
				}
				
				break;
			case REQ_CROP_IMAGE:
				if(resultCode == RESULT_OK) {
					CCImagePicker.onImagePicked();
				} else {
					CCImagePicker.onImagePickingCancelled();
				}
				
				// finish self
				finish();
				
				break;
			default:
				super.onActivityResult(requestCode, resultCode, data);
				break;
		}
	}
}
