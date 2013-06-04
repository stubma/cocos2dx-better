/****************************************************************************
 Author: Luma (stubma@gmail.com)
 
 https://github.com/stubma/cocos2dx-common
 
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
import java.util.ArrayList;
import java.util.List;

import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Typeface;
import android.text.Layout.Alignment;
import android.text.Spannable;
import android.text.SpannableString;
import android.text.StaticLayout;
import android.text.TextPaint;
import android.text.style.ForegroundColorSpan;
import android.util.Log;

public class RichLabelBitmap {
	/* The values are the same as cocos2dx/platform/CCImage.h. */
	private static final int HORIZONTALALIGN_LEFT = 1;
	private static final int HORIZONTALALIGN_RIGHT = 2;
	private static final int HORIZONTALALIGN_CENTER = 3;
	private static final int VERTICALALIGN_TOP = 1;
	private static final int VERTICALALIGN_BOTTOM = 2;
	private static final int VERTICALALIGN_CENTER = 3;
	
	final static class Span {
		public int start;
		public int end;
		public int color;
		public ForegroundColorSpan style;
	}
	
	public static void createRichLabelBitmap(String pString, final String pFontName, final int pFontSize,
	        final float fontTintR, final float fontTintG, final float fontTintB, final int pAlignment,
	        final int pWidth, final int pHeight, final boolean shadow, final float shadowDX, final float shadowDY,
	        final float shadowBlur, final boolean stroke, final float strokeR, final float strokeG,
	        final float strokeB, final float strokeSize) {
		// extract span info and return text without span style
		List<Span> spans = new ArrayList<Span>();
		int color = 0xff000000 | ((int)(fontTintR * 255) << 16) | ((int)(fontTintG * 255) << 8) | (int)(fontTintB * 255);
		String plain = buildSpan(pString, spans, color);
		
		// build spannable string, install span color
		SpannableString rich = new SpannableString(plain);
		for(Span span : spans) {
			span.style = new ForegroundColorSpan(span.color);
			rich.setSpan(span.style, span.start, span.end, Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
		}
		
		// alignment
		int horizontalAlignment = pAlignment & 0x0F;
		int verticalAlignment = (pAlignment >> 4) & 0x0F;
		Alignment align = Alignment.ALIGN_NORMAL;
		switch(horizontalAlignment) {
			case HORIZONTALALIGN_CENTER:
				align = Alignment.ALIGN_CENTER;
				break;
			case HORIZONTALALIGN_RIGHT:
				align = Alignment.ALIGN_OPPOSITE;
				break;
		}
		
		// create paint
		TextPaint paint = new TextPaint();
		paint.setColor(Color.WHITE);
		paint.setTextSize(pFontSize); 
		paint.setAntiAlias(true);
		if (pFontName.endsWith(".ttf")) {
			try {
				Typeface typeFace = Cocos2dxTypefaces.get(Cocos2dxActivity.getContext(), pFontName);
				paint.setTypeface(typeFace);
			} catch (final Exception e) {
				Log.e("ColorLabelBitmap", "error to create ttf type face: " + pFontName);

				/* The file may not find, use system font. */
				paint.setTypeface(Typeface.create(pFontName, Typeface.NORMAL));
			}
		} else {
			paint.setTypeface(Typeface.create(pFontName, Typeface.NORMAL));
		}
		
		// shadow
		float bitmapPaddingX = 0.0f;
		float bitmapPaddingY = 0.0f;
		float renderTextDeltaX = 0.0f;
		float renderTextDeltaY = 0.0f;
		if (shadow) {
			int shadowColor = 0xff333333;
			paint.setShadowLayer(shadowBlur, shadowDX, shadowDY, shadowColor);

			bitmapPaddingX = Math.abs(shadowDX);
			bitmapPaddingY = Math.abs(shadowDY);

			if (shadowDX < 0.0) {
				renderTextDeltaX = bitmapPaddingX;
			}

			if (shadowDY < 0.0) {
				renderTextDeltaY = bitmapPaddingY;
			}
		}
		
        // compute the padding needed by shadow and stroke
        float shadowStrokePaddingX = 0.0f;
        float shadowStrokePaddingY = 0.0f;
        if (stroke) {
            shadowStrokePaddingX = (float)Math.ceil(strokeSize);
            shadowStrokePaddingY = (float)Math.ceil(strokeSize);
        }
        if (shadow) {
            shadowStrokePaddingX = Math.max(shadowStrokePaddingX, (float)Math.abs(shadowDX));
            shadowStrokePaddingY = Math.max(shadowStrokePaddingY, (float)Math.abs(shadowDY));
        }
		
		// layout this text
		StaticLayout layout = new StaticLayout(rich,
				paint, 
				pWidth <= 0 ? Integer.MAX_VALUE : pWidth, 
				align, 
				1, 
				0, 
				false);
		
		// size of layout
		int width = layout.getWidth();
		int height = layout.getHeight();
		
		// add padding of stroke
		width += shadowStrokePaddingX;
		height += shadowStrokePaddingY;
		
        // vertical alignment
        int startH = 0;
        if (pHeight > height) {
            // vertical alignment
            if (verticalAlignment == VERTICALALIGN_TOP) {
                startH = 0;
            } else if (verticalAlignment == VERTICALALIGN_CENTER) {
                startH = (pHeight - height) / 2;
            } else {
                startH = pHeight - height;
            }
        }
		
		// adjust layout
		if(pWidth > 0 && pWidth > width)
			width = pWidth;
		if(pHeight > 0 && pHeight > height)
			height = pHeight;
		
		// create bitmap and canvas
		Bitmap bitmap = Bitmap.createBitmap(width, height, Config.ARGB_8888);
		Canvas c = new Canvas(bitmap);
		
		// translate for shadow
		c.translate(renderTextDeltaX, renderTextDeltaY);
		
		// translate for vertical alignment
		c.translate(0, startH);
		
		// draw text
		layout.draw(c);
		
		// draw again if stroke is enabled
		if(stroke) {
			paint.setStyle(Paint.Style.STROKE);
			paint.setStrokeWidth(strokeSize);
			paint.setARGB(255, (int)strokeR * 255, (int)strokeG * 255, (int)strokeB * 255);
			paint.clearShadowLayer();
			for(Span span : spans) {
				if(span.style != null) {
					rich.removeSpan(span.style);
				}
			}
			layout.draw(c);
		}
		
		// transfer bitmap data to native layer, and release bitmap when done
		initNativeObject(bitmap);
		bitmap.recycle();
	}
	
	private static String buildSpan(String text, List<Span> spans, int defaultColor) {
		boolean inSpan = false;
		int spanStart = 0;
		int uniLen = text.length();
		StringBuilder plain = new StringBuilder();
		for(int i = 0; i < uniLen; i++) {
			char c = text.charAt(i);
			switch(c) {
				case '\\':
					if(i < uniLen - 1) {
						char cc = text.charAt(i + 1);
						if(cc == '[' || cc == ']') {
							plain.append(cc);
							i++;
						}
					} else {
						plain.append(c);
					}
					break;
				case '[':
				{
					// find close bracket
					int j;
					for(j = i + 1; j < uniLen; j++) {
						char cc = text.charAt(j);
						if(cc == ']') {
							break;
						}
					}
					
					// if no close bracket, discard those content
					if(j >= uniLen) {
						i = j;
						break;
					}
					
					// if find, we need check whether we are in a span
					if(inSpan) {
						if(text.charAt(i) == '['
	                       && text.charAt(i + 1) == '/'
	                       && text.charAt(i + 2) == 'c'
	                       && text.charAt(i + 3) == 'o'
	                       && text.charAt(i + 4) == 'l'
	                       && text.charAt(i + 5) == 'o'
	                       && text.charAt(i + 6) == 'r'
	                       && text.charAt(i + 7) == ']') {
							inSpan = false;
							spanStart = plain.length();
						}
						
						// reset i to skip this part
						i = j;
					} else {
						// parse this span, if not recognized, need ignore
						if(text.charAt(i + 1) == 'c'
	                       && text.charAt(i + 2) == 'o'
	                       && text.charAt(i + 3) == 'l'
	                       && text.charAt(i + 4) == 'o'
	                       && text.charAt(i + 5) == 'r'
	                       && text.charAt(i + 6) == '=') {
							// add previous span
							if(plain.length() > spanStart) {
								Span span = new Span();
								span.start = spanStart;
								span.end = plain.length();
								span.color = defaultColor;
								spans.add(span);
							}
							spanStart = plain.length();
							
							// add color span
							int color = parseColor(text, i + 7, j - i - 7);
							Span span = new Span();
							span.start = spanStart;
							span.end = spanStart;
							span.color = color;
							spans.add(span);
							
							// set flag
							inSpan = true;
						}
						
						// reset i to skip this part
						i = j;
					}
					
					break;
				}
				case ']':
					// just discard it
					break;
				default:
					plain.append(c);
					
					// if in span, update last span end position
					if(inSpan) {
						Span span = spans.get(spans.size() - 1);
						span.end++;
					}
					break;
			}
		}
		
		// last span
		if(spanStart < uniLen && plain.length() > spanStart) {
			Span span = new Span();
			span.start = spanStart;
			span.end = plain.length();
			span.color = defaultColor;
			spans.add(span);
		}
		
		// debug output span info
//		int i = 0;
//		for(Span span : spans) {
//			Log.d("test", String.format("span %d: %d - %d, color: 0x%08x", i++, span.start, span.end, span.color));
//		}
	    
		// return plain str
		return plain.toString();
	}
	
	private static int parseColor(String text, int start, int len) {
		int color = 0;
		int end = start + len;
		for(int i = start; i < end; i++) {
			color <<= 4;
			char c = text.charAt(i);
			if(c >= '0' && c <= '9') {
				color |= c - '0';
			} else if(c >= 'a' && c <= 'f') {
				color |= c - 'a' + 10;
			} else if(c >= 'A' && c <= 'F') {
				color |= c - 'A' + 10;
			}
		}
		
		return color;
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
