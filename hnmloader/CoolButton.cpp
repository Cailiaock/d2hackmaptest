// FlatButton.cpp: implementation of the CCoolButton class.
//
//////////////////////////////////////////////////////////////////////
#include "STDHDRS.H"
#include <TCHAR.h>
#include <windowsx.h>
#include <algorithm>
#include "CoolButton.h"
#ifndef COUNTOF
#define COUNTOF(x)	sizeof((x)) / sizeof(x[0])
#endif

static CCoolButton::OwnerWindowMap s_mapOwner;
static LPCTSTR _This_Cool_Button = _T("THIS COOL BUTTON");
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCoolButton::CCoolButton()
{
	m_MouseOnButton = FALSE;

	m_hwnd = NULL;
	m_hInstResource = NULL;

	m_hIconIn = NULL;
	m_hIconOut = NULL;
	m_cxIcon = 0;
	m_cyIcon = 0;
	m_hCursor = NULL;
  
	// Default type is "flat" button
	m_bIsFlat = TRUE; 
  
	// By default draw border in "flat" button 
	m_bDrawBorder = TRUE; 
  
	// By default icon is aligned horizontally
	m_nAlign = ST_ALIGN_HORIZ; 
  
	// By default show the text button
	m_bShowText = TRUE; 
  
	// By default, for "flat" button, don't draw the focus rect
	m_bDrawFlatFocus = FALSE;

	// By default the button is not the default button
	m_bIsDefault = FALSE;

	SetDefaultInactiveBgColor();
	SetDefaultInactiveFgColor();
	SetDefaultActiveBgColor();
	SetDefaultActiveFgColor();

	// No tooltip created
//	m_ToolTip.m_hWnd = NULL;

	// Do not draw as a transparent button
	m_bDrawTransparent = FALSE;
	m_dcBk = NULL;
	m_bmpBk = NULL;
	m_pbmpOldBk = NULL;
} // End of CCoolButton


CCoolButton::~CCoolButton()
{
	// Restore old bitmap (if any)
	if (m_dcBk != NULL)
	{
		if(m_pbmpOldBk != NULL)
			::SelectObject(m_dcBk, m_pbmpOldBk);
		::DeleteDC(m_dcBk);
	}
	if(m_bmpBk != NULL)
		::DeleteObject(m_bmpBk);

	// Removed by Bounds Checker suggestion
	// Destroy the icons (if any)
	if (m_hIconIn != NULL) ::DestroyIcon(m_hIconIn);
	if (m_hIconOut != NULL && m_hIconOut != m_hIconIn) ::DestroyIcon(m_hIconOut);
	// Destroy the cursor (if any)
	if (m_hCursor != NULL) ::DestroyCursor(m_hCursor);
	
} // End of ~CCoolButton

LRESULT CALLBACK CCoolButton::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// We use the dialog-box's USERDATA to store a _this pointer
	// This is set only once WM_INITDIALOG has been recieved, though!
	CCoolButton *_this = reinterpret_cast<CCoolButton*>(::GetProp(hwnd, _This_Cool_Button));

	switch (uMsg)
	{
	case WM_LBUTTONDBLCLK:
		uMsg = WM_LBUTTONDOWN;

	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		{
		// Erase background befor calling default window proc
		HDC hDC = ::GetDC(hwnd);
		::SendMessage(hwnd, WM_ERASEBKGND, (WPARAM)hDC, 0);
		::ReleaseDC(hwnd, hDC);
		}
		break;

	case WM_SETCURSOR:
		::CallWindowProc(_this->m_pfnDefWndProc, hwnd, uMsg, wParam, lParam);
		_this->OnSetCursor((HWND)wParam, LOWORD(lParam), HIWORD(lParam));
		return 0;

	case WM_KILLFOCUS:
		_this->OnKillFocus((HWND)wParam);
		break;

	case WM_MOUSEMOVE:
		{
			::CallWindowProc(_this->m_pfnDefWndProc, hwnd, uMsg, wParam, lParam);

			POINT pt;
			pt.x = GET_X_LPARAM(lParam);
			pt.y = GET_Y_LPARAM(lParam);
			_this->OnMouseMove(wParam, pt);
		}
		return 0;

	case WM_CAPTURECHANGED:
		::CallWindowProc(_this->m_pfnDefWndProc, hwnd, uMsg, wParam, lParam);
		_this->OnCaptureChanged((HWND)lParam);
		return 0;

	case WM_NCDESTROY:
		::RemoveProp(hwnd, _This_Cool_Button);
		break;
	}
	return 	::CallWindowProc(_this->m_pfnDefWndProc, hwnd, uMsg, wParam, lParam);
}

