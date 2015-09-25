#include "UserWnd.h"
#include "GameManage.h"
DUI_BEGIN_MESSAGE_MAP(CUserWnd, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT,OnSelect)
DUI_END_MESSAGE_MAP()

CUserWnd::CUserWnd(LPCTSTR pszXMLPath): CXMLWnd(pszXMLPath)
{
}

CUserWnd::~CUserWnd(void)
{
}

void CUserWnd::OnFinalMessage( HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}


LPCTSTR CUserWnd::GetWindowClassName( void ) const
{
	return _T("UserWnd");
}
void CUserWnd::OnSelect( TNotifyUI &msg )
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();
	if(sName == _T("cbyear") )
	{
		SetAgeXZSX();

	}
	else if(sName == _T("cbday") )
	{
		SetAgeXZSX();

	}
	else if(sName == _T("cbmon") )
	{
		SetDay();
		SetAgeXZSX();

	}
}
void CUserWnd::SetDay()
{
	int nSel = m_vecPCBox[3]->GetCurSel();
	if(nSel== -1) nSel = 0;

	CControlUI *pctrl = m_vecPCBox[3]->GetItemAt(nSel);
	int mon = _ttoi(pctrl->GetUserData());
	nSel = m_vecPCBox[2]->GetCurSel();
	if(nSel== -1) nSel = 0;

	pctrl = m_vecPCBox[2]->GetItemAt(nSel);
	
	int year = pctrl->GetTag();
	int daycount = CDate::DayOfMonth(year,mon);
	m_vecPCBox[4]->RemoveAll();
	for(int i=1;i<=daycount; i++)
	{
		CString str;
		str.Format(_T("%d日"), i);
		CListLabelElementUI *new_node = new CListLabelElementUI;
		new_node->SetText(str);
		new_node->SetTag(i);
		m_vecPCBox[4]->Add(new_node);
	}
	CLabelUI *pLbl = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lblnn")));
	CDate birth(year, mon, 1);
	birth.GetAge();

}

void CUserWnd::InitYear()
{
	//m_vecPCBox[2]->SetVisible();
	int curyear = CDate::GetCurDate().GetYear();
	for(int i=curyear; i>=1900; i--)
	{
		CString str;
		str.Format(_T("%d年"), i);
		CListLabelElementUI *new_node = new CListLabelElementUI;
		new_node->SetText(str);
		new_node->SetTag(i);
		m_vecPCBox[2]->Add(new_node);
	}
	//m_vecPCBox[2]->SetVisible(false);

}
void CUserWnd::SetAgeXZSX()
{
	int mon,year,day;
	int nSel = m_vecPCBox[3]->GetCurSel();
	if(nSel== -1) nSel = 0;
	CControlUI *pctrl = m_vecPCBox[3]->GetItemAt(nSel);
	mon = _ttoi(pctrl->GetUserData());
	nSel = m_vecPCBox[2]->GetCurSel();
	if(nSel== -1) nSel = 0;

	pctrl = m_vecPCBox[2]->GetItemAt(nSel);
	year = pctrl->GetTag();

	nSel = m_vecPCBox[4]->GetCurSel();
	if(nSel== -1) nSel = 0;

	pctrl = m_vecPCBox[4]->GetItemAt(nSel);
	day = pctrl->GetTag();
	if(day == 0) day = 1;

	CLabelUI *pLbl = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lblnl")));
	CDate birth(year, mon, day);
	int age = birth.GetAge();
	CString str;
	str.Format(_T("%d"), age);
	pLbl->SetText(str);

	pLbl = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lblxz")));
	
	str = birth.constell();
	pLbl->SetText(str);

	pLbl = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lblsx")));
	str = birth.GetZodiac();
	pLbl->SetText(str);
}

