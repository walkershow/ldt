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

char* UnicodeToUtf8( WCHAR*  wszUtf8,int &len)
{
	len= WideCharToMultiByte(CP_UTF8,0, wszUtf8,-1, NULL,0, NULL, NULL);
	char* szUtf8=new char[len+1];
	memset(szUtf8,0, len+1);
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


