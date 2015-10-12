#include "PopWnd.h"
	
#include "GameManage.h"
#include <algorithm>  
#include <functional>  
#include <ShellAPI.h>
#include <Commdlg.h>

//////////////////////////////////////////////////////////////////////////
///

DUI_BEGIN_MESSAGE_MAP(CPopWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT,OnItemSelect)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED,OnSelectChanged)
DUI_END_MESSAGE_MAP()

CPopWnd::CPopWnd(LPCTSTR pszXMLPath): CXMLWnd(pszXMLPath),m_nGameCount(0),m_bShowFileDialog(false),m_bMAN_AUTO(1)
{
	m_bMAN_AUTO = CGameManage::GetInstance().GetControlMode();
}

CPopWnd::~CPopWnd(void)
{
}

void CPopWnd::OnFinalMessage( HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

// DuiLib::CDuiString CPopWnd::GetSkinFolder()
// {
// #ifdef _DEBUG
// 	return _T("");
// #else
// 	return _T("skin\\");
// #endif
// }
// 
// DuiLib::CDuiString CPopWnd::GetSkinFile()
// {
// 	return _T("popup.xml");
//}

// UILIB_RESOURCETYPE CPopWnd::GetResourceType() const
// {
// 	return UILIB_FILE;
// }

LPCTSTR CPopWnd::GetWindowClassName( void ) const
{
	return _T("PopWnd");
}

void CPopWnd::OnClick( TNotifyUI &msg )
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();
	HWND hParent = GetParent(GetHWND());


	if( msg.pSender == m_pCloseBtn ) {
		ShowWindow(false);
		return; 
	}
	else if( msg.pSender == m_pMinBtn ) { 
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return; }
	else if( msg.pSender == m_pMaxBtn ) { 
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return; }
	else if( msg.pSender == m_pRestoreBtn ) { 
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return; }
	else if( msg.pSender == m_pMenuBtn ) 
	{

	}
	else if(msg.pSender == m_pBtnConfirm)
	{
		bool bHasChecked = false;
		if(m_bMAN_AUTO == 2)
		{
			for(int i =0; i<m_vecGameChkManBtns.size(); i++)
			{
				COptionUI* pOpt = static_cast<COptionUI*>(m_vecGameChkManBtns[i]);
				int gameid = pOpt->GetTag();
				if(pOpt->IsSelected())
				{
					
					CGameManage::GetInstance().UpdateGameTopmost(gameid);
					bHasChecked = true;
				}
				else
				{
					CGameManage::GetInstance().UpdateGameTopmost(gameid, 0);
				}
			}
			if(bHasChecked)
			{
				::SendMessage(hParent, WM_REFRESH_GAMELIST_MANUAL, 0, 0 );
			}
		}

		CGameManage::GetInstance().UpdateControlMode(m_bMAN_AUTO);


	}
	else if(msg.pSender == m_pBtnAddGame)
	{

		int n = m_pGItem->GetCount();
		for(int i=0; i<n; i++)
		{
			m_vecGameDelBtns[i]->SetVisible(false);
			m_vecGameChkManBtns[i]->SetVisible(false);
		}
		::SendMessage(hParent, WM_JUMP_YXK, 0, 0 );
	}
}

void CPopWnd::OnSelectChanged( TNotifyUI &msg )
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();

}

void CPopWnd::OnItemSelect( TNotifyUI &msg )
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();
}

