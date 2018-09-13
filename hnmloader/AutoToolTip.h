//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "ToolTip.h"

class CAutoToolTip
{
public:
	CAutoToolTip(void);
	~CAutoToolTip(void);
	void SubClass(HWND hwnd);
private:
	static int GetStringWidth(HWND hwnd, LPTSTR szText);
	static LRESULT CALLBACK SubClassProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	HWND m_hwnd;
	WNDPROC m_lpfnDefWinProc;
	winToolTip	m_toolTip;
};
