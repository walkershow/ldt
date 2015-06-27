#pragma once
#include "duilib.h"
#include <string>
#include <comdef.h>

using namespace std;

class CFrameWnd: public CXMLWnd, public CWebBrowserEventHandler
{
public:
	explicit CFrameWnd(LPCTSTR pszXMLPath);

	virtual void InitWindow();
	virtual void Notify(TNotifyUI& msg);
	virtual CControlUI* CreateControl(LPCTSTR pstrClassName);
	HRESULT STDMETHODCALLTYPE GetHostInfo(DOCHOSTUIINFO __RPC_FAR *pInfo);
private:
	CWebBrowserUI* pWebBrowser;
	CButtonUI* m_pLastClickBtn;
	CDuiString m_lastClickBtn_HotImage;
	string m_shuoshuo;
	string m_shipin;
	string m_luntan;
	string m_zixun;
	string m_shouye;
	string m_yxk;
	string m_yxq;
	string m_dspw;
	string m_jysc;
	string m_start;
};
static bool GetCurExeDir(char* pszExeDir)
{
	char szEexFullpath[MAX_PATH];
	//char szExeDir[MAX_PATH];//得到EXE所有目录（包含"\"）
	//得到程序的模块句柄
	HMODULE hMyModule = GetModuleHandleA(NULL);
	//得到程序的路径
	DWORD dwLen = GetModuleFileNameA(hMyModule,szEexFullpath,MAX_PATH);
	//计算程序的路径(倒数查找最后一个"\")
	char * pPatch = &szEexFullpath[dwLen]; //szexepatch + dwLen;
	while (pPatch!=NULL && *pPatch!='\\')
	{
		pPatch--;
	}
	//得到最后一个反斜扛之后再往后加1,并把它设置为0,这样后面的数据对于字符数组就会忽略
	pPatch++;
	*pPatch=0;

	strcpy_s(pszExeDir, MAX_PATH+1, szEexFullpath);
	return true;
}