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
#ifndef __CCPointList_h__
#define __CCPointList_h__

#include "cocos2d.h"
#include "ccMoreTypes.h"

NS_CC_BEGIN

/**
 * Maintain a buffer of points
 */
class CC_DLL CCPointList : public CCObject {
private:
	/**
	 * point buffer
	 */
	CCPoint* m_buffer;

	/**
	 * point count
	 */
	int m_count;

	/**
	 * capacity of buffer, in point, not bytes
	 */
	int m_capacity;

public:
	CCPointList();
	virtual ~CCPointList();
    static CCPointList* create();

	/**
	 * Add a point
	 */
	void addPoint(float x, float y);

	/**
	 * Add a point
	 */
	void addPoint(CCPoint p) { addPoint(p.x, p.y); }

	/**
	 * Add all points from another point list
	 *
	 * @param plist point list to be copied from
	 */
	void addPoints(CCPointList& plist);

	/**
	 * delete point at given index
	 */
	void deletePointAt(int index);

	/**
	 * Get point buffer
	 */
	CCPoint* getBuffer() { return m_buffer; }

	/**
	 * Get point count
	 */
	int getCount() const { return m_count; }

	/**
	 * Clear all points
	 */
	void clear();

	/**
	 * Get point at given index
	 *
	 * @param index point index
	 * @return \link CCPoint CCPoint\endlink
	 */
	CCPoint getPointAt(int index) const;
};

NS_CC_END

#endif // __CCPointList_h__
