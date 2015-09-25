#pragma once
#include "duilib.h"
#include <vector>
#include "helper.h"
using namespace std;
//////////////////////////////////////////////////////////////////////////
///
typedef CVerticalLayoutUI TileNode;

class CPopWnd : public CXMLWnd
{
public:
	CPopWnd(LPCTSTR pszXMLPath);
	~CPopWnd(void);

public:
	virtual void OnFinalMessage( HWND );
// 	virtual CDuiString GetSkinFolder();
// 	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName( void ) const;
	virtual void Notify( TNotifyUI &msg );
	virtual void InitWindow();
//	virtual UILIB_RESOURCETYPE GetResourceType() const;

	DUI_DECLARE_MESSAGE_MAP()
	virtual void OnClick(TNotifyUI& msg);
	virtual void OnSelectChanged( TNotifyUI &msg );
	virtual void OnItemSelect( TNotifyUI &msg );

	virtual LRESULT OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	TileNode* AddTileNode(const CString& gameName,const CString& iconPath, int gameid, bool bChecked, int type);
	void SetBtnTabVisible();
private:
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
	CButtonUI* m_pMenuBtn;
	vector<CControlUI*> m_vecGameDelBtns;
	vector<COptionUI*> m_vecGameChkManBtns;
	vector<CButtonUI*> m_vecBtntabs;
	CButtonUI* m_pBtnConfirm;
	CButtonUI* m_pBtnAddGame;
	CTileLayoutUI *m_pGItem;
	int m_nGameCount;
	int m_bMAN_AUTO; //1£ºauto 2£ºmanual 3:wwg
	bool m_bShowFileDialog;
	CButtonUI* m_pBtnGo;
	//CButtonUI m_gameDelBtns[16];
};