LRESULT CPopWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
 {
	 // 关闭窗口，退出程序
	 if(uMsg == WM_GAME_RELOAD)
	 {
			m_pGItem = static_cast<CTileLayoutUI*>(m_PaintManager.FindControl(_T("gameitem")));
			if(!m_pGItem)
				return 0;
			if(m_pGItem)
			{
				m_pGItem->RemoveAll();
				m_vecGameChkManBtns.clear();
				m_vecGameDelBtns.clear();
				m_nGameCount = 0;
				int cMode = CGameManage::GetInstance().GetControlMode();
				if(cMode!=3)
				{
					SQLiteDataReader sdr = CGameManage::GetInstance().GetAllGame();
					//CString err = CGameManage::GetInstance().GetLastErrorMsg();
					while(sdr.Read())
					{
						int gameid = sdr.GetIntValue(0);
						CString name = sdr.GetStringValue(1);
						CString iconpath = sdr.GetStringValue(2);
						int topmost= sdr.GetIntValue(3);
						int playtimes= sdr.GetIntValue(4);
						int type = sdr.GetIntValue(5);
						TileNode* tn = AddTileNode(name, iconpath,gameid, topmost, type);
						m_nGameCount++;

					}
				}
				else
				{
					//未玩过要排序
					SQLiteDataReader sdr = CGameManage::GetInstance().GetAllGameByWWGMode();
					while(sdr.Read())
					{
						int gameid = sdr.GetIntValue(0);
						CString name = sdr.GetStringValue(1);
						CString iconpath = sdr.GetStringValue(2);
						int topmost= sdr.GetIntValue(3);
						int playtimes= sdr.GetIntValue(4);
						int type = sdr.GetIntValue(5);
						int status = sdr.GetIntValue(6);
						int rstatus = sdr.GetIntValue(7);
						if(status<0 || rstatus !=-1)
						{
							continue;
						}
						TileNode* tn = AddTileNode(name, iconpath,gameid, topmost, type);

						m_nGameCount++;

					}
				}
				SetBtnTabVisible();
		 }

	 }
	 if(uMsg == WM_DESTROY)
	 {
		 ::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	 }
	 bHandled = FALSE;
	 return 0;
 }

