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
#include "CCDatabase.h"
#include "sqlite3.h"
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "CCUtils.h"

NS_CC_BEGIN

CCDatabase::CCDatabase(string path) :
		m_db(NULL),
		m_databasePath(path),
		m_inUse(false),
		m_inTransaction(false),
		m_shouldCacheStatements(false),
		m_busyRetryTimeout(0) {
}

CCDatabase::~CCDatabase() {
	close();
	
	// release statements
	for(StatementMap::iterator iter = m_cachedStatements.begin(); iter != m_cachedStatements.end(); iter++) {
		iter->second->release();
	}
}

CCDatabase* CCDatabase::create(string path) {
	CCDatabase* d = new CCDatabase(path);
	return (CCDatabase*)d->autorelease();
}

bool CCDatabase::isThreadSafe() {
	return sqlite3_threadsafe() != SQLITE_CONFIG_SINGLETHREAD;
}

string CCDatabase::sqliteLibVersion() {
	return sqlite3_libversion();
}

bool CCDatabase::open(int flags) {
    if(m_db) {
        return true;
    }

	// database path which is not mapped yet
	string path = m_databasePath;
	
	// if null, create it in memory
	// if not null, create intermediate directory
	if(path.empty())
		path = ":memory:";
	else {
		if(!CCUtils::createIntermediateFolders(path)) {
			CCLOGERROR("failed to create containing directory for database");
			return false;
		}
	}
	
	// create database
    int err = SQLITE_OK;
#if SQLITE_VERSION_NUMBER >= 3005000
    if(flags != 0)
		err = sqlite3_open_v2(path.c_str(), &m_db, flags, NULL);
    else
#endif
    	err = sqlite3_open(path.c_str(), &m_db);
	
	// check error
    if(err != SQLITE_OK) {
        CCLOGERROR("CCDatabase:open: error opening: %d", err);
        m_db = NULL;
        return false;
    }

    return true;
}

bool CCDatabase::close() {
	clearCachedStatements();

	// check db
	if(!m_db) {
		return true;
	}

	// variables
    int rc;
	bool retry;
	int numberOfRetries = 0;

	// try to close db
	// if it is busy or locked, wait for a while and retry
	// m_busyRetryTimeout controls max number of retries
    do {
		retry = false;
		rc = sqlite3_close(m_db);
		if(SQLITE_BUSY == rc || SQLITE_LOCKED == rc) {
			retry = true;
			usleep(20);
			if(m_busyRetryTimeout && (numberOfRetries++ > m_busyRetryTimeout)) {
				CCLOGWARN("CCDatabase:close: Database busy, unable to close");
				return false;
			}
		} else if(SQLITE_OK != rc) {
			CCLOGWARN("CCDatabase:close: error closing!: %d", rc);
		}
	} while(retry);

    // nullify reference
	m_db = NULL;

	return true;
}

bool CCDatabase::goodConnection() {
	// basic check
	if(!m_db) {
		return false;
	}

	// select something from sqlite_master to verify the connection
	CCResultSet* rs = executeQuery("select name from sqlite_master where type='table'");

	// if we can get result from sqlite_mast, return true
	if(rs) {
		rs->release();
		return true;
	}

	return false;
}

void CCDatabase::clearCachedStatements() {
	for(StatementMap::iterator iter = m_cachedStatements.begin(); iter != m_cachedStatements.end(); iter++) {
		iter->second->release();
	}
	m_cachedStatements.clear();
}

bool CCDatabase::databaseOpened() {
    if (!m_db) {
        CCLOGWARN("The CCDatabase %p is not open.", this);
        return false;
    }

    return true;
}

bool CCDatabase::executeUpdate(string sql, ...) {
	// generate final sql string
    va_list args;
    va_start(args, sql);
    char buf[512];
    vsprintf(buf, sql.c_str(), args);
    va_end(args);

    // execute the final sql
    return _executeUpdate(buf);
}

