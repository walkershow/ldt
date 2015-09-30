#include "DataSync.h"
#include "GameManage.h"
#include "json.h"
#include <WinInet.h>
#include "helper.h"
#include <map>
#include <string>
using namespace std;
#pragma comment(lib, "WinInet.lib")

CDataSync::CDataSync(LPCTSTR server, int port, LPCTSTR userid):m_server(server),m_port(port),m_userid(userid),m_databuf(0),m_nbuflen(1024*60)
{
	m_databuf = new char[m_nbuflen];
	memset(m_databuf,0,m_nbuflen);
}

CDataSync::~CDataSync(void)
{
}

bool CDataSync::GetData(const CString& sUrl, char* szBuffer, DWORD dwBuffer)
{
	//CString data;
	bool bRet = false;

	HINTERNET hConnect = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, INTERNET_FLAG_ASYNC); 
	
	if (hConnect)
	{
		DWORD dwTimeOut = 3000;
		InternetSetOption(hConnect, INTERNET_OPTION_CONNECT_TIMEOUT, &dwTimeOut, sizeof(dwTimeOut));

		HINTERNET hSession = InternetOpenUrl(hConnect, sUrl, NULL, 0, INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_PRAGMA_NOCACHE, 0);
		if (hSession)
		{
			// 建立数据缓冲区
			DWORD dwRead = 0;
// 			DWORD dwBuffer = 1024 * 1024;
// 			char *szBuffer = new char[dwBuffer];
// 			memset(szBuffer, 0, dwBuffer);

			if(InternetReadFile(hSession, szBuffer, dwBuffer, &dwRead) && (dwRead > 0))
			{
				int nLen = dwRead;
				static char pBuffer[1024*60] = {0} ;
				memset(pBuffer, 0, nLen + 1);
				memcpy(pBuffer, szBuffer, nLen);
// 				if(pBuffer != NULL)
// 				{
// 					data = pBuffer;
// 				}
				bRet = true;
			}

			// 销毁数据缓冲区
			//	delete []szBuffer;
			//	szBuffer = NULL;

			InternetCloseHandle(hSession);
		}
		InternetCloseHandle(hConnect);
	}
	return bRet; 
	//return data;
}



bool CDataSync::PostData(const CString& sUrl, char* data)
{
return true;
}

CString CDataSync::CrackUrl(CString sUrl)
{
	URL_COMPONENTS uc;
	TCHAR Scheme[1000]={0};
	TCHAR HostName[1000]={0};
	TCHAR UserName[1000]={0};
	TCHAR Password[1000]={0};
	TCHAR UrlPath[1000]={0};
	TCHAR ExtraInfo[1000]={0};

	uc.dwStructSize = sizeof(uc);
	uc.lpszScheme = Scheme;
	uc.lpszHostName = HostName;
	uc.lpszUserName = UserName;
	uc.lpszPassword = Password;
	uc.lpszUrlPath = UrlPath;
	uc.lpszExtraInfo = ExtraInfo;

	uc.dwSchemeLength = 1000;
	uc.dwHostNameLength = 1000;
	uc.dwUserNameLength = 1000;
	uc.dwPasswordLength = 1000;
	uc.dwUrlPathLength = 1000;
	uc.dwExtraInfoLength = 1000;

	InternetCrackUrl(sUrl, 0, 0, &uc);
	return UrlPath;
}

CString CDataSync::DownloadFile(const CString& surl)
{
	CString sAppPath = CPaintManagerUI::GetInstancePath().GetData();
	CString sImageName = CrackUrl(surl);
	sImageName.Replace(_T("/"), _T("\\"));
	TCHAR szFile[MAX_PATH] = {0};
	lstrcpy(szFile, sAppPath);
	::PathAppend(szFile, sImageName);
// 	TCHAR szDir[MAX_PATH] = {0};
// 	lstrcpy(szDir, sAppPath);
// 	::PathAppend(szDir, _T("\\icon"));
	//后期继续优化，先判断目录是否存在 20150918
	if(!::PathFileExists(szFile))
	{
		bool bRet = CreateMultipleDirectory(szFile);
		//bool bRet = CreateDirectory(szDir, NULL);
		if(!bRet )
		{
			CString strError;
			strError.Format(_T("MakeSureDirectoryPathExists failed, code=%d"),GetLastError());
			return "";
		}

	}
// 	if(::PathFileExists(szFile))
// 	{
// 		BOOL bRet = DeleteFile(szFile);
// 		if(bRet == FALSE)
// 		{
// 			return "";
// 		}
// 	}

		 HRESULT hr = ::URLDownloadToFile(NULL, surl, szFile, 0, NULL);
		 if (hr != S_OK)
		 {
			 return "";
		 }
	return sImageName;

}


