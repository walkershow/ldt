#include "FrameWnd.h"
#include "resource.h"

CString GetLocalVersion()
{
	return "1.0";
}


static bool is_user_login(const CString& userid)
{
	
	CString MemMapFileName=_T("joysw_")+userid;
	//if (GetFileAttributes(MemMapFileName) != INVALID_FILE_ATTRIBUTES)
	//{
	HANDLE hFileMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, MemMapFileName);
	if (hFileMap)
	{
		TCHAR* ptChar = (TCHAR* )MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		CString strTmp(ptChar);
		if (strTmp != _T("SMT"))
		{
			//::MessageBox(m_hWnd,_T("该账号已经在本机登录，不能重复登录！"),GetLangString("IDS_PRODUCTNAME"),MB_OK | MB_ICONINFORMATION);
			return false;
		}
	}
	else
	{
		return false;
	}
	return true;
}



int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
// 	CPaintManagerUI::SetResourceZip(_T("testui.zip"));
	if(::__argc <2)
	{
		return 0;
	}
	CString strUserID = ::__targv[1];
	CString strUserAcct = ::__targv[2];
	if(!is_user_login(strUserAcct))
	{
		//return 0;
	}
	g_strUserID = strUserID;
	g_strUserAcct = strUserAcct;

	::CoInitialize(NULL);
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());

	CFrameWnd *pFrame = new CFrameWnd(_T("UISkin2.xml"));
	pFrame->Create(NULL, _T("6A游戏"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	pFrame->ShowModal();
	pFrame->SetIcon(IDI_ICON1);
	
	delete pFrame;
	::CoUninitialize();
	return 0;
}
