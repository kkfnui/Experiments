#pragma once
class CMainDlg;
class CLayer;
class COperateDlg : public CDialogImpl<COperateDlg>
{
public:
    COperateDlg(void);
    ~COperateDlg(void);

    enum {IDD = IDD_OPERATE};
    enum Status { SELECT, PREMOVE, MOVE };

    BEGIN_MSG_MAP(COperateDlg)
        MESSAGE_HANDLER(WM_PAINT, OnPaint)
        MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLeftButtonDown)
        MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
        MESSAGE_HANDLER(WM_LBUTTONUP, OnLeftButtonUp)
        MESSAGE_HANDLER(ID_SELECT, OnSelect)
    END_MSG_MAP()

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnLeftButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnLeftButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnSelect(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    void SetMainDlg(CMainDlg * dlg);
    void SetStatus(Status statu);

private:
    CPoint m_ptLastClicked;
    CMainDlg * m_DlgMain;
    Status m_Statu;
    CLayer* m_pSelectedLayer;
};
