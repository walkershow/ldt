//#include "resource.h"
#include "FrameWnd.h"
#include  <io.h>
#include <fcntl.h>
#include <Wininet.h>
#include "PopWnd.h"
#include "GameManage.h"
#include "json.h"
#include "cJSON.h"
#include "DataSync.h"
#include "md5.h"
#include "UserWnd.h"
#include "ListContainerElementUIex.h"
//#include "md5.h"
//#include <uimenu.h>

CString g_strUserID ;
CString g_strUserAcct ;

//////////////////////////////////////////////////////////////////////////
///


DUI_BEGIN_MESSAGE_MAP(CList_Game, CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
// DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED,OnSelectChanged)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK,OnItemClick)
// DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT,OnItemSelect)
DUI_END_MESSAGE_MAP()
static CWebBrowserUI* s_cur_webbrowser_ptr=NULL;
static bool s_need_reload = false;

CList_Game::CList_Game()
{
	m_pPaintManager = NULL;
	m_pLastKsBtn = NULL;
	m_pWebBrowser = NULL;
	m_popHwnd = NULL;
	m_nCurCount = 0;

	//AddGameNode(_T("baba"), 1001);
}

void CList_Game::SetPaintMagager(CPaintManagerUI* pPaintMgr)
{
	m_pPaintManager = pPaintMgr;
}
void CList_Game::SetWebBrowserPtr(CWebBrowserUI*& pWebBrowser)
{
	m_pWebBrowser = pWebBrowser;
}
void CList_Game::OnClick(TNotifyUI& msg)
{

	if(msg.pSender->GetName() == _T("btn_addnewgame"))
	{
		CString filePath = ShowOpenFileDialog(0);
		if(filePath != "")
		{

			int nRet = AddNewGame(filePath);
			if(nRet > 0 )
			{
				s_need_reload = true;
				int cMode = CGameManage::GetInstance().GetControlMode();
				if(cMode == 1)//auto
				{
					::SendMessage(m_frameHwnd, WM_REFRESH_GAMELIST_AUTO, 0, 0 );
				}
				else if(cMode == 2)
				{
					::SendMessage(m_frameHwnd, WM_REFRESH_GAMELIST_MANUAL, 0, 0 );
				}
			}
			else if(nRet == -3)
			{
				::MessageBox(NULL,_T("已添加过此游戏"),_T("错误"),MB_OK);
			}
			else
			{
				::MessageBox(NULL,_T("不支持此游戏"),_T("错误"),MB_OK);

			}
// 			MD5 md5;
// 			string digest = md5.digestFile((LPTSTR)(LPCTSTR) filePath );
// 			CDataSync ds(_T("192.168.1.62"), 80, g_strUserID);
// 			int gameid=ds.GetProg_to_Game_ByProgmd5((LPSTR)digest.c_str());
// 			if(gameid>0)
// 			{
// 				CGameManage::GetInstance()::SetGamePath(gameid, filePath);
// 			}
		}

// 			CListUI *down_list = m_pList;
// 			if(!down_list)
// 				return;
// //		if(m_nCurCount)
// 		if(m_nCurCount>=9)
// 		{
// 			down_list->RemoveAt(m_nCurCount);
// 			//m_addnewgame->SetVisible(false);
// 
// 			//down_list->RemoveAll();
// 			//AddGameBtn();
// 		}		
// 		//AddGameNode(_T("baba"), _T("\\images\\gamesign\\yxgzbz_01.png"), 1001);
// 		m_nCurCount++;
	}
	else if(msg.pSender->GetName() == _T("btnviewgame"))
	{
		CHorizontalLayoutUI* pNodeContainer = static_cast<CHorizontalLayoutUI*>(msg.pSender->GetParent());
		CListContainerElementUI* pNode = static_cast<CListContainerElementUI*>(pNodeContainer->GetParent());
		pNode->Select();


		CTabLayoutUI* pTabLayout = static_cast<CTabLayoutUI*>(m_pPaintManager->FindControl(_T("body_main_tablayout")));
		//CControlUI* pItem = m_pPaintManager->FindControl(_T("start"));
// 
// 
		if(m_pWebBrowser!=NULL)
		{
			pTabLayout->SelectItem(m_pWebBrowser);
			s_cur_webbrowser_ptr = m_pWebBrowser;

// 			//CWebBrowserUI* pWebBrowser = static_cast<CWebBrowserUI*>(pItem);
// 			m_pWebBrowser = static_cast<CWebBrowserUI*>(pItem);
			CString url;
		//CString url = "http://lan.chinau.game.newslist/site/index?game_id=";
			int gameid= msg.pSender->GetTag();
			url.Format(_T("%s%d"),m_pWebBrowser->GetHomePage(), gameid);
			m_pWebBrowser->Navigate2(url);
		}
		
	}
	else if(msg.pSender->GetName() == _T("btn_ks"))
	{

		int gameid= msg.pSender->GetTag();
		CDuiString strType = msg.pSender->GetUserData();
		LPCTSTR stype = strType.GetData();
		int type = _ttoi(stype);
		if(type == 2)
		{
			CString url = CGameManage::GetInstance().GetGameUrl(gameid);
			ShellExecute(NULL, _T("open"), url, NULL,NULL, SW_SHOW);
			return ;
		}
		bool bRet = CGameManage::GetInstance().ExistGamePath(gameid);

		if(!bRet)
		{
			CString filePath = ShowOpenFileDialog(0);
			if(filePath != "")
			{
				bool bRet = CGameManage::GetInstance().SetGamePath(filePath, gameid);
				if(bRet && m_popHwnd)
				{
					s_need_reload = true;
				}
			}
			else
			{
				CGameManage::GetInstance().UpdateGamePlayTimes(gameid);
				//excute game
			}
		}
	}

}


void CList_Game::OnItemClick( TNotifyUI &msg )
{
	OutputDebugString(_T("MOUNSE OnItemClick\r\n"));

	CListContainerElementUIex *pNode = static_cast<CListContainerElementUIex*>(msg.pSender);
	CControlUI* pKs = pNode->FindSubControl(_T("btn_ks"));
	if(pKs!=NULL)
	{
		if(pKs == m_pLastKsBtn)
		{
			return ;
		}
		pKs->SetVisible();

		if(m_pLastKsBtn != NULL)
		{
			m_pLastKsBtn->SetVisible(false);

		}
		m_pLastKsBtn = pKs;

	}
}

void CList_Game::Reset()
{
	m_pList->RemoveAll();
	m_pLastKsBtn = NULL;
	m_nCurCount = 0;
}


void CList_Game::SetListPtr(CListUI* pList)
{
	m_pList = pList;
}

void CList_Game::SetAddGameBtnPtr(CHorizontalLayoutUI* pBtn)
{
	m_addnewgame = pBtn;
}

void CList_Game::AddGameNode(const CString& name, const CString& imgurl, int gameid, int type)
{
	CListContainerElementUIex *new_node = new CListContainerElementUIex;
	new_node->ApplyAttributeList(_T("height=\"47\" name=\"listitem\" "));


	CHorizontalLayoutUI *new_h_lay = new CHorizontalLayoutUI;
	new_h_lay->ApplyAttributeList(_T("float=\"false\" "));

	CButtonUI *new_bk = new CButtonUI;
	new_bk->ApplyAttributeList(_T("textcolor=\"#6AFFFFFF\" showhtml=\"true\" textpadding=\"46,0,0,0\""));
	RECT r={5,1,0,0};
	new_bk->SetPadding(r);
	new_bk->SetName(_T("btnviewgame"));
	new_bk->SetFloat(false);
	new_bk->SetTag(gameid);
	new_bk->SetFixedWidth(111);
	new_bk->SetFixedHeight(44);
	new_bk->SetBorderSize(0);
	new_bk->SetNormalImage(_T("file='\\images\\normal\\yxlg.png' dest='0,0,44,44' "));
	CString imgurlPos = "file='" + imgurl;
	imgurlPos +="' dest='4,4,40,40'";
	new_bk->SetBkImage(imgurlPos);
	new_bk->SetText(name);
	new_bk->SetTextStyle(DT_END_ELLIPSIS|DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	new_bk->SetToolTip(name);

// 	CLabelUI *new_label = new CLabelUI;
// 	new_label->ApplyAttributeList(_T("textcolor=\"#6AFFFFFF\" showhtml=\"true\""));
// 	new_label->SetMaxWidth(75);
// 	new_label->SetText(name);
// 	new_label->SetName(_T("down_name"));
// 	new_label->SetTextStyle(DT_END_ELLIPSIS|DT_VCENTER);

	CButtonUI *new_btn_2 = new CButtonUI;
	new_btn_2->SetName(_T("btn_ks"));
	new_btn_2->SetFixedWidth(36);
	new_btn_2->SetBorderSize(0);
	new_btn_2->SetNormalImage(_T("\\images\\push\\ks.png"));
	new_btn_2->SetTag(gameid);
	new_btn_2->SetVisible(false);
	CString strType;
	strType.Format(_T("%d"), type);
	new_btn_2->SetUserData(strType);
	new_btn_2->SetToolTip(_T("开始游戏"));

	new_h_lay->Add(new_bk);
	new_h_lay->Add(new_btn_2);
	new_node->Add(new_h_lay);
	m_pList->AddAt(new_node, 0);
	if(m_nCurCount>=9)
	{
		m_pList->RemoveAt(m_nCurCount+1);
	}	
	m_nCurCount++;
}
CHorizontalLayoutUI* CList_Game::AddGameBtn()
{
	CListContainerElementUIex *new_node = new CListContainerElementUIex;
	new_node->ApplyAttributeList(_T("height=\"47\""));

	CHorizontalLayoutUI *new_h_lay = new CHorizontalLayoutUI;
	new_h_lay->ApplyAttributeList(_T("float=\"false\" "));
	new_h_lay->SetName(_T("addnewgame"));
	//_T("childpadding=\"10\" inset=\"3,5,3,5\""));

	CButtonUI *new_btn = new CButtonUI;
	new_btn->ApplyAttributeList(_T("textcolor=\"#6AFFFFFF\" showhtml=\"true\" textpadding=\"46,0,0,0\""));
	RECT r={5,1,0,0};
	new_btn->SetPadding(r);

	new_btn->SetName(_T("btn_addnewgame"));
	new_btn->SetFloat(false);
	new_btn->SetFixedWidth(114);
	new_btn->SetFixedHeight(44);
	new_btn->SetBorderSize(0);
	new_btn->SetBkImage(_T("file='\\images\\normal\\bj09_03.png' dest='0,0,44,44' "));
	new_btn->SetText(_T("增加游戏"));
	new_btn->SetTextStyle(DT_END_ELLIPSIS|DT_VCENTER);
	//CLabelUI *new_label = new CLabelUI;
	//new_label->ApplyAttributeList(_T("textcolor=\"#6AFFFFFF\" showhtml=\"true\""));
	//new_label->SetText(_T("增加游戏"));
	//new_label->SetTextStyle(DT_END_ELLIPSIS|DT_VCENTER);

	new_h_lay->Add(new_btn);
	//new_h_lay->Add(new_label);

	new_node->Add(new_h_lay);
	m_addnewgame = new_h_lay;
	m_pList->Add(new_node);

	return m_addnewgame;

}

int CList_Game::GetCount()
{
	return m_nCurCount;
}

int CList_Game::AddNewGame(CString filePath)
{
	MD5 md5;
	string digest = md5.digestFile((LPTSTR)(LPCTSTR) filePath );
	CDataSync ds(_T("192.168.1.62"), 80, g_strUserID);
	int gameid=ds.GetProg_to_Game_ByProgmd5((LPSTR)digest.c_str());
	if(gameid>0)
	{
		if(CGameManage::GetInstance().SetGamePath(filePath, gameid))
		{
			return 1;
		}

	}
	return gameid;
}

void CList_Game::SetFrameHwnd(HWND hWnd)
{
	m_frameHwnd = hWnd;
}

void CList_Game::SetPopHwnd(HWND hWnd)
{
	m_popHwnd = hWnd;
}

//////////////////////////////////////////////////////////////////////////
///

CFrameWnd::CFrameWnd( LPCTSTR pszXMLPath ): CXMLWnd(pszXMLPath),m_pPopWnd(NULL)
{
	m_pWebBrowser = NULL;
	pINDEX = NULL;
	pZX = NULL;
	pSP = NULL;
	pWB = NULL;
	pYXK = NULL;
	pSTART = NULL;
	m_pLastClickBtn = NULL;
	m_pLastKsBtn = NULL;
	char* buf="Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 6.2;6A Browser 1.0)";
	//char* buf="6A Browser 1.0";
	DWORD dwlen = strlen(buf);
	HRESULT r = ::UrlMkSetSessionOption(URLMON_OPTION_USERAGENT,(void*)buf,dwlen,0);
	m_Listgame.SetPaintMagager(&m_PaintManager);
	AddVirtualWnd(_T("list_game"),&m_Listgame);

}

CFrameWnd::~CFrameWnd()
{
	RemoveVirtualWnd(_T("list_game"));

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
	
	 m_pBtntx = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btntx")));
	pINDEX->SetWebBrowserEventHandler(this);

	CString strLoginSSO =  pINDEX->GetUserData();
	strLoginSSO += _T("user_id=") + g_strUserID + _T("&");
	strLoginSSO += _T("user_name=") + g_strUserAcct + _T("&");
	strLoginSSO += _T("turl="); 
	strLoginSSO += pINDEX->GetHomePage() ;
	//tmp.Format(strLoginSSO, g_strUserID, g_strUserAcct);
	pINDEX->Navigate2(_T("about:blank"));
	pINDEX->Navigate2(strLoginSSO);

	//pINDEX->Navigate2(pINDEX->GetHomePage());

	pZX->SetWebBrowserEventHandler(this);
	pSP->SetWebBrowserEventHandler(this);
	pWB->SetWebBrowserEventHandler(this);
	pYXK->SetWebBrowserEventHandler(this);
	pSTART->SetWebBrowserEventHandler(this);


	pZX->Navigate2(_T("about:blank"));
	pSP->Navigate2(_T("about:blank"));
	pWB->Navigate2(_T("about:blank"));
	pYXK->Navigate2(_T("about:blank"));
	pSTART->Navigate2(_T("about:blank"));


	m_vec_url.push_back(pINDEX->GetHomePage());
	m_vec_url.push_back(pZX->GetHomePage());
	m_vec_url.push_back(pSP->GetHomePage());
	m_vec_url.push_back(pWB->GetHomePage());
	m_vec_url.push_back(pYXK->GetHomePage());

	m_vec_wbtext.push_back(pINDEX->GetName().GetData());
	m_vec_wbtext.push_back(pZX->GetName().GetData());
	m_vec_wbtext.push_back(pSP->GetName().GetData());
	m_vec_wbtext.push_back(pWB->GetName().GetData());
	m_vec_wbtext.push_back(pYXK->GetName().GetData());

	m_vec_btntext.push_back(_T("btnindex"));
	m_vec_btntext.push_back(_T("btnzx"));
	m_vec_btntext.push_back(_T("btnsp"));
	m_vec_btntext.push_back(_T("btnwb"));
	m_vec_btntext.push_back(_T("btnyxk"));

	m_pWebBrowser= pINDEX;
	s_cur_webbrowser_ptr = m_pWebBrowser;
	CListUI *down_list = static_cast<CListUI*>(m_PaintManager.FindControl(_T("game_list_tab")));
	//CHorizontalLayoutUI* m_addnewgame = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("addnewgame")));

	m_pList = down_list;
	m_Listgame.SetFrameHwnd(GetHWND());
	m_Listgame.SetListPtr(down_list);
	m_Listgame.SetWebBrowserPtr(pSTART);
	CHorizontalLayoutUI* m_addnewgame = m_Listgame.AddGameBtn();
	m_Listgame.SetAddGameBtnPtr(m_addnewgame);
	int cMode = CGameManage::GetInstance().GetControlMode();
	if(cMode == 1 || cMode == 3)//auto
	{
		SQLiteDataReader sdr = CGameManage::GetInstance().GetAllGameByAutoMode();
		int nCount =0;
		while(sdr.Read())
		{
			int gameid = sdr.GetIntValue(0);
			CString name = sdr.GetStringValue(1);
			CString iconpath = sdr.GetStringValue(2);
			int topmost= sdr.GetIntValue(3);
			int playtimes= sdr.GetIntValue(4);
			int type = sdr.GetIntValue(5);
			m_Listgame.AddGameNode(name, iconpath, gameid, type);
			nCount++;
			if(nCount >= 9)
			{
				break;
			}
		}
	}
	else if(cMode == 2)//mode
	{
		SQLiteDataReader sdr = CGameManage::GetInstance().GetAllGameByManualMode();
		int nCount =0;
		while(sdr.Read())
		{
			int gameid = sdr.GetIntValue(0);
			CString name = sdr.GetStringValue(1);
			CString iconpath = sdr.GetStringValue(2);
			int topmost= sdr.GetIntValue(3);
			int playtimes= sdr.GetIntValue(4);
			int type = sdr.GetIntValue(5);
			m_Listgame.AddGameNode(name, iconpath, gameid, type);
			nCount++;
			if(nCount >= 9) break;
		}

	}
	//SendMessage(WM_JUMP_YXK);

	//m_Listgame.AddGameNode(_T("天涯明月刀"), _T("\\images\\gamesign\\yxgzbz_13.png"), 25);
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
			m_pWebBrowser = static_cast<CWebBrowserUI*>(pItem);
			
			int n = pItem->GetTag();
			if(n == 0)
			{
				m_pWebBrowser->NavigateHomePage();
				SetWbFocus(m_pWebBrowser);
				pItem->SetTag(1);
				s_cur_webbrowser_ptr = m_pWebBrowser;	

			}
		}
		if( msg.pSender->GetName() == _T("btnmenu") ) 
		{
			CMenuWnd* pMenu = new CMenuWnd();
			if( pMenu == NULL ) { return; }
		//	POINT pt = {msg.ptMouse.x, msg.ptMouse.y};
			POINT pt2 = {914, 25};
			::ClientToScreen(*this, &pt2);
			pMenu->Init(NULL,  _T("menutest.xml"), pt2, &m_PaintManager);
		}
		else if( msg.pSender->GetName() == _T("btnback") ) 
		{
			s_cur_webbrowser_ptr->GoBack();
			//m_pWebBrowser->GoBack();
		}
		else if( msg.pSender->GetName() == _T("btn_allgame") ) 
		{
			CDataSync ds(_T("192.168.1.62"), 80, g_strUserID);
			int gameid = ds.GetUser_GameInfo();
			if( m_pPopWnd == NULL )
			{
				m_pPopWnd = new CPopWnd(_T("popup.xml"));
				m_pPopWnd->Create(GetHWND(), _T("游戏列表"), WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW | WS_EX_TOPMOST);
				m_Listgame.SetPopHwnd(m_pPopWnd->GetHWND());

			}
			POINT pt = {152, 315};
			::ClientToScreen(*this, &pt);
			::SetWindowPos(*m_pPopWnd, NULL, pt.x, pt.y,0 ,0,SWP_NOSIZE | SWP_NOZORDER );
			::ShowWindow(*m_pPopWnd, SW_SHOW);
			if(gameid>0 || s_need_reload)
			{	
				s_need_reload = false;
				::SendMessage(m_pPopWnd->GetHWND(), WM_GAME_RELOAD, 0 ,0);
				int cMode = CGameManage::GetInstance().GetControlMode();
				if(cMode == 1)//auto
				{
					::SendMessage(this->GetHWND(), WM_REFRESH_GAMELIST_AUTO, 0, 0 );
				}
				else if(cMode == 2)
				{
					::SendMessage(this->GetHWND(), WM_REFRESH_GAMELIST_MANUAL, 0, 0 );
				}
			}
			
		}
		else if( msg.pSender->GetName() == _T("closebtn") )
		{
			PostQuitMessage(0);
			return;
		}
		else if(msg.pSender->GetName() == _T("btntx"))
		{
			CUserWnd  *pUserWnd= new CUserWnd(_T("userwnd.xml"));
			pUserWnd->Create(GetHWND(), _T("用户信息"), WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW );
			pUserWnd->SetParent(GetHWND());
			pUserWnd->CenterWindow();
			::ShowWindow(*pUserWnd, SW_SHOW);
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

			m_pWebBrowser = static_cast<CWebBrowserUI*>(pItem);
			m_pWebBrowser->NavigateHomePage();
			SetWbFocus(m_pWebBrowser);
			s_cur_webbrowser_ptr = m_pWebBrowser;	

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
	int nCount = 0;
	if(WM_GAME_DETAIL == uMsg)
	{

		CTabLayoutUI* pTabLayout = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("body_main_tablayout")));
		CControlUI* pItem = m_PaintManager.FindControl(_T("start"));
		pTabLayout->SelectItem(pItem);


		if(pItem!=NULL)
		{
			m_pWebBrowser = static_cast<CWebBrowserUI*>(pItem);
			CString url;
			//CString url = "http://lan.chinau.game.newslist/site/index?game_id=";
			int gameid= (int)wParam;
			url.Format(_T("%s%d"),m_pWebBrowser->GetHomePage(), gameid);
			m_pWebBrowser->Navigate2(url);
			s_cur_webbrowser_ptr = m_pWebBrowser;	

		}
	}
	else if(WM_GAME_RESETHEAD == uMsg)
	{
		CString str = (LPCTSTR)lParam;
		m_pBtntx->SetNormalImage(str);

	}
	else if(WM_JUMP_YXK== uMsg)
	{
		JumpToYXK();
	}
	else if(WM_REFRESH_GAMELIST_AUTO== uMsg)
	{
		m_Listgame.Reset();
		SQLiteDataReader sdr = CGameManage::GetInstance().GetAllGameByAutoMode();
		while(sdr.Read())
		{
			int gameid = sdr.GetIntValue(0);
			CString name = sdr.GetStringValue(1);
			CString iconpath = sdr.GetStringValue(2);
			int topmost= sdr.GetIntValue(3);
			int playtimes= sdr.GetIntValue(4);
			int type = sdr.GetIntValue(5);

			m_Listgame.AddGameNode(name, iconpath, gameid, type);
			nCount++;
			if(nCount >= 9)
			{
				break;
			}
		}
		if(nCount <10)
		{
			nCount++;

			m_Listgame.AddGameBtn();
		}
	}
	else if(WM_REFRESH_GAMELIST_MANUAL == uMsg)
	{
		m_Listgame.Reset();
		SQLiteDataReader sdr = CGameManage::GetInstance().GetAllGameByManualMode();
		while(sdr.Read())
		{
			int gameid = sdr.GetIntValue(0);
			CString name = sdr.GetStringValue(1);
			CString iconpath = sdr.GetStringValue(2);
			int topmost= sdr.GetIntValue(3);
			int playtimes= sdr.GetIntValue(4);
			int type = sdr.GetIntValue(5);
			m_Listgame.AddGameNode(name, iconpath, gameid, type);
			nCount++;
			if(nCount >= 9)
			{
				break;
			}
			//TileNode* tn = AddTileNode(name, iconpath,gameid);
			/*for test
			if(m_nGameCount == 0) 
			{
				tn->SetVisible(false);
			}
			*/

		}
		if(nCount <10)
		{
			nCount++;

			m_Listgame.AddGameBtn();
		}

	}
	else if(WM_NCLBUTTONDBLCLK != uMsg)
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
		CString strHome = m_pWebBrowser->GetHomePage();
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

			m_pWebBrowser = static_cast<CWebBrowserUI*>(pItem2);

			m_pWebBrowser->Navigate2(urlSS);
			SetWbFocus(m_pWebBrowser);

			pItem2->SetTag(1);
			s_cur_webbrowser_ptr = m_pWebBrowser;	

		}
	}

}

