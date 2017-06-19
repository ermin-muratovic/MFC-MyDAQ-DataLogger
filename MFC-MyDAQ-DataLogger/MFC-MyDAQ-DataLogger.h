
// MFC-MyDAQ-DataLogger.h: Hauptheaderdatei für die PROJECT_NAME-Anwendung
//

#pragma once

#ifndef __AFXWIN_H__
	#error "'stdafx.h' vor dieser Datei für PCH einschließen"
#endif

#include "resource.h"		// Hauptsymbole


// CMFCMyDAQDataLoggerApp:
// Siehe MFC-MyDAQ-DataLogger.cpp für die Implementierung dieser Klasse
//

class CMFCMyDAQDataLoggerApp : public CWinApp
{
public:
	CMFCMyDAQDataLoggerApp();

// Überschreibungen
public:
	virtual BOOL InitInstance();

// Implementierung

	DECLARE_MESSAGE_MAP()
};

extern CMFCMyDAQDataLoggerApp theApp;
