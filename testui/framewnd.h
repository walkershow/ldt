#pragma once
#include "duilib.h"
#include <string>
#include <comdef.h>
#include <locale.h> 
#include <vector>

using namespace std;

extern CString g_strUserID ;
extern CString g_strUserAcct ;
#define WM_CONTROLPRINT WM_USER+1001
// 以XML生成界面的窗口基类

extern CString ShowOpenFileDialog(HWND hParent);


class CList_Game : public CNotifyPump
{
public:
	CList_Game();
	~CList_Game();
	void SetPaintMagager(CPaintManagerUI* pPaintMgr);
	void SetListPtr(CListUI* pList);
	void SetAddGameBtnPtr(CHorizontalLayoutUI* pBtn);
	CHorizontalLayoutUI* AddGameBtn();
	void AddGameNode(const CString& name, const CString& imgurl, int gameid, int type, int bInsLast=false);
	int	 GetCount();
	void Reset();
	void SetWebBrowserPtr(CWebBrowserUI*& pWebBrowser);
	int AddNewGame( CString filePath);
	void SetFrameHwnd(HWND hWnd);
	void SetPopHwnd(HWND hWnd);
	DUI_DECLARE_MESSAGE_MAP()
	virtual void OnClick(TNotifyUI& msg);
	virtual void OnItemClick( TNotifyUI &msg );
private:
	CPaintManagerUI* m_pPaintManager;
	int m_nCurCount;
	CHorizontalLayoutUI* m_addnewgame;
	CListUI* m_pList;
	CControlUI* m_pLastKsBtn;
	CWebBrowserUI* m_pWebBrowser;
	HWND m_frameHwnd;
	HWND m_popHwnd;
	bool m_bSomeOneSelected;
};


class CPopWnd;
class CFrameWnd: public CXMLWnd, public CWebBrowserEventHandler
{
public:
	explicit CFrameWnd(LPCTSTR pszXMLPath);
	~CFrameWnd();
// DUI_DECLARE_MESSAGE_MAP()
// virtual void OnNavTo(TNotifyUI& msg);
	virtual void InitWindow();
	virtual void Notify(TNotifyUI& msg);
	virtual CControlUI* CreateControl(LPCTSTR pstrClassName);
	virtual LRESULT OnChar( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	HRESULT STDMETHODCALLTYPE GetHostInfo(DOCHOSTUIINFO __RPC_FAR *pInfo);
    void BeforeNavigate2( IDispatch *pDisp,VARIANT *&url,VARIANT *&Flags,VARIANT *&TargetFrameName,VARIANT *&PostData,VARIANT *&Headers,VARIANT_BOOL *&Cancel );
	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	void SetWbFocus(CWebBrowserUI* pWebBrowser);
	void CloseTab(const CString& tabdata);
	void JumpToIndex(const CString& name);
	void SetBtnTabVisible();
	void JumpToYXK();
	void OnHotKey(WPARAM wp,LPARAM lp);
	void ResetNickName();
	void RefreshGameWnd();
private:
	CWebBrowserUI* m_pWebBrowser;
	CControlUI* m_pLastClickBtn;
	CDuiString m_lastClickBtn_HotImage;
	vector<wstring> m_vec_btntext;
	vector<wstring> m_vec_wbtext;
	vector<wstring> m_vec_url;
	CString m_cur_selected;
	//HWND m_hwnd;
	CWebBrowserUI* pINDEX;
	CWebBrowserUI* pZX;
	CWebBrowserUI* pSP;
	CWebBrowserUI* pWB;
	CWebBrowserUI* pYXK;
	CWebBrowserUI* pSTART;
	CWebBrowserUI* pZB;
	CList_Game m_Listgame;
	CPopWnd* m_pPopWnd;
	CListUI* m_pList;
	CControlUI* m_pLastKsBtn;
	CHorizontalLayoutUI* m_addnewgame;
	CButtonUI* m_pBtntx;
	CLabelUI* m_pLblNickName;
	//CDataSync *m_pDSync;

};
