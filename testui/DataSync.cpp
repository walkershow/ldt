#include "DataSync.h"
#include "GameManage.h"
#include "json.h"
#include "helper.h"
#include <map>
#include <string>
using namespace std;
#pragma comment(lib, "WinInet.lib")



CDataSync::CDataSync(LPCTSTR server, int port, LPCTSTR userid, HWND notifyHwnd):m_server(server),m_port(port),m_userid(userid),m_HwndNotify(notifyHwnd)
{


}

CDataSync::~CDataSync(void)
{

}

bool CDataSync::GetData(const CString& sUrl)
{
	HTTP_REQUEST_HEADER   h (HTTP_REQUEST_HEADER::VERB_TYPE_GET) ;
	h.m_url = sUrl;
	this->AddRequest(h);
	return true;
	//return data;
}


bool CDataSync::PostData(const CString& sUrl, char* data, int datalen)
{
	HTTP_REQUEST_HEADER   h (HTTP_REQUEST_HEADER::VERB_TYPE_POST) ;
	
	h.m_url = sUrl;
	h.m_header += _T("Content-Type:application/json\r\n") ;
	h.m_header += _T("Accept: */*\r\n") ;
	h.AddPostData(data, datalen);
	AddRequest(h);

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

// int CDataSync::GetControlMode()
// {
// 	CString sUrl;
// 	memset(m_databuf, 0, m_nbuflen);
// 
// 	sUrl.Format(_T("http://%s:%d/userctrlmode?userid=%s"),m_server, m_port, m_userid);
// 	bool bRet = GetData(sUrl, m_databuf, m_nbuflen);
// 	if(!bRet)
// 	{
// 		return -1 ;
// 	}
// 	Json::Reader reader;
// 	Json::Value root;
// 	if (!reader.parse(m_databuf, root, false))
// 	{
// 		return -2;
// 	}
// 	int mode = 0;
// 	int size = root.size();
// 	for (int i=0; i<size; ++i)
// 	{
// 		mode= root[i]["man_auto"].asInt();
// 		if(CGameManage::GetInstance().GetControlMode() == -1)
// 		{
// 			CGameManage::GetInstance().InsertControlMode(mode);
// 		}
// 		else
// 		{
// 			CGameManage::GetInstance().UpdateControlMode(mode);
// 		}
// 		break;
// 	}
// 
// 	return 1;
// }

//增加游戏
int CDataSync::GetProg_to_Game_ByProgmd5(CString progmd5)
{
	int gameid = 0;
	gameid = CGameManage::GetInstance().SelectProgToGame(progmd5);
	if( gameid == 0 )
	{
		CString sUrl;
		sUrl.Format(_T("http://%s:%d/progtogame?progmd5=%s&userid=%s"),m_server, m_port, progmd5, m_userid);
		GetData(sUrl);

	}
	else
	{
		//已存在
		return -3;
	}
	return gameid;
}
//增加游戏服务端会根据用户所传信息添加到用户喜欢游戏列表，然后服务器返回游戏数据，客户端在存到本地数据库
int CDataSync::HandleProg_to_Game_ByProgmd5(const string& data)
{
	bool bRet = true;
	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(data.c_str(), root, false))
	{
		return -2;
	}
	string name,iconurl;
	int topmost,playtimes,type;
	string upsign;
	CString progmd5;
	int size = root.size();
	for (int i=0; i<size; ++i)
	{
		int gameid= root[i]["gameid"].asInt();

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
		progmd5 = root[i]["progmd5"].asCString();
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

	return 1;
}

// int CDataSync::GetProg_to_Game()
// {
// 	CString sUrl;
// 
// 	sUrl.Format(_T("http://%s:%d/prog_to_game?progmd5=%s"),m_server, m_port, _T("all"));
// 	GetData(sUrl);
// 
// 	
// 	
// 	Json::Reader reader;
// 	Json::Value root;
// 	if (!reader.parse(m_databuf, root, false))
// 	{
// 		return -2;
// 	}
// 	int gameid = 0;
// 	string progmd5;
// 	int size = root.size();
// 	for (int i=0; i<size; ++i)
// 	{
// 		gameid= root[i]["gameid"].asInt();
// 		progmd5 = root[i]["progmd5"].asString();
// 	}
// 	return 1;
// }

//gameid -1:all data
// int CDataSync::GetGame_Manage_ByGameID(int gameid)
// {
// 	CString sUrl;
// 	memset(m_databuf, 0, m_nbuflen);
// 
// 	sUrl.Format(_T("http://%s:%d/gm?gameid=%d&userid=%s"),m_server, m_port, gameid, m_userid);
// 	bool bRet = GetData(sUrl, m_databuf, m_nbuflen);
// 	if(!bRet)
// 	{
// 		return -1 ;
// 	}
// 	Json::Reader reader;
// 	Json::Value root;
// 	if (!reader.parse(m_databuf, root, false))
// 	{
// 		return -2;
// 	}
// 	string name,iconurl;
// 	int topmost, playtimes,type;
// 	long upsign;
// 	int size = root.size();
// 	int lgameid = 0;
// 	for (int i=0; i<size; ++i)
// 	{
// 		lgameid = root[i]["gameid"].asInt();
// 		name = root[i]["name"].asString();
// 		iconurl = root[i]["iconurl"].asString();
// 		topmost = root[i]["topmost"].asInt();
// 		playtimes = root[i]["playtimes"].asInt();
// 		type = root[i]["type"].asInt();
// 		upsign =root[i]["updatesign"].asUInt();
// 		CString icon_url_uni = UTF8ToUnicode((char*)iconurl.c_str());
// 		CString iconpath = DownloadFile(icon_url_uni);
// 		CString name_uni = UTF8ToUnicode((char*)name.c_str());
// 		CGameManage::GetInstance().UpdateGameByGameID(lgameid, name_uni, icon_url_uni, topmost, type, playtimes, upsign,_T(""));
// 	}
// 	return 1;
// }

int CDataSync::GetUser_GameInfo()
{
	CString sUrl;
	long maxTime = CGameManage::GetInstance().SelectMaxTimeFromGameManage();
	sUrl.Format(_T("http://%s:%d/usergameinfo?userid=%s&curtime=%d"),m_server, m_port, m_userid, maxTime);
	CLog::getInstance()->AgentLog((LPTSTR)(LPCTSTR)sUrl);
	//MessageBox(NULL,sUrl, _T("hi"), 0);
	GetData(sUrl);
	return 0;
}

int CDataSync::HandleUser_GameInfo(const string& data)
{
	CString sUrl;

	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(data.c_str(), root, false))
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
		bool bRet = CGameManage::GetInstance().UpdateGameByGameID(gameid, strName, fPath, topmost,  type, playtimes, upsign,url,rstatus);
		if(type != 2)
		{
			sUrl.Empty();
			sUrl.Format(_T("http://%s:%d/getprogmd5?gameid=%d"),m_server, m_port,gameid);
			GetData(sUrl);
		}
		//break;
	}
	return gameid;
}
int CDataSync::HandleProgmd5(const string& data)
{
	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(data.c_str(), root, false))
	{
		return -2;
	}
	string progmd5;
	int size = root.size();
	for (int i=0; i<size; ++i)
	{
		progmd5 = root[i]["progmd5"].asString();
		int gameid = root[i]["gameid"].asInt();
		bool bRet = CGameManage::GetInstance().InsertProgmd5byGameid(gameid, progmd5.c_str());

	}
	return 0;
}


