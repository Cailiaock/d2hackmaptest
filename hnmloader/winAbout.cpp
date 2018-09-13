//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#include "stdhdrs.h"
#include "winabout.h"
#include "resource.h"

winAbout::winAbout(void)
		:CDialogBase(IDD_ABOUT)
		,m_hbrBG(NULL)
{
}

winAbout::~winAbout(void)
{
}


void winAbout::SetButtonFace(CCoolButton& btn, UINT nBtnID, UINT nIconID)
{
	btn.SubclassDlgItem(m_hwnd, nBtnID);
	btn.SetIcon(nIconID);
	// Set some color effect
	btn.SetActiveFgColor(RGB(0,0,255));
	//	btn.SetInactiveBgColor(RGB(220,220,220));
	btn.SetActiveBgColor(RGB(220,220,220));
	btn.DrawTransparent();
}

BOOL winAbout::OnInitDialog()
{
	SendMessage(m_hwnd, WM_SETICON, ICON_BIG,
		(LPARAM)LoadIcon(g_hAppInstance, MAKEINTRESOURCE(IDI_PORTRAIT)));

	SendMessage(GetDlgItem(m_hwnd, IDC_STATIC_ABOUT),
		STM_SETICON,
		(WPARAM)LoadIcon(g_hAppInstance, MAKEINTRESOURCE(IDI_PORTRAIT)),
		0L);

	SetButtonFace(m_btnOK, IDOK, IDI_OK);
	m_btnOK.SetShowText(FALSE);
	return TRUE;
}


BOOL winAbout::OnDialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		// set background color
	case WM_CTLCOLORBTN:
		//	case WM_CTLCOLORSCROLLBAR:
		//	case WM_CTLCOLOREDIT:
		//	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORDLG:
		if(m_hbrBG)
		{
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (BOOL)m_hbrBG;
		}
	}
	return FALSE;
}
