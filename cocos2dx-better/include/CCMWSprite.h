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
#ifndef __CCMWSprite_h__
#define __CCMWSprite_h__

#include "CCAFCSprite.h"
#include "CCMWFileData.h"

NS_CC_BEGIN

/**
 * @class CCMWSprite
 *
 * Motion welder sprite, most logic is encapsulated in parent class and
 * it just provides some methods proprietary to motion welder
 *
 * \note
 * \link CCMWSprite CCMWSprite\endlink doesn't support clip mapping
 */
class CC_DLL CCMWSprite : public CCAFCSprite {
private:
	/**
	 * Animation data loaded from motion welder file
	 */
	CCMWFileData* m_mw;

protected:
	CCMWSprite();

public:
	/**
	 * Create motion welder sprite from path
	 *
	 * @param path path of anu file
	 * @param animIndex animation index
	 * @param tex related textures, if more than one, can follow this argument. Must ends
	 * 		with NULL.
	 */
	static CCMWSprite* create(const char* path, int animIndex, CCTexture2D* tex, ...);

	/**
	 * Create motion welder sprite from path
	 *
	 * @param path path of anu file
	 * @param animIndex animation index
	 * @param tex texture array
	 * @param count count of \c tex array
	 */
	static CCMWSprite* create(const char* path, int animIndex, CCTexture2D** tex, int count);

	virtual ~CCMWSprite();

	/// @see CCAFCSprite::getFileData
	virtual CCAFCFileData* getFileData() { return m_mw; }

	/// @see CCAFCSprite::getAnimationAt
	virtual CCAFCAnimation* getAnimationAt(int index, CCAFCClipMapping* mapping = NULL);

	/// @see CCAFCSprite::getAnimationCount
	virtual int getAnimationCount();

	/// @see CCAFCSprite::getImageCount
	virtual int getImageCount();
};

NS_CC_END

#endif // __CCMWSprite_h__