void CUserWnd::SetAgeXZSXByVal(int year,int mon,int day)
{

	CLabelUI *pLbl = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lblnl")));
	CDate birth(year, mon, day);
	int age = birth.GetAge();
	CString str;
	str.Format(_T("%d"), age);
	pLbl->SetText(str);

	pLbl = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lblxz")));

	str = birth.constell();
	pLbl->SetText(str);

	pLbl = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lblsx")));
	str = birth.GetZodiac();
	pLbl->SetText(str);
}


void CUserWnd::OnClick( TNotifyUI &msg )
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();
	if(sName == _T("btnedit") )
	{
		
		m_pDate->SetVisible(false);
		m_pSzd->SetVisible(false);
		m_pEdit->SetVisible(false);
		m_pSave->SetVisible();
		m_pCancel->SetVisible();
		for(int i=0;i<m_vecPLbl.size(); i++)
		{
			m_vecPLbl[i]->SetVisible(false);

		}
		for(int i=0;i<m_vecPCBox.size(); i++)
		{
			m_vecPCBox[i]->SetVisible(true);
		}
		m_PRedit->SetVisible(true);
		InitCtrlVal();

	}
	else if(sName == _T("btnconfirm") )
	{

		CString sex = m_vecPCBox[0]->GetText();
		CString bt = m_vecPCBox[1]->GetText();
		int mon,year,day;
		int nSel = m_vecPCBox[3]->GetCurSel();
		if(nSel== -1) nSel = 0;
		CControlUI *pctrl = m_vecPCBox[3]->GetItemAt(nSel);
		mon = _ttoi(pctrl->GetUserData());
		nSel = m_vecPCBox[2]->GetCurSel();
		if(nSel== -1) nSel = 0;

		pctrl = m_vecPCBox[2]->GetItemAt(nSel);
		year = pctrl->GetTag();

		nSel = m_vecPCBox[4]->GetCurSel();
		if(nSel== -1) nSel = 0;

		pctrl = m_vecPCBox[4]->GetItemAt(nSel);
		day = pctrl->GetTag();
		if(day == 0) day = 1;

		CString country = m_vecPCBox[5]->GetText();
		int countryindex = m_vecPCBox[5]->GetCurSel();
		CString prov = m_vecPCBox[6]->GetText();
		int provindex = m_vecPCBox[6]->GetCurSel();
		CString city = m_vecPCBox[7]->GetText();
		int cityindex = m_vecPCBox[7]->GetCurSel();
		CString nickname = m_PRedit->GetText();
		int sexindex = m_vecPCBox[0]->GetCurSel();
		int btindex = m_vecPCBox[0]->GetCurSel();
		
		bool bRet =	CGameManage::GetInstance().SetUser(100, g_strUserAcct, bt,sex,year,mon,day,country,prov,city,nickname, countryindex, provindex, cityindex, sexindex, btindex);

		m_pDate->SetVisible();
		m_pSzd->SetVisible();
		m_pEdit->SetVisible(true);
		m_pSave->SetVisible(false);
		m_pCancel->SetVisible(false);
		for(int i=0;i<m_vecPLbl.size(); i++)
		{
			m_vecPLbl[i]->SetVisible(true);
		}
		for(int i=0;i<m_vecPCBox.size(); i++)
		{
			m_vecPCBox[i]->SetVisible(false);
		}
		m_PRedit->SetVisible(false);
		InitCtrlVal();
	}
	else if(sName == _T("btncancel") )
	{
		m_pSzd->SetVisible();
		m_pDate->SetVisible();
		m_pEdit->SetVisible(true);
		m_pSave->SetVisible(false);
		m_pCancel->SetVisible(false);
		for(int i=0;i<m_vecPLbl.size(); i++)
		{
			m_vecPLbl[i]->SetVisible(true);
		}
		for(int i=0;i<m_vecPCBox.size(); i++)
		{
			m_vecPCBox[i]->SetVisible(false);
		}
		m_PRedit->SetVisible(false);
		InitCtrlVal();

	}
	else if(sName == _T("btnpass"))
	{
		CString url = _T("http://baidu.com");
		ShellExecute(NULL, _T("open"), url, NULL,NULL, SW_SHOW);
	}

	else if(sName == _T("btnsec"))
	{
		CString url = _T("http://baidu.com");

		ShellExecute(NULL, _T("open"), url, NULL,NULL, SW_SHOW);
	}
	else if(sName.Find(_T("btnhead"))>=0)
	{
		CButtonUI* pbtn = static_cast<CButtonUI*>(msg.pSender);
		if(pbtn != NULL)
		{
			m_pHDBtn->SetNormalImage(pbtn->GetNormalImage());
			m_pShowBtn->SetNormalImage(pbtn->GetNormalImage());
		}

	}
	else if(sName.Find(_T("btnconfirmhead"))>=0)
	{
		CString str = m_pShowBtn->GetNormalImage();
// 		CString sAppPath = CPaintManagerUI::GetInstancePath().GetData();
// 		TCHAR szFile[MAX_PATH] = {0};
// 		lstrcpy(szFile, sAppPath);
// 		::PathAppend(szFile, _T("\\images\head\tx.jpg"));
		::SendMessage(m_parentHwnd, WM_GAME_RESETHEAD, str.GetLength(), (LPARAM)(LPCTSTR)str);
/*		MoveFileEx((LPCSTR)(LPCTSTR)str,szFile_T(,MOVEFILE_REPLACE_EXISTING);*/

	}
	else if(sName.Find(_T("btncancelhead"))>=0)
	{

	}
	__super::OnClick(msg);
}

