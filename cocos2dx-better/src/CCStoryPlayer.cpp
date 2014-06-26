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

// z order
#define Z_DIALOG_LAYER MAX_INT

NS_CC_BEGIN

CCStoryPlayer::CCStoryPlayer() :
m_owner(NULL),
m_curCmd(NULL),
m_done(false),
m_curCmdDone(false),
m_curCmdIndex(-1) {
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
    m_curCmdDone = false;
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
    m_curCmdDone = false;
}

NS_CC_END