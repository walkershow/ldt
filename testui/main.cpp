#include "FrameWnd.h"
#include "resource.h"
#include "GameManage.h"
#include "DataSync.h"
#include "helper.h"
CString g_server;
int g_port;
int g_runtimes;
CDataSync *g_pDSync  ;
CBGThread *g_pBGTread;

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

CString CreateUserDB(CString appPath)
{
	CString strModPath = appPath + SCHEMA_FILENAME;
	CString strUserDbDir =  appPath + USER_DBDIR;
	CString strUserDBPath = strUserDbDir + g_strUserID;
	if(!PathFileExists(strUserDBPath))
	{
		bool bRet = CreateMultipleDirectory(strUserDbDir);
		if(!bRet) 
		{
		//log
			exit(-1);
		}
		else SetFileAttributes(strUserDbDir, FILE_ATTRIBUTE_HIDDEN ); 
		CopyFile(strModPath,strUserDBPath, true); 
		CGameManage::GetInstance().Open(strUserDBPath);
		CString pwd = _T("");
		CGameManage::GetInstance().ResetPassword(pwd,pwd.GetLength());
		CGameManage::GetInstance().Close();
	}
	return strUserDBPath;
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
// 	g_strUserID = _T("100");
// 	g_strUserAcct = _T("test200");

	::CoInitialize(NULL);
	
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	CDuiString str = CPaintManagerUI::GetInstancePath();
	CString strPath = str ;

	CString userDb = CreateUserDB(strPath);
	CGameManage::GetInstance().Open(userDb);
	CString pwd = _T("");
	CGameManage::GetInstance().SetPassword(pwd,pwd.GetLength());
	
	bool bRet = CGameManage::GetInstance().GetSysConfig(g_server, g_port, g_runtimes);
	if(!bRet)
	{
		::MessageBox(NULL,_T("初始化错误"),_T("错误"),MB_OK);
		exit(1);
	}
	CGameManage::GetInstance().CreateUser();
	CFrameWnd *pFrame = new CFrameWnd(_T("UISkin2.xml"));
	pFrame->Create(NULL, _T("6A游戏数据研究中心"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);

	pFrame->ShowModal();
	pFrame->SetIcon(IDI_ICON1);
	
	delete pFrame;
	::CoUninitialize();
	CGameManage::GetInstance().Close();
	delete g_pDSync;
	g_pDSync = NULL;
	return 0;
}
