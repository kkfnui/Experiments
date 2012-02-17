#include "StdAfx.h"
#include "resource.h"
#include "OperateDlg.h"
#include "MainDlg.h"
#include "PropertyDlg.h"

COperateDlg::COperateDlg(void)
{
}

COperateDlg::~COperateDlg(void)
{
}

LRESULT COperateDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{

    return S_OK;
}

LRESULT COperateDlg::OnPaint( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
    PAINTSTRUCT ps;
    HDC dc;
    CRect rcClient;
    CRect rcCaven;
    dc = BeginPaint(&ps);
    GetClientRect(&rcClient);
    rcCaven = rcClient;
    rcCaven.InflateRect(-2, -2);

    HDC dcMemory = CreateCompatibleDC(dc);  
    CBitmap bmp;
    HGDIOBJ bmpOld;
    bmp.CreateCompatibleBitmap(dc, rcClient.Width(), rcClient.Height());
    bmpOld = SelectObject(dcMemory, bmp);
    CDC cdc(dcMemory); 

    cdc.DrawFocusRect(&rcClient);
    cdc.FillSolidRect(&rcCaven, RGB(240, 240, 240)); 
    std::vector<CLayer*>::iterator it = m_DlgMain->m_Layers.begin();     
    HDC hdc = CreateCompatibleDC(dc);
    for (;it != m_DlgMain->m_Layers.end(); ++it)
    {
        HGDIOBJ hOld =SelectObject(hdc, (*it)->m_hBitmap);
        CRect rect = (*it)->GetArea();
        BitBlt(dcMemory, rect.left, rect.top, rect.Width(), rect.Height(), hdc, 0, 0, SRCCOPY);
        SelectObject(hdc, hOld);
    }
    DeleteObject(hdc);
    BitBlt(dc, rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), dcMemory, 0, 0, SRCCOPY);
    SelectObject(dcMemory, bmpOld);
    DeleteObject(dcMemory);

    EndPaint(&ps);

    return S_OK;
}

