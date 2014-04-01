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
#include "CCRichLabelTTF.h"
#include "CCDirector.h"
#include "shaders/CCGLProgram.h"
#include "shaders/CCShaderCache.h"
#include "CCTexture2D_richlabel.h"
#include "CCImage_richlabel.h"
#include "CCMenuItemColor.h"
#include "CCRichLabelTTFLinkStateSynchronizer.h"
#include "CCTextureCache_richlabel.h"

NS_CC_BEGIN

#if CC_USE_LA88_LABELS
#define SHADER_PROGRAM kCCShader_PositionTextureColor
#else
#define SHADER_PROGRAM kCCShader_PositionTextureA8Color
#endif

// the start tag of menu item, for link tag
#define START_TAG_LINK_ITEM 0x80000
#define TAG_MENU 0x70000

//
//CCRichLabelTTF
//
CCRichLabelTTF::CCRichLabelTTF() :
m_hAlignment(kCCTextAlignmentCenter),
m_vAlignment(kCCVerticalTextAlignmentTop),
m_pFontName(NULL),
m_fFontSize(0.0),
m_string(""),
m_shadowEnabled(false),
m_shadowColor(0xff333333),
m_strokeEnabled(false),
m_textFillColor(ccWHITE),
m_stateListener(NULL),
m_decryptFunc(NULL),
m_textChanging(true) {
	m_stateListener = new CCRichLabelTTFLinkStateSynchronizer(this);
}

CCRichLabelTTF::~CCRichLabelTTF()
{
    CC_SAFE_DELETE(m_pFontName);
	
	// release callfunc
	CCMenu* menu = (CCMenu*)getChildByTag(TAG_MENU);
	if(menu) {
		CCObject* obj;
		CCARRAY_FOREACH(menu->getChildren(), obj) {
			CCNode* item = (CCNode*)obj;
			CCObject* data = (CCObject*)item->getUserData();
			CC_SAFE_RELEASE(data);
		}
	}
	
	// release other
	m_stateListener->release();
}

void CCRichLabelTTF::reloadAll() {
    CCTextureCache_richlabel::reloadAllTextures();
}

CCRichLabelTTF * CCRichLabelTTF::create(CC_DECRYPT_FUNC decryptFunc)
{
    CCRichLabelTTF * pRet = new CCRichLabelTTF();
    if (pRet && pRet->init(decryptFunc))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

CCRichLabelTTF * CCRichLabelTTF::create(const char *string, const char *fontName, float fontSize, CC_DECRYPT_FUNC decryptFunc)
{
    return CCRichLabelTTF::create(string, fontName, fontSize,
                              CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop, decryptFunc);
}

CCRichLabelTTF * CCRichLabelTTF::create(const char *string, const char *fontName, float fontSize,
                                const CCSize& dimensions, CCTextAlignment hAlignment, CC_DECRYPT_FUNC decryptFunc)
{
    return CCRichLabelTTF::create(string, fontName, fontSize, dimensions, hAlignment, kCCVerticalTextAlignmentTop, decryptFunc);
}

CCRichLabelTTF* CCRichLabelTTF::create(const char *string, const char *fontName, float fontSize,
                               const CCSize &dimensions, CCTextAlignment hAlignment,
                               CCVerticalTextAlignment vAlignment, CC_DECRYPT_FUNC decryptFunc)
{
    CCRichLabelTTF *pRet = new CCRichLabelTTF();
    if(pRet && pRet->initWithString(string, strcmp(fontName, "") == 0 ? "Helvetica" : fontName , fontSize, dimensions, hAlignment, vAlignment, decryptFunc))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CCRichLabelTTF::init(CC_DECRYPT_FUNC decryptFunc)
{
    return this->initWithString("", "Helvetica", 12, decryptFunc);
}

bool CCRichLabelTTF::initWithString(const char *label, const char *fontName, float fontSize,
                                const CCSize& dimensions, CCTextAlignment alignment, CC_DECRYPT_FUNC decryptFunc)
{
    return this->initWithString(label, fontName, fontSize, dimensions, alignment, kCCVerticalTextAlignmentTop, decryptFunc);
}

bool CCRichLabelTTF::initWithString(const char *label, const char *fontName, float fontSize, CC_DECRYPT_FUNC decryptFunc)
{
    return this->initWithString(label, fontName, fontSize,
                                CCSizeZero, kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop, decryptFunc);
}

bool CCRichLabelTTF::initWithString(const char *string, const char *fontName, float fontSize,
                                const CCSize &dimensions, CCTextAlignment hAlignment,
                                CCVerticalTextAlignment vAlignment, CC_DECRYPT_FUNC decryptFunc)
{
    if (CCGradientSprite::init())
    {
        // shader program
        this->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(SHADER_PROGRAM));
        
        m_tDimensions = CCSizeMake(dimensions.width, dimensions.height);
        m_hAlignment  = hAlignment;
        m_vAlignment  = vAlignment;
        m_pFontName   = new std::string(fontName);
        m_fFontSize   = fontSize;
		m_decryptFunc = decryptFunc;
        
        this->setString(string);
        
        return true;
    }
    
    return false;
}

bool CCRichLabelTTF::initWithStringAndTextDefinition(const char *string, ccRichFontDefinition &textDefinition)
{
    if (CCGradientSprite::init())
    {
        // shader program
        this->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(SHADER_PROGRAM));
        
        // prepare everythin needed to render the label
        _updateWithTextDefinition(textDefinition, false);
        
        // set the string
        this->setString(string);
        
        //
        return true;
    }
    else
    {
        return false;
    }
}


