#pragma once
#include "sqlite.h"
#include <atlstr.h>

class CGameManage
{
public:
	static CGameManage & GetInstance()  
	{  
		static CGameManage instance;   //局部静态变量  
		return instance;  
	} 
	// 打开数据库
	BOOL Open(LPCTSTR lpDbFlie){return m_db.Open(lpDbFlie);}
	// 关闭数据库
	void Close(){m_db.Close();}

	// 执行非查询操作（更新或删除）
	BOOL ExcuteNonQuery(LPCTSTR lpSql){return m_db.ExcuteNonQuery(lpSql);}
	BOOL ExcuteNonQuery(SQLiteCommand* pCmd){return m_db.ExcuteNonQuery(pCmd);}

	// 查询
	SQLiteDataReader ExcuteQuery(LPCTSTR lpSql){return m_db.ExcuteQuery(lpSql);}
	LPCTSTR GetLastErrorMsg(){return m_db.GetLastErrorMsg();}
	//0:auto 1:manual
	int GetControlMode()
	{
		SQLiteDataReader sdr = m_db.ExcuteQuery(_T("select man_auto from use_ctrlmode;"));
		bool bRet = sdr.Read();
		if(bRet)
			return sdr.GetIntValue(0);
		return -1;
	}
	SQLiteDataReader GetAllGameInMode()
	{
		int m = GetControlMode();
		switch(m)
		{
		case 1:
			return GetAllGameByAutoMode();
		case 2:
			return GetAllGameByManualMode();
		case 3:
			return GetAllGameByWWGMode();
		default:
			return GetAllGameByAutoMode();
		}
	}
	SQLiteDataReader GetAllGameByWWGMode()
	{
		return m_db.ExcuteQuery(_T("select  a.gameid,name,iconpath,topmost,playtimes,type from game_manage a left OUTER JOIN  game_path b on a.gameid=b.gameid order by path,lastplaytime"));

	}
	SQLiteDataReader GetAllGameByAutoMode()
	{
		return m_db.ExcuteQuery(_T("select gameid,name,iconpath,topmost,playtimes,type from game_manage where status>=0 and rstatus=-1 order by lastplaytime asc ;"));
	}

	SQLiteDataReader GetAllGameByManualMode()
	{
		return m_db.ExcuteQuery(_T("select gameid,name,iconpath,topmost,playtimes,type from game_manage where topmost=1 and status>=0 and rstatus=-1 order by topmost,lastplaytime asc;"));
	}
	SQLiteDataReader GetAllGame()
	{
		return m_db.ExcuteQuery(_T("select gameid,name,iconpath,topmost,playtimes,type from game_manage where status>=0 and rstatus=-1 order by lastplaytime asc;"));
	}
	//local update status 0: 1:修改 -1：删除
	bool UpdateGameTopmost(int gameid, int topmost=1)
	{
		CString str;
		str.Format(_T("update game_manage set topmost=%d,status=1 where gameid=%d"), topmost, gameid);
		return m_db.ExcuteNonQuery(str);
	}
	//local update
	bool UpdateGamePlayTimes(int gameid)
	{
		CString str;
		str.Format(_T("update game_manage set playtimes=playtimes+1,status=1 where gameid=%d"), gameid);
		return m_db.ExcuteNonQuery(str);
	}
	//updated from server
	long SelectMaxTimeFromGameManage()
	{
		CString sql = _T("select max(updatesign) from game_manage");
		SQLiteDataReader sdr = m_db.ExcuteQuery(sql);
		bool bRet = sdr.Read();
		if(bRet)
			return sdr.GetInt64Value(0);
		return -1;
	}
	bool ExistGame(int gameid)
	{
		CString sql;
		sql.Format(_T("select 1 from game_manage where gameid=%d;"), gameid);
		SQLiteDataReader sdr = m_db.ExcuteQuery(sql);
		return sdr.Read();

	}
#include <stdio.h>
#include <stdlib.h>
	bool UpdateGameByGameID(int gameid, LPCTSTR name, LPCTSTR iconpath, int topmost, int type, int playtimes, long updatesign, LPCTSTR url ,int rstatus=-1)
	{
		if(ExistGame(gameid))
		{
			CString str;
			str.Format(_T("update game_manage set name='%s', iconpath='%s', topmost=%d ,type=%d, playtimes=%d,status=0, lastplaytime=CURRENT_TIMESTAMP,rstatus=%d,updatesign=%d,url='%s' where gameid=%d"), name, iconpath, topmost, type, playtimes, rstatus, updatesign, url,gameid );
			return m_db.ExcuteNonQuery(str);
		}
		else
		{
			CString str;
			str.Format(_T("insert into game_manage(gameid,name,iconpath,topmost,playtimes,type,status,lastplaytime,updatesign,url, rstatus) values(%d, '%s', '%s', %d, %d, %d, %d, CURRENT_TIMESTAMP,%d, '%s', %d)"),  gameid,name, iconpath, topmost,  playtimes, type, 0,updatesign, url, rstatus);
			return m_db.ExcuteNonQuery(str);
		}
		return true;
	}

	//1:auto 2:manual 0:none
	bool UpdateControlMode(int mode)
	{
		CString str;
		str.Format(_T("%s%d"), _T("update use_ctrlmode set man_auto="), mode);
		return m_db.ExcuteNonQuery(str);
	}

	bool InsertControlMode(int mode)
	{
		CString str;
		str.Format(_T("insert into use_ctrlmode(man_auto) values(%d)"), mode);
		return m_db.ExcuteNonQuery(str);
	}
	
