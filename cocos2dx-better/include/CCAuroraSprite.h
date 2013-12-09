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
#ifndef __CCAuroraSprite_h__
#define __CCAuroraSprite_h__

#include "CCAFCSprite.h"
#include "CCAuroraFileData.h"

NS_CC_BEGIN

/**
 * @class CCAuroraSprite
 *
 * AuroraGT is designed by gameloft and it names exported file as BSprite format. Actually
 * AuroraGT is not only an animation designer, but also a map designer and game element manager.
 */
class CC_DLL CCAuroraSprite : public CCAFCSprite {
private:
	/**
	 * BSprite file data
	 */
	CCAuroraFileData* m_aurora;

protected:
	CCAuroraSprite();

public:
	virtual ~CCAuroraSprite();

	/**
	 * Create aurora sprite from path
	 *
	 * @param path path of BSprite file
	 * @param animIndex animation index
	 * @param tex related textures, if more than one, can follow this argument. Must ends
	 * 		with NULL.
	 */
	static CCAuroraSprite* create(const char* path, int animIndex, CCTexture2D* tex, ...);

	/**
	 * Create aurora sprite from path
	 *
	 * @param path path of BSprite file
	 * @param animIndex animation index
	 * @param tex texture array
	 * @param count count of \c tex array
	 */
	static CCAuroraSprite* create(const char* path, int animIndex, CCTexture2D** tex, int count);

	/// @see CCAFCSprite::getFileData
	virtual CCAFCFileData* getFileData() { return m_aurora; }

	/// @see CCAFCSprite::getAnimationAt
	virtual CCAFCAnimation* getAnimationAt(int index, CCAFCClipMapping* mapping = NULL);

	/// @see CCAFCSprite::getAnimationCount
	virtual int getAnimationCount();

	/// @see CCAFCSprite::getImageCount
	virtual int getImageCount();
};

NS_CC_END

#endif // __CCAuroraSprite_h__
