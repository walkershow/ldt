//#include "resource.h"
#include "FrameWnd.h"
#include  <io.h>
#include <fcntl.h>
#include <Wininet.h>

CFrameWnd::CFrameWnd( LPCTSTR pszXMLPath )
: CXMLWnd(pszXMLPath)
{
	pWebBrowser = NULL;
	m_pLastClickBtn = NULL;
	char* buf="6A Browser 1.0";
	DWORD dwlen = strlen(buf);
	HRESULT r = ::UrlMkSetSessionOption(URLMON_OPTION_USERAGENT,(void*)buf,dwlen,0);

}

HRESULT STDMETHODCALLTYPE CFrameWnd::GetHostInfo( DOCHOSTUIINFO __RPC_FAR *pInfo)
{
	if (pInfo != NULL)
	{
		//pInfo->dwFlags |= DOCHOSTUIFLAG_NO3DBORDER|DOCHOSTUIFLAG_THEME |DOCHOSTUIFLAG_NO3DOUTERBORDER ;
		pInfo->dwFlags |= DOCHOSTUIFLAG_NO3DBORDER | DOCHOSTUIFLAG_THEME |   
			DOCHOSTUIFLAG_NO3DOUTERBORDER | DOCHOSTUIFLAG_DIALOG |  
			DOCHOSTUIFLAG_DISABLE_HELP_MENU;// | DOCHOSTUIFLAG_SCROLL_NO;; 
	}
	return S_OK;
}

void CFrameWnd::InitWindow()
{
	//    SetIcon(IDR_MAINFRAME); // 设置任务栏图标
	CenterWindow();

	// 初始化CActiveXUI控件
	CWebBrowserUI* pINDEX = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("index")));
	CWebBrowserUI* pZX = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("zx")));
	CWebBrowserUI* pSP = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("sp")));
	CWebBrowserUI* pWB = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("wb")));
	CWebBrowserUI* pYXK = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("yxk")));
	CWebBrowserUI* pSTART = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("start")));

	pINDEX->SetWebBrowserEventHandler(this);
	pINDEX->Navigate2(_T("about:blank"));
	pINDEX->Navigate2(pINDEX->GetHomePage());
	pWebBrowser= pINDEX;
	
}



void CFrameWnd::Notify( TNotifyUI& msg )
{
	if( msg.pSender->GetName() == _T("minbtn"))
	{
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		return;

	}
	if( msg.pSender->GetName() == _T("closebtn") )
	{
		PostQuitMessage(0);
		return;
	}

	if( msg.sType == _T("dbclick") ) 
	{
		CTabLayoutUI* pTabLayout = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("body_main_tablayout")));
		CControlUI* pItem = m_PaintManager.FindControl(msg.pSender->GetUserData());
		pTabLayout->SelectItem(pItem);

		if(pItem!=NULL)
		{
			pWebBrowser = static_cast<CWebBrowserUI*>(pItem);
			pWebBrowser->SetWebBrowserEventHandler(this);
			pWebBrowser->Navigate2(_T("about:blank"));
			pWebBrowser->Navigate2(pWebBrowser->GetHomePage());
		}

	}
	else if( msg.sType == _T("click") ) 
	{
		CTabLayoutUI* pTabLayout = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("body_main_tablayout")));
		CControlUI* pItem = m_PaintManager.FindControl(msg.pSender->GetUserData());
		pTabLayout->SelectItem(pItem);
		
		if(pItem!=NULL)
		{
			pWebBrowser = static_cast<CWebBrowserUI*>(pItem);
			int n = pItem->GetTag();
			if(n ==0)
			{
				pWebBrowser->SetWebBrowserEventHandler(this);
				pWebBrowser->Navigate2(_T("about:blank"));
				pWebBrowser->Navigate2(pWebBrowser->GetHomePage());
				pItem->SetTag(1);
			}
		}
		if( msg.pSender->GetName() == _T("btnrefresh") ) 
		{
			pWebBrowser->Refresh();
		}
		if( msg.pSender->GetName() == _T("btnback") ) 
		{
			pWebBrowser->GoBack();
		}
		if( msg.pSender->GetName() == _T("btnforward") ) 
		{
			pWebBrowser->GoForward();
		}

	}

	__super::Notify(msg);
}

