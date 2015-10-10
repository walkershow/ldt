#pragma once
#include "duilib.h"
#include <WinInet.h>
//#include "curl/curl.h"
#pragma comment(lib, "WinInet.lib")
//#pragma comment(lib, "LIBCURL.lib")
extern CString ShowOpenFileDialog(HWND hParent);
extern void WebReadFile(CString sUrl,char*& data);

extern int test();
extern CString QueryInfo(HINTERNET hRequest, DWORD dwInfoLevel);
extern CString RealWebFile(CString sUrl);
extern void UTF8toANSI(CString &strUTF8);
extern CString UTF8ToUnicode(char* UTF8);
extern void SendRequest();
extern void SendReq2();
extern bool CreateMultipleDirectory(const CString& szPath);

class CHttpHelper
{
public:
	virtual void __stdcall Inet_Callback(HINTERNET hInternet,
		DWORD dwContext,
		DWORD dwInternetStatus,
		LPVOID lpStatusInfo,
		DWORD dwStatusInfoLen);
	CHttpHelper();
	~CHttpHelper(){};	
	bool GetData(const CString& sUrl, char* buf, DWORD dwBuffer);
	bool PostData(const CString& sUrl, char* data);
	CString CrackUrl(CString sUrl);
	CString DownloadFile(const CString& surl);

	HANDLE m_hConnectedEvent;
	HANDLE m_hRequestOpenedEvent;
	HANDLE m_hRequestCompleteEvent;
	bool m_bAllDone ;
	bool m_bVerbose ;
	bool m_bError;

};