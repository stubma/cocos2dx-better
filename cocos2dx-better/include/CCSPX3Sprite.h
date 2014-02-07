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
#ifndef __CCSPX3Sprite_h__
#define __CCSPX3Sprite_h__

#include "CCAFCSprite.h"
#include "CCSPX3FileData.h"

NS_CC_BEGIN

/**
 * @class CCSPX3Sprite
 *
 * \par
 * SpriteX 2011 is a tool of animation, exported file has *.sprite name. The structure of sprite
 * file is similar with anu file but different terms used.
 *
 * \par
 * This class supports exported file of SpriteX 2011 2011. SpriteX 2011 uses an incompatible file format with
 * 3.9.4, so we creates \link CCSPX3Sprite CCSPX3Sprite\endlink.
 *
 * \note
 * \link CCSPX3Sprite CCSPX3Sprite\endlink doesn't support clip mapping
 */
class CC_DLL CCSPX3Sprite : public CCAFCSprite {
private:
	/**
	 * SpriteX 2011 file data
	 */
	CCSPX3FileData* m_spx;

protected:
	CCSPX3Sprite();

public:
	virtual ~CCSPX3Sprite();

	/**
	 * Create a \link CCSPX3Sprite CCSPX3Sprite\endlink from a SpriteX 2011 file path
	 *
	 * @param spxPath path of sprite file
	 * @param actionIndex animation index
	 * @param tex related textures, if more than one, can follow this argument. Must ends
	 * 		with NULL.
	 * @return \link CCSPX3Sprite CCSPX3Sprite\endlink
	 */
	static CCSPX3Sprite* create(const char* spxPath, int actionIndex, CCTexture2D* tex, ...);

	/**
	 * Create SpriteX 2011 sprite from path
	 *
	 * @param spxPath path of sprite file
	 * @param actionIndex animation index
	 * @param tex texture array
	 * @param count count of \c tex array
	 */
	static CCSPX3Sprite* create(const char* spxPath, int actionIndex, CCTexture2D** tex, int count);

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

#endif // __CCSPX3Sprite_h__