CControlUI* CFrameWnd::CreateControl( LPCTSTR pstrClassName )
{


	if (_tcsicmp(pstrClassName, _T("Wnd")) == 0)
	{

		CWndUI  *pUI  = new CWndUI;            
		HWND    hWnd  = CreateWindow(_T("BUTTON"), _T("win32"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 0, 0, 0, 0, m_PaintManager.GetPaintWindow(), NULL, NULL, NULL);
		pUI->Attach(hWnd);  

		//// 上面用的是win32的按钮，下面这段用MFC的按钮
		//CButton *pBtn = new CButton;    // 记得释放内存
		//pBtn->Create(_T("MFC"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, CRect(0, 0, 0, 0), CWnd::FromHandle(m_PaintManager.GetPaintWindow()), 0);
		//pUI->Attach(*pBtn);            
		return pUI;
	}

	return NULL;
}

LRESULT CFrameWnd::HandleMessage( UINT uMsg,WPARAM wParam,LPARAM lParam )
{
/*MSG_NAV 浏览消息
	if(MSG_NAV == uMsg)
	{
		NvToParam* pNvTo = (NvToParam*)(wParam);
		TCHAR* szUrl = (TCHAR*)(lParam);
		CVariant url;
		url.vt=VT_BSTR;
		url.bstrVal=T2BSTR(szUrl);
		//CString szUrl =  (TCHAR*)(_bstr_t)pNvTo->URL;
		OutputDebugString(_T("========nav start=========\r\n"));
		OutputDebugString(szUrl);
		OutputDebugString( (TCHAR*)(_bstr_t)pNvTo->Headers);
		OutputDebugString(_T("========nav end========\r\n"));
		pWebBrowser->GetWebBrowser2()->Navigate2(&url,&pNvTo->Flags, &pNvTo->TargetFrameName,&pNvTo->PostedData,&pNvTo->Headers);
		//MessageBoxW( NULL,szUrl,NULL,NULL );
		delete pNvTo;
		return 1;
	}
	*/
	if(WM_NCLBUTTONDBLCLK != uMsg)
	{
		return WindowImplBase::HandleMessage(uMsg,wParam,lParam);
	}


	return 0;
}

/*修改http 头user-agent 然后发消息重新浏览
void CFrameWnd::BeforeNavigate2( IDispatch *pDisp,VARIANT *&url,VARIANT *&Flags,VARIANT *&TargetFrameName,VARIANT *&PostData,VARIANT *&Headers,VARIANT_BOOL *&Cancel )
{
	return ;
	//Headers->
	CString strTest,strOut;

	TCHAR header[MAX_PATH];
	_tcscpy( header, (TCHAR*)(_bstr_t)Headers );
	CString str="header:";
		str+=header ;
	OutputDebugString(str+"\r\n");

	TCHAR *szUrl = new TCHAR[MAX_PATH];
	_tcscpy( szUrl, (TCHAR*)(_bstr_t)url );
	CString urlSS="URL:";
	urlSS+=szUrl;

	OutputDebugString(urlSS+"\r\n");

	//TRACE(header);
	CString hh = header;
	int r = hh.Find(_T("User-Agent:6ABrowser 1.0"));
	if( r >=0)
	{
		OutputDebugString(_T("===================\r\n"));
		OutputDebugString(str);
		OutputDebugString(urlSS);
		OutputDebugString(_T("===================\r\n"));


		return;
	}
	*Cancel = TRUE;// 没有，取消这次导航

	strTest = header;
	if (!strTest.IsEmpty())
		strTest+=_T("\r\n");
	strTest+=_T("User-Agent:6ABrowser 1.0");

	NvToParam* pNvTo = new NvToParam;
	pNvTo->URL = (bstr_t)url;
	pNvTo->Flags = *Flags;
	pNvTo->TargetFrameName = *TargetFrameName;
	pNvTo->PostedData = *PostData;
	pNvTo->Headers.vt= VT_BSTR;
	pNvTo->Headers.bstrVal =strTest.AllocSysString();
	::PostMessage(m_hWnd, MSG_NAV,(WPARAM)pNvTo,(LPARAM)szUrl);
}
*/



