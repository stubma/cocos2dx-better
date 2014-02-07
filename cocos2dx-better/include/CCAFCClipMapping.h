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
#ifndef __CCAFCClipMapping_h__
#define __CCAFCClipMapping_h__

#include "cocos2d.h"
#include "ccMoreTypes.h"
 
NS_CC_BEGIN

class CCSpriteBatchNode;
class CCTexture2D;

/**
 * @typedef CCAFCClipMappingRuleType
 *
 * Definition of all clip mapping rule supported by cocos2dx-better AFC feature
 */
typedef enum {
	/**
	 * map a clip to another clip which is in same file data
	 */
	AFC_CMR_INTERNAL_CLIP,

	/**
	 * map a clip to another clip which is not in same file data
	 */
	AFC_CMR_EXTERNAL_CLIP,

	/**
	 * map a clip to an atlas sprite
	 */
	AFC_CMR_EXTERNAL_ATLAS
} CCAFCClipMappingRuleType;

/**
 * @struct CCAFCClipMappingRule
 *
 * mapping rule info structure
 */
typedef struct CC_DLL CCAFCClipMappingRule {
	/**
	 * rule type
	 */
	CCAFCClipMappingRuleType type;

	/**
	 * index of clip which need to be mapped
	 */
	int sourceClipIndex;

	union {
		/**
		 * icr means internal clip rule
		 */
		struct {
			/**
			 * dest clip index
			 */
			int destClipIndex;
		} icr;

		/**
		 * ecr means external clip rule
		 */
		struct {
			/**
			 * afc file data path
			 */
			const char* path;

			/**
			 * dest clip index in that file data
			 */
			int destClipIndex;

			/**
			 * the atlas sheet where the dest clip resides in
			 */
			CCSpriteBatchNode* sheet;
		} ecr;

		/**
		 * ear means external atlas rule
		 */
		struct {
			/**
			 * the atlas sheet for texture
			 */
			CCSpriteBatchNode* sheet;

			/**
			 * texture rect
			 */
			ccRect texRect;

			/**
			 * clip center position
			 */
			ccPoint pos;

			/**
			 * is image flipped in x axis
			 */
			bool flipX;

			/**
			 * rotation degree of image, positive value is clockwise
			 */
			float rotation;
		} ear;
	};
} CCAFCClipMappingRule;

/**
 * @class CCAFCClipMapping
 *
 * Clip mapping defines a set of rules of replacing a clip to another clip. Then
 * you can use clip mapping to change animation easily and dynamically.
 */
class CC_DLL CCAFCClipMapping : public CCObject {
private:
	/**
	 * tag for this mapping rule set
	 */
	int m_tag;

	/**
	 * how many mapping rules in buffer
	 */
	int m_count;

	/**
	 * capacity of buffer
	 */
	int m_capacity;

	/**
	 * the clip mapping rules
	 */
	CCAFCClipMappingRule* m_rules;

	/**
	 * indices buffer of source clips
	 */
	int* m_sourceClipIndices;

private:
	/**
	 * ensure buffer capacity is ok
	 */
	void ensureCapacity();

protected:
	CCAFCClipMapping(int tag);

	/**
	 * static constructor
	 *
	 * @param tag tag of this mapping rules
	 * @param data raw data of AuroraGT module mapping file
	 * @param length byte length of \c data
	 * @return \link CCAFCClipMapping CCAFCClipMapping\endlink
	 */
	static CCAFCClipMapping* createWithAuroraGT(int tag, const char* data, size_t length);

public:
	virtual ~CCAFCClipMapping();

	/**
	 * static constructor
	 *
	 * @param tag tag of this mapping rules
	 * @return \link CCAFCClipMapping CCAFCClipMapping\endlink
	 */
	static CCAFCClipMapping* create(int tag);

	/**
	 * static constructor
	 *
	 * @param tag tag of this mapping rules
	 * @param ammPath path of aurora module mapping file
	 * @return \link CCAFCClipMapping CCAFCClipMapping\endlink
	 */
	static CCAFCClipMapping* createWithAuroraGT(int tag, const char* ammPath);

	/**
	 * Map a clip to another clip. Source and dest clip are all in same sheet
	 *
	 * @param fromClipIndex source clip index
	 * @param toClipIndex dest clip index
	 */
	void mapClip(int fromClipIndex, int toClipIndex);

	/**
	 * Map a clip to another clip. Source and dest clip are all in different file data
	 *
	 * @param fromClipIndex source clip index
	 * @param externalFilePath unmapped path of dest clip file data
	 * @param toClipIndex dest clip index
	 */
	void mapClip(int fromClipIndex, const char* externalFilePath, int toClipIndex);

	/**
	 * Map a clip to external atlas texture
	 *
	 * @param fromClipIndex source clip index
	 * @param tex external texture
	 * @param pos clip center position, default is zero
	 * @param texRect texture rect in atlas, default is zero which means use whole texture
	 * @param flipX flip clip in x axis or not
	 * @param rotation is clip rotated, positive value means clockwise
	 */
	void mapClip(int fromClipIndex, CCTexture2D* tex, ccPoint pos = ccpZero, ccRect texRect = ccrZero, bool flipX = false, float rotation = 0);

	/**
	 * Get tag
	 *
	 * @return tag
	 */
	int getTag() { return m_tag; }

	/**
	 * Search mapping rule set, and return a matched rule if found
	 *
	 * @param sourceClipIndex index of clip
	 * @return \link CCAFCClipMappingRule CCAFCClipMappingRule\endlink, or NULL if not found
	 */
	CCAFCClipMappingRule* findRule(int sourceClipIndex);
};

NS_CC_END

#endif // __CCAFCClipMapping_h__
