#pragma once
#include "duilib.h"
#include <WinInet.h>
#pragma comment(lib, "WinInet.lib")
extern CString ShowOpenFileDialog(HWND hParent);

extern int test();
extern void UTF8toANSI(CString &strUTF8,string& strAnsi);
extern CString UTF8ToUnicode(char* UTF8);
extern char* UnicodeToUtf8( WCHAR*  wszUtf8,int &len);
extern bool CreateMultipleDirectory(const CString& szPath);
extern HWND GetWinHandle(long Inst);
extern long StartProc(CString path);
extern string GetJbTemplateContent(CString path);
extern void ReplaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace);
extern CString GetCurDir();
extern CString GetDir(CString appPath);
extern bool WriteJbFile(CString jbPath,const string& str);
extern std::string ReplaceAll(std::string str, const std::string& from, const std::string& to);
extern string GetJbTemplateContent2(CString path);