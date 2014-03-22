/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2009-2010 Ricardo Quesada
Copyright (C) 2009      Matt Oswald
Copyright (c) 2011      Zynga Inc.

http://www.cocos2d-x.org

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

#ifndef __CB_SPRITE_BATCH_NODE_H__
#define __CB_SPRITE_BATCH_NODE_H__

#include "cocos2d.h"

using namespace std;

NS_CC_BEGIN

class CBTMXLayer;

/** 
 * XXX: copy CCSpriteBatchNode because my TMX need some protected method of it,
 * so let CBTMXLayer be his friend class
 */
class CC_DLL CBSpriteBatchNode : public CCSpriteBatchNode
{
	friend class CBTMXLayer;
	
public:
    CBSpriteBatchNode();
    virtual ~CBSpriteBatchNode();

    /** creates a CBSpriteBatchNode with a texture2d and capacity of children.
    The capacity will be increased in 33% in runtime if it run out of space.
    */
    static CBSpriteBatchNode* createWithTexture(CCTexture2D* tex, unsigned int capacity);
    static CBSpriteBatchNode* createWithTexture(CCTexture2D* tex) {
        return CBSpriteBatchNode::createWithTexture(tex, kDefaultSpriteBatchCapacity);
    }

    /** creates a CBSpriteBatchNode with a file image (.png, .jpeg, .pvr, etc) and capacity of children.
    The capacity will be increased in 33% in runtime if it run out of space.
    The file will be loaded using the TextureMgr.
    */
    static CBSpriteBatchNode* create(const char* fileImage, unsigned int capacity);
    static CBSpriteBatchNode* create(const char* fileImage) {
        return CBSpriteBatchNode::create(fileImage, kDefaultSpriteBatchCapacity);
    }
};

// end of sprite_nodes group
/// @}

NS_CC_END

#endif // __CB_SPRITE_BATCH_NODE_H__
