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

import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Stack;

import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Point;
import android.graphics.PointF;
import android.graphics.Typeface;
import android.text.Layout.Alignment;
import android.text.Spannable;
import android.text.SpannableString;
import android.text.StaticLayout;
import android.text.TextPaint;
import android.text.TextUtils;
import android.text.style.AbsoluteSizeSpan;
import android.text.style.DynamicDrawableSpan;
import android.text.style.ForegroundColorSpan;
import android.text.style.TypefaceSpan;
import android.text.style.UnderlineSpan;
import android.util.Log;

public class CCImage_richlabel {
	/* The values are the same as cocos2dx/platform/CCImage.h. */
	private static final int HORIZONTALALIGN_LEFT = 1;
	private static final int HORIZONTALALIGN_RIGHT = 2;
	private static final int HORIZONTALALIGN_CENTER = 3;
	private static final int VERTICALALIGN_TOP = 1;
	private static final int VERTICALALIGN_BOTTOM = 2;
	private static final int VERTICALALIGN_CENTER = 3;
	
	private static final char TAG_START = '[';
	private static final char TAG_END = ']';
	
	private enum SpanType {
		UNKNOWN,
	    COLOR,
	    FONT,
	    SIZE,
	    BOLD,
	    ITALIC,
	    UNDERLINE,
	    IMAGE,
	    LINK
	}
	
	// link meta info
	final static class LinkMeta {
	    int normalBgColor;
	    int selectedBgColor;
		
		// the tag of link, multiple link can have same tag (in line break situation)
		int tag;
	    
	    // link rect area
	    float x;
	    float y;
	    float width;
	    float height;
	}
	
	// span info
	final static class Span {
		public SpanType type;
		public boolean close;
		public int pos;
		
		// only for color
		public int color;
		
		// only for size
		public float fontSize;
		
		// only for font
		public String fontName;
		
		// only for image
		// offsetY follow opengl rule
		public String imageName;
		public float scaleX;
		public float scaleY;
		public float width;
		public float height;
		public float offsetY;
		
	    // for link tag
	    int normalBgColor;
	    int selectedBgColor;
	}
	
	// tag parse result
	final static class TagParseResult {
		SpanType type;
		boolean close;
		int endTagPos;
		int dataStart;
		int dataEnd;
	}
	
	// tag parse state
	private enum TagParseState {
	    READY,
	    START_TAG,
	    CLOSE_TAG,
	    EQUAL,
	    SUCCESS,
	    FAIL
	}
	
	static class CustomTypefaceSpan extends TypefaceSpan {
        private final Typeface newType;

        public CustomTypefaceSpan(String family, Typeface type) {
            super(family);
            newType = type;
        }

        @Override
        public void updateDrawState(TextPaint ds) {
            applyCustomTypeFace(ds, newType);
        }

        @Override
        public void updateMeasureState(TextPaint paint) {
            applyCustomTypeFace(paint, newType);
        }

        private void applyCustomTypeFace(Paint paint, Typeface tf) {
            int oldStyle;
            Typeface old = paint.getTypeface();
            if (old == null) {
                oldStyle = 0;
            } else {
                oldStyle = old.getStyle();
            }

            int fake = oldStyle & ~tf.getStyle();
            if ((fake & Typeface.BOLD) != 0) {
                paint.setFakeBoldText(true);
            }

            if ((fake & Typeface.ITALIC) != 0) {
                paint.setTextSkewX(-0.25f);
            }

            paint.setTypeface(tf);
        }
    }
	
