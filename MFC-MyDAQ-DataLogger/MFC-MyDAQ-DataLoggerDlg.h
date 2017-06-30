
// MFC-MyDAQ-DataLoggerDlg.h: Headerdatei
//

#pragma once
#include "afxwin.h"
#include <string>

// CMFCMyDAQDataLoggerDlg-Dialogfeld
class CMFCMyDAQDataLoggerDlg : public CDialogEx
{
// Konstruktion
public:
	CMFCMyDAQDataLoggerDlg(CWnd* pParent = NULL);	// Standardkonstruktor

// Dialogfelddaten
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCMYDAQDATALOGGER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung


// Implementierung
protected:
	HICON m_hIcon;

	// Generierte Funktionen für die Meldungstabellen
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox deviceCombobox;
};