void CPopWnd::Notify( TNotifyUI &msg )
{
	if( msg.sType == _T("click") ) 
	{
		HWND hParent = GetParent(GetHWND());

		if( (msg.pSender->GetName().Find(_T("btngl_wwg")) )>=0 )
		{
			m_pBtnGo->SetVisible(false);
			m_pBtnConfirm->SetVisible(false);
			CGameManage::GetInstance().UpdateControlMode(3);
			::SendMessage(GetHWND(), WM_GAME_RELOAD, 0 ,0);

		}
		else if( (msg.pSender->GetName().Find(_T("btn_game")) )>=0 )
		{
			m_pBtnGo->SetUserData(msg.pSender->GetUserData());
			m_pBtnGo->SetTag(msg.pSender->GetTag());
			m_pBtnGo->SetVisible(true);
		}
		else if( (msg.pSender->GetName().Find(_T("btngl_delgame")) )>=0 )
		{
			int n = m_pGItem->GetCount();
			for(int i=0; i<n; i++)
			{
				m_vecGameDelBtns[i]->SetVisible(true);
				m_vecGameChkManBtns[i]->SetVisible(false);
			}

		}
		else if( (msg.pSender->GetName().Find(_T("chk_order_manual")) )>=0 )
		{
			m_pBtnConfirm->SetVisible(true);
			m_pBtnGo->SetVisible(false);
			m_bMAN_AUTO = 2;
			int n = m_pGItem->GetCount();
			for(int i=0; i<n; i++)
			{
				m_vecGameChkManBtns[i]->SetVisible(true);
				m_vecGameDelBtns[i]->SetVisible(false);
			}
		}
		else if( (msg.pSender->GetName().Find(_T("chk_order_auto")) )>=0 )
		{
			m_pBtnConfirm->SetVisible(false);
			m_pBtnGo->SetVisible(false);
			m_bMAN_AUTO = 1;
			int n = m_pGItem->GetCount();
			for(int i=0; i<n; i++)
			{
				m_vecGameChkManBtns[i]->SetVisible(false);
				m_vecGameDelBtns[i]->SetVisible(false);
			}
			CGameManage::GetInstance().UpdateControlMode(1);
			::SendMessage(hParent, WM_REFRESH_GAMELIST_AUTO, 0, 0 );

		}
		else if( (msg.pSender->GetName().Find(_T("delonegame")) )>=0 )
		{
			CControlUI* pP = msg.pSender->GetParent()->GetParent();
			int nIndex = m_pGItem->GetItemIndex(pP);

			m_pGItem->Remove( pP );
			CGameManage::GetInstance().DelGame(pP->GetTag());
			m_vecGameDelBtns.erase(std::remove(m_vecGameDelBtns.begin(),m_vecGameDelBtns.end(),msg.pSender), m_vecGameDelBtns.end());
			CControlUI* p_chk = m_vecGameChkManBtns[nIndex];
			m_vecGameChkManBtns.erase(std::remove(m_vecGameChkManBtns.begin(),m_vecGameChkManBtns.end(),p_chk), m_vecGameChkManBtns.end());
			m_nGameCount--;
			SetBtnTabVisible();
			if(m_bMAN_AUTO == 1 || m_bMAN_AUTO == 3)
			{
				::SendMessage(hParent, WM_REFRESH_GAMELIST_AUTO, 0, 0 );

			}
			else if(m_bMAN_AUTO == 2)
			{
				::SendMessage(hParent, WM_REFRESH_GAMELIST_MANUAL, 0, 0 );

			}
			int gameid = msg.pSender->GetTag();
			CGameManage::GetInstance().DelGame(gameid);
			//m_vecGameDelBtns.erase(pP);

		}
		else if( (msg.pSender->GetName().Find(_T("btn_tab")) )>=0 )
		{
			CDuiString str_upound= msg.pSender->GetUserData();
			int pound = _ttoi(str_upound.GetData());
			int uppound = pound>m_nGameCount ? m_nGameCount : pound;
			int downpound = pound>m_nGameCount ? pound : m_nGameCount;
			for(int i=0; i<uppound; i++)
			{
				CControlUI* pItem = m_pGItem->GetItemAt(i);
				pItem->SetVisible(false);
			}
			for(int i=uppound; i<downpound; i++)
			{
				CControlUI* pItem = m_pGItem->GetItemAt(i);
				pItem->SetVisible(true);
			}
		}
		else if(msg.pSender->GetName() == _T("btn_go"))
		{
			int gameid= msg.pSender->GetTag();
			CDuiString strType = msg.pSender->GetUserData();
			LPCTSTR stype = strType.GetData();
			int type = _ttoi(stype);
			if(type == 2)
			{
				CString url = CGameManage::GetInstance().GetGameUrl(gameid);
				ShellExecute(NULL, _T("open"), url, NULL,NULL, SW_SHOW);
				CGameManage::GetInstance().UpdateGamePlayTimes(gameid);

				return ;
			}
			CString  filePath = CGameManage::GetInstance().ExistGamePath(gameid);

			if(filePath == "")
			{
				CString filePath = ShowOpenFileDialog(0);
				if(filePath != "")
				{
					bool bRet = CGameManage::GetInstance().SetGamePath(filePath, gameid);
					if(bRet )
					{
						::SendMessage(GetHWND(), WM_GAME_RELOAD, 0 ,0);
					}
				}
			}
			else
			{
				if(!::PathFileExists(filePath))
				{
					filePath = ShowOpenFileDialog(0);
					if(filePath != "")
					{
						bool bRet = CGameManage::GetInstance().SetGamePath(filePath, gameid);
					}
				}
				CGameManage::GetInstance().UpdateGamePlayTimes(gameid);
				ShellExecute(0 ,_T("open"), filePath, _T(""), _T(""),SW_SHOWNORMAL);
			}
		}
	}
	else if( msg.sType == _T("dbclick") ) 
	{
		if( (msg.pSender->GetName().Find(_T("btn_game")) )>=0 )
		{
			HWND hParent = GetParent(GetHWND());
			::SendMessage(hParent, WM_GAME_DETAIL, (WPARAM)msg.pSender->GetTag(), 0 );
		}

	}
	return WindowImplBase::Notify(msg);
}

