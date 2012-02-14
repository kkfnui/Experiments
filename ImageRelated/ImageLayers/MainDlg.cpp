// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"


CMainDlg::CMainDlg( void )
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
    m_operateDlg = new COperateDlg;
    m_operateDlg->Create(m_hWnd);
    m_operateDlg->ShowWindow(SW_NORMAL);
    m_operateDlg->SetMainDlg(this);
    
    m_list.Attach(GetDlgItem(IDC_LIST_IMAGE_LAYERS));
    m_list.InsertColumn(0, L"layer name", LVCFMT_LEFT, -1);
    CRect rect;
    rect.left = 0;
    rect.top = 40;
    rect.right = 750;
    rect.bottom = 680;
    m_operateDlg->MoveWindow(&rect);
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
    CFileDialog fileOpenDlg(TRUE, _T(".bmp"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("BMP图片文件\0*.bmp\0\0"), NULL);

    if (fileOpenDlg.DoModal() == IDCANCEL)
    {
        return S_FALSE;
    }

    HANDLE hFile = CreateFile(fileOpenDlg.m_szFileName, 
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

    CString strTitle;
    strTitle.Format(L"图片%d", m_Layers.size()-1);
    int nId = m_list.InsertItem(m_Layers.size()-1, strTitle);
    SendMessage(m_operateDlg->m_hWnd, ID_SELECT, NULL, NULL);
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
    CRect rect;
    GetClientRect(&rect);
    HDC dcMemory = CreateCompatibleDC(dc);
    CBitmap bmp;
    HGDIOBJ bmpOld;
    bmp.CreateCompatibleBitmap(dc, rect.Width(), rect.Height());
    bmpOld = SelectObject(dcMemory, bmp);
    CDCHandle cdc(dcMemory);
    cdc.FillSolidRect(&rect, RGB(240, 240, 240));
	for (;it != m_Layers.end(); ++it)
	{
		HDC hdc = CreateCompatibleDC(dc);
		HGDIOBJ hOld = SelectObject(hdc, (*it)->m_hBitmap);
        CRect rect = (*it)->GetArea();
        BitBlt(dcMemory, rect.left, rect.top, rect.Width(), rect.Height(), hdc, 0, 0, SRCCOPY);
        SelectObject(hdc, hOld);
	}
    BitBlt(dc, 0, 0, rect.Width(), rect.Height(), dcMemory, 0, 0, SRCCOPY);
    SelectObject(dcMemory, bmpOld);
	EndPaint(&ps);
	return S_OK;
}


CMainDlg::~CMainDlg()
{
    if (m_operateDlg != NULL)
    {
        delete m_operateDlg;
    }
}

LRESULT CMainDlg::OnLvnItemActivateListImageLayers(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: Add your control notification handler code here
    std::vector<CLayer*>::iterator it = m_Layers.begin();
    for (int i=0; i< pNMIA->iItem; it++,i++)
        NULL;
    m_pSelectedLayer = *it;
    SendMessage(m_operateDlg->m_hWnd, ID_SELECT, NULL, NULL);
    return 0;
}
