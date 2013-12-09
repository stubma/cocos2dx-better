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
#ifndef __CCArcticSprite_h__
#define __CCArcticSprite_h__

#include "CCAFCSprite.h"
#include "CCArcticFileData.h"

NS_CC_BEGIN

/**
 * @class CCArcticSprite
 *
 * \par
 * Arctic editor derives from ArcticGT editor. It uses a simplified format
 * called ASprite which is different with ASprite, the original format of ArcticGT
 *
 * \par
 * So terms of Arctic and ArcticGT are same: Animation, Frame, Module.
 */
class CC_DLL CCArcticSprite : public CCAFCSprite {
private:
	/**
	 * ASprite file data
	 */
	CCArcticFileData* m_arctic;

protected:
	CCArcticSprite();

public:
	virtual ~CCArcticSprite();

	/**
	 * Create arctic sprite from path
	 *
	 * @param path path of ASprite file
	 * @param animIndex animation index
	 * @param tex related textures, if more than one, can follow this argument. Must ends
	 * 		with NULL.
	 */
	static CCArcticSprite* create(const char* path, int animIndex, CCTexture2D* tex, ...);

	/**
	 * Create arctic sprite from path
	 *
	 * @param path path of ASprite file
	 * @param animIndex animation index
	 * @param tex texture array
	 * @param count count of \c tex array
	 */
	static CCArcticSprite* create(const char* path, int animIndex, CCTexture2D** tex, int count);

	/// @see CCAFCSprite::getFileData
	virtual CCAFCFileData* getFileData() { return m_arctic; }

	/// @see CCAFCSprite::getAnimationAt
	virtual CCAFCAnimation* getAnimationAt(int index, CCAFCClipMapping* mapping = NULL);

	/// @see CCAFCSprite::getAnimationCount
	virtual int getAnimationCount();

	/// @see CCAFCSprite::getImageCount
	virtual int getImageCount();
};

NS_CC_END

#endif // __CCArcticSprite_h__
