
// MFC-MyDAQ-DataLoggerDlg.cpp: Implementierungsdatei
//
#include "stdafx.h"
#include "MFC-MyDAQ-DataLogger.h"
#include "MFC-MyDAQ-DataLoggerDlg.h"
#include "afxdialogex.h"
#include "MyDAQ.h"
#include "CustomGraph.h"
#include <NIDAQmx.h>
#include <string>
#include <list>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

// CAboutDlg-Dialogfeld für Anwendungsbefehl "Info"

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialogfelddaten
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

// Implementierung
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCMyDAQDataLoggerDlg-Dialogfeld



CMFCMyDAQDataLoggerDlg::CMFCMyDAQDataLoggerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCMYDAQDATALOGGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	isRunning = false;
	myDAQ = MyDAQ::MyDAQ();
}

void CMFCMyDAQDataLoggerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DEVICECOMBO, deviceCombobox);
	DDX_Control(pDX, IDC_CHANNELSLIST, channelsList);
	DDX_Control(pDX, IDC_DATALOGGERLABEL, dataLoggerLabel);
	DDX_Control(pDX, IDC_SAMPLINGRATE, samplingRateTextfield);
	DDX_Control(pDX, IDC_MINV, minVTextfield);
	DDX_Control(pDX, IDC_MAXV, maxVTextfield);
	DDX_Control(pDX, IDC_SPIN1, samplingRateSpinner);
	DDX_Control(pDX, IDC_SPIN2, minVSpinner);
	DDX_Control(pDX, IDC_SPIN3, maxVSpinner);
	DDX_Control(pDX, IDC_BUTTONSTART, startButton);
	DDX_Control(pDX, IDC_BUTTONSTOP, stopButton);
	DDX_Control(pDX, IDC_GRAPH, graphComponent);
}

BEGIN_MESSAGE_MAP(CMFCMyDAQDataLoggerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CMFCMyDAQDataLoggerDlg::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CMFCMyDAQDataLoggerDlg::OnDeltaposSpin2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CMFCMyDAQDataLoggerDlg::OnDeltaposSpin3)
	ON_EN_CHANGE(IDC_MAXV, &CMFCMyDAQDataLoggerDlg::OnEnChangeMaxv)
	ON_EN_CHANGE(IDC_MINV, &CMFCMyDAQDataLoggerDlg::OnEnChangeMinv)
	ON_EN_CHANGE(IDC_SAMPLINGRATE, &CMFCMyDAQDataLoggerDlg::OnEnChangeSamplingrate)
	ON_BN_CLICKED(IDC_BUTTONSTART, &CMFCMyDAQDataLoggerDlg::OnBnClickedButtonstart)
	ON_BN_CLICKED(IDC_BUTTONSTOP, &CMFCMyDAQDataLoggerDlg::OnBnClickedButtonstop)
	ON_EN_KILLFOCUS(IDC_SAMPLINGRATE, &CMFCMyDAQDataLoggerDlg::OnEnKillfocusSamplingrate)
	ON_EN_KILLFOCUS(IDC_MINV, &CMFCMyDAQDataLoggerDlg::OnEnKillfocusMinv)
	ON_EN_KILLFOCUS(IDC_MAXV, &CMFCMyDAQDataLoggerDlg::OnEnKillfocusMaxv)
	ON_LBN_SELCHANGE(IDC_CHANNELSLIST, &CMFCMyDAQDataLoggerDlg::OnLbnSelchangeChannelslist)
	ON_CBN_DROPDOWN(IDC_DEVICECOMBO, &CMFCMyDAQDataLoggerDlg::OnCbnDropdownDevicecombo)
	ON_CBN_SELCHANGE(IDC_DEVICECOMBO, &CMFCMyDAQDataLoggerDlg::OnCbnSelchangeDevicecombo)
END_MESSAGE_MAP()


// CMFCMyDAQDataLoggerDlg-Meldungshandler

BOOL CMFCMyDAQDataLoggerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Hinzufügen des Menübefehls "Info..." zum Systemmenü.

	// IDM_ABOUTBOX muss sich im Bereich der Systembefehle befinden.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Symbol für dieses Dialogfeld festlegen.  Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden

	initGUI();

	return TRUE;  // TRUE zurückgeben, wenn der Fokus nicht auf ein Steuerelement gesetzt wird
}

void CMFCMyDAQDataLoggerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// Wenn Sie dem Dialogfeld eine Schaltfläche "Minimieren" hinzufügen, benötigen Sie
//  den nachstehenden Code, um das Symbol zu zeichnen.  Für MFC-Anwendungen, die das 
//  Dokument/Ansicht-Modell verwenden, wird dies automatisch ausgeführt.

void CMFCMyDAQDataLoggerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Gerätekontext zum Zeichnen

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Symbol in Clientrechteck zentrieren
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Symbol zeichnen
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Die System ruft diese Funktion auf, um den Cursor abzufragen, der angezeigt wird, während der Benutzer
//  das minimierte Fenster mit der Maus zieht.
HCURSOR CMFCMyDAQDataLoggerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCMyDAQDataLoggerDlg::updateDevicesCombobox(list<string> devices)
{
	deviceCombobox.ResetContent();
	list<string>::iterator it;
	for (it = devices.begin(); it != devices.end(); it++) {
		deviceCombobox.AddString((LPCTSTR)(CA2T)(*it).c_str());
	}
	if (devices.size() > 0)
		deviceCombobox.SetCurSel(0);
}

void CMFCMyDAQDataLoggerDlg::initGUI()
{
	// modify font
	CFont *f1 = new CFont;
	string font = "Arial Bold";
	f1->CreatePointFont(260, (LPCTSTR)font.c_str());
	dataLoggerLabel.SetFont(f1);

	updateDevicesCombobox(myDAQ.getConnectedDevices());
	updateChannelsList();

	updateNumericTextField(&samplingRateTextfield, myDAQ.getSamplingRate(), L"%.f");
	updateNumericTextField(&minVTextfield, myDAQ.getMinV(), L"%.1f");
	updateNumericTextField(&maxVTextfield, myDAQ.getMaxV(), L"%.1f");

	graphComponent.setMinV(myDAQ.getMinV());
	graphComponent.setMaxV(myDAQ.getMaxV());
	graphComponent.setSamplingRate(myDAQ.getSamplingRate());

	graphComponent.Invalidate();

	updateButtons();
}

void CMFCMyDAQDataLoggerDlg::updateChannelsList()
{
	CString myDAQname;
	int index = deviceCombobox.GetCurSel();

	if (index >= 0) {
		deviceCombobox.GetLBText(index, myDAQname);
		char aiBuffer[1000] = { '\0' };
		DAQmxGetDevAIPhysicalChans((CT2CA)myDAQname, aiBuffer, 1000);
		list<string> aichannels = myDAQ.splitString(string(aiBuffer), ',', 2);

		channelsList.ResetContent();

		list<string>::iterator it;
		for (it = aichannels.begin(); it != aichannels.end(); it++) {
			int32 types[10] = { 0 };
			DAQmxGetPhysicalChanAISupportedMeasTypes((*it).c_str(), types, 10);
			CString aichan((*it).c_str());

			int32* element = find(begin(types), end(types), 10303);
			if (element != end(types)) {
				list<string> channeldescriptor = myDAQ.splitString((*it), '/', 1);
				list<string>::iterator cdit = channeldescriptor.begin();
				advance(cdit, 1);

				channelsList.AddString((LPCTSTR)(CA2T)(*cdit).c_str());
			}
		}
	}
}

void CMFCMyDAQDataLoggerDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	myDAQ.setSamplingRate(myDAQ.getSamplingRate() - pNMUpDown->iDelta);
	updateNumericTextField(&samplingRateTextfield, myDAQ.getSamplingRate(), L"%.f");

	*pResult = 0;
}


void CMFCMyDAQDataLoggerDlg::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	myDAQ.setMinV(myDAQ.getMinV() - (float)(pNMUpDown->iDelta/10.0));
	updateNumericTextField(&minVTextfield, myDAQ.getMinV(), L"%.1f");

	graphComponent.setMinV(myDAQ.getMinV());
	graphComponent.Invalidate();

	*pResult = 0;
}


void CMFCMyDAQDataLoggerDlg::OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	myDAQ.setMaxV(myDAQ.getMaxV() - (float)(pNMUpDown->iDelta / 10.0));
	updateNumericTextField(&maxVTextfield, myDAQ.getMaxV(), L"%.1f");

	graphComponent.setMaxV(myDAQ.getMaxV());
	graphComponent.Invalidate();

	*pResult = 0;
}


void CMFCMyDAQDataLoggerDlg::OnEnChangeMaxv()
{
	CString str;
	maxVTextfield.GetWindowTextW(str);
	myDAQ.setMaxV(atof((CT2CA)str));

	graphComponent.setMaxV(myDAQ.getMaxV());
	graphComponent.Invalidate();
}


