#pragma once
#include "duilib.h"
#include <string>
#include <comdef.h>
#include <locale.h> 
#include "resource.h"
using namespace std;

// 以XML生成界面的窗口基类
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


// 将HWND显示到CControlUI上面
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



// 需包含locale、string头文件、使用setlocale函数。
static std::wstring StringToWstring(const std::string str)
{// string转wstring
	unsigned len = str.size() * 2;// 预留字节数
	setlocale(LC_CTYPE, "");     //必须调用此函数
	wchar_t *p = new wchar_t[len];// 申请一段内存存放转换后的字符串
	mbstowcs(p,str.c_str(),len);// 转换
	std::wstring str1(p);
	delete[] p;// 释放申请的内存
	return str1;
}
