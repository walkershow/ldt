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
void UTF8toANSI(CString &strUTF8,string& strAnsi)
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
	
	strAnsi = szBuffer;
	//strUTF8 = szBuffer;
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
	si.wShowWindow = TRUE; //TRUE表示显示创建的进程的窗口  
	TCHAR cmdline[] =TEXT(""); //注意前面有空格，否则打开的是主页。  
// 	BOOL bRet = ::CreateProcess (   
// 		_T("D:\\Program Files\\TortoiseGit\\bin\\notepad2.exe"),  
// 		(LPTSTR)(LPCTSTR)path, //在Unicode版本中此参数不能为常量字符串，因为此参数会被修改    
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
		cmdline, //在Unicode版本中此参数不能为常量字符串，因为此参数会被修改    
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

		printf(" 新进程的进程ID号：%d /n", pi.dwProcessId);   
		printf(" 新进程的主线程ID号：%d /n", pi.dwThreadId);   
	}   
	else  
	{  
		printf("error code:%d/n",error );  
		return 0;
	}  
	return pi.dwProcessId;   
}

//路径中有中文都不出内容
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
	char* buffer=new char[filesize+1]; // 最后一位为 '/0',C-Style 字符串的结束符
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
	//滤去文件名称，只保留路径
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
	HANDLE hFile = ::CreateFile(path,     //创建文件的名称。
		GENERIC_WRITE|GENERIC_READ,          // 写和读文件。
		0,                      // 不共享读写。
		NULL,                   // 缺省安全属性。
		CREATE_ALWAYS,          // 如果文件存在，也创建。
		FILE_ATTRIBUTE_NORMAL, // 一般的文件。      
		NULL);                 // 模板文件为空。
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


