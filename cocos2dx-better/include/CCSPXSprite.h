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
#ifndef __CCSPXSprite_h__
#define __CCSPXSprite_h__

#include "CCAFCSprite.h"
#include "CCSPXFileData.h"

NS_CC_BEGIN

/**
 * @class CCSPXSprite
 *
 * \par
 * SpriteX is a tool of animation, exported file has *.sprite name. The structure of sprite
 * file is similar with anu file but different terms used.
 *
 * \par
 * This class supports exported file of SpriteX 3.9.4. For SpriteX 2011, you need \link CCSPX3Sprite CCSPX3Sprite\endlink
 * because file format is totally changed. Remember SpriteX 3.9.4 is second generation not third, that's why SpriteX 2011 uses
 * SPX3 prefix. Maybe a little confusing so be careful.
 */
class CC_DLL CCSPXSprite : public CCAFCSprite {
private:
	/**
	 * SpriteX file data
	 */
	CCSPXFileData* m_spx;

protected:
	CCSPXSprite();

public:
	virtual ~CCSPXSprite();

	/**
	 * Create a \link CCSPXSprite CCSPXSprite\endlink from a SpriteX file path
	 *
	 * @param spxPath path of sprite file
	 * @param tex related texture
	 * @param actionIndex animation index
	 * @return \link CCSPXSprite CCSPXSprite\endlink
	 */
	static CCSPXSprite* create(const char* spxPath, CCTexture2D* tex, int actionIndex);

	/// @see CCAFCSprite::getFileData
	virtual CCAFCFileData* getFileData() { return m_spx; }

	/// @see CCAFCSprite::getAnimationAt
	virtual CCAFCAnimation* getAnimationAt(int index, CCAFCClipMapping* mapping = NULL);

	/// @see CCAFCSprite::getAnimationCount
	virtual int getAnimationCount();

	/// @see CCAFCSprite::getImageCount
	virtual int getImageCount();
};

NS_CC_END

#endif // __CCSPXSprite_h__
