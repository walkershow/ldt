#pragma once
#include "duilib.h"
#include <string>
#include <comdef.h>
#include <locale.h> 
#include "resource.h"
using namespace std;

// ��XML���ɽ���Ĵ��ڻ���
class CXMLWnd : public WindowImplBase
{
public:
	explicit CXMLWnd(LPCTSTR pszXMLPath) 
		: m_strXMLPath(pszXMLPath){}

public:
	virtual LPCTSTR GetWindowClassName() const
	{
		return _T("XMLWnd");
	}

	virtual CDuiString GetSkinFile()
	{
		return m_strXMLPath;
	}

	virtual CDuiString GetSkinFolder()
	{
		return _T("images");
	}
	virtual UILIB_RESOURCETYPE GetResourceType() const
	{
		return UILIB_ZIPRESOURCE;
	}
	virtual LPCTSTR GetResourceID() const
	{
		return MAKEINTRESOURCE(IDR_ZIPRES1);
	}

protected:
	CDuiString m_strXMLPath;
};


// ��HWND��ʾ��CControlUI����
class CWndUI: public CControlUI
{
public:
	CWndUI(): m_hWnd(NULL){}

	virtual void SetInternVisible(bool bVisible = true)
	{
		__super::SetInternVisible(bVisible);
		::ShowWindow(m_hWnd, bVisible);
	}

	virtual void SetPos(RECT rc)
	{
		__super::SetPos(rc);
		::SetWindowPos(m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
	}

	BOOL Attach(HWND hWndNew)
	{
		if (! ::IsWindow(hWndNew))
		{
			return FALSE;
		}

		m_hWnd = hWndNew;
		return TRUE;
	}

	HWND Detach()
	{
		HWND hWnd = m_hWnd;
		m_hWnd = NULL;
		return hWnd;
	}

protected:
	HWND m_hWnd;
};

class CFrameWnd: public CXMLWnd, public CWebBrowserEventHandler
{
public:
	explicit CFrameWnd(LPCTSTR pszXMLPath);

	virtual void InitWindow();
	virtual void Notify(TNotifyUI& msg);
	virtual CControlUI* CreateControl(LPCTSTR pstrClassName);
	HRESULT STDMETHODCALLTYPE GetHostInfo(DOCHOSTUIINFO __RPC_FAR *pInfo);
	void WriteWebBrowserRegKey(LPCTSTR lpKey, DWORD dwValue);
	void feature_browser();
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



// �����locale��stringͷ�ļ���ʹ��setlocale������
static std::wstring StringToWstring(const std::string str)
{// stringתwstring
	unsigned len = str.size() * 2;// Ԥ���ֽ���
	setlocale(LC_CTYPE, "");     //������ô˺���
	wchar_t *p = new wchar_t[len];// ����һ���ڴ���ת������ַ���
	mbstowcs(p,str.c_str(),len);// ת��
	std::wstring str1(p);
	delete[] p;// �ͷ�������ڴ�
	return str1;
}