void CCRichLabelTTF::setString(const char *string)
{
    CCAssert(string != NULL, "Invalid string");
    
    if (m_string.compare(string))
    {
        m_string = string;
        m_textChanging = true;
        this->updateTexture();
    }
}

const char* CCRichLabelTTF::getString(void)
{
    return m_string.c_str();
}

const char* CCRichLabelTTF::description()
{
    return CCString::createWithFormat("<CCRichLabelTTF | FontName = %s, FontSize = %.1f>", m_pFontName->c_str(), m_fFontSize)->getCString();
}

CCTextAlignment CCRichLabelTTF::getHorizontalAlignment()
{
    return m_hAlignment;
}

void CCRichLabelTTF::setHorizontalAlignment(CCTextAlignment alignment)
{
    if (alignment != m_hAlignment)
    {
        m_hAlignment = alignment;
        
        // Force update
        if (m_string.size() > 0)
        {
            this->updateTexture();
        }
    }
}

CCVerticalTextAlignment CCRichLabelTTF::getVerticalAlignment()
{
    return m_vAlignment;
}

void CCRichLabelTTF::setVerticalAlignment(CCVerticalTextAlignment verticalAlignment)
{
    if (verticalAlignment != m_vAlignment)
    {
        m_vAlignment = verticalAlignment;
        
        // Force update
        if (m_string.size() > 0)
        {
            this->updateTexture();
        }
    }
}

CCSize CCRichLabelTTF::getDimensions()
{
    return m_tDimensions;
}

void CCRichLabelTTF::setDimensions(const CCSize &dim)
{
    if (dim.width != m_tDimensions.width || dim.height != m_tDimensions.height)
    {
        m_tDimensions = dim;
        
        // Force update
        if (m_string.size() > 0)
        {
            this->updateTexture();
        }
    }
}

float CCRichLabelTTF::getFontSize()
{
    return m_fFontSize;
}

void CCRichLabelTTF::setFontSize(float fontSize)
{
    if (m_fFontSize != fontSize)
    {
        m_fFontSize = fontSize;
        
        // Force update
        if (m_string.size() > 0)
        {
            this->updateTexture();
        }
    }
}

const char* CCRichLabelTTF::getFontName()
{
    return m_pFontName->c_str();
}

void CCRichLabelTTF::setFontName(const char *fontName)
{
    if (m_pFontName->compare(fontName))
    {
        delete m_pFontName;
        m_pFontName = new std::string(fontName);
        
        // Force update
        if (m_string.size() > 0)
        {
            this->updateTexture();
        }
    }
}

void CCRichLabelTTF::setColor(const ccColor4B& start, const ccColor4B& end, const CCPoint& v) {
    CCGradientSprite::setColor(start, end, v);
}

