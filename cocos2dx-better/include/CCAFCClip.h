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
#ifndef __CCAFCClip_h__
#define __CCAFCClip_h__

#include "cocos2d.h"
#include "ccMoreTypes.h"
#include "CCAFCConstants.h"

NS_CC_BEGIN

class CCSpriteBatchNode;

/**
 * @struct CCAFCClipData
 *
 * Additional data of clip
 */
typedef struct CC_DLL CCAFCClipData {
	/**
	 * Clip center position in frame
	 */
	ccPoint clipPos;

	union {
		/**
		 * info of image clip
		 */
		struct {
			/**
			 * the batch node related to this clip currently
			 */
			CCSpriteBatchNode* sheet;

			/**
			 * Image index of clip, it is the original sheet texture related
			 * to this clip
			 */
			int imageIndex;

			/**
			 * image rect in atlas
			 */
			ccRect rect;

			/**
			 * is image flipped in x axis
			 */
			bool flipX;

			/**
			 * rotation degree of image, positive value is clockwise
			 */
			float rotation;
		} i;

		/**
		 * info of ellipse clip
		 */
		struct {
			/**
			 * fill color in 0xaarrggbb format
			 */
			int color;

			/**
			 * size of ellipse circumscribed rectangle
			 */
			ccSize size;

			/**
			 * start angle of ellipse
			 */
			float startAngle;

			/**
			 * end angle of ellipse
			 */
			float endAngle;
		} e;

		/**
		 * info of line clip
		 */
		struct {
			/**
			 * color in 0xaarrggbb format
			 */
			int color;

			/**
			 * end point of line, the start point is \c m_clipPos of \link CCAFCClip CCAFCClip\endlink
			 */
			ccPoint endPoint;
		} l;

		/**
		 * info of rect clip
		 */
		struct {
			/**
			 * fill color in 0xaarrggbb format
			 */
			int color;

			/**
			 * size of rectangle
			 */
			ccSize size;
		} r;

		/**
		 * info of roundrect clip
		 */
		struct {
			/**
			 * fill color in 0xaarrggbb format
			 */
			int color;

			/**
			 * size of round rectangle
			 */
			ccSize size;

			/**
			 * width of round corner
			 */
			float arcWidth;

			/**
			 * height of round corner
			 */
			float arcHeight;
		} rr;

		/**
		 * info of collision rect clip
		 */
		struct {
			/**
			 * size of collision rectangle
			 */
			ccSize size;
		} cr;

		/**
		 * info of collision triangle clip
		 */
		struct {
			/**
			 * second point
			 */
			ccPoint pt2;

			/**
			 * third point
			 */
			ccPoint pt3;
		} t;
	};
} CCAFCClipData;

/**
 * @class CCAFCClip
 *
 * A clip in a frame
 */
class CC_DLL CCAFCClip : public CCObject {
private:
	/**
	 * clip type
	 */
	CCAFCClipType m_type;

	/**
	 * Clip index in editor. However, some editor doesn't assign an index to
	 * clip, such as Motion Welder, SpriteX, etc., and the clip index is meaningless
	 * for those editors
	 */
	int m_index;

	/**
	 * additional data of clip
	 */
	CCAFCClipData m_data;

protected:
	CCAFCClip();

public:
	virtual ~CCAFCClip();
	static CCAFCClip* create();

	/**
	 * Get type of clip
	 *
	 * @return type of clip
	 * \see CCAFCClipType
	 */
	CCAFCClipType getType() { return m_type; }

	/**
	 * Set type of clip
	 *
	 * @param type type of clip
	 * \see CCAFCClipType
	 */
	void setType(CCAFCClipType type) { m_type = type; }

	/**
	 * Clip index in editor. However, some editor doesn't assign an index to
	 * clip, such as Motion Welder, SpriteX 3.9.4, etc., and the clip index is meaningless
	 * for those editors. Furthermore, if this clip is a mapped clip of external atlas,
	 * then index is also useless.
	 *
	 * \note
	 * If index is -1, then index field is useless for this clip
	 *
	 * @return clip index, returns -1 if index field is meaningless for this clip
	 */
	int getIndex() { return m_index; }

	/**
	 * Set clip index
	 *
	 * @param i clip index
	 */
	void setIndex(int i) { m_index = i; }

	/**
	 * Get clip data
	 *
	 * @return clip data
	 */
	CCAFCClipData& getData() { return m_data; }
};

NS_CC_END

#endif // __CCAFCClip_h__
