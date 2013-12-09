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
#ifndef __CCAFCAnimation_h__
#define __CCAFCAnimation_h__

#include "cocos2d.h"
#include "CCAFCFrame.h"

using namespace std;

NS_CC_BEGIN

/**
 * @struct CCAFCAnimation
 *
 * This class represents an animation in AFC architecture
 */
class CC_DLL CCAFCAnimation : public CCObject {
private:
	/**
	 * Frame list, all are \link CCAFCFrame CCAFCFrame\endlink object
	 */
	typedef vector<CCAFCFrame*> AFCFramePtrList;
	AFCFramePtrList m_frameList;

protected:
	CCAFCAnimation();

public:
	virtual ~CCAFCAnimation();
	static CCAFCAnimation* create();

	/**
	 * Get frame count in this animation
	 *
	 * @return frame count
	 */
	int getFrameCount() { return m_frameList.size(); }

	/**
	 * Add a frame to animation
	 *
	 * @param f \link CCAFCFrame CCAFCFrame\endlink
	 */
	void addFrame(CCAFCFrame* f);

	/**
	 * Get frame at specified index
	 *
	 * @param index index, starts from 0
	 * @return \link CCAFCFrame CCAFCFrame\endlink object, or NULL if index is invalid
	 */
	CCAFCFrame* getFrameAt(int index) { return m_frameList.at(index); }
};

NS_CC_END

#endif // __CCAFCAnimation_h__