bool CCDatabase::_executeUpdate(const char* sql) {
	// database check
    if (!databaseOpened()) {
        return false;
    }

    // is in use?
    if (m_inUse) {
        warnInUse();
        return false;
    }

    // use it now
    setInUse(true);

    // variables
    int rc = 0;
	sqlite3_stmt* pStmt = NULL;
	CCStatement* cachedStmt = NULL;

	// get cached statement
	cachedStmt = getCachedStatement(sql);
	pStmt = cachedStmt ? cachedStmt->getStatement() : NULL;

	// reset if statement is cached
	if(cachedStmt)
		cachedStmt->reset();
	
    // retry flags
    int numberOfRetries = 0;
	bool retry = false;

	// compile statement until success or fail
    if(!pStmt) {
		do {
			// prepare statement
			retry = false;
			rc = sqlite3_prepare_v2(m_db, sql, -1, &pStmt, 0);

			// wait if busy
			if(SQLITE_BUSY == rc || SQLITE_LOCKED == rc) {
				retry = true;
				usleep(20);

				if(m_busyRetryTimeout && (numberOfRetries++ > m_busyRetryTimeout)) {
					CCLOGWARN("CCDatabase:_executeUpdate: Database busy");
					sqlite3_finalize(pStmt);
					setInUse(false);
					return false;
				}
			} else if(SQLITE_OK != rc) {
				// log error
				CCLOGERROR("CCDatabase:_executeUpdate: DB Error: %d \"%s\"", lastErrorCode(), lastErrorMessage().c_str());

				// release statement
				sqlite3_finalize(pStmt);

				// set in use
				setInUse(false);
				return false;
			}
		} while(retry);
	}

    /*
     * Call sqlite3_step() to run the virtual machine. Since the SQL being
     * executed is not a SELECT statement, we assume no data will be returned.
     */
    numberOfRetries = 0;
	do {
		rc = sqlite3_step(pStmt);
		retry = false;

		if(SQLITE_BUSY == rc || SQLITE_LOCKED == rc) {
			// this will happen if the db is locked, like if we are doing an update or insert.
			// in that case, retry the step... and maybe wait just 10 milliseconds.
			retry = true;
			if(SQLITE_LOCKED == rc) {
				rc = sqlite3_reset(pStmt);
				if(rc != SQLITE_LOCKED) {
					CCLOGERROR("CCDatabase:_executeUpdate: Unexpected result from sqlite3_reset (%d) eu", rc);
				}
			}
			usleep(20);

			if(m_busyRetryTimeout && (numberOfRetries++ > m_busyRetryTimeout)) {
				CCLOGWARN("CCDatabase:_executeUpdate: Database busy 2");
				retry = false;
			}
		} else if(SQLITE_DONE == rc || SQLITE_ROW == rc) {
			// all is well, let's return.
		} else if(SQLITE_ERROR == rc) {
			CCLOGERROR("Error calling sqlite3_step (%d: %s) SQLITE_ERROR", rc, lastErrorMessage().c_str());
		} else if(SQLITE_MISUSE == rc) {
			CCLOGERROR("Error calling sqlite3_step (%d: %s) SQLITE_MISUSE", rc, lastErrorMessage().c_str());
		} else {
			CCLOGERROR("Unknown error calling sqlite3_step (%d: %s) eu", rc, lastErrorMessage().c_str());
		}
	} while(retry);

	// cache statement
    if(m_shouldCacheStatements && !cachedStmt) {
		cachedStmt = new CCStatement();
		cachedStmt->setStatement(pStmt);
		cachedStmt->setQuery(sql);
		cachedStmt->m_useCount = 1;
		setCachedStatement(sql, cachedStmt);
	}

    // if statement is cached, don't finalize it
    if(cachedStmt) {
		rc = sqlite3_reset(pStmt);
	} else {
		/*
		 * Finalize the virtual machine. This releases all memory and other
		 * resources allocated by the sqlite3_prepare() call above.
		 */
		rc = sqlite3_finalize(pStmt);
	}

    // release usage
    setInUse(false);

    // returb
    return rc == SQLITE_OK;
}

CCResultSet* CCDatabase::executeQuery(string sql, ...) {
	// generate final sql string
    va_list args;
    va_start(args, sql);
    char buf[512];
    vsprintf(buf, sql.c_str(), args);
    va_end(args);

    // execute the final sql
    return _executeQuery(buf);
}

