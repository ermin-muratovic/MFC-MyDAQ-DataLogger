
// MFC-MyDAQ-DataLogger.h: Hauptheaderdatei f�r die PROJECT_NAME-Anwendung
//

#pragma once

#ifndef __AFXWIN_H__
	#error "'stdafx.h' vor dieser Datei f�r PCH einschlie�en"
#endif

#include "resource.h"		// Hauptsymbole


// CMFCMyDAQDataLoggerApp:
// Siehe MFC-MyDAQ-DataLogger.cpp f�r die Implementierung dieser Klasse
//

class CMFCMyDAQDataLoggerApp : public CWinApp
{
public:
	CMFCMyDAQDataLoggerApp();

// �berschreibungen
public:
	virtual BOOL InitInstance();

// Implementierung

	DECLARE_MESSAGE_MAP()
};

extern CMFCMyDAQDataLoggerApp theApp;
