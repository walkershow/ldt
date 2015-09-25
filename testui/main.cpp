#include "FrameWnd.h"
#include "resource.h"
#include "GameManage.h"
#include "DataSync.h"
//#include "md52.h"
#include "md5.h"
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
	if(::__argc <3)
	{
		return 0;
	}
	CString strUserID = ::__targv[1];
	CString strUserAcct = ::__targv[2];
	CString strDBPath = ::__targv[3];
	if(!is_user_login(strUserAcct))
	{
		//return 0;
	}
	g_strUserID = strUserID;
	g_strUserAcct = strUserAcct;

	::CoInitialize(NULL);
	

	//SQLite.Instance().Open(strDBPath);

	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	CDuiString str = CPaintManagerUI::GetInstancePath();
	CString strPath = str ;
	strPath += strDBPath;
	CGameManage::GetInstance().Open(strPath);
	MD5 md5;
	string str22 = md5.digestFile( _T("d:\\appcfg.ini" ));
 	//MD5 md5;
// 	md5.reset(); 
// 	md5.update("HELLO THERE I AM MD5!");
// 	//md5.update(ifstream(_T("D:\\为Win7安装软件和优化.exe")), ios::binary);
// 	const byte* dig = md5.digest();
	//md5.update( ifstream("d:\\appcfg.ini" ), ios::binary);
 	//string digest = md5.toString();
// 	CDataSync ds(_T("192.168.1.62"), 80, g_strUserID);
// 	int id = 0;
// 	CString name2, iconpath;
// 	//ds.SyncUserInfo(id , name2, iconpath);
// 	ds.GetGame_Manage_ByGameID(-1);

	CFrameWnd *pFrame = new CFrameWnd(_T("UISkin2.xml"));
	pFrame->Create(NULL, _T("6A游戏"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);

	pFrame->ShowModal();
	//pFrame->SetWbFocus(pFrame->pINDEX);

	pFrame->SetIcon(IDI_ICON1);
	
	delete pFrame;
	::CoUninitialize();
	CGameManage::GetInstance().Close();
	return 0;
}
