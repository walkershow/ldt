#include "FrameWnd.h"
#include "resource.h"

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
// 	CPaintManagerUI::SetResourceZip(_T("testui.zip"));
	::CoInitialize(NULL);
	
	//CWkeWebkitUI::WkeWebkit_Init();
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());

	CFrameWnd *pFrame = new CFrameWnd(_T("UISkin2.xml"));
	pFrame->Create(NULL, _T("6AÓÎÏ·"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	pFrame->ShowModal();
	
	
	//CWkeWebkitUI::WkeWebkit_Shutdown();
	delete pFrame;
	::CoUninitialize();
	return 0;
}
