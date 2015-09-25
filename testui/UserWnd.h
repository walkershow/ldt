#pragma once
#include "duilib.h"
#include <vector>
#include "helper.h"
#include "Date.h"
using namespace  std;

class CUserWnd: public CXMLWnd
{
public:
	CUserWnd(LPCTSTR pszXMLPath);
	~CUserWnd(void);

public:
	virtual void OnFinalMessage( HWND );
	// 	virtual CDuiString GetSkinFolder();
	// 	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName( void ) const;
	//virtual void Notify( TNotifyUI &msg );
	virtual void InitWindow();
	virtual void Notify( TNotifyUI &msg );
	void SetDay();
	void SetAgeXZSX();
	void InitYear();
	void InitCtrlVal();
	void SetParent(HWND hWnd);
	void SetAgeXZSXByVal(int year,int mon,int day);

	DUI_DECLARE_MESSAGE_MAP()
		virtual void OnClick(TNotifyUI& msg);
		void OnSelect( TNotifyUI &msg );
private:
	vector<CLabelUI*> m_vecPLbl;
	vector<CComboBoxUI*> m_vecPCBox;
	CRichEditUI* m_PRedit;
	CDate m_date;
	CButtonUI *m_pSave;
	CButtonUI *m_pCancel;
	CButtonUI *m_pEdit;
	CLabelUI *m_pDate;
	CLabelUI *m_pSzd;
	CButtonUI *m_pShowBtn;
	CButtonUI *m_pHDBtn;
	HWND m_parentHwnd;


};
