#pragma once

#include "duilib.h"
#include <vector>
#include "Date.h"
using namespace  std;

class CTaskWnd: public CXMLWnd, public CWebBrowserEventHandler
{
public:
	CTaskWnd(LPCTSTR pszXMLPath);
	~CTaskWnd(void);
	void OnFinalMessage( HWND hWnd);
	LPCTSTR GetWindowClassName( void ) const;
	void InitWindow();
	void BeforeNavigate2( IDispatch *pDisp,VARIANT *&url,VARIANT *&Flags,VARIANT *&TargetFrameName,VARIANT *&PostData,VARIANT *&Headers,VARIANT_BOOL *&Cancel );
	HRESULT STDMETHODCALLTYPE GetHostInfo( DOCHOSTUIINFO __RPC_FAR *pInfo);
	void SetParent(HWND hWnd);

private:
	CWebBrowserUI* m_pWebBrowser;
	HWND m_parentHwnd;
};
