#include "helper.h"
#include <string>
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
	CString strDir(szPath);//���Ҫ������Ŀ¼�ַ���
	//ȷ����'\'��β�Դ������һ��Ŀ¼
// 	if (strDir.GetAt(strDir.GetLength()-1)!=_T('\\'))
// 	{
// 		strDir.AppendChar(_T('\\'));
// 	}
	std::vector<CString> vPath;//���ÿһ��Ŀ¼�ַ���
	CString strTemp;//һ����ʱ����,���Ŀ¼�ַ���
	bool bSuccess = false;//�ɹ���־
	//����Ҫ�������ַ���
	for (int i=0;i<strDir.GetLength();++i)
	{
		if (strDir.GetAt(i) != _T('\\')) 
		{//�����ǰ�ַ�����'\\'
			strTemp.AppendChar(strDir.GetAt(i));
		}
		else 
		{//�����ǰ�ַ���'\\'
			vPath.push_back(strTemp);//����ǰ����ַ������ӵ�������
			strTemp.AppendChar(_T('\\'));
		}
	}

	//�������Ŀ¼������,����ÿ��Ŀ¼
	std::vector<CString>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++) 
	{
		//���CreateDirectoryִ�гɹ�,����true,���򷵻�false
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
//UTF8תANSI
void UTF8toANSI(CString &strUTF8)
{
	//��ȡת��Ϊ���ֽں���Ҫ�Ļ�������С���������ֽڻ�����
	UINT nLen = MultiByteToWideChar(CP_UTF8,NULL,(LPCSTR)(LPCTSTR)strUTF8,-1,NULL,NULL);
	WCHAR *wszBuffer = new WCHAR[nLen+1];
	nLen = MultiByteToWideChar(CP_UTF8,NULL,(LPCSTR)(LPCTSTR)strUTF8,-1,wszBuffer,nLen);
	wszBuffer[nLen] = 0;

	nLen = WideCharToMultiByte(936,NULL,wszBuffer,-1,NULL,NULL,NULL,NULL);
	CHAR *szBuffer = new CHAR[nLen+1];
	nLen = WideCharToMultiByte(936,NULL,wszBuffer,-1,szBuffer,nLen,NULL,NULL);
	szBuffer[nLen] = 0;

	strUTF8 = szBuffer;
	//�����ڴ�
	delete []szBuffer;
	delete []wszBuffer;
}

CString UTF8ToUnicode(char* UTF8)

{

	DWORD dwUnicodeLen;        //ת����Unicode�ĳ���

	TCHAR *pwText;            //����Unicode��ָ��

	CString strUnicode;        //����ֵ

	//���ת����ĳ��ȣ��������ڴ�

	dwUnicodeLen = MultiByteToWideChar(CP_UTF8,0,UTF8,-1,NULL,0);

	pwText = new TCHAR[dwUnicodeLen];

	if (!pwText)

	{

		return strUnicode;

	}

	//תΪUnicode

	MultiByteToWideChar(CP_UTF8,0,UTF8,-1,pwText,dwUnicodeLen);

	//תΪCString

	strUnicode.Format(_T("%s"),pwText);

	//����ڴ�

	delete []pwText;

	//����ת���õ�Unicode�ִ�

	return strUnicode;

}

char* UnicodeToUtf8( WCHAR*  wszUtf8)
{
	int len= WideCharToMultiByte(CP_UTF8,0, wszUtf8,-1, NULL,0, NULL, NULL);
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
	// ��http����
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
	// ��http����
	HINTERNET hConnect = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0); 

	if (hConnect)
	{
		DWORD dwTimeOut = 0;
		InternetSetOption(hConnect, INTERNET_OPTION_CONNECT_TIMEOUT, &dwTimeOut, sizeof(dwTimeOut));

		HINTERNET hSession = InternetOpenUrl(hConnect, sUrl, NULL, 0, INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_PRAGMA_NOCACHE, 0);
		if (hSession)
		{
			// �������ݻ�����
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

			// �������ݻ�����
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
		if( str[size] >= 0 && str[size] <= 127 ) //����ȫ���ַ�
			size+=sizeof(wchar_t);
		else //��ȫ���ַ���������
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

// int main(void)
// {
// 	setlocale(LC_ALL,"zh_CN.utf8");
// 	printf("1 print chinese by wprintf test: \n");
// 	wchar_t *wstr = L"����";
// 	//wprintf(L"%ls",wstr); //�˴���Ҫʹ��wprintf����Ϊ�����Ѿ�ʹ����printf��������ʹ�õ��ǲ�ͬ���������ܻ���
// 	printf("%ls\n",wstr); //ls
// 
// 	printf("2 print chinese by printf test: \n");
// 	char *str = "����";
// 	printf("%s\n\n\n",str); //s
// 
// 	printf("3 char and wchar_t size of system test: \n");
// 	printf("%s%d\n","the size of wchar_t is : ",sizeof(wchar_t)); //4
// 	printf("%s%d\n\n\n","the size of char is : ",sizeof(char));   //1
// 
// 	char *pc;
// 	wchar_t *pw = L"������abcһ������abc";
// 	char *tmp = "������abcһ������abc";
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
// 	char *pmc = "abc����abc";
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


FILE *fp;
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) //���������Ϊ�˷���CURLOPT_WRITEFUNCTION, �������
{
	CString sInfo = (LPCTSTR)ptr;
	wchar_t *p = (wchar_t *)ptr;
	wchar_t *cw1 = c2w((const char*)ptr);   //char to wchar
	char *buf = new char[nmemb+1];
	memcpy(buf, ptr, nmemb);
	buf[nmemb]=_T('\0');
	//TCHAR *buf = (TCHAR*)ptr;
	CString str= buf;
		int written = fwrite(ptr, size, nmemb, (FILE *)fp);
	return written;
}

// int test()
// {
// 	CURL *curl;
// 
// 	curl_global_init(CURL_GLOBAL_ALL); 
// 	curl=curl_easy_init();
// 	curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.1.128:8080/user?id=1");
// 	if((fp=fopen("d:\\1.txt","w"))==NULL)
// 	{
// 		curl_easy_cleanup(curl);
// 		exit(1);
// 	}
// 	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); //CURLOPT_WRITEFUNCTION ����̵Ķ�������write_data��������
// 	CURLcode res = curl_easy_perform(curl);
// 	if(CURLE_OK != res)
// 		fprintf(stderr, "curl told us %d\n", res);
// 	curl_easy_cleanup(curl);
// 	exit(0);
//}


bool CHttpHelper::GetData(const CString& sUrl, char* szBuffer, DWORD dwBuffer)
{
	//CString data;
	bool bRet = false;

	HINTERNET hInstance = InternetOpen(_T("asynchttp"),
		INTERNET_OPEN_TYPE_PRECONFIG,
		NULL,
		NULL,
		INTERNET_FLAG_ASYNC); // ASYNC Flag

	if (hInstance == NULL)
	{
		cout << "InternetOpen failed, error " << GetLastError();
		return 0;
	}
	unsigned long pThis = (unsigned long) this;
	INTERNET_STATUS_CALLBACK TempFuncAddr = NULL;
	_asm
	{
		mov eax,pThis;
		mov edx,dword ptr [eax];
		mov eax, dword ptr [edx+0D8h];
		mov TempFuncAddr,eax;
	}
	// Setup callback function
	if (InternetSetStatusCallback(hInstance,
		(INTERNET_STATUS_CALLBACK)&TempFuncAddr) == INTERNET_INVALID_STATUS_CALLBACK)
	{
		cout << "InternetSetStatusCallback failed, error " << GetLastError();
		return 0;
	}

	// First call that will actually complete asynchronously even
	// though there is no network traffic
	HINTERNET hConnect = InternetConnect(hInstance, 
		_T("192.168.1.242"), 
		INTERNET_DEFAULT_HTTP_PORT,
		NULL,
		NULL,
		INTERNET_SERVICE_HTTP,
		0,
		1); // Connection handle's Context
	if (hConnect == NULL)
	{
		if (GetLastError() != ERROR_IO_PENDING)
		{
			cout << "InternetConnect failed, error " << GetLastError();
			return 0;
		}
		// Wait until we get the connection handle
		WaitForSingleObject(m_hConnectedEvent, 3000);
		if(hConnect == NULL)
		{
			return 0;
		}
	}


	CString Url = _T("/usergameinfo?userid=100&curtime=1443374306");

	// Open the request
	HINTERNET hRequest = HttpOpenRequest(hConnect, 
		_T("GET"),
		(LPCTSTR)Url,
		NULL,
		NULL,
		NULL,
		INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE,
		2);  // Request handle's context 
	if (hRequest == NULL)
	{
		if (GetLastError() != ERROR_IO_PENDING)
		{
			cout << "HttpOpenRequest failed, error " << GetLastError();
			return 0;
		}
		// Wait until we get the request handle
		WaitForSingleObject(m_hRequestOpenedEvent, 3000);
		if (hRequest == NULL)
		{
			return 0;
		}
	}

	if (!HttpSendRequest(hRequest, 
		NULL, 
		0, 
		NULL,
		0))
	{
		if (GetLastError() != ERROR_IO_PENDING)
		{
			cout << "HttpSendRequest failed, error " << GetLastError();
			return 0;
		}
	}

	if (m_bVerbose)
	{
		cout << "HttpSendRequest called successfully" << endl;
		cout.flush();
	}

	WaitForSingleObject(m_hRequestCompleteEvent, 3000);
	if(m_bError) return 0;

	cout << "------------------- Read the response -------------------" << endl;
	char lpReadBuff[256];
	int nRecved = 0;
	do
	{
		INTERNET_BUFFERS InetBuff;
		FillMemory(&InetBuff, sizeof(InetBuff), 0);
		InetBuff.dwStructSize = sizeof(InetBuff);
		InetBuff.lpvBuffer = lpReadBuff;
		InetBuff.dwBufferLength = sizeof(lpReadBuff) - 1;

		if (m_bVerbose)
		{
			cout << "Calling InternetReadFileEx" << endl;
			cout.flush();
		}

		if (!InternetReadFileEx(hRequest,
			&InetBuff,
			0, 2))
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				if (m_bVerbose)
				{
					cout << "Waiting for InternetReadFileEx to complete" << endl;
					cout.flush();
				}
				WaitForSingleObject(m_hRequestCompleteEvent, INFINITE);
			}
			else
			{
				cout << "InternetReadFileEx failed, error " << GetLastError();
				cout.flush();
				return 0;
			}
		}

		lpReadBuff[InetBuff.dwBufferLength] = 0;
		memcpy(szBuffer + nRecved, lpReadBuff, InetBuff.dwBufferLength);
		nRecved += InetBuff.dwBufferLength;

		cout << lpReadBuff;
		cout.flush();

		if (InetBuff.dwBufferLength == 0) 
			m_bAllDone = true;

	} while (m_bAllDone == false);

	cout << endl << endl << "------------------- Request Complete ----------------" << endl;
	return bRet; 
	//return data;
}



