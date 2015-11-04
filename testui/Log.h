#pragma once
#include <fstream>
#include <string>
#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <time.h>
using namespace std;

#define AGENTLOG(format,...)    CLog::getInstance()->AgentLog(format,__VA_ARGS__);
#define AGENTLOG2(format,...)    CLog::getInstance()->test(format,__VA_ARGS__);
#define AGENTLOG1(format,...)    {\
	char theloggg[4096];	\
	memset(theloggg, 0, 4096); \
	char formatbuf[1024];	\
	memset(formatbuf, 0, 1024); \
	sprintf_s(formatbuf,"%s:","%s");	\
	strcat_s(formatbuf, sizeof(formatbuf), format);	\
	sprintf_s(theloggg, formatbuf, /*__DATE__,*/ /*__TIME__*/ __FUNCTION__, ##__VA_ARGS__); \
	MessageBoxA(NULL,theloggg,NULL,NULL);	\
}
class CLog
{
public:
	void test(TCHAR * format ,...)
	{
		TCHAR destBuf[1024];
		va_list arg_ptr = NULL; 
		va_start(arg_ptr, format);     
		//wprintf(formatBuf,format);
		wvsprintf(destBuf,format, arg_ptr);
		wprintf(destBuf);
		va_end(arg_ptr);
	}
	void AgentLog(TCHAR* format,...)
	{
		TCHAR theLoggg[2048];
		TCHAR formatBuf[1024];
		memset(formatBuf, 0, 1024*sizeof(wchar_t));
		wsprintf(formatBuf,_T("%s:"),__FUNCTIONW__);
		//wcscat_s(formatBuf,sizeof(formatBuf),format);
		_tcscat_s(formatBuf,1024,format);
		va_list arg_ptr = NULL; 
		va_start(arg_ptr, format);     
		wvsprintf(theLoggg, formatBuf, arg_ptr); 
		CLog::outPut(theLoggg);
		va_end(arg_ptr);
	}
private:
	CLog(void);
	//����ģʽ��Ψһ����
	static CLog*	g_Instance;
	//�ļ������
	wofstream*		m_pFout;
	//msg����
	int				m_iMsgCount;
	//Ӧ�ó���Ŀ¼
	wstring			m_strCurrentExePath;
	CRITICAL_SECTION m_cs;
public:
	wstring m_strLogEdit;
private:
	//�½���־�ļ�
	void createNewLog();
	void theOutPut(wstring msg);
public:
	~CLog(void);
	//Ψһʵ�����ɺ���
	static CLog*	getInstance();
	//ʵ��ɾ��
	static void DeleteInstance();
	static	void outPut(wstring msg);
	LPCTSTR getCurrentEXEPath();
};

