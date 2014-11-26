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
#include "CCProgressHUD.h"
#include "CBDrawingPrimitives.h"

NS_CC_BEGIN

// spinner image
static const unsigned int s_spinner_png[] = {
    0x474e5089,0xa1a0a0d,0xd000000,0x52444849,0x25000000,0x25000000,0x608,0x209ec500,0x3000003,0x414449da,0xed095854,0x55884f97,0xdfc71c55,0x45fd9338,0x143fa492,0x1068c2f5,0x8d407227,0xd258c5a0,0x22ab4dc2,0xced23708,0x4da42d36,0xa17455b8,0x82612ed0,0x184add11,0xb85c9b41,0xc10a9ad1,0x4d0c2345,0xca2a54c3,0xd8d14938,0xbcf9f8d8,0x79c7bfb9,0xf77bee77,0xe30379be,0xe67c2fa2,0xbf3bf39c,0xfbbfbbf3,0xfbdce7bb,0x71716fa6,0x69bf76b1,0xda129560,0x61e79c4c,0x8d7b261a,0xd7aeb9,0xad3afad9,0xb3db6837,0x14f526aa,0x6c0d7a57,0xa271148b,0xad94fbcd,0x176ff16f,0xc189c5a4,0x4300d816,0x237428f0,0x7ae75dfd,0x6aaa94e3,0x24597352,0xa34b07fd,0x1fe83ab6,0xad3889d7,0x3851b91e,0x86cbcce7,0x31c4b057,0xa6fe827d,0x6c51e65,
    0xae57f4fd,0xaaa94e37,0x9117f526,0xde689266,0x2e325f7d,0x569f3aea,0x29c6f5d5,0xca4d555,0x37ff51e4,0xad08f88b,0x7fd080af,0x875a2759,0x5ca7ebce,0x9155f957,0x3253ca4f,0xdf90c302,0x75c0f9d0,0x7453427c,0x5436c016,0x7a6d0e8f,0xbe366143,0x968119d7,0x797f9d53,0x7871d6,0xf2582f62,0xa7e3a034,0x13f871e1,0x2be4fa42,0x6e71bce1,0xc642eb81,0x1cfb65f7,0xd3a816c4,0xf50d06bf,0x66333ee5,0x2b4df1ec,0xb0321518,0x6e40d59b,0xbb859de,0xc4cfd4ed,0xb109b846,0xc84c44ce,0xf23e42c4,0x2936d0cb,0x1bab2d1d,0x368d4e34,0x438f1de5,0x556d429c,0x76c8de5,0x84ca9c2b,0xe5a027,0x2e952be4,0x1726f4b0,0x9fb9d59b,0x2e9d86e0,0x4ff72c65,0x263a6fe8,0x206b1853,0x175a0abc,0xa29e2778,0x7c47b83a,0xeb912b64,0x7d5be78c,0x6da1a60a,0x8a553bc5,
    0x74a36b75,0x64daf43,0xc78d5705,0xfb7331f7,0xcf817d60,0x70a36c1d,0x5bae60ac,0xeaf2793,0x14f0c545,0x4c64c14e,0x945065ee,0xdd86e610,0xfb90d5f9,0x8f99ae33,0xa6f9dda7,0x56fa49c3,0x9dda48a4,0x134d0d5f,0x49148ac4,0xffb5f3bb,0x79517a37,0x9eed6c8a,0x107af86f,0x703387de,0x95861e04,0xc16f38c8,0x3a15b0bb,0xe3d3b75e,0xfbc59d7b,0xe855efc1,0x63173a13,0x2cfb45d0,0xf21526be,0x330e3c45,0x78db4861,0x17b6a425,0x3b047c18,0x3f7d2352,0x6ce43f9a,0xe07f8755,0xcc9e1d78,0x7f66f816,0xef877e13,0xca9a0e61,0x99846a57,0xf64d214f,0xe02fe336,0x4157f04b,0x3f02bc3f,0x4ed03fc3,0xf2ac62be,0xfcb27814,0xabc20f28,0x9c8420f0,0x86bf0f3b,0xb6b8bd21,0x266c200f,0x21461670,0x17b93b7e,0x2fc1820e,0xa76821c1,0x11789877,0xe0d3837e,0xf91bceb7,
    0x65ec4c05,0x410c9e30,0xa90d2933,0x32b0314e,0x7087c1df,0xba21221c,0x84c1b7b5,0x77f1bd54,0x9a1ecfe9,0x8a1b60c3,0x1a7ee547,0x3ced230e,0x302bff18,0x6e83df61,0x8ff2a68d,0xd05139af,0x499b0c79,0xc06f2ada,0x60a3aa4a,0xe01785,0x62309da8,0x3f98f907,0x1dc75acf,0x2754af95,0x8c98f7e7,0xffd2ae84,0x478e713a,0xe353ad1e,0x3cf99b30,0xd646fb44,0x26aa6d2f,0x9ddd86e5,0xefa31b44,0x3724eb9e,0xc389b7f0,0xd8fefa36,0xa4d5552b,0xaf8720fc,0xd2c7fc1b,0x930e4bc8,0xe56c6e34,0x5e8d5dfa,0x50b89fc9,0xa5ed5b89,0x8ceac1cc,0xe6e73995,0x535596ad,0x9d352ccf,0xea4d56ff,0xe3bc6104,0xd313c0cf,0xed49fc8d,0x95bff081,0x4c2caa6f,0xff276637,0x3b4a4c0b,0x658cea79,0x8c09a45b,0x2b554952,0x1cec1655,0xdb60f30e,0x4f79bab3,0xaab17cf5,0x2501ee01,
    0x3f04c73a,0xba9fda,0x49000000,0xae444e45,0x826042,
};

