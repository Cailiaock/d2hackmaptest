//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#include "stdhdrs.h"
#include <windowsx.h>
#include "autotooltip.h"

static LPCTSTR _AUTO_TOOLTIP_CONTROL_ID = _T("AUTO TOOLTIP CONTROL ID");

CAutoToolTip::CAutoToolTip(void)
			:m_lpfnDefWinProc(NULL)
{
}

CAutoToolTip::~CAutoToolTip(void)
{
	if(m_lpfnDefWinProc)
		SetWindowLong(m_hwnd, GWL_WNDPROC, (LPARAM)m_lpfnDefWinProc);
}

void CAutoToolTip::SubClass(HWND hwnd)
{
	CAutoToolTip *_this = reinterpret_cast<CAutoToolTip*>(::GetProp(hwnd, _AUTO_TOOLTIP_CONTROL_ID));
	if(_this != NULL) return;
	if(m_toolTip.Create(hwnd, NULL))
	{
		::SetProp(hwnd, _AUTO_TOOLTIP_CONTROL_ID, (HANDLE)this);
		m_lpfnDefWinProc = (WNDPROC)SetWindowLong(hwnd, GWL_WNDPROC, (LPARAM)SubClassProc);
		m_hwnd = hwnd;
	}
}

int CAutoToolTip::GetStringWidth(HWND hwnd, LPTSTR szText)
{
	SIZE   sz;
	HDC hdc = GetDC(hwnd);
	GetTextExtentPoint32(hdc, szText, _tcslen(szText), &sz);
	ReleaseDC(hwnd, hdc);

	// Adjust the width for the vertical scroll bar and the left and right border.
	sz.cx += GetSystemMetrics(SM_CXVSCROLL) + 2*GetSystemMetrics(SM_CXEDGE);
	return sz.cx;
}

LRESULT CALLBACK CAutoToolTip::SubClassProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CAutoToolTip *_this = reinterpret_cast<CAutoToolTip*>(::GetProp(hwnd, _AUTO_TOOLTIP_CONTROL_ID));
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			RECT rt;
			TCHAR szText[1024];
			GetWindowText(hwnd, szText, COUNTOF(szText)-1);
			GetWindowRect(hwnd, &rt);
			if(szText[0] != _T('\0') && rt.right-rt.left < GetStringWidth(hwnd, szText))
				_this->m_toolTip.Show(szText);
			else
			{
				GlobalReportStatus(IDS_TIP_LISTIVEW);
				_this->m_toolTip.Hide();
			}
		}
		break;
	}

	return ::CallWindowProc(_this->m_lpfnDefWinProc, hwnd, uMsg, wParam, lParam);
}
