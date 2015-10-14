#include "helper.h"
#include <string>
#include "Date.h"
using namespace std;
CString ShowOpenFileDialog(HWND hParent)
{
	TCHAR FilePathName[260] ={0};
	OPENFILENAME ofn ;
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFilter = _T("All Files\0*.*\0EXE Files\0*.exe\\0\0");
	ofn.lpstrFile = FilePathName;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = MAX_PATH;
	ofn.hwndOwner = hParent;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	bool bRet = GetOpenFileName( &ofn );  
	if(bRet)
	{
		return ofn.lpstrFile;
	}
	else
	{
		return "";
	}
}

bool CreateMultipleDirectory(const CString& szPath)
{
	CString strDir(szPath);//存放要创建的目录字符串
	//确保以'\'结尾以创建最后一个目录
// 	if (strDir.GetAt(strDir.GetLength()-1)!=_T('\\'))
// 	{
// 		strDir.AppendChar(_T('\\'));
// 	}
	std::vector<CString> vPath;//存放每一层目录字符串
	CString strTemp;//一个临时变量,存放目录字符串
	bool bSuccess = false;//成功标志
	//遍历要创建的字符串
	for (int i=0;i<strDir.GetLength();++i)
	{
		if (strDir.GetAt(i) != _T('\\')) 
		{//如果当前字符不是'\\'
			strTemp.AppendChar(strDir.GetAt(i));
		}
		else 
		{//如果当前字符是'\\'
			vPath.push_back(strTemp);//将当前层的字符串添加到数组中
			strTemp.AppendChar(_T('\\'));
		}
	}

	//遍历存放目录的数组,创建每层目录
	std::vector<CString>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++) 
	{
		//如果CreateDirectory执行成功,返回true,否则返回false
		if(PathIsDirectory(*vIter))
		{
			bSuccess = true;
		}
		else
		{
			bSuccess = CreateDirectory(*vIter, NULL) ? true : false;    
		}
	}

	return bSuccess;
}
// void AddNewGame()
// {
// 	MD5 md5;
// 	string digest = md5.digestFile((LPTSTR)(LPCTSTR) filePath );
// 	CDataSync ds(_T("192.168.1.62"), 80, g_strUserID);
// 	ds.GetProg_to_Game_ByProgmd5((LPSTR)digest.c_str());
// }
//UTF8转ANSI
void UTF8toANSI(CString &strUTF8)
{
	//获取转换为多字节后需要的缓冲区大小，创建多字节缓冲区
	UINT nLen = MultiByteToWideChar(CP_UTF8,NULL,(LPCSTR)(LPCTSTR)strUTF8,-1,NULL,NULL);
	WCHAR *wszBuffer = new WCHAR[nLen+1];
	nLen = MultiByteToWideChar(CP_UTF8,NULL,(LPCSTR)(LPCTSTR)strUTF8,-1,wszBuffer,nLen);
	wszBuffer[nLen] = 0;

	nLen = WideCharToMultiByte(936,NULL,wszBuffer,-1,NULL,NULL,NULL,NULL);
	CHAR *szBuffer = new CHAR[nLen+1];
	nLen = WideCharToMultiByte(936,NULL,wszBuffer,-1,szBuffer,nLen,NULL,NULL);
	szBuffer[nLen] = 0;

	strUTF8 = szBuffer;
	//清理内存
	delete []szBuffer;
	delete []wszBuffer;
}

CString UTF8ToUnicode(char* UTF8)

{

	DWORD dwUnicodeLen;        //转换后Unicode的长度

	TCHAR *pwText;            //保存Unicode的指针

	CString strUnicode;        //返回值

	//获得转换后的长度，并分配内存

	dwUnicodeLen = MultiByteToWideChar(CP_UTF8,0,UTF8,-1,NULL,0);

	pwText = new TCHAR[dwUnicodeLen];

	if (!pwText)

	{

		return strUnicode;

	}

	//转为Unicode

	MultiByteToWideChar(CP_UTF8,0,UTF8,-1,pwText,dwUnicodeLen);

	//转为CString

	strUnicode.Format(_T("%s"),pwText);

	//清除内存

	delete []pwText;

	//返回转换好的Unicode字串

	return strUnicode;

}

