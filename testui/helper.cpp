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
			vPath.push_back(strTemp);//����ǰ����ַ�����ӵ�������
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
void UTF8toANSI(CString &strUTF8,string& strAnsi)
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
	
	strAnsi = szBuffer;
	//strUTF8 = szBuffer;
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

char* UnicodeToUtf8( WCHAR*  wszUtf8,int &len)
{
	len= WideCharToMultiByte(CP_UTF8,0, wszUtf8,-1, NULL,0, NULL, NULL);
	static char* szUtf8=new char[1024*50];
	memset(szUtf8,0, 1024*50);
	WideCharToMultiByte (CP_UTF8,0, wszUtf8,-1, szUtf8, len, NULL,NULL);
	return szUtf8;
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

extern HWND GetWinHandle(long id)
{
	HWND tmpHwnd = FindWindow(NULL, NULL);
	while (tmpHwnd != 0)
	{
		if(GetParent(tmpHwnd) == 0)
		{
			DWORD procid = 0;
			DWORD threadId = GetWindowThreadProcessId(tmpHwnd, &procid);
			if(id == procid)
			{
				return tmpHwnd;
			}

		}
		tmpHwnd = GetWindow(tmpHwnd, GW_HWNDNEXT);
	}
	return tmpHwnd;
}

extern long StartProc(CString path)
{
	STARTUPINFO si = { sizeof(si) };   
	PROCESS_INFORMATION pi;   

	si.dwFlags = STARTF_USESHOWWINDOW;   
	si.wShowWindow = TRUE; //TRUE��ʾ��ʾ�����Ľ��̵Ĵ���  
	TCHAR cmdline[] =TEXT(""); //ע��ǰ���пո񣬷���򿪵�����ҳ��  
// 	BOOL bRet = ::CreateProcess (   
// 		_T("D:\\Program Files\\TortoiseGit\\bin\\notepad2.exe"),  
// 		(LPTSTR)(LPCTSTR)path, //��Unicode�汾�д˲�������Ϊ�����ַ�������Ϊ�˲����ᱻ�޸�    
// 		NULL,   
// 		NULL,   
// 		FALSE,   
// 		CREATE_NEW_CONSOLE,   
// 		NULL,   
// 		NULL,   
// 		&si,   
// 		&pi);   

	BOOL bRet = ::CreateProcess (   
		path,  
		cmdline, //��Unicode�汾�д˲�������Ϊ�����ַ�������Ϊ�˲����ᱻ�޸�    
		NULL,   
		NULL,   
		FALSE,   
		CREATE_NEW_CONSOLE,   
		NULL,   
		NULL,   
		&si,   
		&pi); 

	int error = GetLastError();  
	if(bRet)   
	{   
		::CloseHandle (pi.hThread);   
		::CloseHandle (pi.hProcess);   

		printf(" �½��̵Ľ���ID�ţ�%d /n", pi.dwProcessId);   
		printf(" �½��̵����߳�ID�ţ�%d /n", pi.dwThreadId);   
	}   
	else  
	{  
		printf("error code:%d/n",error );  
		return 0;
	}  
	return pi.dwProcessId;   
}

//·���������Ķ���������
string  GetJbTemplateContent(CString path)
{
	CString curDir = GetCurDir();
	CString JbPath = curDir + path;

	CString log;
	log.Format(_T("file:%s ,content is null"),JbPath);
	CLog::getInstance()->AgentLog((LPTSTR)(LPCTSTR)log);
	char   szANSIString   [MAX_PATH]= {0};     
	WideCharToMultiByte   (   CP_ACP,   WC_COMPOSITECHECK,   JbPath,   -1,   szANSIString,   sizeof(szANSIString),   NULL,   NULL   );  
	std::ifstream ifs(szANSIString);
	std::string content;
	ifs.seekg(0, std::ios::end);   
	int n = ifs.tellg();
	content.reserve(ifs.tellg());
	ifs.seekg(0, std::ios::beg);

	content.assign( (std::istreambuf_iterator<char>(ifs) ),
		(std::istreambuf_iterator<char>()    ) );
	return content;
}

string  GetJbTemplateContent2(CString path)
{
	CString curDir = GetCurDir();
	CString JbPath = curDir + path;
	char   szANSIString   [MAX_PATH]= {0};     
	WideCharToMultiByte   (   CP_ACP,   WC_COMPOSITECHECK,   JbPath,   -1,   szANSIString,   sizeof(szANSIString),   NULL,   NULL   );  

	OFSTRUCT   ofStruct; 
	HFILE   hFile   =   OpenFile( 
		szANSIString, 
		&ofStruct, 
		OF_READ 
		); 
	DWORD filesize=GetFileSize((HANDLE)hFile,NULL);
	char* buffer=new char[filesize+1]; // ���һλΪ '/0',C-Style �ַ����Ľ�����
	DWORD readsize;
	ReadFile((HANDLE)hFile,buffer,filesize,&readsize,NULL);
	buffer[filesize]=0;
	string content = buffer;
	delete [] buffer;
	return content;
}

void ReplaceStringInPlace(std::string& subject, const std::string& search,
						  const std::string& replace)
{
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) 
	{
	  subject.replace(pos, search.length(), replace);
	  pos += replace.length();
	}
}

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
	size_t start_pos = 0;
	while((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}

CString GetCurDir()
{
	CString    sPath;   
	GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);   
	sPath.ReleaseBuffer    ();   
	int    nPos;   
	nPos=sPath.ReverseFind('\\');   
	sPath=sPath.Left(nPos);   
	return    sPath;
}

CString GetDir(CString app_path)
{
	int len = app_path.GetLength(); 
	//��ȥ�ļ����ƣ�ֻ����·��
	for(int i=0; i<=len-1; i++) 
	{ 
		if(app_path.Right(1)!="\\") 
			app_path.Delete(len-1-i,1); 
		else 
		{
			app_path.Delete(len-1-i,1); 
			break; 
		}
	}
	return app_path;
}

bool WriteJbFile(CString jbPath,const string& str)
{
	CString path = jbPath;
	path.Replace( _T("jbt\\"), _T("jb\\") );
	CString curDir = GetCurDir();
	path = curDir + path;
	//ReplaceStringInPlace( path, _T("jbt\\"), _T("jb\\") );

	CString dir = GetDir(path);
	dir += "\\";
	if(!path.IsEmpty())
	{
		if(!PathFileExists(dir))
		{
			bool bRet = CreateMultipleDirectory(dir);
			if(!bRet) 
			{
				//log
				CLog::getInstance()->AgentLog(_T("create dir failed!"));
				return false;
			}
		}
	}
	HANDLE hFile = ::CreateFile(path,     //�����ļ������ơ�
		GENERIC_WRITE|GENERIC_READ,          // д�Ͷ��ļ���
		0,                      // �������д��
		NULL,                   // ȱʡ��ȫ���ԡ�
		CREATE_ALWAYS,          // ����ļ����ڣ�Ҳ������
		FILE_ATTRIBUTE_NORMAL, // һ����ļ���      
		NULL);                 // ģ���ļ�Ϊ�ա�
	DWORD dwWritenSize = 0;
	BOOL bRet = ::WriteFile(hFile,str.c_str(),str.length(),&dwWritenSize,NULL);
	CString log;
	log.Format(_T("write content length:%d,actuall write:%d"), str.length(), dwWritenSize);
	CLog::getInstance()->AgentLog((LPTSTR)(LPCTSTR)log);
	CloseHandle(hFile);
	return bRet;

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


