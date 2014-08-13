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
#include "CCBase64.h"

NS_CC_BEGIN

static char inAlphabets[256];
static char base64DecodeTable[256];
static const char base64Char[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static void initBase64DecodeTable() {
    for(int i = sizeof(base64Char) - 1; i >= 0 ; i--) {
        inAlphabets[base64Char[i]] = 1;
        base64DecodeTable[base64Char[i]] = i;
    }
}

static char* strDup(char const* str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str) + 1;
    char* copy = new char[len];
    if (copy != NULL) {
        memcpy(copy, str, len);
    }
    return copy;
}

static char* strDupSize(char const* str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str) + 1;
    char* copy = new char[len];
    return copy;
}

string CCBase64::encode(const void* data, int len) {
	unsigned char const* orig = (unsigned char const*)data; // in case any input bytes have the MSB set
	if (orig == NULL || len <= 0)
		return "";
	
	int numOrig24BitValues = len / 3;
	bool havePadding = len > numOrig24BitValues * 3;
	bool havePadding2 = len == numOrig24BitValues * 3 + 2;
	int numResultBytes = 4 * (numOrig24BitValues + havePadding);
	char* result = new char[numResultBytes + 1]; // allow for trailing '/0'
	
	// Map each full group of 3 input bytes into 4 output base-64 characters:
	unsigned i;
	for (i = 0; i < numOrig24BitValues; ++i) {
		result[4 * i] = base64Char[(orig[3 * i] >> 2) & 0x3F];
		result[4 * i + 1] = base64Char[(((orig[3 * i] & 0x3) << 4) | (orig[3 * i + 1] >> 4)) & 0x3F];
		result[4 * i + 2] = base64Char[((orig[3 * i + 1] << 2) | (orig[3 * i + 2] >> 6)) & 0x3F];
		result[4 * i + 3] = base64Char[orig[3 * i + 2] & 0x3F];
	}
	
	// Now, take padding into account.  (Note: i == numOrig24BitValues)
	if (havePadding) {
		result[4 * i + 0] = base64Char[(orig[3 * i] >> 2) & 0x3F];
		if (havePadding2) {
			result[4 * i + 1] = base64Char[(((orig[3 * i] & 0x3) << 4) | (orig[3 * i + 1] >> 4)) & 0x3F];
			result[4 * i + 2] = base64Char[(orig[3 * i + 1] << 2) & 0x3F];
		} else {
			result[4 * i + 1] = base64Char[((orig[3 * i] & 0x3) << 4) & 0x3F];
			result[4 * i + 2] = '=';
		}
		result[4 * i + 3] = '=';
	}
	
	result[numResultBytes] = '\0';
    string ret = result;
    delete[] result;
    
	return ret;
}

const char* CCBase64::decodeAsCString(const string& data, int* outLen) {
	int len;
	const char* v = decode(data, &len);
	
	// save len
	if(outLen)
		*outLen = len;
	
	// append zero
	char* r = (char*)malloc((len + 1) * sizeof(char));
	memcpy(r, v, len);
	r[len] = 0;
	
	// free
	free((void*)v);
	
	return r;
}

const char* CCBase64::decode(const string& data, int* outLen) {
    // init table
	static bool haveInitedBase64DecodeTable = false;
	if (!haveInitedBase64DecodeTable) {
		initBase64DecodeTable();
		haveInitedBase64DecodeTable = true;
	}
    
    // prepare
    int k = 0;
    char* result = NULL;
	const char* in = data.c_str();
    size_t inLen = strlen(in);
    unsigned char* out = (unsigned char*)strDupSize(in); // ensures we have enough space
    
    // decode
    int char_count = 0;
    int bits = 0;
    unsigned char c;
    for(int i = 0; i < inLen; i++) {
        c = in[i];
        if (c == '=')
            break;
        if (c > 255 || !inAlphabets[c])
            continue;
        bits += base64DecodeTable[c];
        char_count++;
        if (char_count == 4) {
            out[k++] = (bits >> 16);
            out[k++] = ((bits >> 8) & 0xff);
            out[k++] = (bits & 0xff);
            bits = 0;
            char_count = 0;
        } else {
            bits <<= 6;
        }
    }
    
    // last seg
    if(c == '=') {
        switch (char_count) {
            case 2:
                out[k++] = bits >> 10;
                break;
            case 3:
                out[k++] = bits >> 16;
                out[k++] = (bits >> 8) & 0xff;
                break;
        }
    }
    
    // create returned array
    if(k > 0) {
        result = (char*)malloc(sizeof(char) * k);
        memmove(result, out, k);
    }
    delete[] out;
    
    // length of decoded
    if(outLen)
        *outLen = k;
	
	return result;
}

NS_CC_END