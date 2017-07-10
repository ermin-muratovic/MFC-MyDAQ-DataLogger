
// MFC-MyDAQ-DataLoggerDlg.h: Headerdatei
//

#pragma once
#include "afxwin.h"
#include <list>
#include <string>
#include "afxcmn.h"
#include "MyDAQ.h"
#include "CustomGraph.h"

using namespace std;

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
	CListBox channelsList;
	CStatic dataLoggerLabel;
	CEdit samplingRateTextfield;
	CEdit minVTextfield;
	CEdit maxVTextfield;
	CSpinButtonCtrl samplingRateSpinner;
	CSpinButtonCtrl minVSpinner;
	CSpinButtonCtrl maxVSpinner;
	CButton startButton;
	CButton stopButton;
	CCustomGraph graphComponent;
	
	MyDAQ myDAQ;
	bool isRunning;

	void updateNumericTextField(CEdit* field, float value, LPCWSTR format);
	void updateDevicesCombobox(list<string> devices);
	void enableFields(bool enable);
	void updateChannels();
	void updateChannelsList();
	void updateButtons();
	void initGUI();

	afx_msg void OnBnClickedButtonstart();
	afx_msg void OnBnClickedButtonstop();
	afx_msg void OnEnKillfocusSamplingrate();
	afx_msg void OnEnKillfocusMinv();
	afx_msg void OnEnKillfocusMaxv();
	afx_msg void OnLbnSelchangeChannelslist();
	afx_msg void OnEnChangeMaxv();
	afx_msg void OnEnChangeMinv();
	afx_msg void OnEnChangeSamplingrate();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnDropdownDevicecombo();
	afx_msg void OnCbnSelchangeDevicecombo();
	afx_msg void OnBnClickedButtonselectfile();
	CEdit filePath;
	CButton logToFileCheckbox;
};