	//更新为删除状态，待同步到服务器后再删除
	bool DelGame(int gameid)
	{
		CString str;
		str.Format(_T("update game_manage set status=-1 where gameid=%d"), gameid);
		return m_db.ExcuteNonQuery(str);
	}
	bool ExistGamePath(int gameid)
	{
		CString sql;
		sql.Format(_T("select 1 from game_path where gameid=%d;"), gameid);
		SQLiteDataReader sdr = m_db.ExcuteQuery(sql);
		bool bRet = sdr.Read();
		return bRet;
	}
	bool SetGamePath(const CString& path, int gameid)
	{
		CString sql;
		sql.Format(_T("select 1 from game_path where gameid=%d;"), gameid);
		SQLiteDataReader sdr = m_db.ExcuteQuery(sql);
		bool bRet = sdr.Read();

		if(!bRet)
		{
			sql.Format(_T("insert into game_path(gameid,path) values(%d,'%s')"), gameid, path);
			return m_db.ExcuteNonQuery(sql);
		}
		else
		{
			sql.Format(_T("update game_path set path='%s' where gameid=%d"), path, gameid);
			return m_db.ExcuteNonQuery(sql);
		}
		return false;

	}
	//用户信息
	bool SetUser(int userid,CString useracct,CString bt,CString sex,int y,int m,int d,CString country,CString prov,CString city,CString nickName, int countryindex, int provindex, int cityindex, int btindex, int sexindex)
	{
		CString sql;
		sql.Format(_T("select 1 from user where userid=%d;"), userid);
		SQLiteDataReader sdr = m_db.ExcuteQuery(sql);
		bool bRet = sdr.Read();

		if(!bRet)
		{
			sql.Format(_T("insert into user(userid,useracct,bloodtype,sex,birthyear,birthmon,birthday,country,prov,city,nickname,countryindex, provindex, cityindex, sexindex, btindex) values(%d,'%s','%s','%s',%d,%d,%d,'%s','%s','%s','%s',%d,%d,%d,%d,%d)"),userid, useracct,bt, sex,y,m,d,country,prov,city, nickName,countryindex, provindex, cityindex, sexindex, btindex);
			return m_db.ExcuteNonQuery(sql);
		}
		else
		{
			sql.Format(_T("update user set bloodtype='%s',sex='%s',birthyear=%d,birthmon=%d,birthday=%d,country='%s',prov='%s',city='%s',nickname='%s',countryindex=%d, provindex=%d, cityindex=%d, sexindex=%d, btindex=%d where userid=%d"), bt, sex,y,m,d,country,prov,city, nickName,countryindex, provindex, cityindex, sexindex, btindex, userid);
			return m_db.ExcuteNonQuery(sql);
		}
		return false;

	}

	SQLiteDataReader GetUser(int userid)
	{
		CString str;
		str.Format(_T("select userid,useracct,bloodtype,sex,birthyear,birthmon,birthday,country,prov,city,nickname,countryindex, provindex, cityindex, sexindex, btindex from user where userid=%d"),userid);
		return m_db.ExcuteQuery(str);
	}

	CString GetGamePath(int gameid)
	{
		CString str;
		str.Format(_T("select path from game_path where gameid=%d"), gameid);
		SQLiteDataReader sdr = m_db.ExcuteQuery(str);
		CString path = "";
		while(sdr.Read())
		{
			path = sdr.GetStringValue(0);
		}
		return path;

	}
	CString GetGameUrl(int gameid)
	{
		CString str;
		str.Format(_T("select url from game_manage where gameid=%d"), gameid);
		SQLiteDataReader sdr = m_db.ExcuteQuery(str);
		CString url = "";
		while(sdr.Read())
		{
			url = sdr.GetStringValue(0);
		}
		return url;

	}
	bool GetUserInfo(int& id, CString& name, CString& iconpath)
	{
		SQLiteDataReader sdr = m_db.ExcuteQuery(_T("select userid,name,iconurl from user_info;"));
		bool bRet = sdr.Read();
		
		if(bRet)
		{
			id = sdr.GetIntValue(0);
			name = sdr.GetStringValue(1);
			iconpath = sdr.GetStringValue(2);
		}

		return bRet;
	}

	SQLiteDataReader GetDataStatus()
	{
		return m_db.ExcuteQuery(_T("select datatable, modifytime from datastatus ;"));
	}
	
	int SelectProgToGame(CString progmd5)
	{
		CString sql;
		sql.Format(_T("select gameid from prog_to_game where progmd5='%s';"), progmd5);
		SQLiteDataReader sdr = m_db.ExcuteQuery(sql);
		bool bRet = sdr.Read();
		int gameid = 0;
		if(bRet)
		{
			gameid = sdr.GetIntValue(0);
		}
		return gameid;
	}

	bool UpdateProgToGame(int gameid, CString Progmd5)
	{
		if(!SelectProgToGame(Progmd5))
		{
			CString str;
			str.Format( _T("insert into prog_to_game(progmd5,gameid) values('%s',%d)"), Progmd5, gameid);
			return m_db.ExcuteNonQuery(str);
		}
		else
		{
			CString str;
			str.Format( _T("update prog_to_game set gameid=%d where progmd5='%s'"),  gameid, Progmd5);
			return m_db.ExcuteNonQuery(str);
		}
		return false;
	}
	bool InsertProgmd5byGameid(int gameid, CString Progmd5)
	{
		CString str;
		str.Format( _T("insert into prog_to_game(progmd5,gameid) values('%s',%d)"), Progmd5, gameid);
		return m_db.ExcuteNonQuery(str);
	}

private:
	CGameManage()   //构造函数是私有的  
	{  
	}  
	CGameManage(const CGameManage &);  
	CGameManage & operator = (const CGameManage &);   
	SQLite m_db;
};
