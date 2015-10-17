/********************************************************************
filename: 	SQLite.cpp
created:	2012-11-05
author:		firehood

purpose:	SQLite数据库操作类
*********************************************************************/
#include "SQLite.h"
#include <stdio.h>
const char* WcharToUtf8(const wchar_t *pwStr)
{
	if (pwStr == NULL)
	{
		return NULL;
	}

	int len = WideCharToMultiByte(CP_UTF8, 0, pwStr, -1, NULL, 0, NULL, NULL);
	if (len <= 0)
	{
		return NULL;
	}
	char *pStr = new char[len];
	WideCharToMultiByte(CP_UTF8, 0, pwStr, -1, pStr, len, NULL, NULL);
	return pStr;
}

const wchar_t* Utf8ToWchar(const char *pStr)
{
	if (pStr == NULL)
	{
		return NULL;
	}

	int len = MultiByteToWideChar(CP_UTF8, 0, pStr, -1, NULL, 0);
	if (len <= 0)
	{
		return NULL;
	}
	wchar_t *pwStr = new wchar_t[len];
	MultiByteToWideChar(CP_UTF8, 0, pStr, -1, pwStr, len);
	return pwStr;
}

SQLite::SQLite(void):
m_db(NULL)
{
}

SQLite::~SQLite(void)
{
	Close();
}

BOOL SQLite::Open(LPCTSTR lpDbFlie)
{
	if(lpDbFlie == NULL)
	{
		return FALSE;
	}
#ifdef  UNICODE 
	if(sqlite3_open16(lpDbFlie,&m_db) != SQLITE_OK)
#else
	if(sqlite3_open(lpDbFlie,&m_db) != SQLITE_OK)
#endif
	{
		return FALSE;
	}
	return TRUE;
}

BOOL SQLite::SetPassword(LPCTSTR pwd,int len)
{
	BOOL bRet = sqlite3_key(m_db, pwd, len);
	return bRet;
}

BOOL SQLite::ResetPassword(LPCTSTR pwd,int len)
{
	BOOL bRet = sqlite3_rekey(m_db, pwd, len);
	return bRet;
}
void SQLite::Close()
{
	if(m_db)
	{
		sqlite3_close(m_db);
		m_db = NULL;
	}
}

BOOL SQLite::ExcuteNonQuery(LPCTSTR lpSql)
{
	if(lpSql == NULL)
	{
		return FALSE;
	}
	sqlite3_stmt* stmt;  
#ifdef  UNICODE 
	if(sqlite3_prepare16_v2(m_db, lpSql, -1, &stmt, NULL) != SQLITE_OK)
#else
	if(sqlite3_prepare_v2(m_db, lpSql, -1, &stmt, NULL) != SQLITE_OK)
#endif
	{ 
		return FALSE;  
	}  
	sqlite3_step(stmt);

	return (sqlite3_finalize(stmt) == SQLITE_OK) ? TRUE : FALSE ;
}

BOOL SQLite::ExcuteNonQuery(SQLiteCommand* pCmd)
{
	if(pCmd == NULL)
	{
		return FALSE;
	}
	return pCmd->Excute();
}

// 查询(回调方式)
BOOL SQLite::ExcuteQuery(LPCTSTR lpSql,QueryCallback pCallBack)
{
	if(lpSql == NULL || pCallBack == NULL)
	{
		return FALSE;
	}
	char *errmsg = NULL;
#ifdef  UNICODE 
	const char *szSql = WcharToUtf8(lpSql);
	if(sqlite3_exec(m_db, szSql, pCallBack, NULL, &errmsg) != SQLITE_OK)
	{
		delete[] szSql;
		return FALSE;
	} 
	delete[] szSql;
#else
	if(sqlite3_exec(m_db, lpSql, pCallBack, NULL, &errmsg) != SQLITE_OK)
	{
		return FALSE;
	} 
#endif
	return TRUE;
}

