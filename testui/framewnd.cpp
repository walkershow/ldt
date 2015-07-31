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
			DOCHOSTUIFLAG_DISABLE_HELP_MENU | DOCHOSTUIFLAG_SCROLL_NO;
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

	pZX->SetWebBrowserEventHandler(this);
	pSP->SetWebBrowserEventHandler(this);
	pWB->SetWebBrowserEventHandler(this);
	pYXK->SetWebBrowserEventHandler(this);
	pSTART->SetWebBrowserEventHandler(this);


	pZX->Navigate2(_T("about:blank"));
	//pZX->NavigateHomePage();
	pSP->Navigate2(_T("about:blank"));
	pWB->Navigate2(_T("about:blank"));
	pYXK->Navigate2(_T("about:blank"));
	pSTART->Navigate2(_T("about:blank"));
	//pWebBrowser->NavigateHomePage();


	m_vec_url.push_back(pINDEX->GetHomePage());
	m_vec_url.push_back(_T("www.devmachine.com"));
	m_vec_url.push_back(pSP->GetHomePage());
	m_vec_url.push_back(pWB->GetHomePage());
	m_vec_url.push_back(pYXK->GetHomePage());
	//m_vec_url.push_back(pSTART->GetHomePage());

	m_vec_wbtext.push_back(pINDEX->GetName().GetData());
	m_vec_wbtext.push_back(pZX->GetName().GetData());
	m_vec_wbtext.push_back(pSP->GetName().GetData());
	m_vec_wbtext.push_back(pWB->GetName().GetData());
	m_vec_wbtext.push_back(pYXK->GetName().GetData());
	//m_vec_wbtext.push_back(pSTART->GetName());

	m_vec_btntext.push_back(_T("btnindex"));
	m_vec_btntext.push_back(_T("btnzx"));
	m_vec_btntext.push_back(_T("btnsp"));
	m_vec_btntext.push_back(_T("btnwb"));
	m_vec_btntext.push_back(_T("btnyxk"));
	//m_vec_btntext.push_back(_T("btnstart"));

	pWebBrowser= pINDEX;
	
}



void CFrameWnd::Notify( TNotifyUI& msg )
{

	//HWND wnd = this->GetHWND();
	//OutputDebugString(msg.sType+"\r\n");
	if( msg.sType == _T("click") ) 
	{
		CTabLayoutUI* pTabLayout = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("body_main_tablayout")));
		CControlUI* pItem = m_PaintManager.FindControl(msg.pSender->GetUserData());
		pTabLayout->SelectItem(pItem);


		if(pItem!=NULL)
		{
			pWebBrowser = static_cast<CWebBrowserUI*>(pItem);
			m_hwnd = pWebBrowser->GetHostWindow();
			HWND tmp_hwnd = FindWindowEx(m_hwnd,0,_T("Shell Embedding"),NULL);
			tmp_hwnd = FindWindowEx(tmp_hwnd,0,_T("Shell DocObject View"),NULL);
			tmp_hwnd = FindWindowEx(tmp_hwnd,0,_T("Internet Explorer_Server"),NULL);
			::SetFocus(tmp_hwnd);
			//lngHwnd = FindWindowEx(hwndParent, 0, "Shell Embedding", vbNullString)
			//lngHwnd = FindWindowEx(lngHwnd, 0, "Shell DocObject View", vbNullString)
			//lngHwnd = FindWindowEx(lngHwnd, 0, "Internet Explorer_Server", vbNullString)
			int n = pItem->GetTag();
			if(n ==0)
			{
				pWebBrowser->Activate();
				//HWND wnd = pWebBrowser->GetHostWindow();
				//pWebBrowser->GetWebBrowser2()
				//::SetFocus(wnd);
				//SetWebPageFocusEx(pWebBrowser);
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
	else if( msg.sType == _T("dbclick") ) 
	{
		CTabLayoutUI* pTabLayout = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("body_main_tablayout")));
		CControlUI* pItem = m_PaintManager.FindControl(msg.pSender->GetUserData());
		pTabLayout->SelectItem(pItem);
       // COptionUI *pop = static_cast<COptionUI*>pItem;
		if(pItem!=NULL)
		{
			pWebBrowser = static_cast<CWebBrowserUI*>(pItem);
			pWebBrowser->NavigateHomePage();

			SetWebPageFocusEx(pWebBrowser);

			
		}

	}