LRESULT COperateDlg::OnLeftButtonDown( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/ )
{
    CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    if (m_Statu == PREMOVE)
    {
        std::vector<CLayer*>::iterator it = m_DlgMain->m_Layers.begin();
        for (;it != m_DlgMain->m_Layers.end(); ++it)
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


LRESULT COperateDlg::OnRightButtonUp( WPARAM wParam, CPoint point )
{
    CMenu menu;
    menu.LoadMenu(IDR_MENU);
    ClientToScreen(&point);
    CMenuHandle menuSubAll 	= menu.GetSubMenu(0);
    menuSubAll.TrackPopupMenu(TPM_VERTICAL|TPM_LEFTALIGN|TPM_BOTTOMALIGN, 
        point.x, point.y, m_hWnd, NULL);
    return S_OK;
}


LRESULT COperateDlg::OnMouseMove( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/ )
{
    CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    if (m_Statu == MOVE)
    {
        SetCursor(LoadCursor(NULL, IDC_CROSS));
        int xOffset = pt.x - m_ptLastClicked.x;
        int yOffset = pt.y - m_ptLastClicked.y;
        m_ptLastClicked = pt;
        CRect rect = m_pSelectedLayer->GetArea();
        rect.OffsetRect(xOffset, yOffset);
        m_pSelectedLayer->SetArea(rect);
        Invalidate(FALSE);
    }
    return S_OK;
}

LRESULT COperateDlg::OnSetProperty(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CPropertyDlg dlg;
    dlg.DoModal();
    return S_OK;
}


LRESULT COperateDlg::OnLeftButtonUp( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
    if (m_Statu == MOVE)
    {
        m_Statu = SELECT;
    }
    return S_OK;
}

LRESULT COperateDlg::OnSelect(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    m_Statu = PREMOVE;
    return S_OK;
}


LRESULT COperateDlg::OnSave( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
    CFileDialog fileOpenDlg(TRUE, _T(".bmp"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("BMPÍ¼Æ¬ÎÄ¼þ\0*.bmp\0\0"), NULL);
    if (fileOpenDlg.DoModal() == IDCANCEL)
    {
        return S_FALSE;
    }

    PAINTSTRUCT ps;
    HDC dc;
    CRect rcClient;
    CRect rcCaven;
    dc = BeginPaint(&ps);
    GetClientRect(&rcClient);
    rcCaven = rcClient;
    rcCaven.InflateRect(-2, -2);

    HDC dcMemory = CreateCompatibleDC(dc);  
    CBitmap bmp;
    HGDIOBJ bmpOld;
    bmp.CreateCompatibleBitmap(dc, rcClient.Width(), rcClient.Height());
    bmpOld = SelectObject(dcMemory, bmp);
    CDC cdc(dcMemory); 

    cdc.DrawFocusRect(&rcClient);
    cdc.FillSolidRect(&rcCaven, RGB(240, 240, 240)); 
    std::vector<CLayer*>::iterator it = m_DlgMain->m_Layers.begin();     
    HDC hdc = CreateCompatibleDC(dc);
    for (;it != m_DlgMain->m_Layers.end(); ++it)
    {
        HGDIOBJ hOld =SelectObject(hdc, (*it)->m_hBitmap);
        CRect rect = (*it)->GetArea();
        BitBlt(dcMemory, rect.left, rect.top, rect.Width(), rect.Height(), hdc, 0, 0, SRCCOPY);
        SelectObject(hdc, hOld);
    }
    DeleteObject(hdc);
    BitBlt(dc, rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), dcMemory, 0, 0, SRCCOPY);

    CString strFileName(fileOpenDlg.m_szFileName);
    SaveBitmapFile(strFileName, bmp, &cdc);  
    SelectObject(dcMemory, bmpOld);
    DeleteObject(dcMemory);

    EndPaint(&ps);

    return S_OK;
}

void COperateDlg::SetMainDlg( CMainDlg* dlg )
{
    m_DlgMain = dlg;
}

void COperateDlg::SetStatus( Status statu )
{
    m_Statu = statu;
}

BOOL COperateDlg::SaveBitmapFile( CString strFileName, CBitmap &bmp, CDC *pdc )
{
    BITMAP bm;
    bmp.GetBitmap (&bm);

    int nWidth = bm.bmWidth;
    int nHeight = bm.bmHeight;

    int nLineBits = bm.bmWidthBytes;
    if ((nLineBits % 8) != 0)
        nLineBits += 8 - nLineBits%8;

    int nBitCounts = nLineBits * bm.bmHeight ;

    int nBits = 24;//bm.bmBitsPixel;
    int nBitmapInfoSize = 0;

    if (nBits <= 8)
    {
        int nColors = 1 << nBits;
        int nPalUnitSize = sizeof(RGBQUAD);
        nBitmapInfoSize = sizeof(BITMAPINFOHEADER) + nPalUnitSize * nColors;    
    }
    else
        nBitmapInfoSize = sizeof(BITMAPINFOHEADER);

    BITMAPINFO*    pbmpinfo = NULL;

    pbmpinfo =(BITMAPINFO *)(new BYTE[nBitmapInfoSize]);
    ZeroMemory((void *)pbmpinfo,nBitmapInfoSize);

    BITMAPINFOHEADER* pInfoHead = (BITMAPINFOHEADER *)pbmpinfo;
    ZeroMemory((void *)pInfoHead,sizeof(BITMAPINFOHEADER));

    pInfoHead->biSize = sizeof(BITMAPINFOHEADER);
    pInfoHead->biBitCount = nBits;
    pInfoHead->biPlanes = 1;
    pInfoHead->biSizeImage = nBitCounts;
    pInfoHead->biCompression = BI_RGB;
    pInfoHead->biWidth = nWidth;
    pInfoHead->biHeight = nHeight;


    CDC dc;
    dc.CreateDC(L"DISPLAY", NULL, NULL, NULL);

    int nLx = dc.GetDeviceCaps(LOGPIXELSX);
    int nLy = dc.GetDeviceCaps(LOGPIXELSY);

    double dbInchPerMeter = 39.375;

    int nPMx = (int)((double)nLx * dbInchPerMeter);
    int nPMy = (int)((double)nLy * dbInchPerMeter);

    pInfoHead->biXPelsPerMeter = nPMx;//3780;
    pInfoHead->biYPelsPerMeter = nPMy;//3780;

    if (nBits <= 8)
    {
        int nColors = 1 << nBits;
        GetDIBColorTable(dc.m_hDC,0,nColors,pbmpinfo->bmiColors);
    }

    BYTE* pBits = NULL;

    pBits=new byte[nBitCounts];
    ZeroMemory((void *)pBits,nBitCounts);

    GetDIBits(pdc->m_hDC,(HBITMAP)bmp,0,nHeight,pBits,pbmpinfo,DIB_RGB_COLORS);

    BITMAPFILEHEADER    bmfHeader;
    int nFileHeadSize = sizeof(BITMAPFILEHEADER);

    ZeroMemory((void *)&bmfHeader,sizeof(BITMAPFILEHEADER));
    bmfHeader.bfType = *(WORD *)"BM";
    bmfHeader.bfSize = nFileHeadSize + nBitmapInfoSize + nBitCounts;
    bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + nBitmapInfoSize;

    HANDLE hFile = CreateFile(strFileName, 
        GENERIC_WRITE,
        FILE_SHARE_READ, 
        NULL, 
        CREATE_ALWAYS,
        FILE_FLAG_SEQUENTIAL_SCAN,
        NULL);
    ATLASSERT(INVALID_HANDLE_VALUE != hFile);
    SetFilePointer(hFile, 0, 0, FILE_BEGIN);
    DWORD dwWrittenSize;
    WriteFile(hFile, &bmfHeader, sizeof(BITMAPFILEHEADER), &dwWrittenSize, NULL);
    WriteFile(hFile, pbmpinfo, nBitmapInfoSize, &dwWrittenSize, NULL);
    WriteFile(hFile, pBits, nBitCounts, &dwWrittenSize, NULL);
    CloseHandle(hFile);

    delete pBits;
    delete[] pbmpinfo;

    return TRUE;
}
