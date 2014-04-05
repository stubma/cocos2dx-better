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
#include "CBData.h"

NS_CC_BEGIN

CBData::CBData() {
    m_size = 0;
    m_bytes = (uint8_t*)malloc(m_size * sizeof(uint8_t));
}

CBData::CBData(uint8_t* bytes, size_t size) {
    m_size = size;
    m_bytes = (uint8_t*)malloc(m_size * sizeof(uint8_t));
    memcpy(m_bytes, bytes, m_size);
}

CBData::CBData(CBData* data) {
    m_size = data->m_size;
    m_bytes = (uint8_t*)malloc(m_size * sizeof(uint8_t));
    memcpy(m_bytes, data->m_bytes, m_size);
}

CBData::~CBData() {
    CC_SAFE_FREE(m_bytes);
}

CBData* CBData::create() {
    CBData* d = new CBData();
    return (CBData*)d->autorelease();
}

CBData* CBData::createWithBytes(uint8_t* bytes, size_t size) {
    CBData* d = new CBData(bytes, size);
    return (CBData*)d->autorelease();
}

CBData* CBData::createWithData(CBData* data) {
    CBData* d = new CBData(data);
    return (CBData*)d->autorelease();
}

void CBData::appendData(CBData* data) {
    appendBytes(data->getBytes(), data->getSize());
}

void CBData::appendBytes(uint8_t* bytes, size_t size) {
    m_size += size;
    m_bytes = (uint8_t*)realloc(m_bytes, m_size);
    memcpy(m_bytes + m_size - size, bytes, size);
}

NS_CC_END
