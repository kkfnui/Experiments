// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Layer.h"
#include "OperateDlg.h"

class CMainDlg : public CDialogImpl<CMainDlg>
{
    friend class COperateDlg;
public:
    CMainDlg(void);
    ~CMainDlg();
	enum { IDD = IDD_MAINDLG };
	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		//MESSAGE_HANDLER(WM_PAINT, OnPaint)
        //MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLeftButtonDown)
        //MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
        //MESSAGE_HANDLER(WM_LBUTTONUP, OnLeftButtonUp)
		COMMAND_ID_HANDLER(ID_SAVE, OnSave)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
        NOTIFY_HANDLER(IDC_LIST_IMAGE_LAYERS, LVN_ITEMACTIVATE, OnLvnItemActivateListImageLayers)
    END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//LRESULT OnLeftButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//LRESULT OnLeftButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    HBITMAP ConvertDIB2DDB(CDCHandle dc, LPBITMAPINFO bitmapInfo);
    LRESULT OnLvnItemActivateListImageLayers(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
private:
	std::vector<CLayer*> m_Layers;
    CPoint m_ptLastClicked;
    CLayer* m_pSelectedLayer; 
    COperateDlg* m_operateDlg; 
    CListViewCtrl m_list;
};
