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
#ifndef __CCMemory_h__
#define __CCMemory_h__

#include "cocos2d.h"

#ifdef CC_CFLAG_MEMORY_TRACKING

extern "C" CC_DLL void* _ccMalloc(size_t size, const char* file, int line, const char* logTag);
extern "C" CC_DLL void* _ccCalloc(size_t nitems, size_t size, const char* file, int line);
extern "C" CC_DLL void* _ccRealloc(void* ptr, size_t size, const char* file, int line);
extern "C" CC_DLL void _ccFree(void* ptr, const char* file, int line);

inline void* operator new(size_t n, const char* file, int line) {
	return _ccMalloc(n, file, line, "NEW");
}

inline void* operator new[](size_t n, const char* file, int line) {
	return _ccMalloc(n, file, line, "NEW[]");
}

inline void operator delete(void* p, const char* file, int line) {
	_ccFree(p, file, line);
}

inline void operator delete(void* p) throw() {
	_ccFree(p, __FILE__, __LINE__);
}

inline void operator delete[](void* p) throw() {
	_ccFree(p, __FILE__, __LINE__);
}

#define ccMalloc(size) _ccMalloc(size, __FILE__, __LINE__, "MALLOC")
#define ccCalloc(nitems, size) _ccCalloc(nitems, size, __FILE__, __LINE__)
#define ccRealloc(ptr, size) _ccRealloc(ptr, size, __FILE__, __LINE__)
#define ccFree(ptr) _ccFree(ptr, __FILE__, __LINE__)
#define CCNEW new(__FILE__, __LINE__)
#define CCDELETE(object) delete object
#define CCNEWARR(t, s) new(__FILE__, __LINE__) t[(s)]
#define CCDELETEARR(object) delete[] (object)

#else

#define ccMalloc malloc
#define ccCalloc calloc
#define ccRealloc realloc
#define ccFree free
#define CCNEW new
#define CCDELETE(object) delete object
#define CCNEWARR(t, s) new t[(s)]
#define CCDELETEARR(object) delete[] (object)

#endif // #if CC_CFLAG_MEMORY_TRACKING

NS_CC_BEGIN

/**
 * Memory profile helper class
 */
class CCMemory {
public:
    /// print a summary memory usage
    static void usageReport();
    
    /// print all leaked memory record
    static void dumpRecord();
};

NS_CC_END

#endif // __CCMemory_h__
