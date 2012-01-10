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
    InitData();
    SetTimer(REFLASH_TIME, 1000);
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
    HDC dc;
    CRect rect;
    PAINTSTRUCT ps;
    dc = BeginPaint(&ps);   
    GetClientRect(&rect);
    SYSTEMTIME sysTime;
    GetLocalTime(&sysTime);
    int num = sysTime.wHour /10;
    DrawNum(dc, num, rect.left);
    num = sysTime.wHour % 10;
    DrawNum(dc, num, WIDTH_RATE);
    num = sysTime.wMinute /10;
    DrawNum(dc, num, 2*WIDTH_RATE + 10);
    num = sysTime.wMinute % 10;
    DrawNum(dc, num, 3*WIDTH_RATE + 10);
    num = sysTime.wSecond / 10;
    DrawNum(dc, num, 4*WIDTH_RATE + 20);
    num = sysTime.wSecond % 10;
    DrawNum(dc, num, 5*WIDTH_RATE + 20);
    EndPaint(&ps);
    return S_OK;
}

LRESULT CMainDlg::OnTimer( UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
    UINT_PTR nEventId = (UINT_PTR)wParam;
    KillTimer(nEventId);
    if (nEventId == REFLASH_TIME)
    {
        Invalidate();
        SetTimer(REFLASH_TIME, 1000);
    }
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
    UnInitData();
	DestroyWindow();
	::PostQuitMessage(nVal);
}