CCResultSet* CCDatabase::_executeQuery(const char* sql) {
	// database check
    if (!databaseOpened()) {
        return NULL;
    }

    // is in use?
    if (m_inUse) {
        warnInUse();
        return NULL;
    }

    // use it now
    setInUse(true);

    // variables
    CCResultSet* rs = NULL;
	int rc = 0;
	sqlite3_stmt* pStmt = NULL;
	CCStatement* statement = NULL;

	// get cached statement
	statement = getCachedStatement(sql);
	pStmt = statement ? statement->getStatement() : NULL;
	
	// reset if statement is cached
	if(statement)
		statement->reset();

    // retry flags
    int numberOfRetries = 0;
	bool retry = false;

	// compile statement until success or fail
    if(!pStmt) {
		do {
			// prepare statement
			retry = false;
			rc = sqlite3_prepare_v2(m_db, sql, -1, &pStmt, 0);

			// wait if busy
			if(SQLITE_BUSY == rc || SQLITE_LOCKED == rc) {
				retry = true;
				usleep(20);

				if(m_busyRetryTimeout && (numberOfRetries++ > m_busyRetryTimeout)) {
					CCLOGWARN("CCDatabase:_executeQuery: Database busy");
					sqlite3_finalize(pStmt);
					setInUse(false);
					return NULL;
				}
			} else if(SQLITE_OK != rc) {
				// log error
				CCLOGERROR("CCDatabase:_executeQuery: DB Error: %d \"%s\"", lastErrorCode(), lastErrorMessage().c_str());

				// release statement
				sqlite3_finalize(pStmt);

				// set in use
				setInUse(false);
				return NULL;
			}
		} while(retry);
	}

    // create CCStatement
    if (!statement) {
        statement = new CCStatement();
        statement->setStatement(pStmt);
        statement->setQuery(sql);
        statement->m_useCount = 1;
		setCachedStatement(sql, statement);
    } else {
    	statement->m_useCount++;
    }

    // now query
    rs = CCResultSet::create(this, statement);

    // set in use flag
    setInUse(false);

    // return
    return rs;
}

void CCDatabase::postResultSetClosed(CCResultSet* rs) {
	// find related statement
	StatementMap::iterator iter = m_cachedStatements.find(rs->m_sql);
	if(iter != m_cachedStatements.end()) {
		// decrease use count and release it if it is zero as well as cache flag is false
		iter->second->m_useCount--;
		if(iter->second->m_useCount <= 0 && !m_shouldCacheStatements) {
			iter->second->release();
			m_cachedStatements.erase(iter);
		}
	}
}

void CCDatabase::warnInUse() {
    CCLOGWARN("The CCDatabase %p is currently in use.", this);
}

string CCDatabase::lastErrorMessage() {
	if(m_db)
		return sqlite3_errmsg(m_db);
	else
		return "";
}

int CCDatabase::lastErrorCode() {
	if(m_db)
		return sqlite3_errcode(m_db);
	else
		return SQLITE_OK;
}

bool CCDatabase::hadError() {
    int lastErrCode = lastErrorCode();
    return (lastErrCode > SQLITE_OK && lastErrCode < SQLITE_ROW);
}

int64_t CCDatabase::lastInsertRowId() {
    if(m_inUse) {
    	warnInUse();
        return false;
    }

    setInUse(true);
    sqlite_int64 ret = sqlite3_last_insert_rowid(m_db);
    setInUse(false);

    return (int64_t)ret;
}

void CCDatabase::setShouldCacheStatements(bool value) {
	m_shouldCacheStatements = value;

	if(!m_shouldCacheStatements) {
		clearCachedStatements();
	}
}

int CCDatabase::changes() {
    if (m_inUse) {
        warnInUse();
        return 0;
    }

    setInUse(true);
    int ret = sqlite3_changes(m_db);
    setInUse(false);

    return ret;
}

CCStatement* CCDatabase::getCachedStatement(const char* sql) {
	StatementMap::iterator iter = m_cachedStatements.find(sql);
	if(iter != m_cachedStatements.end()) {
		return iter->second;
	} else {
		return NULL;
	}
}

void CCDatabase::setCachedStatement(const char* sql, CCStatement* statement) {
	// release old
	StatementMap::iterator iter = m_cachedStatements.find(sql);
	if(iter != m_cachedStatements.end()) {
		iter->second->release();
		m_cachedStatements.erase(iter);
	}
	
	// add new
	m_cachedStatements[sql] = statement;
}