// single instance
static CCProgressHUD* s_hud = NULL;

CCProgressHUD::CCProgressHUD() :
m_cancellable(false),
m_dimColor(cc4TRANSPARENT),
m_panelColor(ccc4(0, 0, 0, 200)),
m_icon(NULL),
m_panelSize(CCSizeZero),
m_msgLabel(NULL),
m_indicator(NULL),
m_time(0),
m_forceSquare(false) {
}

CCProgressHUD::~CCProgressHUD() {
    CC_SAFE_RELEASE(m_icon);
    CC_SAFE_RELEASE(m_indicator);
}

bool CCProgressHUD::initWithMessage(const string& message) {
    if(!CCLayerColor::initWithColor(m_dimColor)) {
        return false;
    }
    
    // set message
    m_message = message;
    
    // create label
    m_msgLabel = CCRichLabelTTF::create(m_message.c_str(),
                                        "Helvetica",
                                        28 / CC_CONTENT_SCALE_FACTOR());
    
    // indicator
    CCImage* image = new CCImage();
    image->initWithImageData((void*)s_spinner_png, 1043);
    CCTexture2D* tex = new CCTexture2D();
    tex->initWithImage(image);
    m_indicator = CCSprite::createWithTexture(tex);
    CC_SAFE_RETAIN(m_indicator);
    image->release();
    tex->release();
    
    // layout
    relayout();
    addChild(m_indicator);
    addChild(m_msgLabel);
    
    // enable event
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(-MAX_INT);
    setKeypadEnabled(true);
    
    // schedule update
    scheduleUpdate();
    
    return true;
}

CCProgressHUD* CCProgressHUD::create(const string& message) {
    CCProgressHUD* hud = new CCProgressHUD();
    hud->initWithMessage(message);
    return (CCProgressHUD*)hud->autorelease();
}

CCProgressHUD* CCProgressHUD::show(const string& message) {
    if(!s_hud) {
        s_hud = CCProgressHUD::create(message);
        CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
        if(scene) {
            scene->addChild(s_hud, MAX_INT);
        }
    }
    
    return s_hud;
}

CCProgressHUD* CCProgressHUD::show(const string& message, CCNode* parent) {
    if(!s_hud) {
        s_hud = CCProgressHUD::create(message);
        parent->addChild(s_hud, MAX_INT);
    }
    
    return s_hud;
}

