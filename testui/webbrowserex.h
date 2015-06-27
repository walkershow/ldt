#pragma once

#include "Utils/WebBrowserEventHandler.h"

class CWebBrowserEventHandlerEx :
	public CWebBrowserEventHandler
{
public:
	CWebBrowserEventHandlerEx(void);
	~CWebBrowserEventHandlerEx(void);
	//��Ҫ���������������
	virtual HRESULT STDMETHODCALLTYPE GetHostInfo(
		/* [out][in] */ DOCHOSTUIINFO __RPC_FAR *pInfo)
	{
		if (pInfo != NULL)
		{
			pInfo->dwFlags |= DOCHOSTUIFLAG_NO3DBORDER;
		}
		return S_OK;
	}
};