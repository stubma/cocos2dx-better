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
#ifndef __CCRichLabelTTF__
#define __CCRichLabelTTF__

#include "cocos2d.h"
#include "CCGradientSprite.h"
#include "ccMoreTypes.h"

using namespace std;

NS_CC_BEGIN

class CCTexture2D_richlabel;
class CCRichLabelTTFLinkStateSynchronizer;

/**
 * TTF label which support different style in a string. By using a tag description such as:
 * 'Hell[color=ffff0000]o[/color]", the 'o' character will be in red color. If you want 
 * a '[' character, use '\' to escape.
 *
 * \par
 * CCRichLabelTFF won't perform any format validation, so you must ensure the text is
 * correct. If text is wrong, no warning will be shown and final result will be unpredictable.
 *
 * \par
 * supported tags:
 * [color=aarrggbb][/color]: set text color
 * [font=font name or path][/font]: set font
 * [size=integer][/size]: set font size
 * [b][/b]: bold font
 * [i][/i]: italic font
 * [u][/u]: underline
 * [image=image path][/image]: embed a image, The text between image tag will be ignored, no matter how long it is.
 *      Other tags should not be embedded inside an image tag, crash may happen if you do that.
 *      If image path starts with a '/', which means an absolute path, CCRichLabelTFF treats it like an external image.
 *      Optional attributes: scale, scalex, scaley, w, h, offsety.
 *      In iOS, the absolute path will be redirected to ~/Documents, so "/sdcard/a.png" will be "~/Document/sdcard/a.png"
 *			1. w/h: If w is specified, x scale property will be ignored. If h is specified, y scale property will be ignored.
 *			2. scale/scalex/scaley: image scale, if scale x and y is same, you can just set scale
 *			3. offsety: image vertical offset, following opengl rule. So positive value will move image up.
 *			4. plist/atlas: when those two attributes are specified, the image path will be treated as a frame name in atlas plist
 *		The image name can be empty or anything not existent if you just want to add a placeholder
 * [link bg=aarrggbb bg_click=aarrggbb][/link]: set one segment can be clicked as a hyperlink. bg and bg_click are
 *		optional and they will be transparent color if not set. Link tag doesn't add any decoration, you must use other
 *		tag to add visual style for clickable area.
 *
 * \par
 * CCRichLabelTFF is a subclass of CCGradientSprite, so it also can set a gradient
 * effect.
 *
 * \note
 * Currently it only supports iOS and Android, please do it yourself if you want other platform.
 */
class CC_DLL CCRichLabelTTF : public CCGradientSprite, public CCLabelProtocol {
private:
	/// menu item state listener
	CCRichLabelTTFLinkStateSynchronizer* m_stateListener;
	
	/// rectangle of every embeded image
	vector<CCRect> m_imageRects;
	
protected:
    CCRichLabelTTF();
	
	// when link item is clicked
	void onLinkMenuItemClicked(CCObject* sender);
    
public:
    virtual ~CCRichLabelTTF();
	
	/** 
	 * Creates an label.
     */
    static CCRichLabelTTF* create();
	
	/**
	 * creates a CCRichLabelTTF with a font name and font size in points
     */
    static CCRichLabelTTF* create(const char *string, const char *fontName, float fontSize);
    
    /** 
	 * creates a CCRichLabelTTF from a fontname, horizontal alignment, dimension in points,  and font size in points.
     */
    static CCRichLabelTTF* create(const char *string, const char *fontName, float fontSize,
                               const CCSize& dimensions, CCTextAlignment hAlignment);
	
    /** 
	 * creates a CCRichLabelTTF from a fontname, alignment, dimension in points and font size in points
     */
    static CCRichLabelTTF* create(const char *string, const char *fontName, float fontSize,
                               const CCSize& dimensions, CCTextAlignment hAlignment,
                               CCVerticalTextAlignment vAlignment);
    
    /**
     * To recreate rich labels, because CCRichLabelTTF is a custom visual node, the 
     * restoration must be done separately. Generally, you can call this method in
     * JNI Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInit. Check the main.cpp
     * of test project
     */
    static void reloadAll();
	
	const char* description();
	
    /** initializes the CCRichLabelTTF with a font name and font size */
    bool initWithString(const char *string, const char *fontName, float fontSize);
    
    /** initializes the CCRichLabelTTF with a font name, alignment, dimension and font size */
    bool initWithString(const char *string, const char *fontName, float fontSize,
                        const CCSize& dimensions, CCTextAlignment hAlignment);
	
