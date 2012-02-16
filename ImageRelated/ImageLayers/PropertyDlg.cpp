#include "StdAfx.h"
#include "PropertyDlg.h"

CPropertyDlg::CPropertyDlg(void)
{
}

CPropertyDlg::~CPropertyDlg(void)
{
}

LRESULT CPropertyDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    EndDialog(wID);
    return S_OK;
}

LRESULT CPropertyDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    EndDialog(wID);
    return S_OK;
}

LRESULT CPropertyDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
    CenterWindow(GetParent());
    return S_OK;
}
