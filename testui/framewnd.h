#pragma once
#include "duilib.h"
#include <string>
#include <comdef.h>
#include <locale.h> 
#include <vector>
using namespace std;

// 以XML生成界面的窗口基类
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

// class NvToParam
// {
// public:
// 		BSTR URL;
// 		VARIANT Flags;
// 		VARIANT TargetFrameName;
// 		VARIANT PostedData;
// 		VARIANT Headers;
// 	
// };

//#define  MSG_NAV WM_USER+100
class CFrameWnd: public CXMLWnd, public CWebBrowserEventHandler
{
public:
	explicit CFrameWnd(LPCTSTR pszXMLPath);
// DUI_DECLARE_MESSAGE_MAP()
// virtual void OnNavTo(TNotifyUI& msg);
	virtual void InitWindow();
	virtual void Notify(TNotifyUI& msg);
	virtual CControlUI* CreateControl(LPCTSTR pstrClassName);
	HRESULT STDMETHODCALLTYPE GetHostInfo(DOCHOSTUIINFO __RPC_FAR *pInfo);
    void BeforeNavigate2( IDispatch *pDisp,VARIANT *&url,VARIANT *&Flags,VARIANT *&TargetFrameName,VARIANT *&PostData,VARIANT *&Headers,VARIANT_BOOL *&Cancel );
	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	void SetWebPageFocusEx(CWebBrowserUI * pWebPage);
private:
	CWebBrowserUI* pWebBrowser;
	CControlUI* m_pLastClickBtn;
	CDuiString m_lastClickBtn_HotImage;
	vector<wstring> m_vec_btntext;
	vector<wstring> m_vec_wbtext;
	vector<wstring> m_vec_url;
	HWND m_hwnd;
};
