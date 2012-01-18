#include "StdAfx.h"
#include "resource.h"
#include "OperateDlg.h"
#include "MainDlg.h"

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

LRESULT COperateDlg::OnMouseMove( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/ )
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
        Invalidate(FALSE);
    }
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

void COperateDlg::SetMainDlg( CMainDlg* dlg )
{
    m_DlgMain = dlg;
}
