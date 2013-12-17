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
#ifndef __CCTextureAtlasEx_h__
#define __CCTextureAtlasEx_h__

#include "cocos2d.h"

NS_CC_BEGIN

/** 
 * Workround version of original CCTextureAtlas, it fixes bugs:
 * 1. VBO bug in Samsung Galaxy (I9001). However, the workaround is not compatible in iOS. Finally I
 * have to remove vbo support
 */
class CC_DLL CCTextureAtlasEx : public CCTextureAtlas {
public:
    CCTextureAtlasEx();
    virtual ~CCTextureAtlasEx();

    /** 
     * creates a TextureAtlas with an filename and with an initial capacity for Quads.
     * The TextureAtlas capacity can be increased in runtime.
     */
    static CCTextureAtlasEx* create(const char* file , unsigned int capacity);

   /** 
    * creates a TextureAtlas with a previously initialized Texture2D object, and
    * with an initial capacity for n Quads. 
    * The TextureAtlas capacity can be increased in runtime.
    */
    static CCTextureAtlasEx* createWithTexture(CCTexture2D *texture, unsigned int capacity);

    /** 
     * draws n quads
     * n can't be greater than the capacity of the Atlas
     *
     * \note
     * it is a subsititute for original drawNumberOfQuads and it doesn't use VBO or VAO.
     * Maybe performance not so good as VBO but best compatibility
     */
    void drawNumberOfQuadsEx(unsigned int n);

    /** 
     * draws n quads from an index (offset).
     * n + start can't be greater than the capacity of the atlas
     *
     * \note
     * it is a subsititute for original drawNumberOfQuads and it doesn't use VBO or VAO.
     * Maybe performance not so good as VBO but best compatibility
     */
    void drawNumberOfQuadsEx(unsigned int n, unsigned int start);

    /** 
     * draws all the Atlas's Quads
     *
     * \note
     * it is a subsititute for original drawQuads and it doesn't use VBO or VAO.
     * Maybe performance not so good as VBO but best compatibility
     */
    void drawQuadsEx();
};

NS_CC_END

#endif //__CCTextureAtlasEx_h__


