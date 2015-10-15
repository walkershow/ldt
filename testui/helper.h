#pragma once
#include "duilib.h"
#include <WinInet.h>
#pragma comment(lib, "WinInet.lib")
extern CString ShowOpenFileDialog(HWND hParent);

extern int test();
extern void UTF8toANSI(CString &strUTF8);
extern CString UTF8ToUnicode(char* UTF8);
extern char* UnicodeToUtf8( WCHAR*  wszUtf8,int &len);
extern bool CreateMultipleDirectory(const CString& szPath);