int CDataSync::GetUserData()
{
	CString sUrl;
	long maxTime = CGameManage::GetInstance().SelectMaxTimeFromUser();
	sUrl.Format(_T("http://%s:%d/puinfo?userid=%s&curtime=%d"),m_server, m_port, m_userid, maxTime);
	CLog::getInstance()->AgentLog((LPTSTR)(LPCTSTR)sUrl);
	//MessageBox(NULL,sUrl, _T("hi"), 0);
	GetData(sUrl);
	return 0;
}
int CDataSync::HandleUserData(const string& data)
{
	CString sUrl;

	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(data.c_str(), root, false))
	{
		return -2;
	}
	int userid = 0;
	string nickname;
	int birthyear,birthmon,birthday;
	CString prov,city,area,useracct,sex,bloodtype;
	int provid,cityid,areaid;
	int   btindex,headerid;
	CString headerhis;
	unsigned int upsign;
	int size = root.size();
	for (int i=0; i<size; ++i)
	{
		userid= root[i]["id"].asInt();
		useracct= root[i]["username"].asCString();
		nickname= root[i]["nick_name"].asString();
		CString strName = UTF8ToUnicode((char*)nickname.c_str());
		bloodtype= root[i]["blood"].asCString();
		if( bloodtype == _T("A") ) btindex =0;
		else if( bloodtype == _T("B") ) btindex =1;
		else if( bloodtype == _T("AB") ) btindex =2;
		else if( bloodtype == _T("O") ) btindex =3;
		else btindex = -1;

		//CString bt = UTF8ToUnicode((char*)bloodtype.c_str());
		int sexid = root[i]["sex"].asInt();
		sexid -= 1;
		string birthdaystr =  root[i]["birthday"].asString();
		sscanf(birthdaystr.c_str(), "%d-%d-%d", &birthyear,&birthmon,&birthday);
		provid = root[i]["province_id"].asInt();
		cityid = root[i]["city_id"].asInt();
		areaid = root[i]["area_id"].asInt();
		CString prov;
		int providx;
		CGameManage::GetInstance().GetAreaNameAndIdx(provid, prov, providx);
		CString city;
		int cityidx;
		CGameManage::GetInstance().GetAreaNameAndIdx(cityid, city, cityidx);
		CString area;
		int areaidx;
		CGameManage::GetInstance().GetAreaNameAndIdx(areaid, area, areaidx);

		if(sexid == 0) sex=_T("男");
		else if(sexid==1) sex=_T("女");
		else sex=_T("");
		//topmost= root[i]["topmost"].asInt();
		//playtimes = root[i]["playtimes"].asInt();
		headerid = root[i]["image"].asInt();
		headerhis = root[i]["image_history1"].asCString();
		upsign = root[i]["updatetime"].asUInt();
		bool bRet = CGameManage::GetInstance().UpdateUser(userid, strName, useracct, birthyear,birthmon,birthday, sex,bloodtype, headerid, headerhis,prov,city,area, upsign,0, providx, cityidx, btindex,sexid, areaidx);

		//break;
	}
	return 0;
}

void CDataSync::SyncUser(CString url)
{
	GetData(url);
}




