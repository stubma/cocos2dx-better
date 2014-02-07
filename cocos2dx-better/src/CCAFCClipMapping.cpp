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
#include "CCAFCClipMapping.h"
#include "CCAFCSprite.h"
#include "CCMoreMacros.h"
#include "CCUtils.h"

NS_CC_BEGIN

/// data is in parsing
static const char* s_data;

/// position of read
static int s_pos;

static short readShort() {
	short ret = *(short*)(s_data + s_pos);
	ret = letoh16(ret);
	s_pos += sizeof(short);
	return ret;
}

CCAFCClipMapping::CCAFCClipMapping(int tag) :
		m_tag(tag),
		m_count(0),
		m_capacity(10),
		m_rules((CCAFCClipMappingRule*)malloc(sizeof(CCAFCClipMappingRule) * m_capacity)),
		m_sourceClipIndices((int*)malloc(sizeof(int) * m_capacity)) {
}

CCAFCClipMapping::~CCAFCClipMapping() {
	// release batch node in rules
	CCAFCClipMappingRule* r = m_rules;
	for(int i = 0; i < m_count; i++, r++) {
		switch(r->type) {
			case AFC_CMR_EXTERNAL_CLIP:
				CC_SAFE_RELEASE(r->ecr.sheet);
				if(r->ecr.path)
					free((void*)r->ecr.path);
				break;
			case AFC_CMR_EXTERNAL_ATLAS:
				CC_SAFE_RELEASE(r->ear.sheet);
				break;
			default:
				break;
		}
	}

	// free buffer
	free(m_rules);
	free(m_sourceClipIndices);
}

CCAFCClipMapping* CCAFCClipMapping::create(int tag) {
	CCAFCClipMapping* m = new CCAFCClipMapping(tag);
	return (CCAFCClipMapping*)m->autorelease();
}

CCAFCClipMapping* CCAFCClipMapping::createWithAuroraGT(int tag, const char* data, size_t length) {
	// create empty mapping
	CCAFCClipMapping* m = create(tag);

	/*
	 * parse module mapping data, its format is simple
	 * 1. [SRC CLIP INDEX][DEST CLIP INDEX]
	 * 2. [SRC CLIP INDEX][DEST CLIP INDEX]
	 * 3. [SRC CLIP INDEX][DEST CLIP INDEX]
	 * ...
	 *
	 * index are 2 bytes and in little endian
	 */
	s_data = data;
	s_pos = 0;
	while(s_pos + 4 <= length) {
		/*
		 * Don't use m->mapClip(readShort(), readShort()), it looks ok
		 * at first glance, but the readShort invocation order depends on argument
		 * push priority. If right argument is pushed first, then right readShort
		 * will be called first and it is definitely not what we expect
		 */
		short from = readShort();
		short to = readShort();
		m->mapClip(from, to);
	}

	return m;
}

CCAFCClipMapping* CCAFCClipMapping::createWithAuroraGT(int tag, const char* ammPath) {
	unsigned long len;
	char* data = (char*)CCFileUtils::sharedFileUtils()->getFileData(ammPath, "rb", &len);
	CCAFCClipMapping* m = createWithAuroraGT(tag, data, (size_t)len);
	free(data);
	return m;
}

void CCAFCClipMapping::mapClip(int fromClipIndex, int toClipIndex) {
	ensureCapacity();

	int insertion = CCUtils::binarySearch(m_sourceClipIndices, m_count, fromClipIndex);
	if(insertion < 0) {
		// get insertion point
		insertion = -insertion - 1;

		// if need move, move it
		if(insertion < m_count) {
			memmove(m_sourceClipIndices + insertion + 1, m_sourceClipIndices + insertion, sizeof(int) * (m_count - insertion));
			memmove(m_rules + insertion + 1, m_rules + insertion, sizeof(CCAFCClipMappingRule) * (m_count - insertion));
		}

		// set source index
		m_sourceClipIndices[insertion] = fromClipIndex;

		// set rule
		m_rules[insertion].type = AFC_CMR_INTERNAL_CLIP;
		m_rules[insertion].sourceClipIndex = fromClipIndex;
		m_rules[insertion].icr.destClipIndex = toClipIndex;
		
		// increase count
		m_count++;
	}
}

void CCAFCClipMapping::mapClip(int fromClipIndex, const char* externalFilePath, int toClipIndex) {
	ensureCapacity();

	int insertion = CCUtils::binarySearch(m_sourceClipIndices, m_count, fromClipIndex);
	if(insertion < 0) {
		// get insertion point
		insertion = -insertion - 1;

		// if need move, move it
		if(insertion < m_count) {
			memmove(m_sourceClipIndices + insertion + 1, m_sourceClipIndices + insertion, sizeof(int) * (m_count - insertion));
			memmove(m_rules + insertion + 1, m_rules + insertion, sizeof(CCAFCClipMappingRule) * (m_count - insertion));
		}

		// set source index
		m_sourceClipIndices[insertion] = fromClipIndex;

		// set rule
		m_rules[insertion].type = AFC_CMR_EXTERNAL_CLIP;
		m_rules[insertion].sourceClipIndex = fromClipIndex;
		m_rules[insertion].ecr.path = CCUtils::copy(externalFilePath);
		m_rules[insertion].ecr.destClipIndex = toClipIndex;

		// increase count
		m_count++;
	}
}

void CCAFCClipMapping::mapClip(int fromClipIndex, CCTexture2D* tex, ccPoint pos, ccRect texRect, bool flipX, float rotation) {
	ensureCapacity();

	int insertion = CCUtils::binarySearch(m_sourceClipIndices, m_count, fromClipIndex);
	if(insertion < 0) {
		// get insertion point
		insertion = abs(insertion) - 1;

		// if need move, move it
		if(insertion < m_count) {
			memmove(m_sourceClipIndices + insertion + 1, m_sourceClipIndices + insertion, sizeof(int) * (m_count - insertion));
			memmove(m_rules + insertion + 1, m_rules + insertion, sizeof(CCAFCClipMappingRule) * (m_count - insertion));
		}

		// set source index
		m_sourceClipIndices[insertion] = fromClipIndex;

		// set rule
		m_rules[insertion].type = AFC_CMR_EXTERNAL_ATLAS;
		m_rules[insertion].sourceClipIndex = fromClipIndex;
		m_rules[insertion].ear.sheet = CCAFCSprite::createBatchNode(tex);
		m_rules[insertion].ear.sheet->retain();
		if(texRect.width == 0 || texRect.height == 0) {
			CCSize size = tex->getContentSizeInPixels();
			m_rules[insertion].ear.texRect = ccr(0, 0, size.width, size.height);
		} else {
			m_rules[insertion].ear.texRect = texRect;
		}
		m_rules[insertion].ear.pos = pos;
		m_rules[insertion].ear.flipX = flipX;
		m_rules[insertion].ear.rotation = rotation;

		// increase count
		m_count++;
	}
}

void CCAFCClipMapping::ensureCapacity() {
	while(m_count >= m_capacity) {
		m_capacity *= 2;
		m_rules = (CCAFCClipMappingRule*)realloc(m_rules, sizeof(CCAFCClipMappingRule) * m_capacity);
		m_sourceClipIndices = (int*)realloc(m_sourceClipIndices, sizeof(int) * m_capacity);
	}
}

CCAFCClipMappingRule* CCAFCClipMapping::findRule(int sourceClipIndex) {
	int index = CCUtils::binarySearch(m_sourceClipIndices, m_count, sourceClipIndex);
	if(index >= 0)
		return m_rules + index;
	else
		return NULL;
}

NS_CC_END