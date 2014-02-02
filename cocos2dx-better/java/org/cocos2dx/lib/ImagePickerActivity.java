package org.cocos2dx.lib;

import org.cocos2dx.lib.cropimage.CropImageIntentBuilder;

import android.app.Activity;
import android.content.Intent;
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
		
		if(ImagePicker.sFromAlbum) {
			Intent intent = new Intent(Intent.ACTION_PICK);
			intent.setType("image/*");
			startActivityForResult(intent, REQ_SELECT_PHOTO);
		} else {
			Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
			intent.putExtra(MediaStore.EXTRA_OUTPUT, Uri.fromFile(ImagePicker.sDestFile));
			if(ImagePicker.sFront) {
				intent.putExtra("android.intent.extras.CAMERA_FACING", CameraInfo.CAMERA_FACING_FRONT);
			}
			startActivityForResult(intent, REQ_CAPTURE_IMAGE); 
		}
	}
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		switch(requestCode) {
			case REQ_SELECT_PHOTO:
				if(resultCode == RESULT_OK) {
					Uri selectedImage = data.getData();
					Uri uri = Uri.fromFile(ImagePicker.sDestFile);
		            CropImageIntentBuilder cropImage = new CropImageIntentBuilder(ImagePicker.sExpectedWidth, 
		            		ImagePicker.sExpectedHeight, uri);
		            cropImage.setSourceImage(selectedImage);
		            startActivityForResult(cropImage.getIntent(this), REQ_CROP_IMAGE);
				} else {
					// callback
					ImagePicker.onImagePickingCancelled();
					
					// finish self
					finish();
				}
				break;
			case REQ_CAPTURE_IMAGE:
				if(resultCode == RESULT_OK) {
		            Uri uri = Uri.fromFile(ImagePicker.sDestFile);
		            CropImageIntentBuilder cropImage = new CropImageIntentBuilder(ImagePicker.sExpectedWidth, 
		            		ImagePicker.sExpectedHeight, uri);
		            cropImage.setSourceImage(uri);
		            startActivityForResult(cropImage.getIntent(this), REQ_CROP_IMAGE);
				} else {
					// callback
					ImagePicker.onImagePickingCancelled();
					
					// finish self
					finish();
				}
				
				break;
			case REQ_CROP_IMAGE:
				if(resultCode == RESULT_OK) {
					ImagePicker.onImagePicked();
				} else {
					ImagePicker.onImagePickingCancelled();
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