LRESULT CMainDlg::InitData()
{
    m_lZoom = 1.0L;
    int nWidth = WIDTH_RATE * m_lZoom;
    int nHeight = HEIGHT_RATE * m_lZoom;
    POINT* pt1 = new POINT[4];
    pt1[0].x = static_cast<LONG>(nWidth * 0.07L);
    pt1[0].y = static_cast<LONG>(nHeight * 0.05L);
    pt1[1].x = static_cast<LONG>(nWidth * 0.93L);
    pt1[1].y = static_cast<LONG>(nHeight * 0.05L);
    pt1[2].x = static_cast<LONG>(nWidth * 0.85L);
    pt1[2].y = static_cast<LONG>(nHeight * 0.1L);
    pt1[3].x = static_cast<LONG>(nWidth * 0.17L);
    pt1[3].y = static_cast<LONG>(nHeight * 0.1L);
    m_pSections[0].points = pt1;
    m_pSections[0].size= 4;

    POINT* pt2 = new POINT[5];
    pt2[0].x = static_cast<LONG>(nWidth * 0.85L);
    pt2[0].y = static_cast<LONG>(nHeight * 0.12L);
    pt2[1].x = static_cast<LONG>(nWidth * 0.95);
    pt2[1].y = static_cast<LONG>(nHeight * 0.05L);
    pt2[2].x = static_cast<LONG>(nWidth * 0.95);
    pt2[2].y = static_cast<LONG>(nHeight * 0.45L);
    pt2[3].x = static_cast<LONG>(nWidth * 0.90L);
    pt2[3].y = static_cast<LONG>(nHeight * 0.5L);
    pt2[4].x = static_cast<LONG>(nWidth * 0.85L);
    pt2[4].y = static_cast<LONG>(nHeight * 0.44L);
    m_pSections[1].points = pt2;
    m_pSections[1].size = 5;

    POINT* pt3 = new POINT[5];
    pt3[0].x = static_cast<LONG>(nWidth * 0.90L);
    pt3[0].y = static_cast<LONG>(nHeight * 0.5L);
    pt3[1].x = static_cast<LONG>(nWidth * 0.95L);
    pt3[1].y = static_cast<LONG>(nHeight * 0.55L);
    pt3[2].x = static_cast<LONG>(nWidth * 0.95L);
    pt3[2].y = static_cast<LONG>(nHeight * 0.95L);
    pt3[3].x = static_cast<LONG>(nWidth * 0.85L);
    pt3[3].y = static_cast<LONG>(nHeight * 0.88);
    pt3[4].x = static_cast<LONG>(nWidth * 0.85L);
    pt3[4].y = static_cast<LONG>(nHeight * 0.55L);
    m_pSections[2].points = pt3;
    m_pSections[2].size = 5;

    POINT* pt4 = new POINT[4];
    pt4[0].x = static_cast<LONG>(nWidth * 0.08);
    pt4[0].y = static_cast<LONG>(nHeight * 0.95);
    pt4[1].x = static_cast<LONG>(nWidth * 0.18L);
    pt4[1].y = static_cast<LONG>(nHeight * 0.89L);
    pt4[2].x = static_cast<LONG>(nWidth * 0.83L);
    pt4[2].y = static_cast<LONG>(nHeight * 0.89L);
    pt4[3].x = static_cast<LONG>(nWidth * 0.92);
    pt4[3].y = static_cast<LONG>(nHeight * 0.95);
    m_pSections[3].points = pt4;
    m_pSections[3].size = 4;

    POINT* pt5 = new POINT[5];
    pt5[0].x = static_cast<LONG>(nWidth * 0.05L);
    pt5[0].y = static_cast<LONG>(nHeight * 0.55L);
    pt5[1].x = static_cast<LONG>(nWidth * 0.1L);
    pt5[1].y = static_cast<LONG>(nHeight * 0.5L);
    pt5[2].x = static_cast<LONG>(nWidth * 0.15L);
    pt5[2].y = static_cast<LONG>(nHeight * 0.55L);
    pt5[3].x = static_cast<LONG>(nWidth * 0.15L);
    pt5[3].y = static_cast<LONG>(nHeight * 0.9L);
    pt5[4].x = static_cast<LONG>(nWidth * 0.05);
    pt5[4].y = static_cast<LONG>(nHeight * 0.95);
    m_pSections[4].points = pt5;
    m_pSections[4].size = 5;

    POINT* pt6 = new POINT[5];
    pt6[0].x = static_cast<LONG>(nWidth * 0.05L);
    pt6[0].y = static_cast<LONG>(nHeight * 0.05L);
    pt6[1].x = static_cast<LONG>(nWidth * 0.15L);
    pt6[1].y = static_cast<LONG>(nHeight * 0.1L);
    pt6[2].x = static_cast<LONG>(nWidth * 0.15L);
    pt6[2].y = static_cast<LONG>(nHeight * 0.44L);
    pt6[3].x = static_cast<LONG>(nWidth * 0.09L);
    pt6[3].y = static_cast<LONG>(nHeight * 0.5L);
    pt6[4].x = static_cast<LONG>(nWidth * 0.05L);
    pt6[4].y = static_cast<LONG>(nHeight * 0.46L);
    m_pSections[5].points = pt6;
    m_pSections[5].size = 5;

    POINT* pt7 = new POINT[6];
    pt7[0].x = static_cast<LONG>(nWidth * 0.1L);
    pt7[0].y = static_cast<LONG>(nHeight * 0.5L);
    pt7[1].x = static_cast<LONG>(nWidth * 0.16L);
    pt7[1].y = static_cast<LONG>(nHeight * 0.47L);
    pt7[2].x = static_cast<LONG>(nWidth * 0.85L);
    pt7[2].y = static_cast<LONG>(nHeight * 0.47L);
    pt7[3].x = static_cast<LONG>(nWidth * 0.90L);
    pt7[3].y = static_cast<LONG>(nHeight * 0.5L);
    pt7[4].x = static_cast<LONG>(nWidth * 0.83L);
    pt7[4].y = static_cast<LONG>(nHeight * 0.53L);
    pt7[5].x = static_cast<LONG>(nWidth * 0.17L);
    pt7[5].y = static_cast<LONG>(nHeight * 0.53L);
    m_pSections[6].size = 6;
    m_pSections[6].points = pt7;
    
    Section* num0= new Section[6];
    num0[0] = m_pSections[0];
    num0[1] = m_pSections[1];
    num0[2] = m_pSections[2];
    num0[3] = m_pSections[3];
    num0[4] = m_pSections[4];
    num0[5] = m_pSections[5];
    m_pNumbers[0].sections = num0;
    m_pNumbers[0].size = 6;

    Section* num1 = new Section[2];
    num1[0] = m_pSections[1];
    num1[1] = m_pSections[2];
    m_pNumbers[1].sections = num1;
    m_pNumbers[1].size = 2;

    Section* num2 = new Section[5];
    num2[0] = m_pSections[0];
    num2[1] = m_pSections[1];
    num2[2] = m_pSections[6];
    num2[3] = m_pSections[4];
    num2[4] = m_pSections[3];
    m_pNumbers[2].sections = num2;
    m_pNumbers[2].size = 5;

    Section* num3 = new Section[5];
    num3[0] = m_pSections[0];
    num3[1] = m_pSections[1];
    num3[2] = m_pSections[6];
    num3[3] = m_pSections[2];
    num3[4] = m_pSections[3];
    m_pNumbers[3].sections = num3;
    m_pNumbers[3].size = 5;

    Section* num4 = new Section[4];
    num4[0] = m_pSections[5];
    num4[1] = m_pSections[6];
    num4[2] = m_pSections[1];
    num4[3] = m_pSections[2];
    m_pNumbers[4].sections = num4;
    m_pNumbers[4].size = 4;

    Section* num5 = new Section[5];
    num5[0] = m_pSections[0];
    num5[1] = m_pSections[5];
    num5[2] = m_pSections[6];
    num5[3] = m_pSections[2];
    num5[4] = m_pSections[3];
    m_pNumbers[5].sections = num5;
    m_pNumbers[5].size = 5;

    Section* num6 = new Section[6];
    num6[0] = m_pSections[0];
    num6[1] = m_pSections[5];
    num6[2] = m_pSections[4];
    num6[3] = m_pSections[3];
    num6[4] = m_pSections[2];
    num6[5] = m_pSections[6];
    m_pNumbers[6].sections = num6;
    m_pNumbers[6].size = 6;

    Section* num7 = new Section[3];
    num7[0] = m_pSections[0];
    num7[1] = m_pSections[1];
    num7[2] = m_pSections[2];
    m_pNumbers[7].sections = num7;
    m_pNumbers[7].size = 3;

    Section* num8 = new Section[7];
    num8[0] = m_pSections[0];
    num8[1] = m_pSections[1];
    num8[2] = m_pSections[2];
    num8[3] = m_pSections[3];
    num8[4] = m_pSections[4];
    num8[5] = m_pSections[5];
    num8[6] = m_pSections[6];
    m_pNumbers[8].sections = num8;
    m_pNumbers[8].size = 7;

    Section* num9 = new Section[6];
    num9[0] = m_pSections[0];
    num9[1] = m_pSections[1];
    num9[2] = m_pSections[2];
    num9[3] = m_pSections[3];
    num9[4] = m_pSections[6];
    num9[5] = m_pSections[5];
    m_pNumbers[9].sections = num9;
    m_pNumbers[9].size = 6;

    return S_OK;
}

void CMainDlg::DrawNum( HDC dc, int nNum, int left)
{
    Number num = m_pNumbers[nNum];
    HBRUSH hBr = (HBRUSH) ::GetStockObject(BLACK_BRUSH);
    for (int i = 0; i < num.size ;i++)
    {
        CRgn rgn;
        rgn.CreatePolygonRgn(static_cast<LPPOINT>(num.sections[i].points),num.sections[i].size, ALTERNATE);
        rgn.OffsetRgn(left, 20);
        FillRgn(dc, rgn, hBr);
    }
}

LRESULT CMainDlg::UnInitData()
{
    for(int i = 0; i < 7; ++i)
    {
        delete [] m_pSections[i].points;
    }
    return S_OK;
}