// Helper
bool CCRichLabelTTF::updateTexture()
{
    CCTexture2D_richlabel *tex;
    tex = new CCTexture2D_richlabel();
    
    if (!tex)
        return false;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
	ccRichFontDefinition texDef = _prepareTextDefinition(true);
	tex->initWithRichString( m_string.c_str(), &texDef );
    
#else
    
	tex->initWithRichString( m_string.c_str(),
						m_pFontName->c_str(),
						m_fFontSize * CC_CONTENT_SCALE_FACTOR(),
						CC_SIZE_POINTS_TO_PIXELS(m_tDimensions),
						m_hAlignment,
						m_vAlignment);
    
#endif
    
    // set the texture
    this->setTexture(tex);
    
    // release it
    tex->release();
    
    // set the size in the sprite
    CCRect rect =CCRectZero;
    rect.size   = m_pobTexture->getContentSize();
    this->setTextureRect(rect);
	
	// save image rects
	const vector<CCRect>& imageRects = tex->getImageRects();
	m_imageRects.clear();
	m_imageRects.insert(m_imageRects.begin(), imageRects.begin(), imageRects.end());
	for(vector<CCRect>::iterator iter = m_imageRects.begin(); iter != m_imageRects.end(); iter++) {
		CCRect& r = *iter;
		r.origin.x /= CC_CONTENT_SCALE_FACTOR();
		r.origin.y /= CC_CONTENT_SCALE_FACTOR();
		r.size.width /= CC_CONTENT_SCALE_FACTOR();
		r.size.height /= CC_CONTENT_SCALE_FACTOR();
	}
	
	// create link menu
	CCMenu* menu = (CCMenu*)getChildByTag(TAG_MENU);
	const LinkMetaList& linkMetas = tex->getLinkMetas();
	if(!linkMetas.empty() && m_textChanging) {
		// delete old menu if has
		if(menu) {
			menu->removeFromParent();
			menu = NULL;
		}
		
		// now create items for every link
		CCArray* items = CCArray::create();
		for(LinkMetaList::const_iterator iter = linkMetas.begin(); iter != linkMetas.end(); iter++) {
			const LinkMeta& meta = *iter;
			CCMenuItemColor* item = CCMenuItemColor::create(ccc4FromInt(meta.normalBgColor),
															ccc4FromInt(meta.selectedBgColor),
															this,
															menu_selector(CCRichLabelTTF::onLinkMenuItemClicked));
			item->setTag(START_TAG_LINK_ITEM + meta.tag);
			item->setPosition(ccp(meta.x + meta.width / 2,
								  meta.y + meta.height / 2));
			item->setContentSize(CCSizeMake(meta.width, meta.height));
			item->setStateListener(m_stateListener);
			items->addObject(item);
		}
		
		// add menu in -1 z order so that it won't override label UI
		CCMenu* newMenu = CCMenu::createWithArray(items);
		newMenu->setPosition(CCPointZero);
		addChild(newMenu, -1, TAG_MENU);
		
		// clear flag
		m_textChanging = false;
	}
	
	// sync shadow stroke padding to link menu
	// the padding must divide scale factor. Because at this time, texture already scaled for label
	menu = (CCMenu*)getChildByTag(TAG_MENU);
	if(menu) {
		const CCPoint& p = tex->getShadowStrokePadding();
		menu->setPosition(ccp(p.x / CC_CONTENT_SCALE_FACTOR(),
							  p.y / CC_CONTENT_SCALE_FACTOR()));
	}
    
    //ok
    return true;
}

CCRect CCRichLabelTTF::getImageBound(int index) {
	if(index < 0 || index >= m_imageRects.size())
		return CCRectZero;
	
	return m_imageRects.at(index);
}

CCRect CCRichLabelTTF::getImageBoundInParentSpace(int index) {
	CCRect r = getImageBound(index);
	CCAffineTransform t = nodeToParentTransform();
	r = CCRectApplyAffineTransform(r, t);
	return r;
}

CCRect CCRichLabelTTF::getImageBoundInWorldSpace(int index) {
	CCRect r = getImageBound(index);
	CCAffineTransform t = nodeToWorldTransform();
	r = CCRectApplyAffineTransform(r, t);
	return r;
}

