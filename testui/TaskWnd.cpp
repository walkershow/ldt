#include "TaskWnd.h"


CTaskWnd::CTaskWnd(LPCTSTR pszXMLPath): CXMLWnd(pszXMLPath)
{
	m_pWebBrowser = NULL;
}

CTaskWnd::~CTaskWnd(void)
{
	m_pWebBrowser->Navigate2(_T("about:blank"));
}

void CTaskWnd::OnFinalMessage( HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}


LPCTSTR CTaskWnd::GetWindowClassName( void ) const
{
	return _T("TaskWnd");
}
HRESULT STDMETHODCALLTYPE CTaskWnd::GetHostInfo( DOCHOSTUIINFO __RPC_FAR *pInfo)
{
	if (pInfo != NULL)
	{
		//pInfo->dwFlags |= DOCHOSTUIFLAG_NO3DBORDER|DOCHOSTUIFLAG_THEME |DOCHOSTUIFLAG_NO3DOUTERBORDER ;
		pInfo->dwFlags |= DOCHOSTUIFLAG_NO3DBORDER | DOCHOSTUIFLAG_THEME |   
			DOCHOSTUIFLAG_NO3DOUTERBORDER | DOCHOSTUIFLAG_DIALOG |  
			DOCHOSTUIFLAG_DISABLE_HELP_MENU| DOCHOSTUIFLAG_SCROLL_NO;
	}
	return S_OK;
}
void CTaskWnd::InitWindow()
{
	m_pWebBrowser = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("taskview")));
	m_pWebBrowser->SetWebBrowserEventHandler(this);
	m_pWebBrowser->Navigate2(_T("about:blank"));
	m_pWebBrowser->NavigateHomePage();

}

//修改http 头user-agent 然后发消息重新浏览
void CTaskWnd::BeforeNavigate2( IDispatch *pDisp,VARIANT *&url,VARIANT *&Flags,VARIANT *&TargetFrameName,VARIANT *&PostData,VARIANT *&Headers,VARIANT_BOOL *&Cancel )
{
	static TCHAR *szUrl = new TCHAR[MAX_PATH];
	ZeroMemory(szUrl, MAX_PATH);
	_tcscpy( szUrl, (TCHAR*)(_bstr_t)url );
	CString urlSS=szUrl;
	//urlSS+=szUrl;

	OutputDebugString(urlSS+"\r\n");
	CString strClose = _T("close");
	if(urlSS.Find(strClose) !=-1)
	{
		*Cancel = TRUE;
		Close(0);
		return;
		//::ShowWindow(GetHWND(), SW_HIDE);
	}
	CString strTask = m_pWebBrowser->GetHomePage();
	if(urlSS.Find(strTask) == -1 && urlSS.Find(_T("about:blank")) == -1)
	{
		*Cancel = TRUE;
		::SendMessage(m_parentHwnd, WM_JUMP_FORTASK,(WPARAM)szUrl, 0 );
		Close(0);

	}
	
}

void CTaskWnd::SetParent(HWND hWnd)
{
	m_parentHwnd = hWnd;
}
