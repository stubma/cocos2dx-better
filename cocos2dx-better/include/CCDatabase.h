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
#ifndef __CCDatabase_h__
#define __CCDatabase_h__

#include "cocos2d.h"
#include "CCResourceLoader.h"
#include <stdbool.h>
#include "CCStatement.h"
#include "CCResultSet.h"
#include "CCMoreMacros.h"

struct sqlite3;
using namespace std;

NS_CC_BEGIN

/**
 * CCDatabase is a sqlite3 C++ encapsulation. It is FMDB C++ version, and has similar
 * API with original FMDB
 *
 * \par what is FMDB
 * an Objective-C wrapper for sqlite3: https://github.com/ccgus/fmdb
 */
class CC_DLL CCDatabase : public CCObject {
	friend class CCResultSet;

private:
	/// true means compiled statement will be cached for later use
	bool m_shouldCacheStatements;

	/// cache of compiled statements
	typedef map<string, CCStatement*> StatementMap;
	StatementMap m_cachedStatements;

private:
	/// print in use warning
	void warnInUse();

	/// get cached statement
	CCStatement* getCachedStatement(const char* sql);

	/// cache statement
	void setCachedStatement(const char* sql, CCStatement* statement);

	/// execute a sql query statement, return result set if query is ok, or NULL if failed
	CCResultSet* _executeQuery(const char* sql);

	/// execute a sql non-query statement, return true if execution is ok
	bool _executeUpdate(const char* sql);

	/// invoked when result set is closed, called from CCResultSet
	void postResultSetClosed(CCResultSet* rs);

protected:
	/// constructor
	CCDatabase(string path);

public:
	virtual ~CCDatabase();

	/**
	 * create a database object
	 *
	 * @param platform-independent path of database file, will be mapped
	 * @return database object
	 */
	static CCDatabase* create(string path);

	/// is thread safe?
	static bool isThreadSafe();

	/// get sqlite version
	static string sqliteLibVersion();

	/// get sqlite handler
	sqlite3* sqliteHandle() { return m_db; }

	/**
	 * open database
	 *
	 * @param flags flag, only used for sqlite version larger than 3.5.0
	 * @return true means opening is ok
	 */
	bool open(int flags = 0);

	/**
	 * close database, can calling open method to open database again.
	 * close will be invoked in CCDatabase deconstructor so it is not mandatory
	 * to call it
	 *
	 * @return true means closing is ok
	 */
	bool close();

	/**
	 * check database is opened or not
	 *
	 * @return true means database is opened
	 */
	bool databaseOpened();

	/// verify database connection is ok
	bool goodConnection();

	/// clear cached statements
	void clearCachedStatements();

	/// execute a query
	CCResultSet* executeQuery(string sql, ...);

	/// execute update
	bool executeUpdate(string sql, ...);

	/// get error message of last operation, or empty string if no error
	string lastErrorMessage();

	/// get error code of last operation, or SQLITE_OK if no error
	int lastErrorCode();

	/// true means error occurs in last operation
	bool hadError();

	/// get row id of last insertion
	int64_t lastInsertRowId();

	/// true means statement should be cached
	bool shouldCacheStatements() { return m_shouldCacheStatements; }

	/// set flag to cache statement or not
	void setShouldCacheStatements(bool value);

	/// get row count which is affected by last operation
	int changes();

	/// rollback transaction, true means successful
	bool rollback();

	/// commit transaction, true means successful
	bool commit();

	/** 
	 * begin a deferred transaction, true means successful.
	 *
	 * \note
	 * The idea behind a deferred transaction is that all the changes to the database are 
	 * kept in the cache memory (client memory or server memory). When this transaction has 
	 * completed, all the database changes will be written to the database in one short 
	 * transaction
	 */
	bool beginDeferredTransaction();

	/// begin a transaction, true means successful
	bool beginTransaction();

	/// check a table is existent or not
	bool tableExists(string tableName);

	/// a helper method to quickly get integer result from a query
	int intForQuery(string sql, ...);

	/// a helper method to quickly get long result from a query
	long longForQuery(string sql, ...);

	/// a helper method to quickly get int64_t result from a query
	int64_t int64ForQuery(string sql, ...);

	/// a helper method to quickly get bool result from a query
	bool boolForQuery(string sql, ...);

	/// a helper method to quickly get double result from a query
	double doubleForQuery(string sql, ...);

	/// a helper method to quickly get string result from a query
	string stringForQuery(string sql, ...);

	/// a helper method to quickly get blob result from a query
	/// the data is copied so caller should release it
	const void* dataForQuery(string sql, size_t* outLen, ...);

	/// a helper method to quickly get blob result from a query
	/// the data is not copied so caller should NOT release it
	const void* dataNoCopyForQuery(string sql, size_t* outLen, ...);

	/// validate sql statement, return empty string if valid
	string validateSQL(string sql, ...);

	/// get database schema
	CCResultSet* getSchema();

	/// get table schema
	CCResultSet* getTableSchema(string tableName);

	/// check a column is existent or not
	bool columnExists(string tableName, string columnName);

	/**
	 * execute a sql file in a transaction
	 *
	 * @param path sql file absolute path, will be mapped to platform format
     * @param func you can provide a decrypt function if the sql file is encrypted
	 * @return true means execution is ok
	 */
	bool executeSQL(string path, CC_DECRYPT_FUNC func = NULL);

	/**
	 * execute a sql file in a transaction
	 *
	 * @param data raw data of sql file
	 * @param length data byte length
	 * @return true means execution is ok
	 */
	bool executeSQL(const void* data, size_t length);
	
	// get database version
	int getVersion();
	
	/**
	 * set database version. By default, database version is
	 * 1 when it is created
	 */
	void setVersion(int v);
	
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(string, m_databasePath, DatabasePath);
	CC_SYNTHESIZE_READONLY(sqlite3*, m_db, Sqlite3Handle);
	CC_SYNTHESIZE(int, m_busyRetryTimeout, BusyRetryTimeout);
	CC_SYNTHESIZE_READONLY_BOOL(m_inTransaction, InTransaction);
	CC_SYNTHESIZE_BOOL(m_inUse, InUse);
};

NS_CC_END

#endif // __CCDatabase_h__
