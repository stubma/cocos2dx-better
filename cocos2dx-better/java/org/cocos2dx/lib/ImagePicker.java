package org.cocos2dx.lib;

import java.io.File;

import android.content.Context;
import android.content.Intent;
import android.hardware.Camera;
import android.hardware.Camera.CameraInfo;
import android.os.Environment;

public class ImagePicker {
	static long sCallback;
	static String sPath;
	static int sExpectedWidth;
	static int sExpectedHeight;
	static boolean sFront;
	static boolean sKeepRatio;
	static File sDestFile;
	
	static boolean hasCamera() {
		return Camera.getNumberOfCameras() > 0;
	}

	static boolean hasFrontCamera() {
		int c = Camera.getNumberOfCameras();
		for(int i = 0; i < c; i++) {
			CameraInfo cameraInfo = new CameraInfo();
			Camera.getCameraInfo(i, cameraInfo);
			if(cameraInfo.facing == CameraInfo.CAMERA_FACING_FRONT)
				return true;
		}
		return false;
	}

	static void pickFromCamera(String path, long callback, int w, int h, boolean front, boolean keepRatio) {
		sCallback = callback;
		sPath = path;
		sExpectedWidth = w;
		sExpectedHeight = h;
		sFront = front;
		sKeepRatio = keepRatio;
		
		// the full path of image file
		sDestFile = new File(sPath);
		if(!sPath.startsWith("/")) {
			sDestFile = new File(Environment.getExternalStorageDirectory(), sPath);
		}
		if(!sDestFile.getParentFile().exists()) {
			sDestFile.getParentFile().mkdirs();
		}
		
		// start a delegate activity, it will call other activity to finish camera and crop process
		Context ctx = Cocos2dxActivity.getContext();
		Intent intent = new Intent(ctx, ImagePickerActivity.class);
		ctx.startActivity(intent);
	}

	static void pickFromAlbum(String path, long callback, int w, int h, boolean keepRatio) {
		sCallback = callback;
		sPath = path;
		sExpectedWidth = w;
		sExpectedHeight = h;
		sKeepRatio = keepRatio;
		
		// the full path of image file
		sDestFile = new File(sPath);
		if(!sPath.startsWith("/")) {
			sDestFile = new File(Environment.getExternalStorageDirectory(), sPath);
		}
		if(!sDestFile.getParentFile().exists()) {
			sDestFile.getParentFile().mkdirs();
		}
	}
	
	static void onImagePicked() {
		if(sCallback != 0) {
			((Cocos2dxActivity)Cocos2dxActivity.getContext()).runOnGLThread(new Runnable() {
				@Override
				public void run() {
					nativeOnImagePicked(sCallback, sDestFile.getAbsolutePath(), sExpectedWidth, sExpectedHeight);
				}
			});
		}
	}
	
	static void onImagePickingCancelled() {
		if(sCallback != 0) {
			((Cocos2dxActivity)Cocos2dxActivity.getContext()).runOnGLThread(new Runnable() {
				@Override
				public void run() {
					nativeOnImagePickingCancelled(sCallback);
				}
			});
		}
	}
	
	private static native void nativeOnImagePicked(long callback, String fullPath, int w, int h);
	private static native void nativeOnImagePickingCancelled(long callback);
}
