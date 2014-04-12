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
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC

#include "CCAssetInputStream_mac.h"
#include <errno.h>
#include "CCUtils.h"

NS_CC_BEGIN

CCAssetInputStream* CCAssetInputStream::create(const string& path) {
	CCAssetInputStream* ais = new CCAssetInputStream_mac(path);
	return (CCAssetInputStream*)ais->autorelease();
}

CCAssetInputStream_mac::CCAssetInputStream_mac(const string& path) :
		CCAssetInputStream(path),
		m_handle(nil),
		m_length(0) {
    // open file
    NSString* nsPath = [NSString stringWithCString:path.c_str()
                                          encoding:NSUTF8StringEncoding];
    m_handle = [NSFileHandle fileHandleForReadingAtPath:nsPath];
    [m_handle retain];
    
    // get file length
    NSFileManager* fm = [NSFileManager defaultManager];
    NSDictionary* attr = [fm attributesOfItemAtPath:nsPath error:NULL];
    m_length = [[attr objectForKey:NSFileSize] intValue];
}

CCAssetInputStream_mac::~CCAssetInputStream_mac() {
	[m_handle closeFile];
	[m_handle release];
	m_handle = nil;
}

size_t CCAssetInputStream_mac::getLength() {
	return m_length;
}

size_t CCAssetInputStream_mac::getPosition() {
	return [m_handle offsetInFile];
}

size_t CCAssetInputStream_mac::available() {
	return m_length - [m_handle offsetInFile];
}

char* CCAssetInputStream_mac::getBuffer() {
	size_t len = getLength();
	char* buf = (char*)malloc(len * sizeof(char));

	NSData* data = [m_handle availableData];
	memcpy(buf, [data bytes], [data length]);

	return buf;
}

void CCAssetInputStream_mac::close() {
	[m_handle closeFile];
	[m_handle release];
	m_handle = nil;
}

ssize_t CCAssetInputStream_mac::read(char* buffer, size_t length) {
	NSData* data = [m_handle readDataOfLength:length];
	memcpy(buffer, [data bytes], [data length]);
	return [data length];
}

size_t CCAssetInputStream_mac::seek(int offset, int mode) {
	switch (mode) {
		case SEEK_CUR:
			[m_handle seekToFileOffset:getPosition() + offset];
			break;
		case SEEK_END:
			[m_handle seekToFileOffset:m_length + offset];
			break;
		case SEEK_SET:
			[m_handle seekToFileOffset:offset];
			break;
	}

	return [m_handle offsetInFile];
}

NS_CC_END

#endif // #if CC_TARGET_PLATFORM == CC_PLATFORM_MAC