void CMFCMyDAQDataLoggerDlg::OnEnChangeMinv()
{
	CString str;
	minVTextfield.GetWindowTextW(str);
	myDAQ.setMinV(atof((CT2CA)str));

	graphComponent.setMinV(myDAQ.getMinV());
	graphComponent.Invalidate();
}


void CMFCMyDAQDataLoggerDlg::OnEnChangeSamplingrate()
{
	CString str;
	samplingRateTextfield.GetWindowTextW(str);
	myDAQ.setSamplingRate(atoi((CT2CA)str));
}


void CMFCMyDAQDataLoggerDlg::OnEnKillfocusSamplingrate()
{
	updateNumericTextField(&samplingRateTextfield, myDAQ.getSamplingRate(), L"%.f");
}


void CMFCMyDAQDataLoggerDlg::OnEnKillfocusMinv()
{
	updateNumericTextField(&minVTextfield, myDAQ.getMinV(), L"%.1f");
}


void CMFCMyDAQDataLoggerDlg::OnEnKillfocusMaxv()
{
	updateNumericTextField(&maxVTextfield, myDAQ.getMaxV(), L"%.1f");
}


void CMFCMyDAQDataLoggerDlg::updateNumericTextField(CEdit* field, float value, LPCWSTR format)
{
	CString str;
	str.Format(format, value);
	field->SetWindowText(str);
}


void CMFCMyDAQDataLoggerDlg::updateButtons()
{
	if (myDAQ.getConnectedDevices().size() == 0) {
		startButton.EnableWindow(FALSE);
		stopButton.EnableWindow(FALSE);
	}
	else if (isRunning) {
		startButton.EnableWindow(FALSE);
		stopButton.EnableWindow(TRUE);
	}
	else {
		stopButton.EnableWindow(FALSE);
		startButton.EnableWindow(TRUE);
	}
}


UINT run(LPVOID Param) {
	ASSERT(Param != NULL);
	CMFCMyDAQDataLoggerDlg* dlg = reinterpret_cast<CMFCMyDAQDataLoggerDlg*>(Param);

	while (dlg->isRunning) {
		int result = dlg->myDAQ.readChannels();
		dlg->graphComponent.setChannels(dlg->myDAQ.getChannels());
		dlg->graphComponent.Invalidate();
		// if(writeToFile) dlg->myDAQ.getChannels() --> write to File
	}

	return TRUE;
}


void CMFCMyDAQDataLoggerDlg::OnBnClickedButtonstart()
{
	isRunning = true;
	updateButtons();
	enableFields(FALSE);

	graphComponent.setMinV(myDAQ.getMinV());
	graphComponent.setMaxV(myDAQ.getMaxV());
	graphComponent.setSamplingRate(myDAQ.getSamplingRate());
	AfxBeginThread(run, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
}


void CMFCMyDAQDataLoggerDlg::OnBnClickedButtonstop()
{
	isRunning = false;
	updateButtons();
	enableFields(TRUE);
}


void CMFCMyDAQDataLoggerDlg::enableFields(bool enable)
{
	deviceCombobox.EnableWindow(enable);
	samplingRateTextfield.EnableWindow(enable);
	samplingRateSpinner.EnableWindow(enable);
	// minVTextfield.EnableWindow(enable);
	// minVSpinner.EnableWindow(enable);
	// maxVTextfield.EnableWindow(enable);
	// maxVSpinner.EnableWindow(enable);
}

void CMFCMyDAQDataLoggerDlg::updateChannels() {
	CString myDAQname;
	list<CString> channels;
	int idxC = deviceCombobox.GetCurSel();

	if (idxC >= 0) {
		deviceCombobox.GetLBText(idxC, myDAQname);

		int count = channelsList.GetSelCount();
		CArray<int, int> selection;
		selection.SetSize(count);
		channelsList.GetSelItems(count, selection.GetData());

		int *idxCh = selection.GetData();

		for (int i = 0; i < count; i++) {
			CString channelName;
			channelsList.GetText(idxCh[i], channelName);
			channels.push_back(channelName);
		}	
	}
	myDAQ.setChannels(myDAQname, channels);
}

void CMFCMyDAQDataLoggerDlg::OnLbnSelchangeChannelslist()
{
	updateChannels();
}

void CMFCMyDAQDataLoggerDlg::OnCbnDropdownDevicecombo()
{
	updateDevicesCombobox(myDAQ.getConnectedDevices());
	updateButtons();
}


void CMFCMyDAQDataLoggerDlg::OnCbnSelchangeDevicecombo()
{
	updateChannelsList();
	updateChannels();
}