void CUserWnd::InitWindow()
{
	m_pHDBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnhdhead")));
	m_pShowBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnshowhead")));
	m_pSave = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnconfirm")));
	m_pCancel = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btncancel")));
	m_pEdit = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnedit")));
	m_pSzd = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lblszd")));
	m_pDate = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lbldate")));
	CLabelUI *pLbl = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lblzh")));
	pLbl->SetText(g_strUserAcct);
	pLbl = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lblnc")));
	m_vecPLbl.push_back(pLbl);
	pLbl = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lblsex")));
	m_vecPLbl.push_back(pLbl);
	pLbl = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lblbt")));
	m_vecPLbl.push_back(pLbl);
// 	pLbl = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lblyear")));
// 	m_vecPLbl.push_back(pLbl);
// 	pLbl = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lblmon")));
// 	m_vecPLbl.push_back(pLbl);
// 	pLbl = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lblday")));
// 	m_vecPLbl.push_back(pLbl);
// 	pLbl = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lblnat")));
// 	m_vecPLbl.push_back(pLbl);
// 	pLbl = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lblprov")));
// 	m_vecPLbl.push_back(pLbl);
// 	pLbl = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lblcity")));
// 	m_vecPLbl.push_back(pLbl);
	CRichEditUI *pedt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("edtnc")));
	m_PRedit = pedt;
	CComboBoxUI* pcb = static_cast<CComboBoxUI*>(m_PaintManager.FindControl(_T("cbsex")));
	m_vecPCBox.push_back(pcb);
	pcb = static_cast<CComboBoxUI*>(m_PaintManager.FindControl(_T("cbbt")));
	m_vecPCBox.push_back(pcb);

	pcb = static_cast<CComboBoxUI*>(m_PaintManager.FindControl(_T("cbyear")));
	m_vecPCBox.push_back(pcb);

	pcb = static_cast<CComboBoxUI*>(m_PaintManager.FindControl(_T("cbmon")));
	m_vecPCBox.push_back(pcb);

	pcb = static_cast<CComboBoxUI*>(m_PaintManager.FindControl(_T("cbday")));
	m_vecPCBox.push_back(pcb);
	pcb = static_cast<CComboBoxUI*>(m_PaintManager.FindControl(_T("cbnat")));
	m_vecPCBox.push_back(pcb);
	pcb = static_cast<CComboBoxUI*>(m_PaintManager.FindControl(_T("cbprov")));
	m_vecPCBox.push_back(pcb);
	pcb = static_cast<CComboBoxUI*>(m_PaintManager.FindControl(_T("cbcity")));
	m_vecPCBox.push_back(pcb);
	InitYear();
	InitCtrlVal();
		

}
//select userid,useracct,bt,sex,birthyear,birthmon,birthday,country,prov,city,nickname from user where userid=%d