LRESULT CPopWnd::OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if( wParam == SC_CLOSE ) {
		::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if( ::IsZoomed(*this) != bZoomed ) {
		if( !bZoomed ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));
			if( pControl ) pControl->SetVisible(true);
		}
		else {
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));
			if( pControl ) pControl->SetVisible(false);
		}
	}
	return lRes;
}

void CPopWnd::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("closebtn")));
	m_pMaxBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("maxbtn")));
	m_pRestoreBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("restorebtn")));
	m_pMinBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("minbtn")));
	m_pMenuBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("menubtn")));
	m_pBtnGo = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_go")));

	CButtonUI* pBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_tab1")));
	m_vecBtntabs.push_back(pBtn);
	pBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_tab2")));
	m_vecBtntabs.push_back(pBtn);
	pBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_tab3")));
	m_vecBtntabs.push_back(pBtn);
	pBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_tab4")));
	m_vecBtntabs.push_back(pBtn);
	pBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_tab5")));
	m_vecBtntabs.push_back(pBtn);

	m_pBtnConfirm = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_confirm")));
	m_pBtnConfirm->SetVisible(false);
	m_pBtnAddGame = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btngl_addgame")));
	
	m_pGItem = static_cast<CTileLayoutUI*>(m_PaintManager.FindControl(_T("gameitem")));
	if(!m_pGItem)
		return;
	if(m_pGItem)
	{
		//SQLiteDataReader sdr = CGameManage::GetInstance().ExcuteQuery(_T("select gameid,name,iconpath,topmost,playtimes,status from game_manage order by playtimes;"));
		SQLiteDataReader sdr = CGameManage::GetInstance().GetAllGame();
		//CString err = CGameManage::GetInstance().GetLastErrorMsg();
		while(sdr.Read())
		{
			int gameid = sdr.GetIntValue(0);
			CString name = sdr.GetStringValue(1);
			CString iconpath = sdr.GetStringValue(2);
			int topmost= sdr.GetIntValue(3);
			int playtimes= sdr.GetIntValue(4);
			int type = sdr.GetIntValue(5);
			TileNode* tn = AddTileNode(name, iconpath,gameid, topmost, type);

			m_nGameCount++;

		}
		SetBtnTabVisible();
	}	
}

