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
#include "CCStoryPlayer.h"
#include "CCStoryLayer.h"
#include "CCStoryCommandSet.h"
#include "CCRichLabelTTF.h"
#include "CCStoryDialogLayer.h"

// tag of node
#define TAG_DIALOG_LAYER 10000
#define TAG_BG_COLOR_LAYER 10001

// z order
#define Z_DIALOG_LAYER MAX_INT
#define Z_BG_LAYER -MAX_INT
#define Z_BG_COLOR_LAYER (Z_BG_LAYER + 1)

NS_CC_BEGIN

CCStoryPlayer::CCStoryPlayer() :
m_owner(NULL),
m_curCmd(NULL),
m_done(false),
m_curCmdIndex(-1),
m_msgSize(20),
m_nameSize(20),
m_msgColor(0xffffffff),
m_nameColor(0xffffffff),
m_msgPos(CCPointZero),
m_namePos(CCPointZero),
m_msgAnchor(ccp(0.5f, 0.5f)),
m_nameAnchor(ccp(0.5f, 0.5f)) {
}

CCStoryPlayer::~CCStoryPlayer() {
}

CCStoryPlayer* CCStoryPlayer::create(CCStoryLayer* owner) {
    CCStoryPlayer* p = new CCStoryPlayer();
    if(p->initWithOwner(owner)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_RELEASE(p);
    return NULL;
}

bool CCStoryPlayer::initWithOwner(CCStoryLayer* owner) {
    // init
    m_owner = owner;
    m_msgPos = CCUtils::getLocalCenter(m_owner);
    m_namePos = CCUtils::getLocalCenter(m_owner);
    
    return true;
}

void CCStoryPlayer::start() {
    // first command
    fetchNextCommand();
    
    // is fetched?
    if(m_curCmd) {
        executeCurrentCommand();
    } else {
        m_done = true;
    }
}

void CCStoryPlayer::executeCurrentCommand() {
    switch (m_curCmd->getType()) {
        case CCStoryCommand::MSG:
        {
            // remove old dialog and create new dialog layer
            m_owner->removeChildByTag(TAG_DIALOG_LAYER);
            CCStoryDialogLayer* dl = CCStoryDialogLayer::create(this);
            m_owner->addChild(dl, Z_DIALOG_LAYER);
            
            // show dialog
            dl->showMessage(m_curCmd);
            break;
        }
        case CCStoryCommand::MSG_ANCHOR:
        {
            m_msgAnchor.x = m_curCmd->m_param.msganchor.x;
            m_msgAnchor.y = m_curCmd->m_param.msganchor.y;
            
            // next
            start();
            break;
        }
        case CCStoryCommand::MSG_SIZE:
        {
            m_msgSize = m_curCmd->m_param.msgsize.size;
            
            // next
            start();
            break;
        }
        case CCStoryCommand::MSG_COLOR:
        {
            m_msgColor = m_curCmd->m_param.msgcolor.c;
            
            // next
            start();
            break;
        }
        case CCStoryCommand::MSG_POS:
        {
            m_msgPos.x = m_curCmd->m_param.msgpos.x;
            m_msgPos.y = m_curCmd->m_param.msgpos.y;
            
            // next
            start();
            break;
        }
        case CCStoryCommand::NAME_ANCHOR:
        {
            m_nameAnchor.x = m_curCmd->m_param.nameanchor.x;
            m_nameAnchor.y = m_curCmd->m_param.nameanchor.y;
            
            // next
            start();
            break;
        }
        case CCStoryCommand::BG_COLOR:
        {
            // create bg color layer and set color
            CCLayerColor* bg = (CCLayerColor*)m_owner->getChildByTag(TAG_BG_COLOR_LAYER);
            if(!bg) {
                bg = CCLayerColor::create();
                m_owner->addChild(bg, Z_BG_COLOR_LAYER);
            }
            ccColor4B c = ccc4FromInt(m_curCmd->m_param.bgcolor.c);
            bg->setColor(ccc3FromCCC4(c));
            bg->setOpacity(c.a);
            
            // next
            start();
            break;
        }
        default:
            break;
    }
}

void CCStoryPlayer::fetchNextCommand() {
    if(m_curCmdIndex < (int)gStoryCommandSet.count() - 1) {
        m_curCmdIndex++;
        m_curCmd = (CCStoryCommand*)gStoryCommandSet.objectAtIndex(m_curCmdIndex);
    } else {
        m_curCmd = NULL;
    }
}

NS_CC_END