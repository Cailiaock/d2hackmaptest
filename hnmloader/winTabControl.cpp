//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#include "stdhdrs.h"
#include <CommCtrl.h>
#include <windowsx.h>
#include "wintabcontrol.h"

winTabControl::winTabControl(void)
			:m_hwnd(NULL)
{
}

winTabControl::~winTabControl(void)
{
}

void winTabControl::Init(HWND hwnd)
{
	m_hwnd = hwnd;
}

void winTabControl::AddTab(LPTSTR pszText)
{
	TCITEM tie;
	tie.mask = TCIF_TEXT | TCIF_IMAGE;
	tie.iImage = -1;
	tie.pszText = pszText;
	TabCtrl_InsertItem(m_hwnd, TabCtrl_GetItemCount(m_hwnd), &tie);
}
