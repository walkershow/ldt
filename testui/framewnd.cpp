//#include "resource.h"
#include "FrameWnd.h"
#include "cini.h"
#include  <io.h>
#include <fcntl.h>

CFrameWnd::CFrameWnd( LPCTSTR pszXMLPath )
: CXMLWnd(pszXMLPath)
{
	pWebBrowser = NULL;
	m_pLastClickBtn = NULL;
}

HRESULT STDMETHODCALLTYPE CFrameWnd::GetHostInfo( DOCHOSTUIINFO __RPC_FAR *pInfo)
{
	if (pInfo != NULL)
	{
		pInfo->dwFlags |= DOCHOSTUIFLAG_NO3DBORDER|DOCHOSTUIFLAG_THEME |DOCHOSTUIFLAG_NO3DOUTERBORDER ;
	}
	return S_OK;
}

void CFrameWnd::InitWindow()
{
	//    SetIcon(IDR_MAINFRAME); // 设置任务栏图标
	CenterWindow();
	IniFile ini;
	char path[MAX_PATH+1]={0};


	GetCurExeDir(path);
	strcat_s(path, sizeof(path),"testui.ini" );
	string webpath = "";
	//MessageBoxA(NULL, path, "hi", 0);
	if( (_access( path, 0 )) != -1 )
	{
		//MessageBoxA(NULL, path, "File testui.ini exists ", 0);
		ini.SetFile(path);
		int errline =0;
		ini.load(errline);

		//MessageBoxA(NULL, ini.ReadString("URLINFO","SHUOSHUO").c_str(), "File testui.ini exists ", 0);
		m_shuoshuo = webpath + ini.ReadString("URLINFO","SHUOSHUO");
		if (m_shuoshuo == webpath) m_shuoshuo ="about:blank";
		//MessageBoxA(NULL, m_shuoshuo.c_str(), "File testui.ini exists ", 0);
		m_luntan = webpath +ini.ReadString("URLINFO","LUNTAN");
		if (m_luntan == webpath) m_luntan ="about:blank";
		m_zixun = webpath +ini.ReadString("URLINFO","ZIXUN");
		if (m_zixun == webpath) m_zixun ="about:blank";
		m_shipin =webpath + ini.ReadString("URLINFO","SHIPIN");
		if (m_shipin == webpath) m_shipin ="about:blank";
		m_shouye = webpath +ini.ReadString("URLINFO","SHOUYE");
		if (m_shouye == webpath) m_shouye ="about:blank";
		m_yxk = webpath + ini.ReadString("URLINFO","YOUXIKU");
		m_yxq = webpath +ini.ReadString("URLINFO","YOUXIQUAN");
		if (m_yxq == webpath) m_yxq ="about:blank";
		m_dspw = webpath +ini.ReadString("URLINFO","DSPW");
		if (m_dspw == webpath) m_dspw ="about:blank";
		m_jysc = webpath +ini.ReadString("URLINFO","JYSC");
		if (m_jysc == webpath) m_jysc ="about:blank";
		m_start = webpath +ini.ReadString("URLINFO","START");
		if (m_start == webpath) m_start ="about:blank";
	}
	// 初始化CActiveXUI控件
	//CActiveXUI* pActiveXUI = static_cast<CActiveXUI*>(m_PaintManager.FindControl(_T("ActiveXDemo1")));
	pWebBrowser = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("ie")));
	pWebBrowser->SetWebBrowserEventHandler(this);

	pWebBrowser->NavigateUrl("about:blank");
	pWebBrowser->NavigateUrl(m_shouye.c_str());

	
}

void CFrameWnd::Notify( TNotifyUI& msg )
{

	if( msg.sType == _T("click") ) 
	{
		if( msg.pSender->GetName() != _T("btnstart") ) 
		{
			CButtonUI *btn = ( CButtonUI* )msg.pSender;
			if(m_pLastClickBtn != NULL)
			{
				m_pLastClickBtn->SetHotImage(m_lastClickBtn_HotImage);
			}
			m_lastClickBtn_HotImage = btn->GetHotImage();
			m_pLastClickBtn = btn;
			btn->SetHotImage( btn->GetPushedImage() );
		}

		if( msg.pSender->GetName() == _T("btnbbs") ) 
		{
			if( pWebBrowser != NULL ) 
			{
				pWebBrowser->NavigateUrl(m_luntan.c_str());
			}
		}
		else if( msg.pSender->GetName() == _T("btnsp") ) 
		{
			if( pWebBrowser != NULL ) 
			{
				pWebBrowser->NavigateUrl(m_shipin.c_str());
			}
		}
		else if( msg.pSender->GetName() == _T("btnwb") ) 
		{

			if( pWebBrowser != NULL ) 
			{
				pWebBrowser->NavigateUrl(m_shuoshuo.c_str());
			}
		}
		else if( msg.pSender->GetName() == _T("btnzx") ) 
		{
			if( pWebBrowser != NULL ) 
			{
				pWebBrowser->NavigateUrl(m_zixun.c_str());
			}
		}
		else if( msg.pSender->GetName() == _T("btnyxk") ) 
		{
			if( pWebBrowser != NULL ) 
			{
				pWebBrowser->NavigateUrl(m_yxk.c_str());
			}
		}
		else if( msg.pSender->GetName() == _T("btnyxq") ) 
		{
			if( pWebBrowser != NULL ) 
			{
				pWebBrowser->NavigateUrl(m_yxq.c_str());
			}
		}
		else if( msg.pSender->GetName() == _T("btnstart") ) 
		{
			if( pWebBrowser != NULL ) 
			{
				pWebBrowser->NavigateUrl(m_start.c_str());
			}
		}
		else if( msg.pSender->GetName() == _T("btnindex") ) 
		{
			if( pWebBrowser != NULL ) 
			{
				pWebBrowser->NavigateUrl(_T(m_shouye.c_str()));
			}
		}
		else if( msg.pSender->GetName() == _T("btnrefresh") ) 
		{
			if( pWebBrowser != NULL ) 
			{
				pWebBrowser->Refresh();
			}
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



