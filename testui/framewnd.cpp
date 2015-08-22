//#include "resource.h"
#include "FrameWnd.h"
#include  <io.h>
#include <fcntl.h>
#include <Wininet.h>

CString g_strUserID ;
CString g_strUserAcct ;
CFrameWnd::CFrameWnd( LPCTSTR pszXMLPath )
: CXMLWnd(pszXMLPath)
{
	pWebBrowser = NULL;
	pINDEX = NULL;
	pZX = NULL;
	pSP = NULL;
	pWB = NULL;
	pYXK = NULL;
	pSTART = NULL;
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
			DOCHOSTUIFLAG_DISABLE_HELP_MENU| DOCHOSTUIFLAG_SCROLL_NO;
	}
	return S_OK;
}

void CFrameWnd::InitWindow()
{
	//    SetIcon(IDR_MAINFRAME); // 设置任务栏图标
	CenterWindow();

	// 初始化CActiveXUI控件
	 pINDEX = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("index")));
	 pZX = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("zx")));
	 pSP = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("sp")));
	 pWB = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("wb")));
	 pYXK = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("yxk")));
	 pSTART = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("start")));

	pINDEX->SetWebBrowserEventHandler(this);

	CString strLoginSSO =  pINDEX->GetUserData();
	strLoginSSO += _T("user_id=") + g_strUserID + _T("&");
	strLoginSSO += _T("user_name=") + g_strUserAcct + _T("&");
	strLoginSSO += _T("turl="); 
	strLoginSSO += pINDEX->GetHomePage() ;
	//tmp.Format(strLoginSSO, g_strUserID, g_strUserAcct);
	pINDEX->Navigate2(strLoginSSO);
	//pINDEX->Navigate2(pINDEX->GetHomePage());

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
	//m_vec_url.push_back(pZX->GetHomePage());
	m_vec_url.push_back(_T("lan.chinau.news"));
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

	//OutputDebugString(msg.sType+"\r\n");
	if( msg.sType == _T("click") ) 
	{
		if( (msg.pSender->GetName().Find(_T("_close")) )>=0 )
		{
			CloseTab(msg.pSender->GetUserData().GetData());
			return;
		}
		CTabLayoutUI* pTabLayout = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("body_main_tablayout")));
		CControlUI* pItem = m_PaintManager.FindControl(msg.pSender->GetUserData());
		pTabLayout->SelectItem(pItem);

		
		if(pItem!=NULL)
		{
			m_cur_selected = pItem->GetName().GetData();
			pWebBrowser = static_cast<CWebBrowserUI*>(pItem);
			

			int n = pItem->GetTag();
			if(n == 0)
			{
				pWebBrowser->NavigateHomePage();
				SetWbFocus(pWebBrowser);
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

		if(pItem!=NULL)
		{
			m_cur_selected = pItem->GetName().GetData();

			pWebBrowser = static_cast<CWebBrowserUI*>(pItem);
			pWebBrowser->NavigateHomePage();
			SetWbFocus(pWebBrowser);

		}

	}
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
	/*处理切换用户消息，暂时废弃采用关闭后打开方式
	if(WM_CONTROLPRINT == uMsg)
	{
		CString MemMapFileName=_T("senddata_001");
		//if (GetFileAttributes(MemMapFileName) != INVALID_FILE_ATTRIBUTES)
		//{
		HANDLE hFileMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, MemMapFileName);
		if (hFileMap)
		{
			TCHAR* ptChar = (TCHAR* )MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
			CString strTmp(ptChar);
			TCHAR sz1[20] = {0}, sz2[20] = {0};
			_stscanf( strTmp.GetBuffer(), _T("%s %s"), sz1, sz2);
			CString strCompare = sz1;
			if(g_strUserID != strCompare) //user changed
			{
				g_strUserID = sz1;
				g_strUserAcct = sz2;
			}
			::MessageBox(NULL,g_strUserID, g_strUserAcct, 0);
			UnmapViewOfFile(ptChar);

			CloseHandle(hFileMap);

			pZX->Navigate2(_T("about:blank"));
			pSP->Navigate2(_T("about:blank"));
			pWB->Navigate2(_T("about:blank"));
			pYXK->Navigate2(_T("about:blank"));
			pSTART->Navigate2(_T("about:blank"));

			pZX->SetTag(0);
			pSP->SetTag(0);
			pWB->SetTag(0);
			pZX->SetTag(0);
			pYXK->SetTag(0);
			pSTART->SetTag(0);
			pINDEX->SetTag(0);
			CString strLoginSSO =  pINDEX->GetUserData();
			strLoginSSO += _T("user_id=") + g_strUserID + _T("&");
			strLoginSSO += _T("user_name=") + g_strUserAcct + _T("&");
			strLoginSSO += _T("turl="); 
			strLoginSSO += pINDEX->GetHomePage() ;
			//tmp.Format(strLoginSSO, g_strUserID, g_strUserAcct);
			pINDEX->Navigate2(strLoginSSO);
			CTabLayoutUI* pTabLayout = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("body_main_tablayout")));
			CControlUI* pItem = m_PaintManager.FindControl(_T("index"));
			pTabLayout->SelectItem(pItem);

		}
	}
	*/
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
		if(urlSS.Find(str.c_str()) !=-1 && strHome.Find(str.c_str()) == -1 && urlSS.Find(_T("iframe=1"))==-1)
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
			m_cur_selected = pItem2->GetName().GetData();

			pWebBrowser = static_cast<CWebBrowserUI*>(pItem2);

			pWebBrowser->Navigate2(urlSS);
			SetWbFocus(pWebBrowser);

			pItem2->SetTag(1);
		}
	}
}

void CFrameWnd::SetWbFocus(CWebBrowserUI* pWebBrowser)
{
	HWND wnd = pWebBrowser->GetHostWindow();
	HWND tmp_hwnd = FindWindowEx(wnd,0,_T("Shell Embedding"),NULL);
	tmp_hwnd = FindWindowEx(tmp_hwnd,0,_T("Shell DocObject View"),NULL);
	tmp_hwnd = FindWindowEx(tmp_hwnd,0,_T("Internet Explorer_Server"),NULL);
	::SetFocus(tmp_hwnd);
}

void CFrameWnd::CloseTab(const CString& tabdata)
{
	CTabLayoutUI* pTabLayout = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("body_main_tablayout")));
	CControlUI* pItem = m_PaintManager.FindControl(tabdata);
	//pTabLayout->SelectItem(pItem);


	if(pItem!=NULL)
	{
		CWebBrowserUI* pw = static_cast<CWebBrowserUI*>(pItem);
		pw->Navigate2(_T("about:blank"));


		pItem->SetTag(0);
		JumpToIndex(pItem->GetName().GetData());
	}

}

void CFrameWnd::JumpToIndex(const CString& name)
{
	if(m_cur_selected != name)
	{
		return;
	}
	CControlUI* pItem = m_PaintManager.FindControl(_T("btnindex"));
	COptionUI* pui =  static_cast<COptionUI*>(pItem);
	pui->Selected(true);

	CTabLayoutUI* pTabLayout = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("body_main_tablayout")));
	CControlUI* pItem2 = m_PaintManager.FindControl(_T("index"));
	pTabLayout->SelectItem(pItem2);

}