char* UnicodeToUtf8( WCHAR*  wszUtf8,int &len)
{
	len= WideCharToMultiByte(CP_UTF8,0, wszUtf8,-1, NULL,0, NULL, NULL);
	char* szUtf8=new char[len+1];
	memset(szUtf8,0, len+1);
	WideCharToMultiByte (CP_UTF8,0, wszUtf8,-1, szUtf8, len, NULL,NULL);
	return szUtf8;
}

void SendReq2()
{
	static TCHAR hdrs[] = _T("Content-Type: application/x-www-form-urlencoded");

	static TCHAR frmdata[] = _T("name=John Doe&userid=hithere&other=P&Q");
	LPCTSTR accept[2]={_T("*/*"), NULL};
    
// for clarity, error-checking has been removed
	HINTERNET hSession = InternetOpen(NULL,INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	HINTERNET hConnect = InternetConnect(hSession, _T("192.168.1.62"),
										 80,
										 NULL,
										 NULL,
										 INTERNET_SERVICE_HTTP,
										 0,
										 1);
	HINTERNET hRequest = HttpOpenRequest(hConnect, 
										 _T("POST"),
										 _T("/USER?ID=1"), 
										 NULL, 
										 NULL, 
										 accept, 
										 0, 
										 1);
	BOOL isSend = HttpSendRequest(hRequest, 
					hdrs, 
					_tcslen(hdrs), 
					frmdata, 
					_tcslen(frmdata));
	if(!isSend){
		CString strError;
		strError.Format(_T("HttpSendRequest failed, code=%d"),GetLastError());
		return ;
	}
}

void SendRequest()
{
	TCHAR* szHeaders =  _T("Content-Type:application/json\r\n");
	char* szPostData = ("{\"command\":\"remotecontrol\",\"method\":\"countmon.getgatestatus\",\"param\":\"2\"}");

	LPCTSTR accept[2]={_T("*/*"), NULL};
	HINTERNET hSession = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0); 
	CString sUrl =  _T("192.168.1.128");
// 	if (hSession)
// 	{
		DWORD dwTimeOut = 0;
		//InternetSetOption(hConnect, INTERNET_OPTION_CONNECT_TIMEOUT, &dwTimeOut, sizeof(dwTimeOut));

// 		HINTERNET hSession = InternetOpenUrl(hConnect, sUrl, NULL, 0, INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_PRAGMA_NOCACHE, 0);
// 		if (hSession)
// 		{
	//HINTERNET hSession = InternetOpen(NULL,INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	HINTERNET hConnect = InternetConnect(hSession, _T("192.168.1.62"),
		80,
		NULL,
		NULL,
		INTERNET_SERVICE_HTTP,
		0,
		1);
		HINTERNET hRequest = HttpOpenRequest(hConnect, 
			_T("POST"),
			_T("/user?name=2"), 
			NULL, 
			NULL, 
			accept, 
			0, 
			1);
		BOOL isSend = HttpSendRequest(hRequest, 
			szHeaders, 
			_tcslen(szHeaders), 
			szPostData, 
			strlen(szPostData));
		if(!isSend){
			CString strError;
			strError.Format(_T("HttpSendRequest failed, code=%d"),GetLastError());
			//printf("HttpSendRequest failed, code=%d",GetLastError());
			//system("pause>nul");
			return ;
	}
	//	}
