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
#include "CCResultSet.h"
#include "CCDatabase.h"
#include "CCStatement.h"
#include <unistd.h>
#include "sqlite3.h"
#include "CCUtils.h"

NS_CC_BEGIN

CCResultSet::CCResultSet(CCDatabase* db, CCStatement* statement) :
		m_db(db),
		m_statement(statement),
		m_sql(statement->getQuery()) {
	// setup column names
    int columnCount = sqlite3_column_count(statement->getStatement());
    for(int i = 0; i < columnCount; i++) {
    	string name = sqlite3_column_name(statement->getStatement(), i);
    	CCUtils::toLowercase(name);
		m_columnNames.push_back(name);
    }
}

CCResultSet::~CCResultSet() {
	// close
	close();
	
	// nullify
	m_db = NULL;
}

CCResultSet* CCResultSet::create(CCDatabase* db, CCStatement* statement) {
	CCResultSet* rs = new CCResultSet(db, statement);
	return (CCResultSet*)rs->autorelease();
}

bool CCResultSet::next() {
	int rc = 0;
	if(m_statement) {
		bool retry;
		int numberOfRetries = 0;
		do {
			retry = false;

			rc = sqlite3_step(m_statement->getStatement());

			if(SQLITE_BUSY == rc || SQLITE_LOCKED == rc) {
				// this will happen if the db is locked, like if we are doing an update or insert.
				// in that case, retry the step... and maybe wait just 10 milliseconds.
				retry = true;
				if(SQLITE_LOCKED == rc) {
					rc = sqlite3_reset(m_statement->getStatement());
					if(rc != SQLITE_LOCKED) {
						CCLOGERROR("CCResultSet::next: Unexpected result from sqlite3_reset (%d) rs", rc);
					}
				}
				usleep(20);

				if(m_db->getBusyRetryTimeout() && (numberOfRetries++ > m_db->getBusyRetryTimeout())) {
					CCLOGWARN("CCResultSet::next: Database busy");
					break;
				}
			} else if(SQLITE_DONE == rc || SQLITE_ROW == rc) {
				// all is well, let's return.
			} else if(SQLITE_ERROR == rc) {
				CCLOGERROR("Error calling sqlite3_step (%d: %s) rs", rc, m_db->lastErrorMessage().c_str());
				break;
			} else if(SQLITE_MISUSE == rc) {
				CCLOGERROR("Error calling sqlite3_step (%d: %s) rs", rc, m_db->lastErrorMessage().c_str());
				break;
			} else {
				CCLOGERROR("Unknown error calling sqlite3_step (%d: %s) rs", rc, m_db->lastErrorMessage().c_str());
				break;
			}
		} while(retry);
	}

	return rc == SQLITE_ROW;
}

bool CCResultSet::hasAnotherRow() {
	return sqlite3_errcode(m_db->sqliteHandle()) == SQLITE_ROW;
}

void CCResultSet::close() {
	if(m_statement) {
		m_statement->reset();
		m_statement = NULL;
		
		// post close
		if(m_db)
			m_db->postResultSetClosed(this);
	}
}

int CCResultSet::columnCount() {
	return sqlite3_column_count(m_statement->getStatement());
}

bool CCResultSet::columnIndexIsNull(int columnIdx) {
	return sqlite3_column_type(m_statement->getStatement(), columnIdx) == SQLITE_NULL;
}

bool CCResultSet::columnIsNull(string columnName) {
	return columnIndexIsNull(columnIndexForName(columnName));
}

int CCResultSet::columnIndexForName(string columnName) {
	int index = 0;
	for(StringList::iterator iter = m_columnNames.begin(); iter != m_columnNames.end(); iter++, index++) {
		string& name = *iter;
		if(name == columnName) {
			return index;
		}
	}

	CCLOGWARN("Can't find column index for name: %s", columnName.c_str());
	return -1;
}

string CCResultSet::columnNameForIndex(int columnIdx) {
	if(columnIdx < 0 || columnIdx >= m_columnNames.size())
		return NULL;
	else
		return m_columnNames.at(columnIdx);
}

int CCResultSet::intForColumn(string columnName) {
	return intForColumnIndex(columnIndexForName(columnName));
}

int CCResultSet::intForColumnIndex(int columnIdx) {
	return sqlite3_column_int(m_statement->getStatement(), columnIdx);
}

long CCResultSet::longForColumn(string columnName) {
	return longForColumnIndex(columnIndexForName(columnName));
}

long CCResultSet::longForColumnIndex(int columnIdx) {
	return (long)sqlite3_column_int64(m_statement->getStatement(), columnIdx);
}

int64_t CCResultSet::int64ForColumn(string columnName) {
	return int64ForColumnIndex(columnIndexForName(columnName));
}

int64_t CCResultSet::int64ForColumnIndex(int columnIdx) {
	return (int64_t)sqlite3_column_int64(m_statement->getStatement(), columnIdx);
}

bool CCResultSet::boolForColumn(string columnName) {
	return boolForColumnIndex(columnIndexForName(columnName));
}

bool CCResultSet::boolForColumnIndex(int columnIdx) {
	return intForColumnIndex(columnIdx) != 0;
}

double CCResultSet::doubleForColumn(string columnName) {
	return doubleForColumnIndex(columnIndexForName(columnName));
}

double CCResultSet::doubleForColumnIndex(int columnIdx) {
	return sqlite3_column_double(m_statement->getStatement(), columnIdx);
}

string CCResultSet::stringForColumn(string columnName) {
	return stringForColumnIndex(columnIndexForName(columnName));
}

string CCResultSet::stringForColumnIndex(int columnIdx) {
    if (sqlite3_column_type(m_statement->getStatement(), columnIdx) == SQLITE_NULL || (columnIdx < 0)) {
        return NULL;
    }

    return (const char*)sqlite3_column_text(m_statement->getStatement(), columnIdx);
}

const void* CCResultSet::dataForColumn(string columnName, size_t* outLen) {
	return dataForColumnIndex(columnIndexForName(columnName), outLen);
}

const void* CCResultSet::dataForColumnIndex(int columnIdx, size_t* outLen) {
	// check type
    if (sqlite3_column_type(m_statement->getStatement(), columnIdx) == SQLITE_NULL || (columnIdx < 0)) {
    	*outLen = 0;
        return NULL;
    }

    // copy data
    int dataSize = sqlite3_column_bytes(m_statement->getStatement(), columnIdx);
    char* buf = (char*)malloc(dataSize * sizeof(char));
    memcpy(buf, sqlite3_column_blob(m_statement->getStatement(), columnIdx), dataSize);

    // return buffer
    *outLen = dataSize;
    return (const char*)buf;
}

const void* CCResultSet::dataNoCopyForColumn(string columnName, size_t* outLen) {
	return dataNoCopyForColumnIndex(columnIndexForName(columnName), outLen);
}

const void* CCResultSet::dataNoCopyForColumnIndex(int columnIdx, size_t* outLen) {
    if (sqlite3_column_type(m_statement->getStatement(), columnIdx) == SQLITE_NULL || (columnIdx < 0)) {
    	*outLen = 0;
        return NULL;
    }

    *outLen = sqlite3_column_bytes(m_statement->getStatement(), columnIdx);
    return (const char*)sqlite3_column_blob(m_statement->getStatement(), columnIdx);
}

NS_CC_END