bool CCDatabase::rollback() {
	bool b = executeUpdate("ROLLBACK TRANSACTION;");
    if (b) {
        m_inTransaction = false;
    }
    return b;
}

bool CCDatabase::commit() {
    bool b = executeUpdate("COMMIT TRANSACTION;");
    if (b) {
    	m_inTransaction = false;
    }
    return b;
}

bool CCDatabase::beginDeferredTransaction() {
	bool b = executeUpdate("BEGIN DEFERRED TRANSACTION;");
    if (b) {
    	m_inTransaction = true;
    }
    return b;
}

bool CCDatabase::beginTransaction() {
	bool b = executeUpdate("BEGIN EXCLUSIVE TRANSACTION;");
    if (b) {
    	m_inTransaction = true;
    }
    return b;
}

bool CCDatabase::tableExists(string tableName) {
    bool returnBool;

    // search in sqlite_master table if table exists
	CCUtils::toLowercase(tableName);
    CCResultSet* rs = executeQuery("select [sql] from sqlite_master where [type] = 'table' and lower(name) = '%s'", tableName.c_str());

    // if at least one next exists, table exists
    returnBool = rs->next();

    // close and free object
    rs->close();

    return returnBool;
}

int CCDatabase::getVersion() {
	return intForQuery("PRAGMA user_version;");
}

void CCDatabase::setVersion(int v) {
	executeUpdate("PRAGMA user_version = %d;", v);
}

int CCDatabase::intForQuery(string sql, ...) {
	// generate final sql string
    va_list args;
    va_start(args, sql);
    char buf[512];
    vsprintf(buf, sql.c_str(), args);
    va_end(args);

    CCResultSet* rs = _executeQuery(buf);
    if(rs->next())
		return rs->intForColumnIndex(0);
	else
		return 0;
}

long CCDatabase::longForQuery(string sql, ...) {
	// generate final sql string
    va_list args;
    va_start(args, sql);
    char buf[512];
    vsprintf(buf, sql.c_str(), args);
    va_end(args);

    CCResultSet* rs = _executeQuery(buf);
    if(rs->next())
		return rs->longForColumnIndex(0);
	else
		return 0;
}

int64_t CCDatabase::int64ForQuery(string sql, ...) {
	// generate final sql string
    va_list args;
    va_start(args, sql);
    char buf[512];
    vsprintf(buf, sql.c_str(), args);
    va_end(args);

    CCResultSet* rs = _executeQuery(buf);
    if(rs->next())
		return rs->int64ForColumnIndex(0);
	else
		return 0;
}

bool CCDatabase::boolForQuery(string sql, ...) {
	// generate final sql string
    va_list args;
    va_start(args, sql);
    char buf[512];
    vsprintf(buf, sql.c_str(), args);
    va_end(args);

    CCResultSet* rs = _executeQuery(buf);
    if(rs->next())
		return rs->boolForColumnIndex(0);
	else
		return false;
}

double CCDatabase::doubleForQuery(string sql, ...) {
	// generate final sql string
    va_list args;
    va_start(args, sql);
    char buf[512];
    vsprintf(buf, sql.c_str(), args);
    va_end(args);

    CCResultSet* rs = _executeQuery(buf);
    if(rs->next())
		return rs->doubleForColumnIndex(0);
	else
		return 0;
}

string CCDatabase::stringForQuery(string sql, ...) {
	// generate final sql string
    va_list args;
    va_start(args, sql);
    char buf[512];
    vsprintf(buf, sql.c_str(), args);
    va_end(args);

    CCResultSet* rs = _executeQuery(buf);
    if(rs->next())
		return rs->stringForColumnIndex(0);
	else
		return "";
}

const void* CCDatabase::dataForQuery(string sql, size_t* outLen, ...) {
	// generate final sql string
    va_list args;
    va_start(args, outLen);
    char buf[512];
    vsprintf(buf, sql.c_str(), args);
    va_end(args);

    CCResultSet* rs = _executeQuery(buf);
    if(rs->next())
		return rs->dataForColumnIndex(0, outLen);
	else
		return NULL;
}