int CDataSync::SyncUserInfo()
{
	int id;
	CString name;
	CString iconpath;
	bool bRet = CGameManage::GetInstance().GetUserInfo(id, name, iconpath);
	if(!bRet || iconpath.IsEmpty())
	{
		CString sUrl;
		sUrl.Format(_T("http://%s:%d/user?id=%s"),m_server, m_port, m_userid);
		DWORD dwBuffer = 1024 * 60;
		static char szBuffer[1024*60] = {0};
		//char *szBuffer = new char[dwBuffer];
		memset(szBuffer, 0, dwBuffer);
		bRet = GetData(sUrl, szBuffer, dwBuffer);
		if(!bRet)
		{
			return -1 ;
		}
		//sImageName.Replace(_T("/"), _T("\\"))
		//CString str = UTF8ToUnicode(szBuffer);

		Json::Reader reader;
		Json::Value root;
		if (!reader.parse(szBuffer, root, false))
		{
			return -2;
		}
		std::string name;
		std::string iconpath;
		int id;
		int size = root.size();
		for (int i=0; i<size; ++i)
		{
			name = root[i]["name"].asString();
			CString str2 = UTF8ToUnicode((char*)name.c_str());
			id = root[i]["userid"].asInt();
			iconpath = root[i]["iconurl"].asString();
			CString fPath = DownloadFile(iconpath.c_str());
			//std::cout<<name<<" "<<id<<std::endl;
		}
	}
	return 1;
}
int CDataSync::GetControlMode()
{
	CString sUrl;
	memset(m_databuf, 0, m_nbuflen);

	sUrl.Format(_T("http://%s:%d/userctrlmode?userid=%s"),m_server, m_port, m_userid);
	bool bRet = GetData(sUrl, m_databuf, m_nbuflen);
	if(!bRet)
	{
		return -1 ;
	}
	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(m_databuf, root, false))
	{
		return -2;
	}
	int mode = 0;
	int size = root.size();
	for (int i=0; i<size; ++i)
	{
		mode= root[i]["man_auto"].asInt();
		if(CGameManage::GetInstance().GetControlMode() == -1)
		{
			CGameManage::GetInstance().InsertControlMode(mode);
		}
		else
		{
			CGameManage::GetInstance().UpdateControlMode(mode);
		}
		break;
	}

	return 1;
}

int CDataSync::GetProg_to_Game_ByProgmd5(CString progmd5)
{
	int gameid = 0;
	gameid = CGameManage::GetInstance().SelectProgToGame(progmd5);
	if( gameid == 0 )
	{
		
		CString sUrl;
		memset(m_databuf, 0, m_nbuflen);

		sUrl.Format(_T("http://%s:%d/progtogame?progmd5=%s&userid=%s"),m_server, m_port, progmd5, m_userid);
		bool bRet = GetData(sUrl, m_databuf, m_nbuflen);
		if(!bRet)
		{
			return -1 ;
		}
		Json::Reader reader;
		Json::Value root;
		if (!reader.parse(m_databuf, root, false))
		{
			return -2;
		}
		string name,iconurl;
		int topmost,playtimes,status,type;
		string upsign;
		int size = root.size();
		for (int i=0; i<size; ++i)
		{
			gameid= root[i]["gameid"].asInt();
// 			if(CGameManage::GetInstance().ExistGame(gameid))
// 			{
// 				return -3;
// 			}
			name= root[i]["name"].asString();
			CString strName = UTF8ToUnicode((char*)name.c_str());
			iconurl= root[i]["iconurl"].asString();
			CString strIconurl = UTF8ToUnicode((char*)iconurl.c_str());
			CString fPath = DownloadFile(strIconurl);
			if(fPath == "") 
			{
				return -4; //下载失败
			}
			upsign = root[i]["updatesign"].asString();
			long ups = atoi(upsign.c_str());
			topmost = 0; playtimes=0;
			//topmost= root[i]["topmost"].asInt();
			//playtimes = root[i]["playtimes"].asInt();
			//status = root[i]["status"].asInt();
			type = root[i]["type"].asInt();
			//游戏资料先入库，MD5后入，保证本地查询MD5时，游戏资料肯定存在（事务完整）
			bRet = CGameManage::GetInstance().UpdateGameByGameID(gameid, strName, fPath, topmost,  type, playtimes, ups,_T(""));
			if( !bRet ) return 0;
			bRet = CGameManage::GetInstance().UpdateProgToGame(gameid, progmd5);
			if( !bRet) return 0;
			break;
		}
	}
	else
	{
		//已存在
		return -3;
	}
	return gameid;
}