void CCRichLabelTTF::enableShadow(const CCSize &shadowOffset, int shadowColor, float shadowBlur, bool updateTexture)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
	bool valueChanged = false;
	
	if (false == m_shadowEnabled)
	{
		m_shadowEnabled = true;
		valueChanged    = true;
	}
	
	if ( (m_shadowOffset.width != shadowOffset.width) || (m_shadowOffset.height!=shadowOffset.height) )
	{
		m_shadowOffset.width  = shadowOffset.width;
		m_shadowOffset.height = shadowOffset.height;
		
		valueChanged = true;
	}
    
    if(m_shadowColor != shadowColor) {
        m_shadowColor = shadowColor;
        valueChanged = true;
    }
	
	if (m_shadowBlur    != shadowBlur)
	{
		m_shadowBlur = shadowBlur;
		valueChanged = true;
	}
	
	
	if ( valueChanged && updateTexture )
	{
		this->updateTexture();
	}
    
#else
	CCAssert(false, "Currently only supported on iOS and Android!");
#endif
    
}

void CCRichLabelTTF::disableShadow(bool updateTexture)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
	if (m_shadowEnabled)
	{
		m_shadowEnabled = false;
		
		if (updateTexture)
			this->updateTexture();
		
	}
    
#else
	CCAssert(false, "Currently only supported on iOS and Android!");
#endif
}

void CCRichLabelTTF::enableStroke(const ccColor3B &strokeColor, float strokeSize, bool updateTexture)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
	bool valueChanged = false;
	
	if(m_strokeEnabled == false)
	{
		m_strokeEnabled = true;
		valueChanged = true;
	}
	
	if ( (m_strokeColor.r != strokeColor.r) || (m_strokeColor.g != strokeColor.g) || (m_strokeColor.b != strokeColor.b) )
	{
		m_strokeColor = strokeColor;
		valueChanged = true;
	}
	
	if (m_strokeSize!=strokeSize)
	{
		m_strokeSize = strokeSize;
		valueChanged = true;
	}
	
	if ( valueChanged && updateTexture )
	{
		this->updateTexture();
	}
    
#else
	CCAssert(false, "Currently only supported on iOS and Android!");
#endif
    
}

void CCRichLabelTTF::disableStroke(bool updateTexture)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
	if (m_strokeEnabled)
	{
		m_strokeEnabled = false;
		
		if (updateTexture)
			this->updateTexture();
	}
    
#else
	CCAssert(false, "Currently only supported on iOS and Android!");
#endif
    
}

void CCRichLabelTTF::setFontFillColor(const ccColor3B &tintColor, bool updateTexture)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if (m_textFillColor.r != tintColor.r || m_textFillColor.g != tintColor.g || m_textFillColor.b != tintColor.b)
	{
		m_textFillColor = tintColor;
		
		if (updateTexture)
			this->updateTexture();
	}
#else
	CCAssert(false, "Currently only supported on iOS and Android!");
#endif
}

void CCRichLabelTTF::setTextDefinition(ccRichFontDefinition *theDefinition)
{
    if (theDefinition)
    {
        _updateWithTextDefinition(*theDefinition, true);
    }
}

ccRichFontDefinition *CCRichLabelTTF::getTextDefinition()
{
    ccRichFontDefinition *tempDefinition = new ccRichFontDefinition;
    *tempDefinition = _prepareTextDefinition(false);
    return tempDefinition;
}

void CCRichLabelTTF::_updateWithTextDefinition(ccRichFontDefinition & textDefinition, bool mustUpdateTexture)
{
    m_tDimensions = CCSizeMake(textDefinition.m_dimensions.width, textDefinition.m_dimensions.height);
    m_hAlignment  = textDefinition.m_alignment;
    m_vAlignment  = textDefinition.m_vertAlignment;
    
    m_pFontName   = new std::string(textDefinition.m_fontName);
    m_fFontSize   = textDefinition.m_fontSize;
    
    
    // shadow
    if ( textDefinition.m_shadow.m_shadowEnabled )
    {
        enableShadow(textDefinition.m_shadow.m_shadowOffset, textDefinition.m_shadowColor, false);
    }
    
    // stroke
    if ( textDefinition.m_stroke.m_strokeEnabled )
    {
        enableStroke(textDefinition.m_stroke.m_strokeColor, textDefinition.m_stroke.m_strokeSize, false);
    }
    
    // fill color
    setFontFillColor(textDefinition.m_fontFillColor, false);
    
    if (mustUpdateTexture)
        updateTexture();
}

