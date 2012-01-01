// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szPath, MAX_PATH);
	PathRemoveFileSpec(szPath);
	PathRemoveFileSpec(szPath);
	PathCombine(szPath, szPath, L"Res");
	strLargeImage = szPath;
	strSmallImage = szPath;
	strLargeImage += L"\\LargeImages\\%d.bmp";
	strSmallImage += L"\\SmallImages\\%d.bmp";
	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}


LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnLoadLargeImage( WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	OutputDebugStr(L"LoadImages LargeImage Start");
	HBITMAP hBitmap;
	for (int i = 1; i <= 10; i++)
	{
		strLargeImage.Format(strLargeImage, i);
		hBitmap = (HBITMAP)LoadImage(NULL, strLargeImage, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		ATLASSERT(hBitmap);
		if (hBitmap != NULL)
		{
			DeleteObject(hBitmap);
		}
	}
	OutputDebugStr(L"LoadImages LargeImage End");
	MessageBox(L"加载完成！");
	return TRUE;
}

LRESULT CMainDlg::OnLoadSmallImage( WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	OutputDebugStr(L"LoadImages SmallImage Start");
	HBITMAP hBitmap;
	for (int i = 1; i <= 1000; i++)
	{
		strSmallImage.Format(strSmallImage, i);
		hBitmap = (HBITMAP)LoadImage(NULL, strSmallImage, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		ATLASSERT(hBitmap);
		if (hBitmap != NULL)
		{
			DeleteObject(hBitmap); 
		}
	}
	OutputDebugStr(L"LoadImages SmallImage End");
	MessageBox(L"加载完成！");
	return TRUE;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

