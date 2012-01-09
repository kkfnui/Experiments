// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
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
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

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

LRESULT CMainDlg::OnPaint( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
    PAINTSTRUCT ps;
    HDC dc;
    dc = BeginPaint(&ps);
    //获取资源图片的路径，相对程序的地址
    TCHAR szPath[MAX_PATH] = {0};
    GetModuleFileName(NULL, szPath, MAX_PATH);
    PathRemoveFileSpec(szPath);
    PathRemoveFileSpec(szPath);
    PathCombine(szPath, szPath, L"Res\\1.bmp");

    if (!PathFileExists(szPath))
    {
        return S_FALSE;
    }

    HANDLE hFile = CreateFile(szPath, 
        GENERIC_READ,
        FILE_SHARE_READ, 
        NULL, 
        OPEN_EXISTING,
        FILE_FLAG_SEQUENTIAL_SCAN,
        NULL);
    ATLASSERT(INVALID_HANDLE_VALUE != hFile);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        return S_FALSE;
    }
    DWORD dwFileSize, dwHighSize, dwBytesRead;
    dwFileSize = GetFileSize(hFile, &dwHighSize);
    if (dwHighSize)
    {
        CloseHandle(hFile);
        return S_FALSE;
    }
    LPBITMAPFILEHEADER  pbmfh = (LPBITMAPFILEHEADER)new BYTE[dwFileSize];
    if (!pbmfh)
    {
        CloseHandle(hFile);
        return S_FALSE;
    }
    BOOL bSuccess = ReadFile(hFile, pbmfh, dwFileSize, &dwBytesRead, NULL);
    CloseHandle(hFile);
    if (!bSuccess || (dwBytesRead != dwFileSize)
        || (pbmfh->bfType != *(WORD *)"BM")
        || (pbmfh->bfSize != dwFileSize))
    {
        free(pbmfh);
        return S_FALSE;
    }
    LPBITMAPINFO lpBitmapInfo = (LPBITMAPINFO)(LPVOID)((SIZE_T)pbmfh + sizeof(BITMAPFILEHEADER));
    HBITMAP hBitmap = ConvertDIB2DDB(dc, lpBitmapInfo);
	
	HDC hdc = CreateCompatibleDC(dc);
	SelectObject(hdc, hBitmap);

	BitBlt(dc, 0, 0, 500, 500, hdc, 100, 100, SRCCOPY);
	DeleteObject(hBitmap);
	
    EndPaint(&ps);
	return S_OK;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
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

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

HBITMAP CMainDlg::ConvertDIB2DDB( CDCHandle dc, LPBITMAPINFO bitmapInfo )
{
    //获取lpDIBBits的地址
    LPBITMAPINFOHEADER lpInfoHeader = &bitmapInfo->bmiHeader;
    int nColors = lpInfoHeader->biClrUsed ? lpInfoHeader->biClrUsed : 1 << lpInfoHeader->biBitCount;
    LPVOID lpDIBBits;
    if (bitmapInfo->bmiHeader.biBitCount >8)
    {
        lpDIBBits = (LPVOID)((LPWORD)(bitmapInfo->bmiColors +bitmapInfo->bmiHeader.biClrUsed)+ 
            ((bitmapInfo->bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
    }
    else
    {
		
        lpDIBBits = (LPVOID)(bitmapInfo->bmiColors + nColors);
    }

    //创建、设置并映射系统调色板
    CPalette pal;
    HPALETTE hOldPal = NULL;
    if(nColors <= 256 && dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
    {
        //如果DIB中颜色表的颜色数少于256，并且硬件支持逻辑调色板
        UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
        LOGPALETTE *pLP = (LOGPALETTE *)new BYTE[nSize];
        pLP->palNumEntries = nColors;
        pLP->palVersion = 0x300;
        for (int i = 0; i < nColors; i++)
        {
            pLP->palPalEntry[i].peRed =  bitmapInfo->bmiColors[i].rgbRed;
            pLP->palPalEntry[i].peGreen = bitmapInfo->bmiColors[i].rgbGreen;
            pLP->palPalEntry[i].peBlue = bitmapInfo->bmiColors[i].rgbBlue;
            pLP->palPalEntry[i].peFlags = 0;
        }
        pal.CreatePalette(pLP);
        delete[] pLP;
        hOldPal = dc.SelectPalette(pal, FALSE);
        dc.RealizePalette();
    }

    //根据DIB创建DDB
    HBITMAP hBitmap = CreateDIBitmap(dc,
        lpInfoHeader,
        (DWORD)CBM_INIT,
        lpDIBBits,
        bitmapInfo,
        DIB_RGB_COLORS);

	if (hOldPal != NULL)
	{
		dc.SelectPalette(hOldPal, FALSE);
	}

    return hBitmap;
}
