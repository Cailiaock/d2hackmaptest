//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#include "stdhdrs.h"
#include "ToolTip.h"
#include <commctrl.h>

// make sure to initialize tooltip first
static BOOL init = winToolTip::Init();
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

winToolTip::winToolTip  ()
		:m_hwndParent(NULL)
		,m_hwndTT(NULL)
{
}

winToolTip::~winToolTip  ()
{

}


BOOL winToolTip::Init()
{
	// struct specifying control classes to register
	INITCOMMONCONTROLSEX iccex; 

	/* INITIALIZE COMMON CONTROLS */
	iccex.dwICC = ICC_WIN95_CLASSES;
	iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	return InitCommonControlsEx(&iccex);
}

BOOL winToolTip::Create(HWND hwndParent, HINSTANCE hAppInst)
{
	m_hwndParent = hwndParent;
	m_hAppInst = hAppInst;
	/* CREATE A TOOLTIP WINDOW */
	m_hwndTT = CreateWindowEx(WS_EX_TOPMOST,
			TOOLTIPS_CLASS,
			NULL,
			WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,		
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			hwndParent,	NULL, hAppInst, NULL);

	SetWindowPos(m_hwndTT, HWND_TOPMOST,
				0, 0, 0, 0,
				SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

	return m_hwndTT != NULL;
}

void winToolTip::Show(LPTSTR lpszText, LPRECT lpRect)
{
	// struct specifying info about tool in tooltip control
	TOOLINFO ti;
	unsigned int uid = 0;       // for ti initialization
	RECT rt;
	if(lpRect)
		memcpy(&rt, lpRect, sizeof(RECT));
	else
		GetClientRect (m_hwndParent, &rt);

	/* INITIALIZE MEMBERS OF THE TOOLINFO STRUCTURE */
	ti.cbSize = sizeof(TOOLINFO);
	ti.uFlags = TTF_SUBCLASS;
	ti.hwnd = m_hwndParent;
	ti.hinst = m_hAppInst;
	ti.uId = uid;
	ti.lpszText = lpszText;
		// Tooltip control will cover the whole window
	ti.rect.left = rt.left;    
	ti.rect.top = rt.top;
	ti.rect.right = rt.right;
	ti.rect.bottom = rt.bottom;

	/* SEND AN ADDTOOL MESSAGE TO THE TOOLTIP CONTROL WINDOW */
	SendMessage(m_hwndTT, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti);	
}

void winToolTip::Hide()
{
	ShowWindow(m_hwndTT, SW_HIDE);
}

