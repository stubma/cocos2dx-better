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
#ifndef __CCAFCFrame_h__
#define __CCAFCFrame_h__

#include "cocos2d.h"
#include "CCAFCClip.h"

using namespace std;

NS_CC_BEGIN

/**
 * @class CCAFCFrame
 *
 * A frame in an animation
 */
class CC_DLL CCAFCFrame : public CCObject {
private:
	/**
	 * Clip list, all are \link CCAFCClip CCAFCClip\endlink object
	 */
	typedef vector<CCAFCClip*> AFCClipPtrList;
	AFCClipPtrList m_clipList;

	/**
	 * Duration time of frame
	 */
	float m_delay;

	/**
	 * true means \c m_delay is not a time but a frame count duration
	 */
	bool m_useTickDelay;

	/**
	 * offset relative to previous frame
	 */
	ccPoint m_inc;
    
    /**
     * true means frame is flipped in x axis
     */
    bool m_flipX;
    
    /**
     * true means frame is flipped in y axis
     */
    bool m_flipY;
	
protected:
	CCAFCFrame();

public:
	virtual ~CCAFCFrame();
	static CCAFCFrame* create();

	/**
	 * Get clip count in this animation
	 *
	 * @return clip count
	 */
	int getClipCount() { return m_clipList.size(); }

	/**
	 * Get clip count of specific type
	 *
	 * @param type clip type
	 * @return clip count of that type
	 */
	int getClipCount(CCAFCClipType type);

	/**
	 * Add a clip to frame
	 *
	 * @param clip \link CCAFCClip CCAFCClip\endlink
	 */
	void addClip(CCAFCClip* clip);

	/**
	 * Get clip at specified index
	 *
	 * @param index index, starts from 0
	 * @return \link CCAFCClip CCAFCClip\endlink object, or NULL if index is invalid
	 */
	CCAFCClip* getClipAt(int index) { return m_clipList.at(index); }

	/**
	 * Get clip by index and the clip must be given type
	 *
	 * @param type clip type
	 * @para index index of clip with same types, so it may not reflect actuall position
	 * 		in clip array
	 * @return \link CCAFCClip CCAFCClip\endlink, or NULL if not found
	 */
	CCAFCClip* getClipAt(CCAFCClipType type, int index);

	/**
	 * Set frame duration time
	 *
	 * @param delay frame duration time in second
	 */
	void setDelay(float delay) { m_delay = delay; }

	/**
	 * Get frame duration time, maybe in tick or seconds
	 *
	 * @return frame duration time in tick or seconds
	 */
	float getDelay() { return m_delay; }

	/**
	 * is \c m_delay a tick value?
	 *
	 * @return true means \c m_delay is not time, but frames
	 */
	bool isUseTickDelay() { return m_useTickDelay; }

	/**
	 * Set using tick delay or not
	 *
	 * @param flag true means \c m_delay is frames, not time
	 */
	void setUseTickDelay(bool flag) { m_useTickDelay = flag; }

	/**
	 * Get offset relative to previous frame
	 *
	 * @return offset relative to previous frame
	 */
	ccPoint getIncrementation() { return m_inc; }

	/**
	 * Set offset relative to previous frame
	 *
	 * @param inc offset relative to previous frame
	 */
	void setIncrementation(ccPoint inc) { m_inc = inc; }
    
    /**
     * Get x flip flag
     * 
     * @return true means frame is flipped in x axis
     */
    bool isFlipX() { return m_flipX; }
    
    /**
     * Set x flip flag
     * 
     * @param flag true means frame is flipped in x axis
     */
    void setFlipX(bool flag) { m_flipX = flag; }
    
    /**
     * Get y flip flag
     * 
     * @return true means frame is flipped in y axis
     */
    bool isFlipY() { return m_flipY; }
    
    /**
     * Set y flip flag
     * 
     * @param flag true means frame is flipped in y axis
     */
    void setFlipY(bool flag) { m_flipY = flag; }
};

NS_CC_END

#endif // __CCAFCFrame_h__
