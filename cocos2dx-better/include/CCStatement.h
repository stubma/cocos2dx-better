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
#ifndef __CCStatement_h__
#define __CCStatement_h__

#include "cocos2d.h"

struct sqlite3_stmt;
using namespace std;

NS_CC_BEGIN

class CCDatabase;

/**
 * SQL statement encapsulation
 */
class CC_DLL CCStatement : public CCObject {
	friend class CCDatabase;

private:
    /// reference count
    int m_useCount;

private:
    CCStatement();

public:
    virtual ~CCStatement();
	
	/// close statement
    void close();
	
    /// reset statement
    void reset();
	
	/// set statement
	void setStatement(sqlite3_stmt* s);
	
	CC_SYNTHESIZE_PASS_BY_REF(string, m_query, Query);
	CC_SYNTHESIZE_READONLY(sqlite3_stmt*, m_statement, Statement);
};

NS_CC_END

#endif // __CCStatement_h__