// 	}
	char szData[1024]={0};
	CString str;
	if (isSend)
	{
		for(;;)
		{
			// reading data
			DWORD dwByteRead;
			BOOL isRead = ::InternetReadFile(hRequest, 
				szData, sizeof(szData) - 1, &dwByteRead);

			// break cycle if error or end
			if (isRead == FALSE || dwByteRead == 0)
				break;
			str = UTF8ToUnicode(szData);
			//str = szData;
			// saving result
			//str.append(szData, dwByteRead);
			szData[dwByteRead] = 0;
		}
	}


}
CString QueryInfo(HINTERNET hRequest, DWORD dwInfoLevel)
{
	CString sInfo;
	DWORD dwLen = 0;
	if (!HttpQueryInfo (hRequest, dwInfoLevel, NULL, &dwLen, 0) && dwLen)
	{
		void * buf = malloc(dwLen + 2);
		ZeroMemory (buf, dwLen + 2);
		::HttpQueryInfo (hRequest, dwInfoLevel, buf, &dwLen, 0);
		sInfo = (LPCTSTR)buf;
		free(buf);
	}
	return sInfo;
}

CString RealWebFile(CString sUrl)
{
	CString sRealUrl = sUrl;
	// 打开http链接
	HINTERNET hConnect = InternetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0); 

	if (hConnect)
	{
		DWORD dwTimeOut = 0;
		InternetSetOption(hConnect, INTERNET_OPTION_CONNECT_TIMEOUT, &dwTimeOut, sizeof(dwTimeOut));

		HINTERNET hSession = InternetOpenUrl(hConnect, sUrl, NULL, 0, INTERNET_FLAG_NO_AUTO_REDIRECT | INTERNET_FLAG_RELOAD, 0);
		if (hSession)
		{
			DWORD dwStatus = 0;
			DWORD dwBuffLen = sizeof(dwStatus);
			BOOL bSuccess = HttpQueryInfo(hSession, HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER, &dwStatus, &dwBuffLen, 0);
			if(bSuccess) {
				sRealUrl = QueryInfo(hSession, HTTP_QUERY_LOCATION);
			}

			InternetCloseHandle(hSession);
		}
		InternetCloseHandle(hConnect);
	}

	return sRealUrl;
}
void WebReadFile(CString sUrl,char*& data)
{
	//CString data;
	// 打开http链接
	HINTERNET hConnect = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0); 

	if (hConnect)
	{
		DWORD dwTimeOut = 0;
		InternetSetOption(hConnect, INTERNET_OPTION_CONNECT_TIMEOUT, &dwTimeOut, sizeof(dwTimeOut));

		HINTERNET hSession = InternetOpenUrl(hConnect, sUrl, NULL, 0, INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_PRAGMA_NOCACHE, 0);
		if (hSession)
		{
			// 建立数据缓冲区
			DWORD dwRead = 0;
			DWORD dwBuffer = 1024 * 1024;
			char *szBuffer = new char[dwBuffer];
			memset(szBuffer, 0, dwBuffer);

			if(InternetReadFile(hSession, szBuffer, dwBuffer, &dwRead) && (dwRead > 0))
			{
				int nLen = dwRead;
				char* pBuffer = new char[nLen + 1];
				memset(pBuffer, 0, nLen + 1);
				memcpy(pBuffer, szBuffer, nLen);
				if(pBuffer != NULL)
				{
					data = pBuffer;
				}
			}

			// 销毁数据缓冲区
		//	delete []szBuffer;
		//	szBuffer = NULL;

			InternetCloseHandle(hSession);
		}
		InternetCloseHandle(hConnect);
	}

	//return data;
}

CString CrackUrl(CString sUrl)
{
	URL_COMPONENTS uc;
	TCHAR Scheme[1000];
	TCHAR HostName[1000];
	TCHAR UserName[1000];
	TCHAR Password[1000];
	TCHAR UrlPath[1000];
	TCHAR ExtraInfo[1000];

	uc.dwStructSize = sizeof(uc);
	uc.lpszScheme = Scheme;
	uc.lpszHostName = HostName;
	uc.lpszUserName = UserName;
	uc.lpszPassword = Password;
	uc.lpszUrlPath = UrlPath;
	uc.lpszExtraInfo = ExtraInfo;

	uc.dwSchemeLength = 1000;
	uc.dwHostNameLength = 1000;
	uc.dwUserNameLength = 1000;
	uc.dwPasswordLength = 1000;
	uc.dwUrlPathLength = 1000;
	uc.dwExtraInfoLength = 1000;

	InternetCrackUrl(sUrl, 0, 0, &uc);
	return UrlPath;
}

