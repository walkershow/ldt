#pragma once
#include <UIlib.h>
#include <atlstr.h>
#include <vector>
using namespace std;
#include "log.h"
using namespace DuiLib;

#pragma comment(lib, "Urlmon.lib")
#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_ud.lib")
#   else
#       pragma comment(lib, "DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_u.lib")
#   else
#       pragma comment(lib, "DuiLib.lib")
#   endif
#endif

class CXMLWnd : public WindowImplBase
{
public:
	explicit CXMLWnd(LPCTSTR pszXMLPath) 
		: m_strXMLPath(pszXMLPath){}

public:
	virtual LPCTSTR GetWindowClassName() const
	{
		return _T("XMLWnd");
	}

	virtual CDuiString GetSkinFile()
	{
		return m_strXMLPath;
	}

	virtual CDuiString GetSkinFolder()
	{
		return _T("");
	}

protected:
	CDuiString m_strXMLPath;
};


// 将HWND显示到CControlUI上面
class CWndUI: public CControlUI
{
public:
	CWndUI(): m_hWnd(NULL){}

	virtual void SetInternVisible(bool bVisible = true)
	{
		__super::SetInternVisible(bVisible);
		::ShowWindow(m_hWnd, bVisible);
	}

	virtual void SetPos(RECT rc)
	{
		__super::SetPos(rc);
		::SetWindowPos(m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
	}

	BOOL Attach(HWND hWndNew)
	{
		if (! ::IsWindow(hWndNew))
		{
			return FALSE;
		}

		m_hWnd = hWndNew;
		return TRUE;
	}

	HWND Detach()
	{
		HWND hWnd = m_hWnd;
		m_hWnd = NULL;
		return hWnd;
	}

protected:
	HWND m_hWnd;
};

class CListContainerElementUIex :
	public DuiLib::CListContainerElementUI
{
public:
	CListContainerElementUIex(void);
public:
	void DoEvent(TEventUI& event);
};


class CDialogBuilderCallbackEx : public IDialogBuilderCallback
{
public:
	CControlUI* CreateControl(LPCTSTR pstrClass) 
	{
		if( _tcsicmp(pstrClass, _T("ListContainerElementUIex")) == 0 ) return new CListContainerElementUIex;
		return NULL;
	}
};
extern CString g_strUserID ;
extern CString g_strUserAcct ;
extern CString g_server;
extern int g_port;
extern int g_runtimes;

class CDataSync;
extern CDataSync *g_pDSync;

#define WM_REFRESH_GAMELIST_MANUAL WM_USER+100
#define WM_REFRESH_GAMELIST_AUTO   WM_USER+101
#define WM_JUMP_YXK				   WM_USER+102
#define WM_GAME_DETAIL			   WM_USER+103
#define WM_GAME_RELOAD			   WM_USER+104
#define WM_GAME_RESETHEAD		   WM_USER+105
#define WM_GAME_RESETNICKNAME	   WM_USER+106
#define WM_REFRESH_GAMELIST		   WM_USER+107
#define WM_GAME_NOTSUPPORT		   WM_USER+200