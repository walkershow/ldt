/********************************************************************
filename: 	SQLite.cpp
created:	2012-11-05
author:		firehood

purpose:	SQLite���ݿ������
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

// ��ѯ(�ص���ʽ)
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

// ��ѯ
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

// ��ʼ����
BOOL SQLite::BeginTransaction()
{
	char * errmsg = NULL;
	if(sqlite3_exec(m_db,"BEGIN TRANSACTION;",NULL,NULL,&errmsg) != SQLITE_OK)
	{
		return FALSE;
	} 
	return TRUE;
}

// �ύ����
BOOL SQLite::CommitTransaction()
{
	char * errmsg = NULL;
	if(sqlite3_exec(m_db,"COMMIT TRANSACTION;;",NULL,NULL,&errmsg) != SQLITE_OK)
	{
		return FALSE;
	} 
	return TRUE;
}

// �ع�����
BOOL SQLite::RollbackTransaction()
{
	char * errmsg = NULL;
	if(sqlite3_exec(m_db,"ROLLBACK  TRANSACTION;",NULL,NULL,&errmsg) != SQLITE_OK)
	{
		return FALSE;
	} 
	return TRUE;
}

// ��ȡ��һ��������Ϣ
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

// ��ȡһ������
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

// �ر�Reader����ȡ���������
void SQLiteDataReader::Close()
{
	if(m_pStmt)
	{
		sqlite3_finalize(m_pStmt);
		m_pStmt = NULL;
	}
}

// �ܵ�����
int SQLiteDataReader::ColumnCount(void)
{
	return sqlite3_column_count(m_pStmt);
}

// ��ȡĳ�е����� 
LPCTSTR SQLiteDataReader::GetName(int nCol)
{
#ifdef  UNICODE 
	return (LPCTSTR)sqlite3_column_name16(m_pStmt, nCol);
#else
	return (LPCTSTR)sqlite3_column_name(m_pStmt, nCol);
#endif
}

// ��ȡĳ�е���������
SQLITE_DATATYPE SQLiteDataReader::GetDataType(int nCol)
{
	return (SQLITE_DATATYPE)sqlite3_column_type(m_pStmt, nCol);
}

// ��ȡĳ�е�ֵ(�ַ���)
LPCTSTR SQLiteDataReader::GetStringValue(int nCol)
{
#ifdef  UNICODE 
	return (LPCTSTR)sqlite3_column_text16(m_pStmt, nCol);
#else
	return (LPCTSTR)sqlite3_column_text(m_pStmt, nCol);
#endif
}

// ��ȡĳ�е�ֵ(����)
int SQLiteDataReader::GetIntValue(int nCol)
{
	return sqlite3_column_int(m_pStmt, nCol);
}

// ��ȡĳ�е�ֵ(������)
long SQLiteDataReader::GetInt64Value(int nCol)
{
	return (long)sqlite3_column_int64(m_pStmt, nCol);
}

// ��ȡĳ�е�ֵ(������)
double SQLiteDataReader::GetFloatValue(int nCol)
{
	return sqlite3_column_double(m_pStmt, nCol);
}

// ��ȡĳ�е�ֵ(����������)
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

	// �򿪻򴴽����ݿ�
	//******************************************************
	if(!sqlite.Open(szDbPath))
	{
		_tprintf(_T("%s\n"),sqlite.GetLastErrorMsg());
		return;
	}
    //******************************************************

	// �������ݿ��
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

	// �������ݡ���ͨ��ʽ��
	DWORD dwBeginTick = GetTickCount();
	//******************************************************
	// ��һ���Բ��������¼ʱ�򣬲�������ķ�ʽ�����Ч��
	sqlite.BeginTransaction();
	// ������������
	for(int i=0;i<1000;i++)
	{
		memset(sql,0,sizeof(sql));
		_stprintf(sql,_T("insert into Book(name,author,catagory_id) values('�����%d','Ī��',1)"),i);
		if(!sqlite.ExcuteNonQuery(sql))
		{
			_tprintf(_T("%s\n"),sqlite.GetLastErrorMsg());
			break;
		}
	}
	// �ύ����
	sqlite.CommitTransaction();
	printf("Insert Data Take %dMS...\n",GetTickCount()-dwBeginTick);
    //******************************************************


	// �������ݡ�ͨ�������󶨵ķ�ʽ���ύ��������ʱ�����������ͨģʽЧ�ʸ��ߣ����Լ45%����ͬʱ��֧�ֲ�����������ݡ�
	dwBeginTick = GetTickCount();
    //******************************************************
	// ��һ���Բ��������¼ʱ�򣬲�������ķ�ʽ�����Ч��
	sqlite.BeginTransaction();
	memset(sql,0,sizeof(sql));
	_stprintf(sql,_T("insert into Book(name,author,catagory_id,image) values(?,'����',?,?)"));
	SQLiteCommand cmd(&sqlite,sql);
	// ������������
	for(int i=0;i<1000;i++)
	{
		TCHAR strValue[16] = {0};
		_stprintf(strValue,_T("���Ĺ�%d"),i);
		// �󶨵�һ��������name�ֶ�ֵ��
		cmd.BindParam(1,strValue);
		// �󶨵ڶ���������catagory_id�ֶ�ֵ��
		cmd.BindParam(2,20);
		BYTE imageBuf[] = {0xff,0xff,0xff,0xff};
		// �󶨵�����������image�ֶ�ֵ,���������ݣ�
		cmd.BindParam(3,imageBuf,sizeof(imageBuf));
		if(!sqlite.ExcuteNonQuery(&cmd))
		{
			_tprintf(_T("%s\n"),sqlite.GetLastErrorMsg());
			break;
		}
	}
	// ���cmd
	cmd.Clear();
	// �ύ����
	sqlite.CommitTransaction();
	printf("Insert Data Take %dMS...\n",GetTickCount()-dwBeginTick);
    //******************************************************

	// ��ѯ
	dwBeginTick = GetTickCount();
	//******************************************************
	memset(sql,0,sizeof(sql));
	_stprintf(sql,_T("%s"),_T("select * from Book where name = '���Ĺ�345'"));

	SQLiteDataReader Reader = sqlite.ExcuteQuery(sql);

	int index = 0;
	int len = 0;
	while(Reader.Read())
	{
		_tprintf( _T("***************����%d����¼��***************\n"),++index);
		_tprintf( _T("�ֶ���:%s �ֶ�ֵ:%d\n"),Reader.GetName(0),Reader.GetIntValue(0));
		_tprintf( _T("�ֶ���:%s �ֶ�ֵ:%s\n"),Reader.GetName(1),Reader.GetStringValue(1));
		_tprintf( _T("�ֶ���:%s �ֶ�ֵ:%s\n"),Reader.GetName(2),Reader.GetStringValue(2));
		_tprintf( _T("�ֶ���:%s �ֶ�ֵ:%d\n"),Reader.GetName(3),Reader.GetIntValue(3));
		_tprintf( _T("�ֶ���:%s �ֶ�ֵ:%s\n"),Reader.GetName(4),Reader.GetStringValue(4));
		// ��ȡͼƬ�������ļ�
		const BYTE *ImageBuf = Reader.GetBlobValue(6,len);
		_tprintf( _T("*******************************************\n"));
	}
	Reader.Close();
	printf("Query Take %dMS...\n",GetTickCount()-dwBeginTick);
    //******************************************************

	// �ر����ݿ�
	sqlite.Close();
}