const void* CCDatabase::dataNoCopyForQuery(string sql, size_t* outLen, ...) {
	// generate final sql string
    va_list args;
    va_start(args, outLen);
    char buf[512];
    vsprintf(buf, sql.c_str(), args);
    va_end(args);

    CCResultSet* rs = _executeQuery(buf);
    if(rs->next())
		return rs->dataNoCopyForColumnIndex(0, outLen);
	else
		return NULL;
}

string CCDatabase::validateSQL(string sql, ...) {
	// generate final sql string
	va_list args;
	va_start(args, sql);
	char buf[512];
	vsprintf(buf, sql.c_str(), args);
	va_end(args);

	// variables
	sqlite3_stmt* pStmt = NULL;
	bool keepTrying = true;
	int numberOfRetries = 0;
	string ret;

	// set in use flag
	setInUse(true);

	// trying until success or fail
	while(keepTrying) {
		keepTrying = false;
		int rc = sqlite3_prepare_v2(m_db, buf, -1, &pStmt, 0);
		if(rc == SQLITE_BUSY || rc == SQLITE_LOCKED) {
			keepTrying = true;
			usleep(20);

			if(m_busyRetryTimeout && (numberOfRetries++ > m_busyRetryTimeout)) {
				CCLOGWARN("CCDatabase::validateSQL: Database busy");
			}
		} else if(rc != SQLITE_OK) {
			ret = lastErrorMessage();
		}
	}

	// set in use flag
	setInUse(false);

	// release statement
	sqlite3_finalize(pStmt);

	return ret;
}

CCResultSet* CCDatabase::getSchema() {
	// result columns: type[STRING], name[STRING],tbl_name[STRING], rootpage[INTEGER], sql[STRING]
    CCResultSet* rs = executeQuery("SELECT type, name, tbl_name, rootpage, sql FROM (SELECT * FROM sqlite_master UNION ALL SELECT * FROM sqlite_temp_master) WHERE type != 'meta' AND name NOT LIKE 'sqlite_%' ORDER BY tbl_name, type DESC, name");
    return rs;
}

CCResultSet* CCDatabase::getTableSchema(string tableName) {
    // result columns: cid[INTEGER], name, type [STRING], notnull[INTEGER], dflt_value[], pk[INTEGER]
    CCResultSet* rs = executeQuery("PRAGMA table_info(%s)", tableName.c_str());
    return rs;
}

bool CCDatabase::columnExists(string tableName, string columnName) {
    bool existent = false;

    // lower case
	CCUtils::toLowercase(tableName);
	CCUtils::toLowercase(columnName);

    // get table schema
    CCResultSet* rs = getTableSchema(tableName);

    // check if column is present in table schema
    while(rs->next()) {
    	string column = rs->stringForColumn("name");
		CCUtils::toLowercase(column);
    	if(column == columnName) {
    		existent = true;
    		break;
    	}
    }

    return existent;
}

bool CCDatabase::executeSQL(string path, CC_DECRYPT_FUNC func) {
    unsigned long len;
    unsigned char* raw = CCResourceLoader::loadRaw(path, &len, func);
	bool ret = executeSQL(raw, len);
	free(raw);
	return ret;
}

bool CCDatabase::executeSQL(const void* data, size_t length) {
	// begin transaction
	if(!beginTransaction()) {
		CCLOGERROR("CCDatabase::executeSQL: failed to start transaction");
		return false;
	}

	// find every line and execute it
	char* tmp = (char*)data;
	int start = 0;
	for(int i = 0; i < length; i++) {
		if(isspace(tmp[i]) && i == start) {
			start++;
		} else if(tmp[i] == ';') {
			// make a temp c string
			tmp[i] = 0;

			// execute
			bool success = executeUpdate(tmp + start);

			// restore
			tmp[i] = ';';

			// if success, go to next start
			// if failed, abort
			if(success) {
				start = i + 1;
			} else {
				// commit
				if(!rollback()) {
					CCLOGERROR("CCDatabase::executeSQL: failed to rollback transaction");
				}

				return false;
			}
		}
	}

	// commit
	if(!commit()) {
		CCLOGERROR("CCDatabase::executeSQL: failed to commit transaction");
		return false;
	}

	return true;
}

NS_CC_END