// 	if( msg.pSender->GetName() == _T("minbtn"))
// 	{
// 		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
// 		return;
// 
// 	}
	if( msg.pSender->GetName() == _T("closebtn") )
	{
		PostQuitMessage(0);
		return;
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
	if(WM_NCLBUTTONDBLCLK != uMsg)
	{
		return WindowImplBase::HandleMessage(uMsg,wParam,lParam);
	}

	return 0;
}

//修改http 头user-agent 然后发消息重新浏览
void CFrameWnd::BeforeNavigate2( IDispatch *pDisp,VARIANT *&url,VARIANT *&Flags,VARIANT *&TargetFrameName,VARIANT *&PostData,VARIANT *&Headers,VARIANT_BOOL *&Cancel )
{
	CString strTest,strOut;

	TCHAR *szUrl = new TCHAR[MAX_PATH];
	_tcscpy( szUrl, (TCHAR*)(_bstr_t)url );
	CString urlSS=szUrl;
	//urlSS+=szUrl;

	OutputDebugString(urlSS+"\r\n");
	int pos = -1;
	for(int i=0; i<m_vec_url.size(); i++)
	{
		wstring str =m_vec_url[i].c_str();
		CString strHome = pWebBrowser->GetHomePage();
		if(urlSS.Find(str.c_str()) !=-1 && strHome.Find(str.c_str()) == -1)
		{
			*Cancel = TRUE;
			pos = i;
			break;
		}

	}
	if(pos != -1)
	{
		CControlUI* pItem = m_PaintManager.FindControl(m_vec_btntext[pos].c_str());
		COptionUI* pui =  static_cast<COptionUI*>(pItem);
		pui->Selected(true);

		CTabLayoutUI* pTabLayout = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("body_main_tablayout")));
		CControlUI* pItem2 = m_PaintManager.FindControl(m_vec_wbtext[pos].c_str());
		pTabLayout->SelectItem(pItem2);
		if(pItem2!=NULL)
		{
			pWebBrowser = static_cast<CWebBrowserUI*>(pItem2);

				pWebBrowser->Navigate2(urlSS);
				SetWebPageFocusEx(pWebBrowser);

				pItem2->SetTag(1);
		}
	}
}


void CFrameWnd::SetWebPageFocusEx(CWebBrowserUI * pWebPage)
{
	/// 设置网页为焦点, 使鼠标滚动时, 可以让网页元素进行滚动
	/// 相当于当网页打开后，先用鼠标在网页中空白处点击一下，再用鼠标滚轮滚动网页
	/// @ref invalid http://www.cnblogs.com/baoconghui/archive/2012/09/08/2676935.html 
	/// @ref ok http://stackoverflow.com/questions/298932/set-focus-to-embedded-mshtml
	/// DuiLib的 CWebBrowserUI 稍有不同

	HRESULT             hr = S_FALSE;
	IWebBrowser2 *      pIWebBrowser2 = NULL;
	IDispatch *         pHtmlDocDisp = NULL;
	IHTMLDocument2 *    pHtmlDoc2 = NULL;
	IHTMLWindow2 *      pHtmlWindow2 = NULL;

	if (NULL == pWebPage)
		goto END_SetWebPageFocus;

	// get web browser interface
	pIWebBrowser2 = pWebPage->GetWebBrowser2();
	if (NULL == pIWebBrowser2)
		goto END_SetWebPageFocus;

	// get the IDispatch interface of the document
	hr = pIWebBrowser2->get_Document(&pHtmlDocDisp);
	if (FAILED(hr) || (NULL == pHtmlDocDisp))
		goto END_SetWebPageFocus;

	// Query interface for IHTMLDocument2
	hr = pHtmlDocDisp->QueryInterface (IID_IHTMLDocument2, (void**)&pHtmlDoc2);
	if (FAILED(hr) || (NULL == pHtmlDoc2))
		goto END_SetWebPageFocus;
	IHTMLElement* pelem = NULL;
	pHtmlDoc2->get_body(&pelem);
	pelem->click();

 	hr = pHtmlDoc2->get_parentWindow(&pHtmlWindow2);
 	if (FAILED(hr) || (NULL == pHtmlWindow2))
 		goto END_SetWebPageFocus;
 
 	pHtmlWindow2->focus(); ///< !

END_SetWebPageFocus:
	/// 这里不能释放得到的COM指针，否则运行21~36次的时候报错.
	return;
}