void CPopWnd::SetBtnTabVisible()
{
	int btn_shownum = (m_nGameCount-1)/16;
	for(int i=0;i<m_vecBtntabs.size(); i++)
	{
		m_vecBtntabs[i]->SetVisible(false);
	}
	for(int i=0;i<btn_shownum+1;i++)
	{
		m_vecBtntabs[i]->SetVisible();
	}
}
TileNode* CPopWnd::AddTileNode(const CString& gameName,const CString& iconPath, int gameid, bool bChecked, int type)
{
	TileNode *newv = new TileNode;
	newv->ApplyAttributeList(_T("name=\"complex_button\" padding=\"60,20,0,0\" float=\"false\" width=\"56\" height=\"71\""));
	newv->SetTag(gameid);
	CContainerUI *new_v_lay = new CContainerUI;
	new_v_lay->ApplyAttributeList(_T("float=\"false\" height=\"57\" bkimage=\"file='\\images\\normal\\tcyxtb.png' dest='10,10,46,46'\" "));
	CString ipath = _T("file='")+iconPath +_T("' dest='10,10,46,46'");
	new_v_lay->SetBkImage(ipath);

	CButtonUI *new_btn_2 = new CButtonUI;
	new_btn_2->SetFloat(true);
	new_btn_2->SetVisible(false);
	new_btn_2->SetName(_T("delonegame"));
	new_btn_2->SetTag(gameid);
	SIZE xy3 ={37,2};
	new_btn_2->SetFixedXY(xy3);
	new_btn_2->SetFixedHeight(17);
	new_btn_2->SetFixedWidth(15 );
	new_btn_2->SetNormalImage(_T("\\images\\popwnd\\tcscxg.png"));

	COptionUI *new_btn_3 = new COptionUI;
	SIZE xy2 ={37,39};
	new_btn_3->SetFixedXY(xy2);
	new_btn_3->SetName(_T("sdcheck"));
	new_btn_3->SetFloat(true);
	new_btn_3->SetFixedHeight(16);
	new_btn_3->SetFixedWidth(16);
	new_btn_3->SetBorderSize(0);
	new_btn_3->SetVisible(false);
	new_btn_3->SetNormalImage(_T("\\images\\popwnd\\tcsdxxjt.png"));
	new_btn_3->SetSelectedImage(_T("\\images\\popwnd\\tcsdxxxg.png"));
	new_btn_3->SetTag(gameid);
	if(bChecked)
	{
		new_btn_3->Selected(true);
	}

	COptionUI *new_btn_1 = new COptionUI;
	new_btn_1->ApplyAttributeList(_T("float=\"false\" width=\"56\" height=\"57\"  hotimage=\"file='\\images\\popwnd\\tcxzxg.png'\" pushedimage=\"file='\\images\\popwnd\\tcxzxg.png'\""));
	new_btn_1->SetName(_T("btn_game"));
	new_btn_1->SetTag(gameid);
	new_btn_1->SetHotImage(_T("\\images\\popwnd\\tcxzxg.png"));
	new_btn_1->SetSelectedImage(_T("\\images\\popwnd\\tcxzxg.png"));
	new_btn_1->SetToolTip(gameName);
	new_btn_1->SetGroup(_T("gameitems"));
	CString strType;
	strType.Format(_T("%d"), type);
	new_btn_1->SetUserData(strType);

	CHorizontalLayoutUI *new_v_lay1 = new CHorizontalLayoutUI;
	new_v_lay1->ApplyAttributeList(_T("float=\"false\" height=\"14\" "));

	CLabelUI *new_label = new CLabelUI;
	new_label->ApplyAttributeList(_T("textcolor=\"#FFFFFFFF\" showhtml=\"true\""));
	new_label->SetFloat(false);

	new_label->SetText(gameName);
	new_label->SetName(_T("down_name"));
	if(gameName.GetLength()>4)
	{
		new_label->SetTextStyle(DT_END_ELLIPSIS|DT_VCENTER);
	}
	else
	{
		new_label->SetTextStyle(DT_END_ELLIPSIS|DT_CENTER);
	}

	new_v_lay->Add(new_btn_1);
	new_v_lay->Add(new_btn_3);
	new_v_lay->Add(new_btn_2);

	CString strRet = CGameManage::GetInstance().ExistGamePath(gameid);
	if(strRet=="" && type !=2)
	{

		CControlUI *new_btn_4 = new CControlUI;
		new_btn_4->SetFloat(true);
		new_btn_4->SetName(_T("wwg"));
		new_btn_4->SetTag(gameid);
		SIZE xy4 ={4,39};
		new_btn_4->SetFixedXY(xy4);
		new_btn_4->SetFixedHeight(17);
		new_btn_4->SetFixedWidth(15 );
		new_btn_4->SetVisible();
		new_btn_4->SetBkImage(_T("\\images\\popwnd\\tcwwg1.png"));
		new_v_lay->Add(new_btn_4);

	}

	m_vecGameDelBtns.push_back(new_btn_2);
	m_vecGameChkManBtns.push_back(new_btn_3);

	newv->Add(new_v_lay);
	new_v_lay1->Add(new_label);
	newv->Add(new_v_lay1);
	m_pGItem->Add(newv);
	return newv;
}
LRESULT CPopWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::ShowWindow(GetHWND(), SW_HIDE);
	return 0L;
}
