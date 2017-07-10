#include "stdafx.h"
#include "afxdialogex.h"
#include "CustomGraph.h"

#define CUSTOMGRAPH_CLASSNAME    _T("MFCCustomGraphCtrl")  // Window class name

CCustomGraph::CCustomGraph()
{
	RegisterWindowClass();
}


CCustomGraph::~CCustomGraph()
{
}

BOOL CCustomGraph::RegisterWindowClass()
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();

	if (!(::GetClassInfo(hInst, CUSTOMGRAPH_CLASSNAME, &wndcls)))
	{
		// otherwise we need to register a new class
		wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc = ::DefWindowProc;
		wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
		wndcls.hInstance = hInst;
		wndcls.hIcon = NULL;
		wndcls.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wndcls.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
		wndcls.lpszMenuName = NULL;
		wndcls.lpszClassName = CUSTOMGRAPH_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}

	return TRUE;
}

BEGIN_MESSAGE_MAP(CCustomGraph, CWnd)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


void CCustomGraph::OnPaint()
{
	CPaintDC dc(this);

	SetBkMode(dc, TRANSPARENT);

	CRect rect;
	GetClientRect(&rect);
	int height = rect.Height() - 80;
	int width = rect.Width() - 80;
	int parts = 20;

	if (maxV != minV) {
		float dy = height / (float)(maxV - minV);

		CRect textRect;
		GetClientRect(&textRect);
		float range = (float)(maxV - minV);

		/* Draw vertical separators with labels */
		float step = range / parts;
		CString str("Test");
		CSize textSize = dc.GetTextExtent(str);
		textRect.OffsetRect(5, 40);
		for (int i = 0; i <= parts; i++) {
			dc.MoveTo(40, textRect.top);
			dc.LineTo(width+40, textRect.top);
			textRect.OffsetRect(0, -(textSize.cy / 2));
			if(step < 0.1)
				str.Format(L"%.2f", maxV - (i*step));
			else
				str.Format(L"%.1f", maxV - (i*step));
			dc.DrawText(str, -1, &textRect, DT_LEFT);
			textRect.OffsetRect(0, (textSize.cy / 2));
			textRect.OffsetRect(0, step*dy);
		}
		/****************************************/

		/* Draw horizontal separators */
		dc.MoveTo(40, 40);
		dc.LineTo(40, height+40);
		dc.MoveTo((width/2) + 40, 40);
		dc.LineTo((width/2) + 40, height + 40);
		dc.MoveTo(width + 40, 40);
		dc.LineTo(width + 40, height + 40);
		/******************************/

		/* Draw Graph Curves for all channels */
		list<MyDAQChannel>::iterator it;
		for (it = channels.begin(); it != channels.end(); it++) {
			if ((*it).samples.size() > 0) {
				float dx = width / (float)((*it).samples.size()-1);

				int rgb[3] = { 0 };
				rgb[distance(channels.begin(), it) % 3] = 250;
				COLORREF c1 = RGB(rgb[1], rgb[2], rgb[0]);
				CPen* pen = new CPen(0, 1, c1);

				dc.SelectObject(pen);

				list<MyDAQSample>::iterator itS;
				int i = 0;
				for (itS = (*it).samples.begin(); next(itS) != (*it).samples.end(); itS++) {
					float64 v = (*itS).value;
					float64 v1 = (*next(itS)).value;

					if (v > minV && v < maxV) {
						float vx = i*dx;
						float vy = (maxV - v)*dy;

						float vx1 = (i + 1)*dx;
						float vy1 = (maxV - v1)*dy;

						dc.SetPixelV(vx + 40, vy + 40, c1);
						dc.SetPixelV(vx + 40, vy + 40 - 1, c1);
						dc.SetPixelV(vx + 40, vy + 40 + 1, c1);

						if (v1 > minV && v1 < maxV) {
							dc.MoveTo(vx + 40, vy + 40);
							dc.LineTo(vx1 + 40, vy1 + 40);
						}
					}
					i++;
				}

				if (distance(channels.begin(), it) == 0) {
					itS = (*it).samples.begin();
					textRect.OffsetRect(0, -step*dy);
					textRect.OffsetRect(0, textSize.cy);
					dc.DrawText((LPCTSTR)(CA2T)(*itS).time.c_str(), -1, &textRect, DT_LEFT);

					itS = next(itS, (*it).samples.size() - 1);
					textRect.OffsetRect(width - 40, 0);
					dc.DrawText((LPCTSTR)(CA2T)(*itS).time.c_str(), -1, &textRect, DT_LEFT);
				}
			}
		}
		/**************************************/

		/* Draw Graph Legend */
		textRect.MoveToXY(80, 10);
		for (it = channels.begin(); it != channels.end(); it++) {
			str = CString((*it).name.c_str());
			textSize = dc.GetTextExtent(str);

			int rgb[3] = { 0 };
			rgb[distance(channels.begin(), it) % 3] = 250;
			COLORREF c1 = RGB(rgb[1], rgb[2], rgb[0]);

			for (int k = 4; k < 9; k++)
				for (int l = -2; l < 3; l++)
					dc.SetPixelV(textRect.left - k, textRect.top + (textSize.cy / 2) + l, c1);

			dc.DrawText(str, -1, &textRect, DT_LEFT);
			textRect.OffsetRect(textSize.cx + 20, 0);
		}
		/*********************/
	}
}


BOOL CCustomGraph::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return CWnd::OnEraseBkgnd(pDC);
}

void CCustomGraph::setMinV(float mv)
{
	minV = mv;
}

void CCustomGraph::setMaxV(float mv)
{
	maxV = mv;
}

void CCustomGraph::setChannels(list<MyDAQChannel> c)
{
	channels = c;
}