// 查询
SQLiteDataReader SQLite::ExcuteQuery(LPCTSTR lpSql)
{
	if(lpSql == NULL)
	{
		return FALSE;
	}
	sqlite3_stmt* stmt; 
#ifdef  UNICODE 
	if(sqlite3_prepare16_v2(m_db, lpSql, -1, &stmt, NULL) != SQLITE_OK)
#else
	if(sqlite3_prepare_v2(m_db, lpSql, -1, &stmt, NULL) != SQLITE_OK)
#endif
	{ 
		return FALSE;  
	}  
	return SQLiteDataReader(stmt);
}

// 开始事务
BOOL SQLite::BeginTransaction()
{
	char * errmsg = NULL;
	if(sqlite3_exec(m_db,"BEGIN TRANSACTION;",NULL,NULL,&errmsg) != SQLITE_OK)
	{
		return FALSE;
	} 
	return TRUE;
}

// 提交事务
BOOL SQLite::CommitTransaction()
{
	char * errmsg = NULL;
	if(sqlite3_exec(m_db,"COMMIT TRANSACTION;;",NULL,NULL,&errmsg) != SQLITE_OK)
	{
		return FALSE;
	} 
	return TRUE;
}

// 回滚事务
BOOL SQLite::RollbackTransaction()
{
	char * errmsg = NULL;
	if(sqlite3_exec(m_db,"ROLLBACK  TRANSACTION;",NULL,NULL,&errmsg) != SQLITE_OK)
	{
		return FALSE;
	} 
	return TRUE;
}

// 获取上一条错误信息
LPCTSTR SQLite::GetLastErrorMsg()
{
#ifdef UNICODE 
	return (LPCTSTR)sqlite3_errmsg16(m_db);
#else
	return sqlite3_errmsg(m_db);
#endif
}



SQLiteDataReader::SQLiteDataReader(sqlite3_stmt *pStmt):
m_pStmt(pStmt)
{

}

SQLiteDataReader::~SQLiteDataReader()
{
	Close();
}

// 读取一行数据
BOOL SQLiteDataReader::Read()
{
	if(m_pStmt == NULL)
	{
		return FALSE;
	}
	if(sqlite3_step(m_pStmt) != SQLITE_ROW)
	{
		return FALSE;
	}
	return TRUE;
}

// 关闭Reader，读取结束后调用
void SQLiteDataReader::Close()
{
	if(m_pStmt)
	{
		sqlite3_finalize(m_pStmt);
		m_pStmt = NULL;
	}
}

// 总的列数
int SQLiteDataReader::ColumnCount(void)
{
	return sqlite3_column_count(m_pStmt);
}

// 获取某列的名称 
LPCTSTR SQLiteDataReader::GetName(int nCol)
{
#ifdef  UNICODE 
	return (LPCTSTR)sqlite3_column_name16(m_pStmt, nCol);
#else
	return (LPCTSTR)sqlite3_column_name(m_pStmt, nCol);
#endif
}

// 获取某列的数据类型
SQLITE_DATATYPE SQLiteDataReader::GetDataType(int nCol)
{
	return (SQLITE_DATATYPE)sqlite3_column_type(m_pStmt, nCol);
}

// 获取某列的值(字符串)
LPCTSTR SQLiteDataReader::GetStringValue(int nCol)
{
#ifdef  UNICODE 
	return (LPCTSTR)sqlite3_column_text16(m_pStmt, nCol);
#else
	return (LPCTSTR)sqlite3_column_text(m_pStmt, nCol);
#endif
}

// 获取某列的值(整形)
int SQLiteDataReader::GetIntValue(int nCol)
{
	return sqlite3_column_int(m_pStmt, nCol);
}

// 获取某列的值(长整形)
long SQLiteDataReader::GetInt64Value(int nCol)
{
	return (long)sqlite3_column_int64(m_pStmt, nCol);
}

