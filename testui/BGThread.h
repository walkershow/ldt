#pragma once
#include "duilib.h"
#include <vector>
using namespace std;

class CBGThread
{
public:
	CBGThread(void);
	~CBGThread(void);

public:
	void Start();
	void FindGameWindow();
	void AddTask(const Snap_Info& si);
	void DelTask(const Snap_Info& si);
	static DWORD WINAPI Working(LPVOID lpParameter);
	
private:
	HANDLE m_hEvent;
	HANDLE m_hThread;
	DWORD  m_dwThreadID;//¼àÌýÏß³ÌID 
	CRITICAL_SECTION m_cs;
	Snap_Info m_si;
	vector<Snap_Info> m_vecSnapInfo;
	
};
