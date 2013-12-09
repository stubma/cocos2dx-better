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
#ifndef __CCAFCFileData_h__
#define __CCAFCFileData_h__

#include "cocos2d.h"
#include "CCAFCClip.h"

using namespace std;

NS_CC_BEGIN

/**
 * @class CCAFCFileData
 *
 * Base class for all file data object in AFC architecture, it contains
 * common fields for AFC file data
 */
class CC_DLL CCAFCFileData : public CCObject {
public:
	/**
	 * path key for this data file, should be an unmapped path
	 */
	string m_path;

	/**
	 * scale of file data
	 */
	float m_resScale;

protected:
	CCAFCFileData();

public:
	virtual ~CCAFCFileData();
};

NS_CC_END

#endif // __CCAFCFileData_h__
