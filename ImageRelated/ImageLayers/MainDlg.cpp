// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"


CMainDlg::CMainDlg( void ): m_Statu(SELECT)
{

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

	return TRUE;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
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
	HDC dc = GetWindowDC();
    HBITMAP hBitmap = ConvertDIB2DDB(dc, lpBitmapInfo);
	CLayer* layer = new CLayer(0, hBitmap);
	CRect rect;
	rect.left = 50;
	rect.top = 150;
	rect.bottom = 500;
	rect.right = 400;
	layer->SetArea(rect);
	Invalidate();
	m_Layers.push_back(layer);
    m_Statu = PREMOVE;
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
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

LRESULT CMainDlg::OnPaint( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	HDC dc;
	PAINTSTRUCT ps;
	dc = BeginPaint(&ps);
	std::vector<CLayer*>::iterator it = m_Layers.begin();
	for (;it != m_Layers.end(); ++it)
	{
		HDC hdc = CreateCompatibleDC(dc);
		HGDIOBJ hOld = SelectObject(hdc, (*it)->m_hBitmap);
        CRect rect = (*it)->GetArea();
		BitBlt(dc, rect.left, rect.top, rect.Width(), rect.Height(), hdc, 0, 0, SRCCOPY);
        SelectObject(hdc, hOld);
	}
	EndPaint(&ps);
	return S_OK;
}

LRESULT CMainDlg::OnLeftButtonDown( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/ )
{
    CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    if (m_Statu == PREMOVE)
    {
        std::vector<CLayer*>::iterator it = m_Layers.begin();
        for (;it != m_Layers.end(); ++it)
        {
            CRect rect = (*it)->GetArea();
            if (rect.PtInRect(pt))
            {
                m_pSelectedLayer = *it;
                break;
            }
        }
        m_ptLastClicked = pt;
        m_Statu = MOVE;
    }
    return S_OK;
}

LRESULT CMainDlg::OnMouseMove( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/ )
{
    CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    if (m_Statu == MOVE)
    {
        int xOffset = pt.x - m_ptLastClicked.x;
        int yOffset = pt.y - m_ptLastClicked.y;
        m_ptLastClicked = pt;
        CRect rect = m_pSelectedLayer->GetArea();
        rect.OffsetRect(xOffset, yOffset);
        m_pSelectedLayer->SetArea(rect);
        Invalidate();
    }
    return S_OK;
}

LRESULT CMainDlg::OnLeftButtonUp( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
    if (m_Statu == MOVE)
    {
        m_Statu = SELECT;
    }
    return S_OK;
}
