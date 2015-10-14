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
extern char* UnicodeToUtf8( WCHAR*  wszUtf8,int &len);
extern void SendRequest();
extern void SendReq2();
extern bool CreateMultipleDirectory(const CString& szPath);
extern int GetYearIdx(int n);
extern int GetMonIdx(int n);
extern int GetDayIdx(int n);