CString DownloadFile(const CString& surl)
{
	CString sAppPath = CPaintManagerUI::GetInstancePath().GetData();
	CString sImageName = CrackUrl(surl);
	sImageName.Replace(_T("/"), _T("\\"));
	TCHAR szFile[MAX_PATH] = {0};
	lstrcpy(szFile, sAppPath);
	::PathAppend(szFile, sImageName);
	if(!::PathFileExists(szFile))
	{
		CreateDirectory(szFile, NULL);
		::URLDownloadToFile(NULL, surl, szFile, 0, NULL);
	}
	return szFile;

}


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>


size_t get_wchar_size(const char *str)
{
	size_t len = strlen(str);
	size_t size=0;
	size_t i;
	for(i=0; i < len; i++)
	{
		if( str[size] >= 0 && str[size] <= 127 ) //不是全角字符
			size+=sizeof(wchar_t);
		else //是全角字符，是中文
		{
			size+=sizeof(wchar_t);
			i+=2;
		}
	}
	return size;
}

char *w2c(const wchar_t *pw)
{
	if(!pw)
		return NULL;

	size_t size= wcslen(pw)*sizeof(wchar_t);
	char *pc;
	if(!(pc = (char*)malloc(size)))
	{
		printf("malloc fail");
		return NULL;
	}
	wcstombs(pc,pw,size);
	return pc;

}

wchar_t *c2w(const char *pc)
{
	if(!pc)
		return NULL;

	size_t size_of_ch = strlen(pc)*sizeof(char);
	size_t size_of_wc = get_wchar_size(pc);
	wchar_t *pw;
	if(!(pw = (wchar_t*)malloc(size_of_wc)))
	{
		printf("malloc fail");
		return NULL;
	}
	mbstowcs(pw,pc,size_of_wc);
	return pw;

}

extern int GetYearIdx(int n)
{
	int curyear = CDate::GetCurDate().GetYear();
	return curyear-n;
}

extern int GetMonIdx(int n)
{
	return n-1;
}

extern int GetDayIdx(int n)
{
	return n-1;
}

// int main(void)
// {
// 	setlocale(LC_ALL,"zh_CN.utf8");
// 	printf("1 print chinese by wprintf test: \n");
// 	wchar_t *wstr = L"中文";
// 	//wprintf(L"%ls",wstr); //此处不要使用wprintf，因为上面已经使用了printf，而他们使用的是不同的流，不能混用
// 	printf("%ls\n",wstr); //ls
// 
// 	printf("2 print chinese by printf test: \n");
// 	char *str = "汉字";
// 	printf("%s\n\n\n",str); //s
// 
// 	printf("3 char and wchar_t size of system test: \n");
// 	printf("%s%d\n","the size of wchar_t is : ",sizeof(wchar_t)); //4
// 	printf("%s%d\n\n\n","the size of char is : ",sizeof(char));   //1
// 
// 	char *pc;
// 	wchar_t *pw = L"中文是abc一门语言abc";
// 	char *tmp = "中文是abc一门语言abc";
// 	printf("%s%s\n","input test wchar_t* ",tmp);
// 	pc = w2c(pw);
// 	printf("4 print char test after w2c (wchar to char):\n");
// 	printf("%s\n",pc);
// 
// 
// 	wchar_t *cw1 = c2w(pc);   //char to wchar
// 	char *wc1 = w2c(cw1);   //wchar to char
// 	printf("5 print char test after w2c c2w and w2c:\n");
// 	printf("%s\n\n\n",wc1);
// 
// 	char *pmc = "abc中文abc";
// 	wchar_t *pmw;
// 	printf("%s%s\n","input test char* ",pmc);
// 	pmw = c2w(pmc);
// 	char *pmc1 = w2c(pmw);
// 	printf("6 print char test after c2w and w2c:\n");
// 	printf("%s\n",pmc1);
// 
// 	printf("%ls\n",wstr);
// 	//cout<<wstr<<endl;
// 	return 0;
// 
// } 


