#pragma once
#include "duilib.h"
#include <WinInet.h>
#include "./lib/http_request_manager.h"
#include "./utility/md5.h"
#include "GameManage.h"

class CDataSync:public FCHttpRequestManager
{
public:
	CDataSync(LPCTSTR server,int port, LPCTSTR userid,HWND notifyHwnd = NULL);
	~CDataSync(void);

public:
	int GetProg_to_Game_ByProgmd5(CString progmd5);
	//int GetProg_to_Game();
	//int GetControlMode();
	//int GetGame_Manage_ByGameID(int gameid);
	int GetUser_GameInfo();
	bool PostData(const CString& sUrl, char* data, int datalen);
	int GetUserData();
	void SyncUser(CString url);

private:
	bool GetData(const CString& sUrl);
	CString CrackUrl(CString sUrl);
	CString DownloadFile(const CString& surl);

private:
	virtual void OnAfterRequestFinish (FCHttpRequest& rTask)
	{
		const HTTP_RESPONSE_INFO   & r = rTask.GetResponseInfo() ;
		CString url = rTask.GetURL();
		
		bool   bOK = false ;
		if (r.m_status_code == HTTP_STATUS_OK)
		{
			if (r.m_content_length)
			{
				// avoid network broken during downloading
				if (r.m_content_length == rTask.GetTotalReceiveByte())
					bOK = true ;
			}
			else
			{
				// no Content-Type field, maybe is a dynamic page, such as PHP, ASP...
				if (r.m_final_read_result)
					bOK = true ;
			}
		}
		else
		{
			CLog::getInstance()->AgentLog(_T("server:%s,port:%d http exp happend status code:%d"),(LPTSTR)(LPCTSTR)m_server, m_port, r.m_status_code);
		}

		if (bOK)
		{
			std::string   receive_data ;
			rTask.PopReceived (receive_data) ;
			//memset(m_databuf, 0, m_nbuflen);
			//memcpy(m_databuf, receive_data.c_str(), receive_data.length());
			if(url.Find(_T("usergameinfo")) !=-1)
			{
				if(HandleUser_GameInfo(receive_data)>0)
				{
					::SendMessage(m_HwndNotify, WM_REFRESH_GAMELIST, 0, 0 );
				}
			}
			else if(url.Find(_T("getprogmd5"))!=-1)
			{
				HandleProgmd5(receive_data);
				//::SendMessage(m_HwndNotify, WM_REFRESH_GAMELIST, 0, 0 );
			}
			else if(url.Find(_T("progtogame"))!=-1)
			{
				int nRet = HandleProg_to_Game_ByProgmd5(receive_data);
				if(nRet >0 )
				{
					::SendMessage(m_HwndNotify, WM_REFRESH_GAMELIST, 0, 0 );
					::SendMessage(m_HwndNotify, WM_ADDGAME_SUCC, (WPARAM)nRet, 0 );

				}
				else
				{
					::SendMessage(m_HwndNotify, WM_GAME_NOTSUPPORT, 0, 0 );
				}

			}
			else if(url.Find(_T("puinfo"))!=-1)
			{
				HandleUserData(receive_data);
				::SendMessage(m_HwndNotify, WM_GAME_RESETNICKNAME, 0, 0 );
				::SendMessage(m_HwndNotify, WM_GAME_RESETHEAD, 0, 0 );

			}

			// ... process received data
		}
	}
	int HandleUser_GameInfo(const string& data);
	int HandleProgmd5(const string& data);
	int HandleProg_to_Game_ByProgmd5(const string& data);
	int HandleUserData(const string& data);
private:
	CString m_server;
	int m_port;
	CString m_userid;
	HWND m_HwndNotify;

};
