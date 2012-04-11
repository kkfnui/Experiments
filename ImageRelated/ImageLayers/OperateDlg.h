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
        MSG_WM_RBUTTONUP(OnRightButtonUp)
        MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLeftButtonDown)
        MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
        MESSAGE_HANDLER(WM_LBUTTONUP, OnLeftButtonUp)
        MESSAGE_HANDLER(ID_SELECT, OnSelect)
        MESSAGE_HANDLER(ID_SAVE, OnSave)
        COMMAND_ID_HANDLER(ID_PROPERTY, OnSetProperty)
		MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel)
	END_MSG_MAP()

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnLeftButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnRightButtonUp(WPARAM wParam, CPoint point);
    LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnLeftButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnSelect(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnSave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnSetProperty(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    void SetMainDlg(CMainDlg * dlg);
    void SetStatus(Status statu);
    BOOL SaveBitmapFile(CString strFileName, CBitmap &bmp, CDC *pdc);
private:
	CRect ZoomRect(const CRect& oldRect, const float& zoom);

private:
    CPoint m_ptLastClicked;
    CMainDlg * m_DlgMain;
    Status m_Statu;
    CLayer* m_pSelectedLayer;
public:
	LRESULT OnMouseWheel(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};