// 获取某列的值(浮点形)
double SQLiteDataReader::GetFloatValue(int nCol)
{
	return sqlite3_column_double(m_pStmt, nCol);
}

// 获取某列的值(二进制数据)
const BYTE* SQLiteDataReader::GetBlobValue(int nCol, int &nLen)
{
	nLen = sqlite3_column_bytes(m_pStmt, nCol);
	return (const BYTE*)sqlite3_column_blob(m_pStmt, nCol);
}

SQLiteCommand::SQLiteCommand(SQLite* pSqlite):
m_pSqlite(pSqlite),
m_pStmt(NULL)
{
}

SQLiteCommand::SQLiteCommand(SQLite* pSqlite,LPCTSTR lpSql):
m_pSqlite(pSqlite),
m_pStmt(NULL)
{
	SetCommandText(lpSql);
}

SQLiteCommand::~SQLiteCommand()
{

}

BOOL SQLiteCommand::SetCommandText(LPCTSTR lpSql)
{
#ifdef  UNICODE 
	if(sqlite3_prepare16_v2(m_pSqlite->m_db, lpSql, -1, &m_pStmt, NULL) != SQLITE_OK)
#else
	if(sqlite3_prepare_v2(m_pSqlite->m_db, lpSql, -1, &m_pStmt, NULL) != SQLITE_OK)
#endif
	{ 
		return FALSE;  
	}  
	return TRUE;
}

BOOL SQLiteCommand::BindParam(int index, LPCTSTR szValue)
{
#ifdef  UNICODE 
	if(sqlite3_bind_text16(m_pStmt, index, szValue, -1, SQLITE_TRANSIENT) != SQLITE_OK)
#else
	if(sqlite3_bind_text(m_pStmt, index, szValue,-1, SQLITE_TRANSIENT) != SQLITE_OK)
#endif
	{
		return FALSE;
	}
	return TRUE;
}

