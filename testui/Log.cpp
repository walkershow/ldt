#include "StdAfx.h"
#include "Log.h"

#define  MAX_NEWFILE	5
#define	 MAX_FLUSHFILE	1

CLog* CLog::g_Instance = 0;

CLog::CLog(void)
{
	m_pFout=0;
	InitializeCriticalSection(&m_cs);
	CLog::getCurrentEXEPath();
	//����LogĿ¼
	wstring logPath=m_strCurrentExePath+_T("\\log");
	CreateDirectory(logPath.c_str(),NULL);
	createNewLog();
}


CLog::~CLog(void)
{
	if(m_pFout==0)
		return;

	m_pFout->flush();
	m_pFout->close();
	delete m_pFout;
	m_pFout = 0;
}
CLog* CLog::getInstance()
{
	if(g_Instance==0)
		g_Instance = new CLog();
	return g_Instance;
}
void CLog::DeleteInstance()
{
	if(g_Instance==0)
		return;
	//�ж���û�д򿪵�ostreamָ��
	if (g_Instance->m_pFout!=0)
	{
		g_Instance->m_pFout->flush();
		g_Instance->m_pFout->close();
		delete g_Instance->m_pFout;
		g_Instance->m_pFout=0;
	}
	delete g_Instance;
	g_Instance  = 0;

}


void CLog::createNewLog()
{
	try
	{
		if (m_pFout!=0)
		{
			m_pFout->flush();
			m_pFout->close();
			delete m_pFout;
			m_pFout=0;
		}
		m_iMsgCount=0;
		time_t tt;
		struct tm	*p;
		time(&tt);
		p=localtime(&tt);
		TCHAR filename[260];
		memset(filename,0,260);
		wsprintf(filename,_T("%s\\log\\%d-%d.log\0"),m_strCurrentExePath.c_str(),p->tm_year+1900,p->tm_mon+1);
		//wsprintf(filename,_T("%s\\log\\%d-%d-%d_%d-%d-%d_nbox.log\0"),m_strCurrentExePath.c_str(),p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
		m_pFout=new wofstream();
		locale::global(locale(""));//��ȫ��������Ϊ����ϵͳĬ������
		m_pFout->open(filename,ios::out|ios::app);
		locale::global(locale("C"));//��ԭ
		{
			return ;
		}
	}
	catch (exception e)
	{

	}
}


LPCTSTR CLog::getCurrentEXEPath()
{
	try{
		if (m_strCurrentExePath.length() > 0)
			return m_strCurrentExePath.c_str();

		TCHAR szEexFullpath[MAX_PATH];
		//�õ�EXE����Ŀ¼������"\"��
		//�õ������ģ����
		HMODULE hMyModule = GetModuleHandle(NULL);
		//�õ������·��
		DWORD dwLen = GetModuleFileName(hMyModule, szEexFullpath, MAX_PATH);
		//��������·��(�����������һ��"\")
		TCHAR  * pPatch = &szEexFullpath[dwLen]; //szexepatch + dwLen;
		while (pPatch != NULL && *pPatch != '\\')
		{
			pPatch--;
		}
		*pPatch=0;

		m_strCurrentExePath = wstring(szEexFullpath);
	
	}
	catch (...)
	{

	}
	return m_strCurrentExePath.c_str();
}


void CLog::outPut(wstring msg)
{
	//std::cout << msg;
	EnterCriticalSection(&getInstance()->m_cs);
	getInstance()->theOutPut(msg);
	LeaveCriticalSection(&getInstance()->m_cs);
}

void CLog::theOutPut(wstring msg)
{
	if(m_pFout==0)
		return;

	time_t timep;

	struct tm *p;

	time(&timep);

	p=localtime(&timep);

	TCHAR filename[260];
	memset(filename,0,260);
	wsprintf(filename,_T("%d:%d:%d -- "),p->tm_hour,p->tm_min , p->tm_sec);

	msg = wstring(filename) + msg;
	//add chenshuai
	getInstance()->m_strLogEdit = getInstance()->m_strLogEdit + _T("\r\n") + msg;

	m_iMsgCount++;

	*m_pFout << msg << _T("\r\n");

	if(m_iMsgCount % MAX_FLUSHFILE ==0)
		m_pFout->flush();

	if(m_iMsgCount % MAX_NEWFILE ==0)
		createNewLog();
}
