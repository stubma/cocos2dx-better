package org.cocos2dx.lib;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Paint.FontMetricsInt;
import android.graphics.Rect;
import android.graphics.drawable.Drawable;
import android.text.style.ImageSpan;

public class CCImageSpan extends ImageSpan {
	private float mOffsetY;
	private float mScale;
	
	public CCImageSpan(Bitmap bitmap, int alignment, float offsetY) {
		super(bitmap, alignment);
		mOffsetY = offsetY;
		mScale = (float)getDrawable().getBounds().bottom / bitmap.getHeight();
	}
	
	@Override
	public int getSize(Paint paint, CharSequence text, int start, int end, FontMetricsInt fm) {
	    Drawable d = getDrawable();
	    Rect rect = d.getBounds();
	    
	    int intOffset = (int)(mOffsetY * mScale);
	    if(fm != null) {
	    	fm.ascent = Math.min(0, -rect.bottom - intOffset);
	    	fm.descent = Math.max(0, -intOffset);
	    	fm.top = fm.ascent;
	    	fm.bottom = fm.descent;
	    }
	    
	    return rect.bottom + Math.max(0, intOffset);
	}	
	
	@Override
	public void draw(Canvas canvas, CharSequence text, int start, int end, float x, int top, int y, int bottom,
	        Paint paint) {
	    Drawable d = getDrawable();
	    int intOffset = (int)(mOffsetY * mScale);
	    canvas.save();
	    float transY = Math.max(0, -intOffset);
    	canvas.translate(x, transY);
	    d.draw(canvas);
	    canvas.restore();
	}
}
