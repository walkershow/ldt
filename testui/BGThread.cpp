#include "BGThread.h"
#include "helper.h"
#include "Log.h"
CBGThread::CBGThread(void):m_dwThreadID(0),m_hThread(NULL),m_hEvent(NULL)
{
	InitializeCriticalSection(&m_cs);
	m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CBGThread::~CBGThread(void)
{
	DeleteCriticalSection(&m_cs);
}

void CBGThread::Start()
{
// 	Snap_Info *psi = (Snap_Info*)lpParam;
// 	m_si.jburl =  psi->jburl;
// 	m_si.classname = psi->classname;
// 	m_si.title = psi->title;
// 	m_si.hwndParent = psi->hwndParent;

	//开启线程  
	m_hThread=::CreateThread(NULL, 0, Working, this, 0, &m_dwThreadID);//创建线程  
	if (!m_hThread)  
	{  
		return;  
	}  
}

DWORD WINAPI CBGThread::Working(LPVOID lpParameter)
{
	CBGThread* pbt = (CBGThread*)(lpParameter);
	while(true)
	{
		pbt->FindGameWindow();
	}
}

void CBGThread::FindGameWindow()
{
	if (WaitForSingleObject(m_hEvent,INFINITE) == WAIT_OBJECT_0)  
	{  
		EnterCriticalSection(&m_cs);
		for(int i =0;i<m_vecSnapInfo.size(); i++)
		{
			//100秒没找到不找
			if(m_vecSnapInfo[i].tryTimes >= 200)
			{
				CLog::getInstance()->AgentLog(_T("task timeout !!!"));
				DelTask(m_vecSnapInfo[i]);
				return;
			}
			m_vecSnapInfo[i].tryTimes++;
			HWND  hwnd = FindWindow(m_vecSnapInfo[i].classname, NULL);
			while( hwnd!=0 )
			{
			//	HWND  hwnd = FindWindow(m_vecSnapInfo[i].classname, NULL);
				TCHAR buf[256] = {0};
				GetWindowText(hwnd, buf, sizeof(buf));
				CString title = buf;
				CString findTitle = m_vecSnapInfo[i].title;
				int n = title.Find(findTitle);
				if(n>=0)
				{

					string str = GetJbTemplateContent2(m_vecSnapInfo[i].jbpath);
					if(str.empty())
					{
						CString log;
						log.Format(_T("file:%s ,content is null"),(LPCTSTR) m_vecSnapInfo[i].jbpath);
						CLog::getInstance()->AgentLog((LPTSTR)(LPCTSTR)log);
						break;
					}
					char strHwnd[12] = {0};
					sprintf(strHwnd, "%d", hwnd);
					str.replace(str.find("{hwnd}"),strlen(strHwnd), strHwnd);
					//str = ReplaceAll(str,"\n", "\r\n");
					//ReplaceStringInPlace(str,"{hwnd}", strHwnd);
					CString path = m_vecSnapInfo[i].jbpath;
					bool bRet = WriteJbFile(path, str);
					if(!bRet)
					{
						CLog::getInstance()->AgentLog(_T("WriteJbFile failed!!!"));
					}

					DelTask(m_vecSnapInfo[i]);
					break;
				}
				
				hwnd = GetWindow(hwnd, GW_HWNDNEXT);
			}

		}

		LeaveCriticalSection(&m_cs);
		Sleep(5000);
	}
}

void CBGThread::AddTask(const Snap_Info& si)
{
	EnterCriticalSection(&m_cs);
	for ( std::vector<Snap_Info>::iterator it = m_vecSnapInfo.begin();  it != m_vecSnapInfo.end(); ) 
	{  
		if ( it->classname == si.classname && it->title == si.title) 
		{  
			return ;
		}  
	}
	m_vecSnapInfo.push_back(si);
	SetEvent(m_hEvent);
	LeaveCriticalSection(&m_cs);
}

void CBGThread::DelTask(const Snap_Info& si)
{
	EnterCriticalSection(&m_cs);

	for ( std::vector<Snap_Info>::iterator it = m_vecSnapInfo.begin();  it != m_vecSnapInfo.end(); ) 
	{  
		if ( it->classname == si.classname && it->title == si.title) 
		{  
			it = m_vecSnapInfo.erase(it);  
		}  
		else
		{  
			it++;  
		}  
	} 
	if(m_vecSnapInfo.empty())
	{
		ResetEvent(m_hEvent);
	}

	LeaveCriticalSection(&m_cs);

}
