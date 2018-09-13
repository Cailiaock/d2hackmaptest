//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////


#ifndef __COOL_BUTTON_H__
#define __COOL_BUTTON_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// Standard Template Library headers
#pragma warning(disable: 4786)
#include <map>

class CCoolButton
{
public:
	CCoolButton();
	virtual ~CCoolButton();
    enum {ST_ALIGN_HORIZ, ST_ALIGN_VERT};

// Implementation
public:
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	BOOL SubclassDlgItem(HWND hwndParent, UINT nID, HINSTANCE hInst = NULL);
	void DrawTransparent();

	BOOL GetDefault();

	void SetTooltipText(int nId, BOOL bActivate = TRUE);
	void SetTooltipText(LPTSTR spText, BOOL bActivate = TRUE);
	void ActivateTooltip(BOOL bEnable = TRUE);

	BOOL SetBtnCursor(int nCursorId = -1);

	void SetFlatFocus(BOOL bDrawFlatFocus, BOOL bRepaint = FALSE);
	BOOL GetFlatFocus();

	void SetDefaultActiveFgColor(BOOL bRepaint = FALSE);
	void SetActiveFgColor(COLORREF crNew, BOOL bRepaint = FALSE);
	const COLORREF GetActiveFgColor();
	
	void SetDefaultActiveBgColor(BOOL bRepaint = FALSE);
	void SetActiveBgColor(COLORREF crNew, BOOL bRepaint = FALSE);
	const COLORREF GetActiveBgColor();
	
	void SetDefaultInactiveFgColor(BOOL bRepaint = FALSE);
	void SetInactiveFgColor(COLORREF crNew, BOOL bRepaint = FALSE);
	const COLORREF GetInactiveFgColor();

	void SetDefaultInactiveBgColor(BOOL bRepaint = FALSE);
	void SetInactiveBgColor(COLORREF crNew, BOOL bRepaint = FALSE);
	const COLORREF GetInactiveBgColor();

	void SetShowText(BOOL bShow = TRUE);
	BOOL GetShowText();

	void SetAlign(int nAlign);
	int GetAlign();

	void SetFlat(BOOL bState = TRUE);
	BOOL GetFlat();

	void DrawBorder(BOOL bEnable = TRUE);
	void SetIcon(int nIconInId, int nIconOutId = NULL);

	static const short GetVersionI();
	static const char* GetVersionC();

protected:
	void OnCaptureChanged(HWND hwnd);
	void OnMouseMove(UINT nFlags, POINT point);
	void OnSetCursor(HWND hwnd, UINT nHitTest, UINT message);
	void OnKillFocus(HWND hwnd);

private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK OwnerWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void DrawTheIcon(HDC hDC, LPTSTR sztitle, RECT* rcItem, RECT* captionRect, BOOL IsPressed, BOOL IsDisabled);
	void InitToolTip();
	void PaintBk(HDC hDC);
	void Invalidate(BOOL bErase = TRUE) {::InvalidateRect(m_hwnd, NULL, bErase);}

	HWND m_hwnd;
	HINSTANCE m_hInstResource;
	WNDPROC m_pfnDefWndProc;

	int m_nAlign;
	BOOL m_bShowText;
	BOOL m_bDrawBorder;
	BOOL m_bIsFlat;
	BOOL m_MouseOnButton;
	BOOL m_bDrawFlatFocus;

	HCURSOR m_hCursor;
//	CToolTipCtrl m_ToolTip;

	HICON m_hIconIn;
	HICON m_hIconOut;
	BYTE m_cyIcon;
	BYTE m_cxIcon;

	HDC m_dcBk;
	HBITMAP m_bmpBk;
	HBITMAP m_pbmpOldBk;
	BOOL m_bDrawTransparent;

	BOOL m_bIsDefault;

	COLORREF  m_crInactiveBg;
    COLORREF  m_crInactiveFg;
    COLORREF  m_crActiveBg;
    COLORREF  m_crActiveFg;

public:
	typedef std::map<UINT, CCoolButton*> CoolButtonMap;
	typedef CoolButtonMap::iterator CoolButtonMapIt;
	typedef struct _OwnerWindow
	{
		WNDPROC pfnDefWndProc;
		CoolButtonMap mapBtn;
	}OwnerWindow,*POwnerWindow;
	typedef std::map<HWND, OwnerWindow> OwnerWindowMap;
	typedef std::map<HWND, OwnerWindow>::iterator OwnerWindowMapIt;
};

#endif // __COOL_BUTTON_H__