bool CHttpHelper::PostData(const CString& sUrl, char* data)
{
	return true;
}

CString CHttpHelper::CrackUrl(CString sUrl)
{
	URL_COMPONENTS uc;
	TCHAR Scheme[1000]={0};
	TCHAR HostName[1000]={0};
	TCHAR UserName[1000]={0};
	TCHAR Password[1000]={0};
	TCHAR UrlPath[1000]={0};
	TCHAR ExtraInfo[1000]={0};

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

CString CHttpHelper::DownloadFile(const CString& surl)
{
	CString sAppPath = CPaintManagerUI::GetInstancePath().GetData();
	CString sImageName = CrackUrl(surl);
	sImageName.Replace(_T("/"), _T("\\"));
	TCHAR szFile[MAX_PATH] = {0};
	lstrcpy(szFile, sAppPath);
	::PathAppend(szFile, sImageName);
	// 	TCHAR szDir[MAX_PATH] = {0};
	// 	lstrcpy(szDir, sAppPath);
	// 	::PathAppend(szDir, _T("\\icon"));
	//���ڼ����Ż������ж�Ŀ¼�Ƿ���� 20150918
	if(!::PathFileExists(szFile))
	{
		bool bRet = CreateMultipleDirectory(szFile);
		//bool bRet = CreateDirectory(szDir, NULL);
		if(!bRet )
		{
			CString strError;
			strError.Format(_T("MakeSureDirectoryPathExists failed, code=%d"),GetLastError());
			return "";
		}

	}
	// 	if(::PathFileExists(szFile))
	// 	{
	// 		BOOL bRet = DeleteFile(szFile);
	// 		if(bRet == FALSE)
	// 		{
	// 			return "";
	// 		}
	// 	}

	HRESULT hr = ::URLDownloadToFile(NULL, surl, szFile, 0, NULL);
	if (hr != S_OK)
	{
		return "";
	}
	return sImageName;

}


void __stdcall CHttpHelper::Inet_Callback(HINTERNET hInternet, DWORD dwContext, DWORD dwInternetStatus, LPVOID lpStatusInfo, DWORD dwStatusInfoLen)
{
	if (true)
	{
		cout << "Callback dwInternetStatus: " << dwInternetStatus << " Context: " << dwContext << endl;
		cout.flush();
	}

	switch(dwContext)
	{
	case 1: // Connection handle
		if (dwInternetStatus == INTERNET_STATUS_HANDLE_CREATED)
		{
			INTERNET_ASYNC_RESULT *pRes = (INTERNET_ASYNC_RESULT *)lpStatusInfo;
			HINTERNET hConnect = (HINTERNET)pRes->dwResult;
			if (true)
			{
				cout << "Connect handle created" << endl;
				cout.flush();
			}
			SetEvent(m_hConnectedEvent);
		}
		break;
	case 2: // Request handle
		switch(dwInternetStatus)
		{
		case INTERNET_STATUS_HANDLE_CREATED:
			{
				INTERNET_ASYNC_RESULT *pRes = (INTERNET_ASYNC_RESULT *)lpStatusInfo;
				HINTERNET hRequest = (HINTERNET)pRes->dwResult;
				if (true)
				{
					cout << "Request handle created" << endl;
					cout.flush();
				}
				SetEvent(m_hRequestOpenedEvent);
			}
			break;
		case INTERNET_STATUS_REQUEST_SENT:
			{
				DWORD *lpBytesSent = (DWORD*)lpStatusInfo;
				if (true)
				{
					cout << "Bytes Sent: " << *lpBytesSent << endl;
					cout.flush();
				}
			}
			break;
		case INTERNET_STATUS_REQUEST_COMPLETE:
			{
				INTERNET_ASYNC_RESULT *pAsyncRes = (INTERNET_ASYNC_RESULT *)lpStatusInfo;
				if (true)
				{
					cout << "Function call finished" << endl;
					cout << "dwResult: " << pAsyncRes->dwResult << endl;
					cout << "dwError:  " << pAsyncRes->dwError << endl;
					if(pAsyncRes->dwResult == 0 )
						m_bError = true;
					else
						m_bError = false;
					cout.flush();
				}
				SetEvent(m_hRequestCompleteEvent);
			}
			break;
		case INTERNET_STATUS_RECEIVING_RESPONSE:
			if (true)
			{
				cout << "Receiving Response" << endl;
				cout.flush();
			}
			break;
		case INTERNET_STATUS_RESPONSE_RECEIVED:
			{
				DWORD *dwBytesReceived = (DWORD*)lpStatusInfo;
				if (*dwBytesReceived == 0)
					m_bAllDone = true;
				if (true)
				{
					cout << "Received " << *dwBytesReceived << endl;
					cout.flush();
				}
			}

		}

	}

}

CHttpHelper::CHttpHelper()
{
	 m_hConnectedEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	 m_hRequestOpenedEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	 m_hRequestCompleteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	 m_bAllDone = false;
	 m_bVerbose = true;
}