void CCoolButton::SetIcon(int nIconInId, int nIconOutId)
{
	// Set icon when the mouse is IN the button
	m_hIconIn = (HICON)::LoadImage(m_hInstResource/*AfxGetApp()->m_hInstance*/, MAKEINTRESOURCE(nIconInId), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
  
	// Set icon when the mouse is OUT the button
	m_hIconOut = (nIconOutId == NULL) ? m_hIconIn : (HICON)::LoadImage(m_hInstResource/*AfxGetApp()->m_hInstance*/, MAKEINTRESOURCE(nIconOutId), IMAGE_ICON, 0, 0, 0);
  
	ICONINFO ii;

	// Get icon dimension
	ZeroMemory(&ii, sizeof(ICONINFO));
	::GetIconInfo(m_hIconIn, &ii);
	// delete unused objects
	::DeleteObject(ii.hbmMask);
	::DeleteObject(ii.hbmColor);

	m_cxIcon = (BYTE)(ii.xHotspot * 2);
	m_cyIcon = (BYTE)(ii.yHotspot * 2);

	RedrawWindow(m_hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
} // End of SetIcon


BOOL CCoolButton::SetBtnCursor(int nCursorId)
{
	// Destroy any previous cursor
	if (m_hCursor != NULL) ::DestroyCursor(m_hCursor);
	m_hCursor = NULL;

	// If we want a cursor
	if (nCursorId != -1)
	{
		// Load icon resource
		m_hCursor = (HCURSOR)::LoadImage(m_hInstResource/*AfxGetApp()->m_hInstance*/, MAKEINTRESOURCE(nCursorId), IMAGE_CURSOR, 0, 0, 0);
		// If something wrong then return FALSE
		if (m_hCursor == NULL) return FALSE;
	}

	return TRUE;
} // End of SetBtnCursor


void CCoolButton::SetFlat(BOOL bState)
{
  m_bIsFlat = bState;
  Invalidate();
} // End of SetFlat


BOOL CCoolButton::GetFlat()
{
  return m_bIsFlat;
} // End of GetFlat


void CCoolButton::SetAlign(int nAlign)
{
  switch (nAlign)
  {    
    case ST_ALIGN_HORIZ:
         m_nAlign = ST_ALIGN_HORIZ;
         break;
    case ST_ALIGN_VERT:
         m_nAlign = ST_ALIGN_VERT;
         break;
  }
  Invalidate();
} // End of SetAlign


int CCoolButton::GetAlign()
{
  return m_nAlign;
} // End of GetAlign


void CCoolButton::DrawBorder(BOOL bEnable)
{
  m_bDrawBorder = bEnable;
} // End of DrawBorder


const char* CCoolButton::GetVersionC()
{
  return "2.5";
} // End of GetVersionC


const short CCoolButton::GetVersionI()
{
  return 25; // Divide by 10 to get actual version
} // End of GetVersionI


void CCoolButton::SetShowText(BOOL bShow)
{
  m_bShowText = bShow;
  Invalidate();
} // End of SetShowText


BOOL CCoolButton::GetShowText()
{
  return m_bShowText;
} // End of GetShowText

////////////////////////////////////////////////
// Add by sting feng
static BOOL IsMouseOnWindow(HWND hwnd, POINT pt)
{
	::ClientToScreen(hwnd, &pt);
	return hwnd == ::WindowFromPoint(pt);
}
// Add by sting feng
////////////////////////////////////////////////

void CCoolButton::OnMouseMove(UINT nFlags, POINT point)
{
	GlobalReportStatus(GetDlgCtrlID(m_hwnd));

	HWND hwnd;  // Finestra attiva

	////////////////////////////////////////////////////////
	// Modify by sting feng
	if (nFlags & MK_LBUTTON && m_MouseOnButton == FALSE)
	{
		// If mouse Captured
		if(::GetCapture() == m_hwnd)
		{
			// If the mouse is entering or leaving
			if(m_MouseOnButton != IsMouseOnWindow(m_hwnd, point))
			{
				// Reverse the state and erase background
				m_MouseOnButton = !m_MouseOnButton;
				Invalidate();
			}
		}
		return;
	}
	// Modify by sting feng
	////////////////////////////////////////////////////////

	// If our button is not flat then do nothing
	if (m_bIsFlat == FALSE) return;

	hwnd = ::GetActiveWindow();

	if ((::GetCapture() != m_hwnd) && 
		(
#ifndef ST_LIKEIE
		hwnd!= NULL && 
#endif
		::GetParent(m_hwnd) != NULL)) 
	{
		m_MouseOnButton = TRUE;
//		SetFocus(m_hwnd);	// Thanks Ralph!
		::SetCapture(m_hwnd);
		Invalidate();
	}
	else
	{
		if (!IsMouseOnWindow(m_hwnd, point))
		{
			// Redraw only if mouse goes out
			if (m_MouseOnButton == TRUE)
			{
				m_MouseOnButton = FALSE;
				Invalidate();
			}
			// If user is NOT pressing left button then release capture!
			if (!(nFlags & MK_LBUTTON))
			{
				::ReleaseCapture();
//				Invalidate();
			}
		}
	}
} // End of OnMouseMove


void CCoolButton::OnKillFocus(HWND hwnd)
{
	// If our button is not flat then do nothing
	if (m_bIsFlat == FALSE) return;

	if (m_MouseOnButton == TRUE)
		m_MouseOnButton = FALSE;
	Invalidate();
} // End of OnKillFocus


void CCoolButton::OnCaptureChanged(HWND hwnd) 
{
	if (m_MouseOnButton == TRUE)
	{
		::ReleaseCapture();
	}
	Invalidate();
} // End of OnCaptureChanged


void CCoolButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	HDC hDC = lpDIS->hDC;
	HPEN hOldPen;
	BOOL bIsPressed  = (lpDIS->itemState & ODS_SELECTED);
	BOOL bIsFocused  = (lpDIS->itemState & ODS_FOCUS);
	BOOL bIsDisabled = (lpDIS->itemState & ODS_DISABLED);

	
	RECT itemRect = lpDIS->rcItem;

	if (m_bIsFlat == FALSE)
	{
		if (bIsFocused || (GetDefault() == TRUE))
		{
			HBRUSH br;
			br = ::CreateSolidBrush(RGB(0, 0, 0));
			::FrameRect(hDC, &itemRect, br);
			::InflateRect(&itemRect, -1, -1);
		}
	}

	// Prepare draw... paint button's area with background color
	COLORREF bgColor;
	if ((m_MouseOnButton == TRUE) || (bIsPressed))
		bgColor = GetActiveBgColor();
	else
		bgColor = GetInactiveBgColor();

	// Draw transparent?
	if (m_bDrawTransparent == TRUE)
	{
		PaintBk(hDC);
	}
	else
	{
		HBRUSH br = ::CreateSolidBrush(bgColor);
		::FillRect(hDC, &itemRect, br);
		::DeleteObject(br);
	}

	// Disegno lo sfondo del bottone
	//CBrush br(GetSysColor(COLOR_BTNFACE));  
	//pDC->FillRect(&itemRect, &br);

	// Draw pressed button
	if (bIsPressed)
	{
		if (m_bIsFlat == TRUE)
		{
			if (m_bDrawBorder == TRUE)
			{
				HPEN penBtnHiLight = ::CreatePen(PS_SOLID, 0, GetSysColor(COLOR_BTNHILIGHT));
				HPEN penBtnShadow = ::CreatePen(PS_SOLID, 0, GetSysColor(COLOR_BTNSHADOW));

				// Disegno i bordi a sinistra e in alto
				// Dark gray line
				hOldPen = (HPEN)::SelectObject(hDC, penBtnShadow);
				::MoveToEx(hDC, itemRect.left, itemRect.bottom-1, NULL);
				::LineTo(hDC, itemRect.left, itemRect.top);
				::LineTo(hDC, itemRect.right, itemRect.top);

				::SelectObject(hDC, penBtnHiLight);
				::MoveToEx(hDC, itemRect.left, itemRect.bottom-1, NULL);
				::LineTo(hDC, itemRect.right-1, itemRect.bottom-1);
				::LineTo(hDC, itemRect.right-1, itemRect.top-1);
				//
				::SelectObject(hDC, hOldPen);

				// Release objects
				::DeleteObject(penBtnHiLight);
				::DeleteObject(penBtnShadow);
			}
		}
		else    
		{
			HBRUSH brBtnShadow = ::CreateSolidBrush(GetSysColor(COLOR_BTNSHADOW));
			::FrameRect(hDC, &itemRect, brBtnShadow);
		}
	}
	else // ...else draw non pressed button
	{
		HPEN penBtnHiLight = ::CreatePen(PS_SOLID, 0, GetSysColor(COLOR_BTNHILIGHT)); // White
		HPEN pen3DLight = ::CreatePen(PS_SOLID, 0, GetSysColor(COLOR_3DLIGHT));       // Light gray
		HPEN penBtnShadow = ::CreatePen(PS_SOLID, 0, GetSysColor(COLOR_BTNSHADOW));   // Dark gray
		HPEN pen3DDKShadow = ::CreatePen(PS_SOLID, 0, GetSysColor(COLOR_3DDKSHADOW)); // Black

		if (m_bIsFlat == TRUE)
		{
			if (m_MouseOnButton == TRUE && m_bDrawBorder == TRUE)
			{
				// Disegno i bordi a sinistra e in alto
				// White line
				hOldPen = (HPEN)::SelectObject(hDC, penBtnHiLight);
				::MoveToEx(hDC, itemRect.left, itemRect.bottom-1, NULL);
				::LineTo(hDC, itemRect.left, itemRect.top);
				::LineTo(hDC, itemRect.right, itemRect.top);

				::SelectObject(hDC, penBtnShadow);
				::MoveToEx(hDC, itemRect.left, itemRect.bottom-1, NULL);
				::LineTo(hDC, itemRect.right-1, itemRect.bottom-1);
				::LineTo(hDC, itemRect.right-1, itemRect.top-1);
				//
				::SelectObject(hDC, hOldPen);
			}
		}
		else
		{
			// Disegno i bordi a sinistra e in alto
			// White line
			hOldPen = (HPEN)::SelectObject(hDC, penBtnHiLight);
			::MoveToEx(hDC, itemRect.left, itemRect.bottom-1, NULL);
			::LineTo(hDC, itemRect.left, itemRect.top);
			::LineTo(hDC, itemRect.right, itemRect.top);
			// Light gray line
			::SelectObject(hDC, pen3DLight);
			::MoveToEx(hDC, itemRect.left+1, itemRect.bottom-1, NULL);
			::LineTo(hDC, itemRect.left+1, itemRect.top+1);
			::LineTo(hDC, itemRect.right, itemRect.top+1);
			// Disegno i bordi a destra e in basso
			// Black line
			::SelectObject(hDC, pen3DDKShadow);
			::MoveToEx(hDC, itemRect.left, itemRect.bottom-1, NULL);
			::LineTo(hDC, itemRect.right-1, itemRect.bottom-1);
			::LineTo(hDC, itemRect.right-1, itemRect.top-1);
			// Dark gray line
			::SelectObject(hDC, penBtnShadow);
			::MoveToEx(hDC, itemRect.left+1, itemRect.bottom-2, NULL);
			::LineTo(hDC, itemRect.right-2, itemRect.bottom-2);
			::LineTo(hDC, itemRect.right-2, itemRect.top);
			//
			::SelectObject(hDC, hOldPen);
		}

		// Release Objects
		::DeleteObject(penBtnHiLight); // White
		::DeleteObject(pen3DLight);       // Light gray
		::DeleteObject(penBtnShadow);   // Dark gray
		::DeleteObject(pen3DDKShadow); // Black
	}

	// Read the button's title
	TCHAR szTitle[1024];
	::GetWindowText(m_hwnd, szTitle, COUNTOF(szTitle)-1);

	// If we don't want the title displayed
	if (m_bShowText == FALSE) szTitle[0] = _T('\0');

	RECT captionRect = lpDIS->rcItem;

	// Draw the icon
	if (m_hIconIn != NULL)
	{
		DrawTheIcon(hDC, szTitle, &lpDIS->rcItem, &captionRect, bIsPressed, bIsDisabled);
	}

	// Write the button title (if any)
	if (_tcslen(szTitle) > 0)
	{
		// Disegno la caption del bottone
		// Se il bottone e' premuto muovo la captionRect di conseguenza
		if (bIsPressed)
			::OffsetRect(&captionRect, 1, 1);

		// ONLY FOR DEBUG 
		// Evidenzia il rettangolo in cui verra' centrata la caption 
		//CBrush brBtnShadow(RGB(255, 0, 0));
		//pDC->FrameRect(&captionRect, &brBtnShadow);

		if ((m_MouseOnButton == TRUE) || (bIsPressed)) 
		{
			::SetTextColor(hDC, GetActiveFgColor());
			::SetBkColor(hDC, GetActiveBgColor());
		} 
		else 
		{
			::SetTextColor(hDC, GetInactiveFgColor());
			::SetBkColor(hDC, GetInactiveBgColor());
		}
		// Center text
		RECT centerRect = captionRect;
		::DrawText(hDC, szTitle, -1, &captionRect, DT_SINGLELINE|DT_CALCRECT);
		::OffsetRect(&captionRect,
					( (centerRect.right-centerRect.left)-(captionRect.right-captionRect.left) )/2,
					( (centerRect.bottom-centerRect.top)-(captionRect.bottom-captionRect.top) )/2);
		// RFU
		// captionRect.OffsetRect(0, (centerRect.Height() - captionRect.Height())/2);
		// captionRect.OffsetRect((centerRect.Width() - captionRect.Width())-4, (centerRect.Height() - captionRect.Height())/2);
		

		::DrawState(hDC, NULL, NULL,
			(LPARAM)szTitle, 0,
			captionRect.left, captionRect.top,
			captionRect.right-captionRect.left, captionRect.bottom-captionRect.top,
			((bIsDisabled ? DSS_DISABLED : DSS_NORMAL))|DST_PREFIXTEXT);
	}

	if (m_bIsFlat == FALSE || (m_bIsFlat == TRUE && m_bDrawFlatFocus == TRUE))
	{
		// Draw the focus rect
		if (bIsFocused)
		{
			RECT focusRect = itemRect;
			::InflateRect(&focusRect, -3, -3);
			::DrawFocusRect(hDC, &focusRect);
		}
	}

} // End of DrawItem


void CCoolButton::DrawTheIcon(HDC hDC, LPTSTR title, RECT* rcItem, RECT* captionRect, BOOL IsPressed, BOOL IsDisabled)
{
  RECT iconRect = *rcItem;

  switch (m_nAlign)
  {
    case ST_ALIGN_HORIZ:
         if (_tcslen(title) == 0)
         {
           // Center the icon horizontally
           iconRect.left += (( (iconRect.right-iconRect.left) - m_cxIcon)/2);
         }
         else
         {
           // L'icona deve vedersi subito dentro il focus rect
           iconRect.left += 3;  
           captionRect->left += m_cxIcon + 3;
         }
         // Center the icon vertically
         iconRect.top += (( (iconRect.bottom-iconRect.top) - m_cyIcon)/2);
         break;
    case ST_ALIGN_VERT:
         // Center the icon horizontally
         iconRect.left += (( (iconRect.right-iconRect.left) - m_cxIcon)/2);
         if (_tcslen(title) == 0)
         {
           // Center the icon vertically
           iconRect.top += (( (iconRect.bottom-iconRect.top) - m_cyIcon)/2);           
         }
         else
         {
           captionRect->top += m_cyIcon;
         }
         break;
  }
    
  // If button is pressed then press the icon also
  if (IsPressed)
	  ::OffsetRect(&iconRect, 1, 1);

  // Ole'!
  ::DrawState(hDC, NULL, NULL,
		(LPARAM)((m_MouseOnButton == TRUE || IsPressed) ? m_hIconIn : m_hIconOut),
		0, iconRect.left, iconRect.top,
		iconRect.right-iconRect.left, iconRect.bottom-iconRect.top,
		((IsDisabled ? DSS_DISABLED : DSS_NORMAL))|DST_ICON);
} // End of DrawTheIcon

void CCoolButton::SetDefaultInactiveBgColor(BOOL bRepaint)
{
	m_crInactiveBg = ::GetSysColor(COLOR_BTNFACE); 
	if (bRepaint == TRUE) Invalidate();
} // End of SetDefaultInactiveBgColor


void CCoolButton::SetInactiveBgColor(COLORREF crNew, BOOL bRepaint)
{
	m_crInactiveBg = crNew; 
	if (bRepaint == TRUE) Invalidate();
} // End of SetInactiveBgColor


const COLORREF CCoolButton::GetInactiveBgColor()
{
	return m_crInactiveBg;
} // End of GetInactiveBgColor


void CCoolButton::SetDefaultInactiveFgColor(BOOL bRepaint)
{
	m_crInactiveFg = ::GetSysColor(COLOR_BTNTEXT); 
	if (bRepaint == TRUE) Invalidate();
} // End of SetDefaultInactiveFgColor


void CCoolButton::SetInactiveFgColor(COLORREF crNew, BOOL bRepaint)
{
	m_crInactiveFg = crNew; 
	if (bRepaint == TRUE) Invalidate();
} // End of SetInactiveFgColor


const COLORREF CCoolButton::GetInactiveFgColor()
{
	return m_crInactiveFg;
} // End of GetInactiveFgColor


void CCoolButton::SetDefaultActiveBgColor(BOOL bRepaint)
{
	m_crActiveBg = ::GetSysColor(COLOR_BTNFACE); 
	if (bRepaint == TRUE) Invalidate();
} // End of SetDefaultActiveBgColor


void CCoolButton::SetActiveBgColor(COLORREF crNew, BOOL bRepaint)
{
	m_crActiveBg = crNew;
	if (bRepaint == TRUE) Invalidate();
} // End of SetActiveBgColor


const COLORREF CCoolButton::GetActiveBgColor()
{
	return m_crActiveBg;
} // End of GetActiveBgColor


void CCoolButton::SetDefaultActiveFgColor(BOOL bRepaint)
{
	m_crActiveFg = ::GetSysColor(COLOR_BTNTEXT); 
	if (bRepaint == TRUE) Invalidate();
} // End of SetDefaultActiveFgColor


void CCoolButton::SetActiveFgColor(COLORREF crNew, BOOL bRepaint)
{
	m_crActiveFg = crNew; 
	if (bRepaint == TRUE) Invalidate();
} // End of SetActiveFgColor


const COLORREF CCoolButton::GetActiveFgColor()
{
	return m_crActiveFg;
} // End of GetActiveFgColor


void CCoolButton::SetFlatFocus(BOOL bDrawFlatFocus, BOOL bRepaint)
{
	m_bDrawFlatFocus = bDrawFlatFocus;
	
	// Repaint the button
	if (bRepaint == TRUE) Invalidate();
} // End of SetFlatFocus


BOOL CCoolButton::GetFlatFocus()
{
	return m_bDrawFlatFocus;
} // End of GetFlatFocus


void CCoolButton::OnSetCursor(HWND hwnd, UINT nHitTest, UINT message) 
{
	// If a cursor was specified then use it!
	if (m_hCursor != NULL)
		::SetCursor(m_hCursor);
} // End of OnSetCursor


void CCoolButton::SetTooltipText(LPTSTR spText, BOOL bActivate)
{
/*	// We cannot accept NULL pointer
	if (spText == NULL) return;

	// Initialize ToolTip
	InitToolTip();

	// If there is no tooltip defined then add it
	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip.AddTool(this, (LPCTSTR)*spText, rectBtn, 1);
	}

	// Set text for tooltip
	m_ToolTip.UpdateTipText((LPCTSTR)*spText, this, 1);
	m_ToolTip.Activate(bActivate);
*/
} // End of SetTooltipText


void CCoolButton::SetTooltipText(int nId, BOOL bActivate)
{
	TCHAR szText[1024];

	// load string resource
	::LoadString(m_hInstResource, nId, szText, COUNTOF(szText)-1);
	// If string resource is not empty
	if (_tcslen(szText) > 0) SetTooltipText(szText, bActivate);
} // End of SetTooltipText


void CCoolButton::ActivateTooltip(BOOL bActivate)
{
	// If there is no tooltip then do nothing
//	if (m_ToolTip.GetToolCount() == 0) return;

	// Activate tooltip
//	m_ToolTip.Activate(bActivate);
} // End of EnableTooltip


BOOL CCoolButton::GetDefault()
{
	return m_bIsDefault;
} // End of GetDefault


void CCoolButton::DrawTransparent()
{
	m_bDrawTransparent = TRUE;
} // End of DrawTransparent


void CCoolButton::InitToolTip()
{
/*	if (m_ToolTip.m_hWnd == NULL)
	{
		// Create ToolTip control
		m_ToolTip.Create(this);
		// Create inactive
		m_ToolTip.Activate(FALSE);
	}
*/
} // End of InitToolTip

static void ScreenToClient(HWND hwnd, LPRECT lpRect)
{
	::ScreenToClient(hwnd, (LPPOINT)lpRect);
	::ScreenToClient(hwnd, ((LPPOINT)lpRect)+1);
	if (((DWORD)GetWindowLong(hwnd, GWL_EXSTYLE)) & 0x400000) // WS_EX_LAYOUTRTL
	{
		LONG temp = lpRect->left;
		lpRect->left = lpRect->right;
		lpRect->right = temp;
	}
}

void CCoolButton::PaintBk(HDC hDC)
{
	HDC clDC = ::GetDC(::GetParent(m_hwnd));
	RECT rect, rect1;

	::GetClientRect(m_hwnd, &rect);

	::GetWindowRect(m_hwnd, &rect1);
	ScreenToClient(::GetParent(m_hwnd), &rect1);

	if (m_dcBk == NULL)
	{
		m_dcBk = ::CreateCompatibleDC(clDC);
		m_bmpBk = CreateCompatibleBitmap(clDC, rect.right-rect.left, rect.bottom-rect.top);
		m_pbmpOldBk = (HBITMAP)::SelectObject(clDC, m_bmpBk);
		::BitBlt(m_dcBk,
				0, 0,
				rect.right-rect.left, rect.bottom-rect.top,
				clDC,
				rect1.left, rect1.top,
				SRCCOPY);
	}

	::BitBlt(hDC,
			0, 0,
			rect.right-rect.left, rect.bottom-rect.top,
			m_dcBk,
			0, 0,
			SRCCOPY);

	::ReleaseDC(::GetParent(m_hwnd), clDC);
} // End of PaintBk

BOOL CCoolButton::SubclassDlgItem(HWND hwndParent, UINT nID, HINSTANCE hInst)
{
	OwnerWindowMapIt OwnerIt;
	// Check if this nID has been subclass before
	if( (OwnerIt = s_mapOwner.find(hwndParent)) != s_mapOwner.end() &&
		OwnerIt->second.mapBtn.find(nID) != OwnerIt->second.mapBtn.end())
			return FALSE;

	m_hInstResource = hInst ? hInst : (::GetModuleHandle(NULL));

	m_hwnd = ::GetDlgItem(hwndParent, nID);
	if(m_hwnd == NULL)
		return FALSE;

	UINT nBS = (((UINT)GetWindowLong(m_hwnd, GWL_STYLE)) & 0xff);

	// Check if this is the default button
	if (nBS & BS_DEFPUSHBUTTON)
		m_bIsDefault = TRUE;

	// add BS_OWNERDRAW style
	::SendMessage(m_hwnd, BM_SETSTYLE, nBS | BS_OWNERDRAW, 1L);

	// Subclass the control
	m_pfnDefWndProc = (WNDPROC)::SetWindowLong(m_hwnd, GWL_WNDPROC, (LONG)WndProc);
	::SetProp(m_hwnd, _This_Cool_Button, this);

	if( OwnerIt == s_mapOwner.end()) // create a new owner
	{
		OwnerWindow own;
		own.pfnDefWndProc= (WNDPROC)::SetWindowLong(hwndParent, GWL_WNDPROC, (LONG)OwnerWndProc);
		own.mapBtn[nID] = this;
		s_mapOwner[hwndParent] = own;
	}
	else // add the control to his owner
	{
		OwnerIt->second.mapBtn[nID] = this;
	}

	return TRUE;
}

LRESULT CALLBACK CCoolButton::OwnerWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	OwnerWindowMapIt it;
	if( (it = s_mapOwner.find(hwnd)) != s_mapOwner.end())
	{
		OwnerWindow& win = it->second;
		switch(uMsg)
		{
		case WM_DRAWITEM:
			{
				CoolButtonMapIt BtnIt;
				if( (BtnIt = win.mapBtn.find(wParam)) != win.mapBtn.end())
				{
					BtnIt->second->DrawItem((LPDRAWITEMSTRUCT)lParam);
				}
			}
			break;
		}
		return ::CallWindowProc(win.pfnDefWndProc, hwnd, uMsg, wParam, lParam);
	}
	return 0;
}
