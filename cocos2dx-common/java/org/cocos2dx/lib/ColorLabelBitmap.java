/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

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

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Bitmap.Config;

public class ColorLabelBitmap {

	public static void createColorLabelBitmap(String pString, final String pFontName, final int pFontSize,
	        final float fontTintR, final float fontTintG, final float fontTintB, final int pAlignment,
	        final int pWidth, final int pHeight, final boolean shadow, final float shadowDX, final float shadowDY,
	        final float shadowBlur, final boolean stroke, final float strokeR, final float strokeG,
	        final float strokeB, final float strokeSize) {
		Bitmap bitmap = Bitmap.createBitmap(128, 128, Config.ARGB_8888);
		Canvas c = new Canvas(bitmap);
		c.drawColor(0xff00ffff);
		initNativeObject(bitmap);
		bitmap.recycle();
	}
	
	private static byte[] getPixels(final Bitmap pBitmap) {
		if (pBitmap != null) {
			final byte[] pixels = new byte[pBitmap.getWidth()
					* pBitmap.getHeight() * 4];
			final ByteBuffer buf = ByteBuffer.wrap(pixels);
			buf.order(ByteOrder.nativeOrder());
			pBitmap.copyPixelsToBuffer(buf);
			return pixels;
		}

		return null;
	}

	private static void initNativeObject(final Bitmap pBitmap) {
		final byte[] pixels = getPixels(pBitmap);
		if (pixels == null) {
			return;
		}

		nativeInitBitmapDC(pBitmap.getWidth(), pBitmap.getHeight(), pixels);
	}
	
	private static native void nativeInitBitmapDC(int pWidth, int pHeight, byte[] pPixels);
}
