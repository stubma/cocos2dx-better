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
#include "CCMemory.h"
#include <memory.h>

#ifdef CC_CFLAG_MEMORY_TRACKING

// uncomment if you want to log every allocation in console
//#define CC_CFLAG_ALLOCATION_LOG

/*
 * record max size
 */

#define MEMORY_RECORD_INDEX_SIZE 0x10000
#define MEMORY_RECORD_INDEX_MASK 0xFFFF

/// memory record structure
typedef struct ccMemoryRecord {
	/**
	 * start address of allocated memory
	 */
	void* start;

	/**
	 * memory size
	 */
	int size;

	/**
	 * source file name where allocation occurs
	 */
	const char* file;

	/**
	 * source file line where allocation occurs
	 */
	int line;

	/**
	 * next record
	 */
	struct ccMemoryRecord* next;
} ccMemoryRecord;

/*
 * record array and summary info
 */

static ccMemoryRecord* sMemoryRecord[MEMORY_RECORD_INDEX_SIZE] = { 0 };
static int sTotalUsedMemory = 0;
static int sMaxUsedMemory = 0;
static int sTotalAlloc = 0;
static int sTotalFree = 0;

#ifdef __cplusplus
extern "C" {
#endif

void addRecord(ccMemoryRecord* r) {
	// get hash position
	int hash = (uintptr_t)r->start & MEMORY_RECORD_INDEX_MASK;
	ccMemoryRecord* pTemp = sMemoryRecord[hash];
	ccMemoryRecord* pPrev = NULL;

	// find a block whose start address is larger than incoming record
	while (pTemp) {
		if (pTemp->start > r->start) {
			break;
		}
		pPrev = pTemp;
		pTemp = pTemp->next;
	}

	// insert new record into linked list
	if (pPrev == NULL) {
		sMemoryRecord[hash] = r;
		if (pTemp) {
			r->next = pTemp;
		}
	} else {
		r->next = pPrev->next;
		pPrev->next = r;
	}

	// update statistics
	sTotalUsedMemory += r->size;
	sMaxUsedMemory = (sMaxUsedMemory > sTotalUsedMemory ? sMaxUsedMemory : sTotalUsedMemory);
	sTotalAlloc++;
}

ccMemoryRecord* findRecord(void* p) {
	int hash = (uintptr_t)p & MEMORY_RECORD_INDEX_MASK;
	ccMemoryRecord* pTemp = sMemoryRecord[hash];
	ccMemoryRecord* r = NULL;
	while (pTemp) {
		if (pTemp->start == p) {
			r = pTemp;
			break;
		}
		pTemp = pTemp->next;
	}
	return r;
}

ccMemoryRecord* removeRecord(ccMemoryRecord* r) {
	int hash = (uintptr_t)r->start & MEMORY_RECORD_INDEX_MASK;
	ccMemoryRecord* pTemp = sMemoryRecord[hash];
	ccMemoryRecord* pPrev = NULL;
	while(pTemp) {
		if(pTemp == r) {
			break;
		}
		pPrev = pTemp;
		pTemp = pTemp->next;
	}

	// didn't find?
	if(pTemp != r) {
		CCLOG("[MEMRECORD] Unmatched record (%p)(record=%p):%d [%s:%d]", r->start, r, r->size, r->file, r->line);
		return NULL;
	}

	// remove record
	if(pTemp == sMemoryRecord[hash]) {
		sMemoryRecord[hash] = pTemp->next;
	} else {
		pPrev->next = pTemp->next;
	}

	// statistics
	sTotalUsedMemory -= r->size;
	sTotalFree++;

	// return record
	r->next = NULL;
	return r;
}

void* _ccMalloc(size_t size, const char* file, int line, const char* logTag) {
	// null pointer for zero size
	if (size == 0)
		return NULL;

	void* p = malloc(size);
	if (p) {
		ccMemoryRecord* r = (ccMemoryRecord*)malloc(sizeof(ccMemoryRecord));
		if(!r) {
			free(p);
			p = NULL;
		} else {
			r->start = p;
			r->size = size;
			r->file = file;
			r->line = line;
			r->next = NULL;
			addRecord(r);
		}

#if CC_CFLAG_ALLOCATION_LOG
		CCLOG("[%s](%p)(record=%p):%d [%s:%d]", logTag, r->start, r, r->size, r->file, r->line);
#endif
	}

	return p;
}

void* _ccCalloc(size_t nitems, size_t size, const char* file, int line) {
	void* ptr = _ccMalloc(nitems * size, file, line, "CALLOC");
	memset(ptr, 0, nitems * size);
	return ptr;
}

void* _ccRealloc(void* ptr, size_t size, const char* file, int line) {
	ccMemoryRecord* r = findRecord(ptr);
	if(r) {
		// realloc
		void* newPtr = _ccMalloc(size, file, line, "REALLOC");
		memcpy(newPtr, ptr, r->size);
		free(ptr);

		// remove record
		removeRecord(r);
		free(r);

		return newPtr;
	} else {
		return realloc(ptr, size);
	}
}

void _ccFree(void* ptr, const char* file, int line) {
	// basic validation
	if(!ptr)
		return;

	// find record and remove it
	ccMemoryRecord* r = findRecord(ptr);
	if(r) {
#if CC_CFLAG_ALLOCATION_LOG
		CCLOG("[FREE](%p)(record=%p):%d [%s:%d]", r->start, r, r->size, r->file, r->line);
#endif
		removeRecord(r);
		free(r);
	}

	// even record is not found, we must release it
	free(ptr);
}

#ifdef __cplusplus
}
#endif

#endif // #if CC_CFLAG_MEMORY_TRACKING

NS_CC_BEGIN

void CCMemory::usageReport() {
#ifdef CC_CFLAG_MEMORY_TRACKING
	CCLOG("[MEMREPORT] peak %d bytes, now %d bytes, alloc %d times, free %d times",
          sMaxUsedMemory, sTotalUsedMemory, sTotalAlloc, sTotalFree);
#endif
}

void CCMemory::dumpRecord() {
#ifdef CC_CFLAG_MEMORY_TRACKING
	int leak = 0;
	int leakNum = 0;
	for (int i = 0; i < MEMORY_RECORD_INDEX_SIZE; i++) {
		ccMemoryRecord* r = sMemoryRecord[i];
		while(r) {
            CCLOG("%d.[MEMRECORD](%p)(record=%p):%d [%s:%d]", ++leakNum, r->start, r, r->size, r->file, r->line);
            leak += r->size;
			r = r->next;
		}
	}
    
	if(leakNum > 0) {
		CCLOG("[MEMRECORD] total leak = %d", leak);
	} else {
		CCLOG("[MEMRECORD] no memory leak, congratulations!");
	}
#endif
}

NS_CC_END