int CDataSync::GetProg_to_Game()
{
	CString sUrl;
	memset(m_databuf, 0, m_nbuflen);

	sUrl.Format(_T("http://%s:%d/prog_to_game?progmd5=%s"),m_server, m_port, _T("all"));
	bool bRet = GetData(sUrl, m_databuf, m_nbuflen);
	if(!bRet)
	{
		return -1 ;
	}
	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(m_databuf, root, false))
	{
		return -2;
	}
	int gameid = 0;
	string progmd5;
	int size = root.size();
	for (int i=0; i<size; ++i)
	{
		gameid= root[i]["gameid"].asInt();
		progmd5 = root[i]["progmd5"].asString();
	}
	return 1;
}
//gameid -1:all data
int CDataSync::GetGame_Manage_ByGameID(int gameid)
{
	CString sUrl;
	memset(m_databuf, 0, m_nbuflen);

	sUrl.Format(_T("http://%s:%d/gm?gameid=%d&userid=%s"),m_server, m_port, gameid, m_userid);
	bool bRet = GetData(sUrl, m_databuf, m_nbuflen);
	if(!bRet)
	{
		return -1 ;
	}
	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(m_databuf, root, false))
	{
		return -2;
	}
	string name,iconurl;
	int topmost, playtimes,type;
	long upsign;
	int size = root.size();
	int lgameid = 0;
	for (int i=0; i<size; ++i)
	{
		lgameid = root[i]["gameid"].asInt();
		name = root[i]["name"].asString();
		iconurl = root[i]["iconurl"].asString();
		topmost = root[i]["topmost"].asInt();
		playtimes = root[i]["playtimes"].asInt();
		type = root[i]["type"].asInt();
		upsign =root[i]["updatesign"].asUInt();
		CString icon_url_uni = UTF8ToUnicode((char*)iconurl.c_str());
		CString iconpath = DownloadFile(icon_url_uni);
		CString name_uni = UTF8ToUnicode((char*)name.c_str());
		CGameManage::GetInstance().UpdateGameByGameID(lgameid, name_uni, icon_url_uni, topmost, type, playtimes, upsign,_T(""));
	}
	return 1;
}

int CDataSync::GetUser_GameInfo()
{
	CString sUrl;
	memset(m_databuf, 0, m_nbuflen);
	long maxTime = CGameManage::GetInstance().SelectMaxTimeFromGameManage();
	sUrl.Format(_T("http://%s:%d/usergameinfo?userid=%s&curtime=%d"),m_server, m_port, m_userid, maxTime);
	bool bRet = GetData(sUrl, m_databuf, m_nbuflen);
	if(!bRet)
	{
		return -1 ;
	}
	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(m_databuf, root, false))
	{
		return -2;
	}
	int gameid = 0;
	string name,iconurl,progmd5;
	CString url;
	int topmost,playtimes,rstatus,type;
	unsigned int upsign;
	int size = root.size();
	for (int i=0; i<size; ++i)
	{
		gameid= root[i]["gameid"].asInt();
		name= root[i]["name"].asString();
		CString strName = UTF8ToUnicode((char*)name.c_str());
		iconurl= root[i]["iconurl"].asString();
		CString strIconurl = UTF8ToUnicode((char*)iconurl.c_str());
		CString fPath = DownloadFile(strIconurl);

		topmost = 0; playtimes=0;
		//topmost= root[i]["topmost"].asInt();
		//playtimes = root[i]["playtimes"].asInt();
		rstatus = root[i]["status"].asInt();
		type = root[i]["type"].asInt();
		upsign = root[i]["updatesign"].asUInt();
		url = root[i]["url"].asCString();
		//progmd5 = root[i]["progmd5"].asString();
		bRet = CGameManage::GetInstance().UpdateGameByGameID(gameid, strName, fPath, topmost,  type, playtimes, upsign,url,rstatus);
		if(type !=2)
		{
			memset(m_databuf, 0, m_nbuflen);
			sUrl.Empty();
			sUrl.Format(_T("http://%s:%d/getprogmd5?gameid=%d"),m_server, m_port,gameid);
			bool bRet = GetData(sUrl, m_databuf, m_nbuflen);
			if(!bRet)
			{
				return -1 ;
			}
			Json::Reader reader;
			Json::Value root;
			if (!reader.parse(m_databuf, root, false))
			{
				return -2;
			}
			string progmd5;
			int size = root.size();
			for (int i=0; i<size; ++i)
			{
				progmd5 = root[i]["progmd5"].asString();
				bRet = CGameManage::GetInstance().InsertProgmd5byGameid(gameid, progmd5.c_str());

			}

		}
		//break;
	}
	return gameid;
}


int CDataSync::UpdateLocalData()
{
	CString sUrl;
	memset(m_databuf, 0, m_nbuflen);

	sUrl.Format(_T("http://%s:%d/dstatus?userid=%s"),m_server, m_port, m_userid);
	bool bRet = GetData(sUrl, m_databuf, m_nbuflen);
	if(!bRet)
	{
		return -1 ;
	}
	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(m_databuf, root, false))
	{
		return -2;
	}
	int type;
	int timestamp;
	map<int,int> m_dmap;
	int size = root.size();
	for (int i=0; i<size; ++i)
	{
		type = root[i]["datatype"].asInt();
		timestamp = root[i]["timestamp"].asInt();
		m_dmap[type] = timestamp;
	}

	SQLiteDataReader sdr = CGameManage::GetInstance().GetDataStatus();

	while( sdr.Read() )
	{
		type = sdr.GetIntValue(0);
		timestamp = sdr.GetIntValue(1);
		if( m_dmap[type] > timestamp)
		{
			
		}
	}
}