BOOL SQLiteCommand::BindParam(int index, const int nValue)
{
	if(sqlite3_bind_int(m_pStmt, index, nValue) != SQLITE_OK)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL SQLiteCommand::BindParam(int index, const double dValue)
{
	if(sqlite3_bind_double(m_pStmt, index, dValue) != SQLITE_OK)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL SQLiteCommand::BindParam(int index, const unsigned char* blobBuf, int nLen)
{
	if(sqlite3_bind_blob(m_pStmt, index, blobBuf,nLen,NULL) != SQLITE_OK)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL SQLiteCommand::Excute()
{
	sqlite3_step(m_pStmt);

	return (sqlite3_reset(m_pStmt) == SQLITE_OK) ? TRUE : FALSE ;
}

void SQLiteCommand::Clear()
{
	if(m_pStmt)
	{
		sqlite3_finalize(m_pStmt);
	}
}


void SqliteOperate()
{
	TCHAR *szDbPath = _T("Book.db");

	::DeleteFile(szDbPath);

	SQLite sqlite;

	// 打开或创建数据库
	//******************************************************
	if(!sqlite.Open(szDbPath))
	{
		_tprintf(_T("%s\n"),sqlite.GetLastErrorMsg());
		return;
	}
    //******************************************************

	// 创建数据库表
    //******************************************************
	TCHAR sql[512] = {0};
	_stprintf(sql,_T("%s"),
		_T("CREATE TABLE [Book] (")
		_T("[id] INTEGER NOT NULL PRIMARY KEY, ")
		_T("[name] NVARCHAR(20), ")
		_T("[author] NVARCHAR(20), ")
		_T("[catagory_id] INTEGER REFERENCES [Category]([id]), ")
		_T("[abstruct] NVARCHAR(100) ,")
		_T("[path] NVARCHAR(50), ")
		_T("[image] BLOB);")
		_T("CREATE INDEX [Book_id] ON [Book] ([id]);")
		);
	if(!sqlite.ExcuteNonQuery(sql))
	{
		printf("Create database table failed...\n");
	}
	//******************************************************

	// 插入数据【普通方式】
	DWORD dwBeginTick = GetTickCount();
	//******************************************************
	// 当一次性插入多条记录时候，采用事务的方式，提高效率
	sqlite.BeginTransaction();
	// 批量插入数据
	for(int i=0;i<1000;i++)
	{
		memset(sql,0,sizeof(sql));
		_stprintf(sql,_T("insert into Book(name,author,catagory_id) values('红高粱%d','莫言',1)"),i);
		if(!sqlite.ExcuteNonQuery(sql))
		{
			_tprintf(_T("%s\n"),sqlite.GetLastErrorMsg());
			break;
		}
	}
	// 提交事务
	sqlite.CommitTransaction();
	printf("Insert Data Take %dMS...\n",GetTickCount()-dwBeginTick);
    //******************************************************


	// 插入数据【通过参数绑定的方式，提交批量数据时，比上面的普通模式效率更高（提高约45%），同时可支持插入二进制数据】
	dwBeginTick = GetTickCount();
    //******************************************************
	// 当一次性插入多条记录时候，采用事务的方式，提高效率
	sqlite.BeginTransaction();
	memset(sql,0,sizeof(sql));
	_stprintf(sql,_T("insert into Book(name,author,catagory_id,image) values(?,'韩寒',?,?)"));
	SQLiteCommand cmd(&sqlite,sql);
	// 批量插入数据
	for(int i=0;i<1000;i++)
	{
		TCHAR strValue[16] = {0};
		_stprintf(strValue,_T("他的国%d"),i);
		// 绑定第一个参数（name字段值）
		cmd.BindParam(1,strValue);
		// 绑定第二个参数（catagory_id字段值）
		cmd.BindParam(2,20);
		BYTE imageBuf[] = {0xff,0xff,0xff,0xff};
		// 绑定第三个参数（image字段值,二进制数据）
		cmd.BindParam(3,imageBuf,sizeof(imageBuf));
		if(!sqlite.ExcuteNonQuery(&cmd))
		{
			_tprintf(_T("%s\n"),sqlite.GetLastErrorMsg());
			break;
		}
	}
	// 清空cmd
	cmd.Clear();
	// 提交事务
	sqlite.CommitTransaction();
	printf("Insert Data Take %dMS...\n",GetTickCount()-dwBeginTick);
    //******************************************************

	// 查询
	dwBeginTick = GetTickCount();
	//******************************************************
	memset(sql,0,sizeof(sql));
	_stprintf(sql,_T("%s"),_T("select * from Book where name = '他的国345'"));

	SQLiteDataReader Reader = sqlite.ExcuteQuery(sql);

	int index = 0;
	int len = 0;
	while(Reader.Read())
	{
		_tprintf( _T("***************【第%d条记录】***************\n"),++index);
		_tprintf( _T("字段名:%s 字段值:%d\n"),Reader.GetName(0),Reader.GetIntValue(0));
		_tprintf( _T("字段名:%s 字段值:%s\n"),Reader.GetName(1),Reader.GetStringValue(1));
		_tprintf( _T("字段名:%s 字段值:%s\n"),Reader.GetName(2),Reader.GetStringValue(2));
		_tprintf( _T("字段名:%s 字段值:%d\n"),Reader.GetName(3),Reader.GetIntValue(3));
		_tprintf( _T("字段名:%s 字段值:%s\n"),Reader.GetName(4),Reader.GetStringValue(4));
		// 读取图片二进制文件
		const BYTE *ImageBuf = Reader.GetBlobValue(6,len);
		_tprintf( _T("*******************************************\n"));
	}
	Reader.Close();
	printf("Query Take %dMS...\n",GetTickCount()-dwBeginTick);
    //******************************************************

	// 关闭数据库
	sqlite.Close();
}