    /** initializes the CCRichLabelTTF with a font name, alignment, dimension and font size */
    bool initWithString(const char *string, const char *fontName, float fontSize,
                        const CCSize& dimensions, CCTextAlignment hAlignment,
                        CCVerticalTextAlignment vAlignment);
    
    /** initializes the CCRichLabelTTF with a font name, alignment, dimension and font size */
    bool initWithStringAndTextDefinition(const char *string, ccRichFontDefinition &textDefinition);
    
    /** set the text definition used by this label */
    void setTextDefinition(ccRichFontDefinition *theDefinition);
    
    /** get the text definition used by this label */
    ccRichFontDefinition * getTextDefinition();
    
    
    
    /** enable or disable shadow for the label */
    void enableShadow(const CCSize &shadowOffset, int shadowColor, float shadowBlur, bool mustUpdateTexture = true);
    
    /** disable shadow rendering */
    void disableShadow(bool mustUpdateTexture = true);
    
    /** enable or disable stroke */
    void enableStroke(const ccColor3B &strokeColor, float strokeSize, bool mustUpdateTexture = true);
    
    /** disable stroke */
    void disableStroke(bool mustUpdateTexture = true);
    
    /** set text tinting */
    void setFontFillColor(const ccColor3B &tintColor, bool mustUpdateTexture = true);

    /** initializes the CCRichLabelTTF */
    bool init();
	
    /** changes the string to render
	 * @warning Changing the string is as expensive as creating a new CCRichLabelTTF. To obtain better performance use CCLabelAtlas
	 */
    virtual void setString(const char *label);
    virtual const char* getString(void);
    
    CCTextAlignment getHorizontalAlignment();
    void setHorizontalAlignment(CCTextAlignment alignment);
    
    CCVerticalTextAlignment getVerticalAlignment();
    void setVerticalAlignment(CCVerticalTextAlignment verticalAlignment);
    
    CCSize getDimensions();
    void setDimensions(const CCSize &dim);
    
    float getFontSize();
    void setFontSize(float fontSize);
    
    const char* getFontName();
    void setFontName(const char *fontName);
	
	virtual void setColor(const ccColor3B& color3) {}
    
    virtual void setColor(const ccColor4B& start, const ccColor4B& end, const CCPoint& v);
    
    /**
     * If a rich lable contains link tag, a menu will be created and every link tag is 
     * converted to a CCMenuItemColor in menu. Buy by default, the menu item is not connected
     * to action, so you must call this method to assign an action to every link
     *
     * @param index index of link, start from zero, by literal sequence
     * @param func the function will be invoked when a link is clicked
     */
    void setLinkTarget(int index, CCCallFunc* func);
	
	/**
	 * It set all link action to same callfunc object
	 * 
	 * @param func the function will be invoked when any link is clicked
	 */
	void setLinkTargetForAll(CCCallFunc* func);
	
	/// set touch event priority of link menu
	void setLinkPriority(int p);
	
	/// get image bound in self space
	CCRect getImageBound(int index);
	
	/// get image bound in parent space
	CCRect getImageBoundInParentSpace(int index);
	
	/// get image bound in world space
	CCRect getImageBoundInWorldSpace(int index);
    
private:
    bool updateTexture();
	
protected:
    
    /** set the text definition for this label */
    void                _updateWithTextDefinition(ccRichFontDefinition & textDefinition, bool mustUpdateTexture = true);
    ccRichFontDefinition    _prepareTextDefinition(bool adjustForResolution = false);
    
    /** Dimensions of the label in Points */
    CCSize m_tDimensions;
    /** The alignment of the label */
    CCTextAlignment         m_hAlignment;
    /** The vertical alignment of the label */
    CCVerticalTextAlignment m_vAlignment;
    /** Font name used in the label */
    std::string * m_pFontName;
    /** Font size of the label */
    float m_fFontSize;
    /** label's string */
    std::string m_string;
    
    /** font shadow */
    bool    m_shadowEnabled;
    CCSize  m_shadowOffset;
    float   m_shadowOpacity;
    float   m_shadowBlur;
    int     m_shadowColor;
    
    
    /** font stroke */
    bool        m_strokeEnabled;
    ccColor3B   m_strokeColor;
    float       m_strokeSize;
	
    /** font tint */
    ccColor3B   m_textFillColor;
	
	// text is changing
	bool m_textChanging;
};

NS_CC_END

#endif /* defined(__CCRichLabelTTF__) */