void CUserWnd::InitCtrlVal()
{
	int userid = _ttoi((LPCTSTR)g_strUserID);
	SQLiteDataReader sdr = CGameManage::GetInstance().GetUser(userid);
	while(sdr.Read())
	{
		CLabelUI *pLbl = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lblzh")));
		pLbl->SetText(g_strUserAcct);
		CString str,strdate,strszd;
		str = sdr.GetStringValue(3);
		m_vecPLbl[1]->SetText(str);
		str = sdr.GetStringValue(2);
		m_vecPLbl[2]->SetText(str);
		str.Format(_T("%d年"),sdr.GetIntValue(4));
		strdate = str;
		//m_vecPLbl[3]->SetText(str);
		str.Format(_T("%d月"),sdr.GetIntValue(5));
		strdate += str;
		//m_vecPLbl[4]->SetText(str);
		str.Format(_T("%d日"),sdr.GetIntValue(6));
		strdate += str;
		//m_vecPLbl[5]->SetText(str);

		m_pDate->SetText(strdate);
		
		str.Format(_T("%s"),sdr.GetStringValue(7));
		strszd = str;
		//m_vecPLbl[6]->SetText(str);
		str.Format(_T("%s"),sdr.GetStringValue(8));
		strszd+= str;
		///*m_vecPLbl*/[7]->SetText(str);
		
		str.Format(_T("%s"),sdr.GetStringValue(9));
		strszd+=str;
		//m_vecPLbl[8]->SetText(str);
		str = sdr.GetStringValue(10);
		m_vecPLbl[0]->SetText(str);

		m_pSzd->SetText(strszd);

		m_PRedit->SetText(str);

		int idx= sdr.GetIntValue(14);
		m_vecPCBox[0]->SelectItem(idx);
		idx= sdr.GetIntValue(15);
		m_vecPCBox[1]->SelectItem(idx);

		int year = sdr.GetIntValue(4);
		int nowyear = CDate::GetCurDate().GetYear();
		idx = nowyear-year;
		m_vecPCBox[2]->SelectItem(idx);
		int mon = sdr.GetIntValue(5);
		idx = mon-1;
		m_vecPCBox[3]->SelectItem(idx);
		int day = sdr.GetIntValue(6);
		idx = day-1;
		m_vecPCBox[4]->SelectItem(idx);


		 idx= sdr.GetIntValue(11);
		m_vecPCBox[5]->SelectItem(idx);
		idx= sdr.GetIntValue(12);
		m_vecPCBox[6]->SelectItem(idx);
		idx= sdr.GetIntValue(13);
		m_vecPCBox[7]->SelectItem(idx);

		SetAgeXZSXByVal(year, mon, day);
	}

}
void CUserWnd::Notify(TNotifyUI &msg)
{
	if(msg.sType==_T("selectchanged"))
	{
		CDuiString name = msg.pSender->GetName();
		CTabLayoutUI* pDemoListSwitch = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("body_main_tablayout")));

		if(name.CompareNoCase(_T("btnuinfo")) == 0) pDemoListSwitch->SelectItem(0);
		else if(name.CompareNoCase(_T("btnuhead")) == 0) pDemoListSwitch->SelectItem(1);
		else if(name.CompareNoCase(_T("demo_list_ani_ctrl")) == 0) pDemoListSwitch->SelectItem(2);

	}
	OutputDebugString(msg.sType+"\r\n");
	return WindowImplBase::Notify(msg);
}

void CUserWnd::SetParent(HWND hWnd)
{
	m_parentHwnd = hWnd;
}
