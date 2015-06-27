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
	//char szExeDir[MAX_PATH];//�õ�EXE����Ŀ¼������"\"��
	//�õ������ģ����
	HMODULE hMyModule = GetModuleHandleA(NULL);
	//�õ������·��
	DWORD dwLen = GetModuleFileNameA(hMyModule,szEexFullpath,MAX_PATH);
	//��������·��(�����������һ��"\")
	char * pPatch = &szEexFullpath[dwLen]; //szexepatch + dwLen;
	while (pPatch!=NULL && *pPatch!='\\')
	{
		pPatch--;
	}
	//�õ����һ����б��֮���������1,����������Ϊ0,������������ݶ����ַ�����ͻ����
	pPatch++;
	*pPatch=0;

	strcpy_s(pszExeDir, MAX_PATH+1, szEexFullpath);
	return true;
}