ccRichFontDefinition CCRichLabelTTF::_prepareTextDefinition(bool adjustForResolution)
{
    ccRichFontDefinition texDef;
	
	texDef.decryptFunc = m_decryptFunc;
    
    if (adjustForResolution)
        texDef.m_fontSize       =  m_fFontSize * CC_CONTENT_SCALE_FACTOR();
    else
        texDef.m_fontSize       =  m_fFontSize;
    
    texDef.m_fontName       = *m_pFontName;
    texDef.m_alignment      =  m_hAlignment;
    texDef.m_vertAlignment  =  m_vAlignment;
    
    
    if (adjustForResolution)
        texDef.m_dimensions     =  CC_SIZE_POINTS_TO_PIXELS(m_tDimensions);
    else
        texDef.m_dimensions     =  m_tDimensions;
    
    
    // stroke
    if ( m_strokeEnabled )
    {
        texDef.m_stroke.m_strokeEnabled = true;
        texDef.m_stroke.m_strokeColor   = m_strokeColor;
        
        if (adjustForResolution)
            texDef.m_stroke.m_strokeSize = m_strokeSize * CC_CONTENT_SCALE_FACTOR();
        else
            texDef.m_stroke.m_strokeSize = m_strokeSize;
        
        
    }
    else
    {
        texDef.m_stroke.m_strokeEnabled = false;
    }
    
    
    // shadow
    if ( m_shadowEnabled )
    {
        texDef.m_shadow.m_shadowEnabled         = true;
        texDef.m_shadow.m_shadowBlur            = m_shadowBlur;
        texDef.m_shadow.m_shadowOpacity         = m_shadowOpacity;
        texDef.m_shadowColor           = m_shadowColor;
        
        if (adjustForResolution)
            texDef.m_shadow.m_shadowOffset = CC_SIZE_POINTS_TO_PIXELS(m_shadowOffset);
        else
            texDef.m_shadow.m_shadowOffset = m_shadowOffset;
    }
    else
    {
        texDef.m_shadow.m_shadowEnabled = false;
    }
    
    // text tint
    texDef.m_fontFillColor = m_textFillColor;
    
    return texDef;
}

void CCRichLabelTTF::setLinkTarget(int index, CCCallFunc* func) {
    // if not found, do nothing
    CCMenu* menu = (CCMenu*)getChildByTag(TAG_MENU);
    if(!menu)
        return;
	
	CCObject* obj;
	CCARRAY_FOREACH(menu->getChildren(), obj) {
		CCMenuItemColor* item = (CCMenuItemColor*)obj;
		if(item->getTag() == START_TAG_LINK_ITEM + index) {
			obj = (CCObject*)item->getUserData();
			if(obj) {
				CC_SAFE_RELEASE(obj);
			}
			item->setUserData(func);
			CC_SAFE_RETAIN(func);
		}
	}
}

void CCRichLabelTTF::setLinkTargetForAll(CCCallFunc* func) {
	// if not found, do nothing
    CCMenu* menu = (CCMenu*)getChildByTag(TAG_MENU);
    if(!menu)
        return;
	
	CCObject* obj;
	CCARRAY_FOREACH(menu->getChildren(), obj) {
		CCMenuItemColor* item = (CCMenuItemColor*)obj;
		obj = (CCObject*)item->getUserData();
		if(obj) {
			CC_SAFE_RELEASE(obj);
		}
		item->setUserData(func);
		CC_SAFE_RETAIN(func);
	}
}

void CCRichLabelTTF::onLinkMenuItemClicked(CCObject* sender) {
	CCMenuItemColor* item = (CCMenuItemColor*)sender;
	CCCallFunc* func = (CCCallFunc*)item->getUserData();
	if(func)
		func->execute();
}

void CCRichLabelTTF::setLinkPriority(int p) {
	// if not found, do nothing
    CCMenu* menu = (CCMenu*)getChildByTag(TAG_MENU);
    if(!menu)
        return;
	
	menu->setTouchPriority(p);
}

NS_CC_END