void CFrameWnd::SetWbFocus(CWebBrowserUI* pWebBrowser)
{
	HWND wnd = pWebBrowser->GetHostWindow();
	HWND tmp_hwnd = FindWindowEx(wnd,0,_T("Shell Embedding"),NULL);
	tmp_hwnd = FindWindowEx(tmp_hwnd,0,_T("Shell DocObject View"),NULL);
	tmp_hwnd = FindWindowEx(tmp_hwnd,0,_T("Internet Explorer_Server"),NULL);
	HWND res=::SetFocus(tmp_hwnd);
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
	s_cur_webbrowser_ptr = static_cast<CWebBrowserUI*>(pItem2);	


}

void CFrameWnd::JumpToYXK()
{
	CControlUI* pItem = m_PaintManager.FindControl(_T("btnyxk"));
	COptionUI* pui =  static_cast<COptionUI*>(pItem);
	pui->Selected(true);

	CTabLayoutUI* pTabLayout = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("body_main_tablayout")));
	CControlUI* pItem2 = m_PaintManager.FindControl(_T("yxk"));
	pTabLayout->SelectItem(pItem2);
	CWebBrowserUI* pw = static_cast<CWebBrowserUI*>(pItem2);
	pw->Navigate2(pw->GetHomePage());
	s_cur_webbrowser_ptr = pw;	


}



LRESULT CFrameWnd::OnChar( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	/*演示键盘消息的处理*/
	TCHAR press_char = (TCHAR)wParam;
	if(press_char == VK_TAB || press_char == VK_SPACE)
	{
  		bHandled = TRUE;
	}
	else
	{
		bHandled = FALSE;
	}
	return 0;
}