void CCProgressHUD::hide() {
    if(s_hud) {
        s_hud->removeFromParent();
        s_hud = NULL;
    }
}

CCProgressHUD* CCProgressHUD::current() {
    return s_hud;
}

void CCProgressHUD::relayout() {
    // padding and spacing
    float padding = 20 / CC_CONTENT_SCALE_FACTOR();
    float spacing = 8 / CC_CONTENT_SCALE_FACTOR();
    
    // icon
    CCSprite* icon = m_icon ? m_icon : m_indicator;
    
    // measure size
    float width = MAX(icon->getContentSize().width, m_msgLabel->getContentSize().width);
    float panelWidth = width + padding + padding;
    float height = icon->getContentSize().height + m_msgLabel->getContentSize().height + spacing;
    float panelHeight = height + padding + padding;
    
    // set panel size
    m_panelSize = CCSizeMake(panelWidth, panelHeight);
	
	// center
	const CCSize& contentSize = getContentSize();
    CCPoint center = ccp(contentSize.width / 2, contentSize.height / 2);
	
	// if force square and height will be stretched
	// redefine the spacing and content height
	float maxEdge = MAX(panelWidth, panelHeight);
	if(m_forceSquare && maxEdge > panelHeight) {
		spacing = (maxEdge - icon->getContentSize().height - m_msgLabel->getContentSize().height) / 3;
		height = icon->getContentSize().height + m_msgLabel->getContentSize().height + spacing;
	}
	
	// layout
	m_msgLabel->setPosition(ccp(center.x,
								center.y - height / 2 + m_msgLabel->getContentSize().height / 2));
	icon->setPosition(ccp(center.x,
						  center.y + height / 2 - icon->getContentSize().height / 2));
}

void CCProgressHUD::draw() {
    CCLayerColor::draw();
    
    // draw panel
    const CCSize& contentSize = getContentSize();
	CCPoint size = ccp(m_panelSize.width, m_panelSize.height);
	if(m_forceSquare) {
		size.x = size.y = MAX(size.x, size.y);
	}
    CCPoint origin = ccp(contentSize.width / 2 - size.x / 2,
                         contentSize.height / 2 - size.y / 2);
    CCPoint dest = ccpAdd(origin, size);
    ccDrawSolidRoundRect(origin, dest, 15 / CC_CONTENT_SCALE_FACTOR(), m_panelColor);
}

void CCProgressHUD::update(float delta) {
    m_time += delta;
    while(m_time > 0.08f) {
        m_time -= 0.08f;
        if(!m_icon) {
            float r = m_indicator->getRotation() + 30;
            r = fmodf(r, 360);
            m_indicator->setRotation(r);
        }
    }
}

void CCProgressHUD::setPanelColor(const ccColor4B& c) {
    m_panelColor = c;
}

void CCProgressHUD::setDimColor(const ccColor4B& dimColor) {
    m_dimColor = dimColor;
    setColor(ccc3FromCCC4(m_dimColor));
    setOpacity(m_dimColor.a);
}

void CCProgressHUD::setMessage(const string& msg) {
    m_message = msg;
    m_msgLabel->setString(msg.c_str());
    relayout();
}

void CCProgressHUD::setForceSquare(bool var) {
	m_forceSquare = var;
	relayout();
}

void CCProgressHUD::setIcon(cocos2d::CCSprite* icon) {
	if(m_icon) {
		m_icon->removeFromParent();
		CC_SAFE_RELEASE(m_icon);
		m_icon = NULL;
	}
    m_icon = icon;
	CC_SAFE_RETAIN(m_icon);
	if(m_icon) {
		m_indicator->removeFromParent();
		addChild(m_icon);
	} else {
		addChild(m_indicator);
	}
	relayout();
}

bool CCProgressHUD::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    if(!m_cancellable) {
        return true;
    }
    
    hide();
    return false;
}

void CCProgressHUD::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {
}

void CCProgressHUD::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
}

void CCProgressHUD::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {
}

void CCProgressHUD::keyBackClicked() {
    if(m_cancellable) {
        hide();
    }
}

NS_CC_END