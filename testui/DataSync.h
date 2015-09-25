#pragma once
#include "duilib.h"
#include<string>
class CDataSync
{
public:
	CDataSync(LPCTSTR server,int port, LPCTSTR userid);
	~CDataSync(void);

public:
	int SyncUserInfo();
	int GetProg_to_Game_ByProgmd5(CString progmd5);
	int GetProg_to_Game();
	int GetControlMode();
	int GetGame_Manage_ByGameID(int gameid);
	int UpdateLocalData();
	int GetUser_GameInfo();

private:
	bool GetData(const CString& sUrl, char* buf, DWORD dwBuffer);
	bool PostData(const CString& sUrl, char* data);
	CString CrackUrl(CString sUrl);
	CString DownloadFile(const CString& surl);
	//CString DownloadFile(const CString& surl, const std::string& url);
private:
	CString m_server;
	int m_port;
	CString m_userid;
	char *m_databuf;
	const int m_nbuflen;

};
