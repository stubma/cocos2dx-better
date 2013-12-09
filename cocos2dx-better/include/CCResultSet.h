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
#ifndef __CCResultSet_h__
#define __CCResultSet_h__

#include "cocos2d.h"

using namespace std;

NS_CC_BEGIN

class CCDatabase;
class CCStatement;

/**
 * result set
 */
class CC_DLL CCResultSet : public CCObject {
	friend class CCDatabase;

private:
	/// column names in result set
	typedef vector<string> StringList;
    StringList m_columnNames;

    /// sql string which generates this result set
    string m_sql;

protected:
    /// constructor
    CCResultSet(CCDatabase* db, CCStatement* statement);

	/// create a result set
    static CCResultSet* create(CCDatabase* db, CCStatement* statement);

    /**
	 * close this result set, after closing, result set can NOT be used.
	 * deconstructor will also call it so it is not mandatory to call it
     */
    void close();

public:
    virtual ~CCResultSet();

    /**
	 * move cursor to next row
     *
     * @return true means ok, false mean no more data
     */
    bool next();

    /**
	 * has more row?
     *
     * @return true means there is other rows after current cursor
     */
    bool hasAnotherRow();

	/// get column count in result set
    int columnCount();

	/// is type of a column is null?
    bool columnIndexIsNull(int columnIdx);

	/// is type of a column is null?
    bool columnIsNull(string columnName);

	/// get column index by name
    int columnIndexForName(string columnName);

	/// get column name by index, or empty string if index is invalid
    string columnNameForIndex(int columnIdx);

	/// get integer value in a column at current cursor
    int intForColumn(string columnName);

	/// get integer value in a column at current cursor
    int intForColumnIndex(int columnIdx);

	/// get long value in a column at current cursor
    long longForColumn(string columnName);

	/// get long value in a column at current cursor
    long longForColumnIndex(int columnIdx);

	/// get int64_t value in a column at current cursor
    int64_t int64ForColumn(string columnName);

	/// get int64_t value in a column at current cursor
    int64_t int64ForColumnIndex(int columnIdx);

	/// get bool value in a column at current cursor
    bool boolForColumn(string columnName);

	/// get bool value in a column at current cursor
    bool boolForColumnIndex(int columnIdx);

	/// get double value in a column at current cursor
    double doubleForColumn(string columnName);

	/// get double value in a column at current cursor
    double doubleForColumnIndex(int columnIdx);

	/// get string value in a column at current cursor
    string stringForColumn(string columnName);

	/// get string value in a column at current cursor
    string stringForColumnIndex(int columnIdx);

	/// get blob value in a column at current cursor
	/// returned data is copied from original data so caller should release it
    const void* dataForColumn(string columnName, size_t* outLen);

	/// get blob value in a column at current cursor
	/// returned data is copied from original data so caller should release it
    const void* dataForColumnIndex(int columnIdx, size_t* outLen);

	/// get blob value in a column at current cursor
	/// returned data is not copied so caller should NOT release it
    const void* dataNoCopyForColumn(string columnName, size_t* outLen);

	/// get blob value in a column at current cursor
	/// returned data is not copied so caller should NOT release it
    const void* dataNoCopyForColumnIndex(int columnIdx, size_t* outLen);
	
	CC_SYNTHESIZE_READONLY(CCDatabase*, m_db, Database);
	CC_SYNTHESIZE_READONLY(CCStatement*, m_statement, Statement);
};

NS_CC_END

#endif // __CCResultSet_h__
