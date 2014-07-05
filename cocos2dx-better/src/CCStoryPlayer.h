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
#ifndef __CCStoryPlayer__
#define __CCStoryPlayer__

#include "cocos2d.h"
#include "CCMoreMacros.h"
#include "cocos-ext.h"

USING_NS_CC_EXT;

NS_CC_BEGIN

class CCStoryLayer;
class CCStoryCommand;
class CBArmature;

/// player of story command sequence
class CCStoryPlayer : public CCObject {
private:
    /// old design size
    CCPoint m_oldDesignSize;
    
    /// need restore to original design size?
    bool m_needRestoreDesignSize;
    
private:
    /// fetch next command
    void fetchNextCommand();
    
    /// execute one command
    void executeCurrentCommand();
    
    /// get a role node
    CCNode* getRole(const char* name);
    
    /// get an armature role
    CBArmature* getArmatureRole(const char* name);
    
    /// callback
    void onArmatureAnimationDone(CBArmature* armature, MovementEventType e, const char* name);
    
public:
    CCStoryPlayer();
    virtual ~CCStoryPlayer();
    static CCStoryPlayer* create(CCStoryLayer* owner);
    
    /// init
    virtual bool initWithOwner(CCStoryLayer* owner);
    
    /// start
    void start();
    
    /// when one message is done
    void onMessageDone();
    
    /// when one click is recieved
    void onClickDone();
    
    /// has error?
    bool hasError() { return !m_error.empty(); }
    
    CC_SYNTHESIZE(CCStoryLayer*, m_owner, Owner);
    CC_SYNTHESIZE(CCStoryCommand*, m_curCmd, CurrentCommand);
    CC_SYNTHESIZE(int, m_curCmdIndex, CurrentCommandIndex);
    CC_SYNTHESIZE_READONLY_BOOL(m_done, Done);
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(CCDictionary, m_roleMap, RoleMap);
    
    // global settings
    CC_SYNTHESIZE(float, m_msgSize, MessageSize);
    CC_SYNTHESIZE(int, m_msgColor, MessageColor);
    CC_SYNTHESIZE(CCPoint, m_msgPos, MessagePos);
    CC_SYNTHESIZE(CCPoint, m_msgAnchor, MessageAnchor);
    CC_SYNTHESIZE_PASS_BY_REF(string, m_msgFont, MessageFont);
    CC_SYNTHESIZE_PASS_BY_REF(string, m_nameFont, NameFont);
    CC_SYNTHESIZE_PASS_BY_REF(string, m_labelFont, LabelFont);
    CC_SYNTHESIZE(float, m_nameSize, NameSize);
    CC_SYNTHESIZE(int, m_nameColor, NameColor);
    CC_SYNTHESIZE(CCPoint, m_labelAnchor, LabelAnchor);
    CC_SYNTHESIZE(float, m_labelSize, LabelSize);
    CC_SYNTHESIZE(int, m_labelColor, LabelColor);
    CC_SYNTHESIZE(CCPoint, m_namePos, NamePos);
    CC_SYNTHESIZE(CCPoint, m_nameAnchor, NameAnchor);
    CC_SYNTHESIZE(CCPoint, m_bgPos, BgPos);
    CC_SYNTHESIZE(CCPoint, m_requiredDesignSize, RequiredDesignSize);
    CC_SYNTHESIZE(ResolutionPolicy, m_requiredPolicy, RequiredPolicy);
    CC_SYNTHESIZE(CCSize, m_msgConstraints, MessageConstraints);
    CC_SYNTHESIZE(CCTextAlignment, m_msgAlignment, MessageAlignment);
    CC_SYNTHESIZE(CCSize, m_labelConstraints, LabelConstraints);
    CC_SYNTHESIZE(CCTextAlignment, m_labelAlignment, LabelAlignment);
    
    // error
    CC_SYNTHESIZE_PASS_BY_REF_SETTER(string, m_error, Error);
};

NS_CC_END

#endif /* defined(__CCStoryPlayer__) */
