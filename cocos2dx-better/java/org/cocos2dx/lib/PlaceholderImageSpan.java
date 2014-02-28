package org.cocos2dx.lib;

import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Paint.FontMetricsInt;
import android.graphics.drawable.Drawable;
import android.text.style.DynamicDrawableSpan;

public class PlaceholderImageSpan extends DynamicDrawableSpan {
	private int mOffsetY;
	private int mBitmapWidth;
	private int mBitmapHeight;
	
	public PlaceholderImageSpan(int bitmapWidth, int bitmapHeight, float offsetY) {
		mOffsetY = (int)offsetY;
		mBitmapWidth = bitmapWidth;
		mBitmapHeight = bitmapHeight;
	}
	
	@Override
	public Drawable getDrawable() {
		// no need to implement this method
	    return null;
	}
	
	@Override
	public int getSize(Paint paint, CharSequence text, int start, int end, FontMetricsInt fm) {
        if (fm != null) {
            fm.ascent = Math.min(0, -mBitmapHeight - mOffsetY); 
            fm.descent = Math.max(0, -mOffsetY);
            fm.top = fm.ascent;
            fm.bottom = fm.descent;
        }

        return mBitmapWidth;
	}	
	
	@Override
	public void draw(Canvas canvas, CharSequence text, int start, int end, float x, int top, int y, int bottom,
	        Paint paint) {
		// it doesn't draw here, so it is just a placeholder
	}
}
