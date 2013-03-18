/****************************************************************************
 Author: Luma (stubma@gmail.com)
 
 https://github.com/stubma/cocos2dx-common
 
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
#ifdef ANDROID

#include "CCAssetInputStream_android.h"
#include <errno.h>

NS_CC_BEGIN

CCAssetInputStream* CCAssetInputStream::create(const string& path) {
	CCAssetInputStream* ais = new CCAssetInputStream_android(path);
	return (CCAssetInputStream*)ais->autorelease();
}

CCAssetInputStream_android::CCAssetInputStream_android(const string& path) :
		CCAssetInputStream(path),
		m_length(0) {
}

CCAssetInputStream_android::~CCAssetInputStream_android() {
}

size_t CCAssetInputStream_android::getLength() {
	return m_length;
}

size_t CCAssetInputStream_android::getPosition() {
}

size_t CCAssetInputStream_android::available() {
}

char* CCAssetInputStream_android::getBuffer() {
}

void CCAssetInputStream_android::close() {
}

ssize_t CCAssetInputStream_android::read(char* buffer, size_t length) {
}

size_t CCAssetInputStream_android::seek(int offset, int mode) {
}

NS_CC_END

#endif // ANDROID
