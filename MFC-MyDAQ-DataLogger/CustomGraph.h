#pragma once
#include "afxwin.h"
#include "MyDAQ.h"

class CCustomGraph : public CWnd
{
public:
	CCustomGraph();
	~CCustomGraph();

	BOOL RegisterWindowClass();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

public:
	void setMinV(float mv);
	void setMaxV(float mv);

	void setChannels(list<MyDAQChannel> c);

protected:
	float minV;
	float maxV;
	int samplingRate;
	list<MyDAQChannel> channels;
};