	public static void createRichLabelBitmap(String pString, final String pFontName, final int pFontSize,
	        final float fontTintR, final float fontTintG, final float fontTintB, final int pAlignment,
	        final int pWidth, final int pHeight, final boolean shadow, final float shadowDX, final float shadowDY,
	        final int shadowColor, final float shadowBlur, final boolean stroke, final float strokeR, final float strokeG,
	        final float strokeB, final float strokeSize, float contentScaleFactor, boolean sizeOnly) {
		// reset bitmap dc
		nativeResetBitmapDC();
		
		// extract span info and return text without span style
		List<Span> spans = new ArrayList<Span>();
		String plain = buildSpan(pString, spans);
		
		// bitmap can't be zero size, ensure the plain is not empty string
		if (TextUtils.isEmpty(plain)) {
			plain = " ";
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
		// shadowBlur can be zero in android Paint, so set a min value to 1
		if (shadow) {
			paint.setShadowLayer(Math.max(shadowBlur, 1), shadowDX, shadowDY, shadowColor);
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
        
        // stack of color and font
        int defaultColor = 0xff000000 
        		| ((int)(fontTintR * 255) << 16) 
        		| ((int)(fontTintG * 255) << 8) 
        		| (int)(fontTintB * 255);
        Typeface defaultFont = paint.getTypeface();
        Stack<Integer> colorStack = new Stack<Integer>();
        Stack<Typeface> fontStack = new Stack<Typeface>();
        Stack<Float> fontSizeStack = new Stack<Float>();
        colorStack.push(defaultColor);
        fontStack.push(defaultFont);
        fontSizeStack.push(pFontSize / contentScaleFactor);

        // build spannable string
        SpannableString rich = new SpannableString(plain);
        Map<String, Bitmap> imageMap = new HashMap<String, Bitmap>();
        int colorStart = 0;
        int fontStart = 0;
        int sizeStart = 0;
        int underlineStart = -1;
        Span openSpan = null;
        for(Span span : spans) {
        	if(span.close) {
                switch(span.type) {
                    case COLOR:
                    {
                    	int curColor = colorStack.pop();
                        if(span.pos > colorStart) {
                            rich.setSpan(new ForegroundColorSpan(curColor), 
                            		colorStart, 
                            		span.pos, 
                            		Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
                            
                            // start need to be reset
                            colorStart = span.pos;
                        }
                        
                        break;
                    }
                    case FONT:
                    case BOLD:
                    case ITALIC:
                    {
                        // set previous span
                        Typeface font = fontStack.pop();
                        if(span.pos > fontStart && font != null) {
                        	rich.setSpan(new CustomTypefaceSpan("", font),
                        			fontStart,
                        			span.pos,
                        			Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
                            fontStart = span.pos;
                        }
                        
                        break;
                    }
                    case SIZE:
                    {
                    	float size = fontSizeStack.pop();
                        if(span.pos > sizeStart) {
                        	rich.setSpan(new AbsoluteSizeSpan((int)(size * contentScaleFactor)),
                        			sizeStart,
                        			span.pos,
                        			Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
                        	sizeStart = span.pos;
                        }
                    	break;
                    }
                    case UNDERLINE:
                    {
                        if(underlineStart > -1) {
                        	rich.setSpan(new UnderlineSpan(), 
                        			underlineStart, 
                        			span.pos, 
                        			Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
                            underlineStart = -1;
                        }
                        break;
                    }
                    case IMAGE:
                    {
                    	if(openSpan != null && !TextUtils.isEmpty(openSpan.imageName)) {  
                    		// if imageName starts with '/', treat it like external image
                    		Bitmap bitmap = null;
                    		boolean external = openSpan.imageName.startsWith("/");
                    		InputStream is = null;
                    		try {
                    			if(external) {
                    				is = new FileInputStream(openSpan.imageName);
                    			} else {
                    				AssetManager am = Cocos2dxHelper.getAssetManager();
                    				String fullPath = nativeFullPathForFilename(openSpan.imageName);
                    				if(fullPath.startsWith("assets/"))
                    					fullPath = fullPath.substring("assets/".length());
                    				is = am.open(fullPath);
                    			}
								bitmap = BitmapFactory.decodeStream(is);
							} catch (Throwable e) {
							} finally {
								if(is != null) {
									try {
										is.close();
									} catch (IOException e) {
									}
								}
							}
                    		
                    		// scale image if necessary
							if(bitmap != null) {
								if(openSpan.scaleX != 1 || openSpan.scaleY != 1 || contentScaleFactor != 1) {
									float dstW = openSpan.width != 0 ? (int)openSpan.width : (int)(bitmap.getWidth() * openSpan.scaleX);
									float dstH = openSpan.height != 0 ? (int)openSpan.height : (int)(bitmap.getHeight() * openSpan.scaleY);
									Bitmap scaled = Bitmap.createScaledBitmap(bitmap, 
											(int)dstW,
											(int)dstH,
											true);
									if(scaled != bitmap) {
										bitmap.recycle();
									}
									bitmap = scaled;
								}
							}
                    		
                    		// register bitmap and set span for it
                    		if(bitmap != null) {
								imageMap.put(span.imageName, bitmap);
								rich.setSpan(new CCImageSpan(bitmap, DynamicDrawableSpan.ALIGN_BASELINE, openSpan.offsetY), 
									openSpan.pos,
									span.pos,
									Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
                    		}
                    		
                    		openSpan = null;
                    	}
                    	break;
                    }
                }
        	} else {
                switch(span.type) {
                    case COLOR:
                    {
                        // process previous run
                        if(span.pos > colorStart) {
                            int curColor = colorStack.peek();
                            rich.setSpan(new ForegroundColorSpan(curColor), 
                            		colorStart, 
                            		span.pos, 
                            		Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
                            
                            // start need to be reset
                            colorStart = span.pos;
                        }
                        
                        // push color
                        colorStack.push(span.color);
                        
                        break;
                    }
                    case FONT:
                    {
                        // set previous span
                    	Typeface curFont = fontStack.peek();
                        if(span.pos > fontStart) {
                        	rich.setSpan(new CustomTypefaceSpan("", curFont), 
                        			fontStart, 
                        			span.pos, 
                        			Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
                            fontStart = span.pos;
                        }
                        
                        // create the font
                        Typeface font = null;
                		if (span.fontName.endsWith(".ttf")) {
                			try {
                				font = Cocos2dxTypefaces.get(Cocos2dxActivity.getContext(), span.fontName);
                			} catch (final Exception e) {
                			}
                		} else {
                			font = Typeface.create(span.fontName, Typeface.NORMAL);
                			if(font == null) {
                				font = Typeface.DEFAULT;
                			}
                		}
                		fontStack.push(font);
                        
                        break;
                    }
                    case SIZE:
                    {
                        // set previous span
                        if(span.pos > sizeStart) {
                        	float size = fontSizeStack.peek();
                        	rich.setSpan(new AbsoluteSizeSpan((int)(size * contentScaleFactor)), 
                        			fontStart, 
                        			span.pos, 
                        			Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
                        	sizeStart = span.pos;
                        }
                        
                        // push new size
                        fontSizeStack.push(span.fontSize);
                        
                        break;
                    }
                    case BOLD:
                    {
                        // set previous span
                    	Typeface curFont = fontStack.peek();
                        if(span.pos > fontStart) {
                        	rich.setSpan(new CustomTypefaceSpan("", curFont), 
                        			fontStart, 
                        			span.pos, 
                        			Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
                            fontStart = span.pos;
                        }
                        
                        // create new font
                        Typeface font = Typeface.create(curFont, Typeface.BOLD | curFont.getStyle());
                        fontStack.push(font);
                        
                        break;
                    }
                    case ITALIC:
                    {
                        // set previous span
                    	Typeface curFont = fontStack.peek();
                        if(span.pos > fontStart) {
                        	rich.setSpan(new CustomTypefaceSpan("", curFont), 
                        			fontStart, 
                        			span.pos, 
                        			Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
                            fontStart = span.pos;
                        }
                        
                        // create new font
                        Typeface font = Typeface.create(curFont, Typeface.ITALIC | curFont.getStyle());
                        fontStack.push(font);
                        
                        break;
                    }
                    case UNDERLINE:
                    {
                        underlineStart = span.pos;
                        break;
                    }
                    case IMAGE:
                    {
                    	openSpan = span;
                    	break;
                    }
                }
        	}
        }
        
        // last segment
        if(plain.length() > colorStart) {
            int curColor = colorStack.peek();
            rich.setSpan(new ForegroundColorSpan(curColor), 
            		colorStart, 
            		plain.length(), 
            		Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
        }
        if(plain.length() > sizeStart) {
        	float size = fontSizeStack.peek();
        	rich.setSpan(new AbsoluteSizeSpan((int)(size * contentScaleFactor)), 
        			fontStart, 
        			plain.length(), 
        			Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
        }
        if(plain.length() > fontStart) {
        	Typeface curFont = fontStack.peek();
        	rich.setSpan(new CustomTypefaceSpan("", curFont), 
        			fontStart, 
        			plain.length(), 
        			Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
        }
		
		// layout this text
		StaticLayout layout = new StaticLayout(rich,
				paint, 
				pWidth <= 0 ? (int)StaticLayout.getDesiredWidth(rich, paint) : pWidth, 
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
		
        // text origin
        int startY = 0;
        int startX = 0;
        if (pHeight > height) {
            // vertical alignment
            if (verticalAlignment == VERTICALALIGN_TOP) {
                startY = 0;
            } else if (verticalAlignment == VERTICALALIGN_CENTER) {
                startY = (pHeight - height) / 2;
            } else {
                startY = pHeight - height;
            }
        }
        if(shadow) {
        	if(shadowDY < 0) {
        		startY -= shadowDY; 
        	}
        	if(shadowDX < 0) {
        		startX -= shadowDX;
        	}
        }
		
		// adjust layout
		if(pWidth > 0 && pWidth > width)
			width = pWidth;
		if(pHeight > 0 && pHeight > height)
			height = pHeight;
		
		// if only measure, just pass size to native layer
		// if not, render a bitmap
		if(!sizeOnly) {
			// save padding
			nativeSaveShadowStrokePadding(startX, Math.max(0, height - layout.getHeight() - startY));
			
			// create bitmap and canvas
			Bitmap bitmap = Bitmap.createBitmap(width, height, Config.ARGB_8888);
			Canvas c = new Canvas(bitmap);
			
			// translate for vertical alignment
			c.translate(startX, startY);
			
			// draw text
            layout.draw(c);
			
			// draw again if stroke is enabled
			if(stroke) {
				// reset paint to stroke mode
				paint.setStyle(Paint.Style.STROKE);
				paint.setStrokeWidth(strokeSize);
				paint.setARGB(255, (int)strokeR * 255, (int)strokeG * 255, (int)strokeB * 255);
				paint.clearShadowLayer();
				
				// clear color and underline span
				for(ForegroundColorSpan span : rich.getSpans(0, rich.length(), ForegroundColorSpan.class)) {
					rich.removeSpan(span);
				}
				for(UnderlineSpan span : rich.getSpans(0, rich.length(), UnderlineSpan.class)) {
					rich.removeSpan(span);
				}
				
				layout.draw(c);
			}
			
			// extract link meta info
			extractLinkMeta(layout, spans, contentScaleFactor);
			
			// transfer bitmap data to native layer, and release bitmap when done
			initNativeObject(bitmap);
			bitmap.recycle();
		} else {
			nativeInitBitmapDC(width, height, null);
		}
		
		// release cached images
		for(Bitmap b : imageMap.values()) {
			if(!b.isRecycled()) {
				b.recycle();
			}
		}
	}
	
	private static void extractLinkMeta(StaticLayout layout, List<Span> spans, float contentScaleFactor) {
		// get line count
		int lineCount = layout.getLineCount();
		
		// total height
		float totalHeight = layout.getHeight();
		
		// get line origin
		PointF[] origin = new PointF[lineCount];
		for(int i = 0; i < lineCount; i++) {
			origin[i] = new PointF();
			origin[i].x = layout.getLineLeft(i) / contentScaleFactor;
			origin[i].y = (totalHeight - layout.getLineBaseline(i)) / contentScaleFactor;
		}
		
		// get line range
		Point[] range = new Point[lineCount];
		for(int i = 0; i < lineCount; i++) {
			range[i] = new Point();
			range[i].x = layout.getLineStart(i);
			range[i].y = layout.getLineEnd(i);
		}
		
		// get rect area for every link, or link part
		LinkMeta meta = new LinkMeta();
		int tag = 0;
	    int linkStart = 0, linkEnd = 0;
	    int startLine = 0, endLine = 0;
	    for(Span span : spans) {
	    	if(span.type == SpanType.LINK) {
	    		if(!span.close) {
	                // get start pos
	                linkStart = span.pos;
	                
	                // save color info
	                meta.normalBgColor = span.normalBgColor;
	                meta.selectedBgColor = span.selectedBgColor;
	    		} else {
	                // remember end pos
	                linkEnd = span.pos;
	                
	                // find out the line for start and end pos
	                for (int i = 0; i < lineCount; i++) {
	                    if(linkStart >= range[i].x &&
	                       linkStart < range[i].y) {
	                        startLine = i;
	                    }
	                    
	                    if(linkEnd >= range[i].x &&
	                       linkEnd < range[i].y) {
	                        endLine = i;
	                        break;
	                    }
	                }
	                
	                // get rect area
	                int firstLine = startLine;
					while(startLine <= endLine) {
						float ascent = -layout.getLineAscent(startLine) / contentScaleFactor;
	                    float descent = layout.getLineDescent(startLine) / contentScaleFactor;
						int charEnd = linkEnd;
						if(startLine < endLine)
							charEnd = range[startLine].y;
	                    float startOffsetX = startLine > firstLine 
	                    		? (layout.getLineLeft(startLine) / contentScaleFactor) 
	                    		: (layout.getPrimaryHorizontal(linkStart) / contentScaleFactor);
	                    float endOffsetX = startLine < endLine 
	                    		? (layout.getLineRight(startLine) / contentScaleFactor) 
	                    		: (layout.getPrimaryHorizontal(charEnd) / contentScaleFactor);
	                    meta.x = startOffsetX;
	                    meta.y = origin[startLine].y - descent;
	                    meta.width = endOffsetX - meta.x;
	                    meta.height = descent + ascent;
						meta.tag = tag;
						
						// push meta
						nativeSaveLinkMeta(meta.normalBgColor, 
								meta.selectedBgColor,
								meta.x,
								meta.y,
								meta.width,
								meta.height,
								meta.tag);
						
						// move line
						startLine++;
						
						// move start
						linkStart = charEnd;
					}
					
					// increase tag
					tag++;
	    		}
	    	}
	    }
	}
	
	// if parse failed, endTagPos will be len, otherwise it is end tag position
	private static TagParseResult checkTag(String p, int start) {
		TagParseResult r = new TagParseResult();
		r.type = SpanType.UNKNOWN;
	    TagParseState state = TagParseState.READY;
	    int tagNameStart = 0, tagNameEnd = 0;
	    r.close = false;
	    int len = p.length();
	    r.endTagPos = len;
	    r.dataStart = -1;
	    int i = start;
	    while(i < len) {
	        switch (state) {
	            case READY:
	                if(p.charAt(i) == TAG_START) {
	                    state = TagParseState.START_TAG;
	                    tagNameStart = ++i;
	                } else {
	                    state = TagParseState.FAIL;
	                }
	                break;
	            case START_TAG:
	                if((i == start + 1) && p.charAt(i) == '/') {
	                    state = TagParseState.CLOSE_TAG;
	                    r.close = true;
	                    tagNameStart = ++i;
	                } else if(p.charAt(i) == '=') {
	                    state = TagParseState.EQUAL;
	                    tagNameEnd = i++;
	                    r.type = checkTagName(p, tagNameStart, tagNameEnd);
	                    r.dataStart = i;
	                } else if(p.charAt(i) == TAG_END) {
	                    state = TagParseState.SUCCESS;
	                    r.endTagPos = i;
	                    r.dataEnd = i;
	                    tagNameEnd = i;
	                    if(r.type == SpanType.UNKNOWN) {
	                        r.type = checkTagName(p, tagNameStart, tagNameEnd);
	                    }
	                } else if(p.charAt(i) == ' ') {
	                    state = TagParseState.EQUAL;
	                    tagNameEnd = i++;
	                    r.type = checkTagName(p, tagNameStart, tagNameEnd);
	                    r.dataStart = i;
	                } else {
	                    i++;
	                }
	                break;
	            case CLOSE_TAG:
	                if(p.charAt(i) == TAG_END) {
	                    state = TagParseState.SUCCESS;
	                    r.endTagPos = i;
	                    tagNameEnd = i;
	                    r.type = checkTagName(p, tagNameStart, tagNameEnd);
	                } else {
	                    i++;
	                }
	                break;
	            case EQUAL:
	                if(p.charAt(i) == TAG_END) {
	                    state = TagParseState.SUCCESS;
	                    r.endTagPos = i;
	                    r.dataEnd = i;
	                } else {
	                    i++;
	                }
	                break;
	            default:
	                break;
	        }
	        
	        if(state == TagParseState.FAIL || state == TagParseState.SUCCESS)
	            break;
	    }
	    
	    if(state != TagParseState.SUCCESS)
	        r.type = SpanType.UNKNOWN;
	    
	    return r;
	}
	
	private static SpanType checkTagName(String p, int start, int end) {
	    int len = end - start;
	    switch(len) {
	        case 1:
	            if(p.charAt(start) == 'b') {
	                return SpanType.BOLD;
	            } else if(p.charAt(start) == 'i') {
	                return SpanType.ITALIC;
	            } else if(p.charAt(start) == 'u') {
	                return SpanType.UNDERLINE;
	            }
	            break;
	        case 4:
	            if(p.charAt(start) == 'f' &&
	               p.charAt(start + 1) == 'o' &&
	               p.charAt(start + 2) == 'n' &&
	               p.charAt(start + 3) == 't') {
	                return SpanType.FONT;
	            } else if(p.charAt(start) == 's' &&
	                      p.charAt(start + 1) == 'i' &&
	                      p.charAt(start + 2) == 'z' &&
	                      p.charAt(start + 3) == 'e') {
	                return SpanType.SIZE;
	            } else if(p.charAt(start) == 'l' &&
	                      p.charAt(start + 1) == 'i' &&
	                      p.charAt(start + 2) == 'n' &&
	                      p.charAt(start + 3) == 'k') {
	                return SpanType.LINK;
	            }
	        case 5:
	            if(p.charAt(start) == 'c' &&
	               p.charAt(start + 1) == 'o' &&
	               p.charAt(start + 2) == 'l' &&
	               p.charAt(start + 3) == 'o' &&
	               p.charAt(start + 4) == 'r') {
	                return SpanType.COLOR;
	            } else if(p.charAt(start) == 'i' &&
	            		p.charAt(start + 1) == 'm' &&
	            		p.charAt(start + 2) == 'a' &&
	            		p.charAt(start + 3) == 'g' &&
	            		p.charAt(start + 4) == 'e') {
	            	return SpanType.IMAGE;
	            } 
	            break;
	            
	    }
	    
	    return SpanType.UNKNOWN;
    }

	private static String buildSpan(String text, List<Span> spans) {
		boolean inImageTag = false;
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
				case TAG_START:
				{
	                // parse the tag
	                Span span = new Span();
	                TagParseResult r = checkTag(text, i);
	                
	                // fill span info
	                do {
	                    // if type is unknown, discard
	                    if(r.type == SpanType.UNKNOWN)
	                    	break;
	                    
	                    // you can't embed other tag in image tag, discard
	                    if(inImageTag && (!r.close || r.type != SpanType.IMAGE))
	                    	break;
	                    
	                    // parse span data
	                    span.type = r.type;
	                    span.close = r.close;
	                    span.pos = plain.length();
	                    if(r.close) {
	                    	if(inImageTag) {
	                    		plain.append('\ufffc');
	                    		span.pos++;
	                    		inImageTag = false;
	                    	}
	                    } else {
	                        switch(span.type) {
	                            case COLOR:
	                                span.color = parseColor(text, r.dataStart, r.dataEnd - r.dataStart);
	                                break;
	                            case FONT:
	                            	span.fontName = text.substring(r.dataStart, r.dataEnd);
	                                break;
	                            case SIZE:
	                            	try {
	                                    span.fontSize = Integer.parseInt(text.substring(r.dataStart, r.dataEnd));
                                    } catch (NumberFormatException e) {
                                    	span.fontSize = 16;
                                    }
	                                break;
	                            case IMAGE:
	                            {
	                            	String name = text.substring(r.dataStart, r.dataEnd);
	                            	String[] parts = name.split(" ");
	                            	span.imageName = parts[0];
	                            	span.scaleX = span.scaleY = 1;
	                            	span.width = span.height = 0;
	                            	span.offsetY = 0;
	                            	
	                            	// if parts more than one, parse attributes
	                            	if(parts.length > 1) {
	                            		for(int j = 1; j < parts.length; j++) {
	                            			String[] pair = parts[j].split("=");
	                            			if(pair.length == 2) {
	                            				try {
	                            					if("scale".equals(pair[0])) {
	                                                    span.scaleX = span.scaleY = Float.parseFloat(pair[1]);
	                            					} else if("scalex".equals(pair[0])) {
	                            						span.scaleX = Float.parseFloat(pair[1]);
	                            					} else if("scaley".equals(pair[0])) {
	                            						span.scaleY = Float.parseFloat(pair[1]);
	                            					} else if("w".equals(pair[0])) {
	                            						span.width = Float.parseFloat(pair[1]);
	                            					} else if("h".equals(pair[0])) {
	                            						span.height = Float.parseFloat(pair[1]);
	                            					} else if("offsety".equals(pair[0])) {
	                            						span.offsetY = Float.parseFloat(pair[1]);
	                            					}
	                            				} catch (NumberFormatException e) {
	                            				}
	                            			}
	                            		}
	                            	}
	                            	
	                            	// flag
	                            	inImageTag = true;
	                            	
	                            	break;
	                            }
	                            case LINK:
	                            {
	                            	String v = text.substring(r.dataStart, r.dataEnd);
	                            	String[] parts = v.split(" ");
	                            	for(String part : parts) {
	                            		String[] pair = part.split("=");
	                            		if(pair.length == 2) {
	                            			if("bg".equals(pair[0])) {
	                            				span.normalBgColor = parseColor(pair[1], 0, pair[1].length());
	                            			} else if("bg_click".equals(pair[0])) {
	                            				span.selectedBgColor = parseColor(pair[1], 0, pair[1].length());
	                            			}
	                            		}
	                            	}
	                            	break;
	                            }
	                            default:
	                                break;
	                        }
	                    }
	                    
	                    // add span
	                    spans.add(span);
	                    
	                    // set i pos
	                    i = r.endTagPos;
	                } while(false);
					
					break;
				}
				case TAG_END:
					// just discard it
					break;
				default:
					// ignore text between image tag
					if(!inImageTag) {
						plain.append(c);
					}
					break;
			}
		}
		
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
	private native static void nativeSaveLinkMeta(int normalBgColor, int selectedBgColor, 
			float x, float y, float width, float height, int tag);
	private native static void nativeSaveShadowStrokePadding(float x, float y);
	private native static void nativeResetBitmapDC();
	private native static String nativeFullPathForFilename(String filename);
}
