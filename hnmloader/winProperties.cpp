//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////


#include "stdhdrs.h"
#include <stdarg.h>
#include "HtmlHelp.h"
#include "winProperties.h"
#include "winAbout.h"
#include "ConfigLoader.h"

#pragma comment(lib,"htmlhelp.lib")

static HWND g_hwndStatus;
static HWND g_hwndMainHwnd;
static void LocalReportStatus(LPCTSTR message)
{
	if(g_hwndStatus && IsWindow(g_hwndStatus))
	{
		TCHAR szText[1024];
		GetWindowText(g_hwndStatus,szText, COUNTOF(szText));
		LPCTSTR pmsg = message ? message : _T("Ready");
		if(_tcscmp(szText, pmsg) != 0)
			SetWindowText(g_hwndStatus, pmsg);
	}
}

void GlobalReportStatus(UINT idString = IDS_READY)
{
	TCHAR szText[1024];
	if(LoadString(idString, szText, COUNTOF(szText)))
		LocalReportStatus(szText);
}

void __cdecl GlobalFormatAndReportStatus(UINT nIdString, ...)
{
	TCHAR szFormat[256];
	if(LoadString(nIdString, szFormat, COUNTOF(szFormat)))
	{
		TCHAR szText[1024];
		va_list ap;
		va_start(ap, nIdString);
		_vstprintf(szText, szFormat, ap);
		LocalReportStatus(szText);
		va_end(ap);
	}
}

HWND GlobalGetMainHwnd()
{
	return g_hwndMainHwnd;
}

winProperties::winProperties()
				:CDialogBase(IDD_PROPERTIES)
				,m_nIdEvent(0)
{
	m_views[eGameView] = static_cast<CListViewBase*>(&m_gameView);
	m_views[eDllView] = static_cast<CListViewBase*>(&m_dllView);
	m_views[eIScanView] = static_cast<CListViewBase*>(&m_iscanView);
	
	
	m_hBitmap = LoadBitmap(g_hAppInstance, MAKEINTRESOURCE(IDB_BACKGROUND));
	if(m_hBitmap)
		m_hBrush = CreatePatternBrush(m_hBitmap);
	else
		m_hBrush = NULL;
}

winProperties::~winProperties()
{
	g_hwndMainHwnd = NULL;
	g_hwndStatus = NULL;
	if(m_hBitmap)
		DeleteObject(m_hBitmap);
	if(m_hBrush)
		DeleteObject(m_hBrush);
}

BOOL winProperties::Init()
{
	return TRUE;
}


void winProperties::SetButtonFace(CCoolButton& btn, UINT nBtnID, UINT nIconID)
{
	btn.SubclassDlgItem(m_hwnd, nBtnID);
	btn.SetIcon(nIconID);
	// Set some color effect
	btn.SetActiveFgColor(RGB(0,0,255));
	//	btn.SetInactiveBgColor(RGB(220,220,220));
	btn.SetActiveBgColor(RGB(220,220,220));
	btn.DrawTransparent();
}

BOOL winProperties::OnInitDialog()
{
	g_hwndMainHwnd = m_hwnd;

	TCHAR szTitle[256];
	if (LoadString(IDS_VERSION_INFO, szTitle, COUNTOF(szTitle)-1))
		SetWindowText(m_hwnd, szTitle);

	g_hwndStatus = GetDlgItem(m_hwnd, IDC_STATUS_BAR);

	// Set dialog icon
	SendMessage(m_hwnd, WM_SETICON, ICON_BIG,
		(LPARAM)LoadIcon(g_hAppInstance, MAKEINTRESOURCE(IDI_PORTRAIT)));

	// Bring window to top
	SetForegroundWindow(m_hwnd);

	SetFocus(GetDlgItem(m_hwnd, IDC_BTN_INSTALL));

	DragAcceptFiles(m_hwnd, TRUE);

	HMENU hMenu = GetSystemMenu(m_hwnd, FALSE);

	InsertMenu(hMenu, 5, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
	InsertMenu(hMenu, 6, MF_BYPOSITION | MF_STRING, ID_ABOUT, _T("&About"));

	SetButtonFace(m_btnInstall, IDC_BTN_INSTALL, IDI_INSTALL);
	SetButtonFace(m_btnRemove, IDC_BTN_REMOVE, IDI_UNLOAD);
	SetButtonFace(m_btnEditConfig, IDC_BTN_EDITCONFIG, IDI_EDITCONFIG);
	SetButtonFace(m_btnUpgradeConfig, IDC_BTN_UPGRADECONFIG, IDI_EDITCONFIG);
	SetButtonFace(m_btnRevealAutomapAct, IDC_BTN_REVEAL_AUTOMAP_ACT, IDI_REVEALACT);
	SetButtonFace(m_btnAbout, IDC_BTN_ABOUT, IDI_ABOUTME);
	SetButtonFace(m_btnExit, IDC_BTN_EXIT, IDI_EXIT);

//	Button_SetCheck(GetDlgItem(m_hwnd, IDC_CHECK_AUTOLOAD),1);

	m_tabCtrl.Init(GetDlgItem(m_hwnd, IDC_TAB_VIEW));
	m_tabCtrl.AddTab(_T("Game"));
	m_tabCtrl.AddTab(_T("Dlls"));
	m_tabCtrl.AddTab(_T("IScan"));

	m_currView = static_cast<CListViewBase*>(&m_gameView);
	m_gameView.OnInitialUpdate(GetDlgItem(m_hwnd, IDC_LIST_PROCESS));
	m_dllView.OnInitialUpdate(GetDlgItem(m_hwnd, IDC_LIST_MODULES));
	m_iscanView.OnInitialUpdate(GetDlgItem(m_hwnd, IDC_LIST_INTEGRITYSCAN));

	m_nIdEvent = SetTimer(m_hwnd, 1, g_config.nBaseTimeSlice, NULL );
	return TRUE;
}

BOOL winProperties::OnDialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DROPFILES:
		OnDropFiles((HDROP)wParam);
		break;
		// set background color
//	case WM_CTLCOLOR:
	case WM_CTLCOLORBTN:
//	case WM_CTLCOLORSCROLLBAR:
//	case WM_CTLCOLOREDIT:
//	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORDLG:
		return (BOOL)OnCtlColor((HDC)wParam, (HWND)lParam);
		break;
	case WM_HELP:
		return OnHelpInfo((LPHELPINFO)lParam);
	case WM_SYSCOMMAND:
		switch(wParam)
		{
		case SC_MINIMIZE:
			PostMessage(m_hwnd, WM_COMMAND, IDCANCEL, 0);
			return TRUE;
		case ID_ABOUT:
			OnDlgAbout();
		}
		break;
	case WM_COMMAND:
		return OnCommand(LOWORD(wParam));
	case WM_INITMENUPOPUP:
		m_currView->OnInitContextMenuPopup((HMENU)wParam, LOWORD(lParam));
		break;
	case WM_TIMER:
		OnTimer(wParam);
		break;
	case WM_MOUSEMOVE:
		GlobalReportStatus();
		break;
		// user defined message
	case WM_UNLOADMODULE:
		if(lParam) m_gameView.UnloadModule((HMODULE)lParam);
		break;
	}
	return FALSE;
}

void winProperties::OnBtnUpgradeConfig()
{
	TCHAR szWarning[1024];
	if(LoadString(IDS_WRN_UPGRADE_CONFIG, szWarning, COUNTOF(szWarning)))
	{
		if(MessageBox(m_hwnd, szWarning, g_szAppName, MB_YESNO|MB_ICONWARNING) == IDYES)
		{
			CConfigLoader loader;
			loader.UpgradeConfig("d2hackmap.cfg", "d2hackmap.cfg.tmpl");
			loader.UpgradeConfig("d2hackmap.vcb", "d2hackmap.vcb.tmpl");
			loader.UpgradeConfig("d2hackmap.ini", "d2hackmap.ini.tmpl");
		}
	}
}

void winProperties::OnDlgAbout()
{
	winAbout about;
	about.SetBGBrush(m_hBrush);
	about.DoModal(g_hResInstance, m_hwnd);
}

BOOL winProperties::OnOK()
{
	if(m_nIdEvent)
	{
		KillTimer(m_hwnd, m_nIdEvent);
		m_nIdEvent = 0;
	}
	return TRUE;
}

BOOL winProperties::OnCancel()
{
	ShowWindow(m_hwnd, SW_MINIMIZE);
	ShowWindow(m_hwnd, SW_HIDE);
	return FALSE;
}

BOOL winProperties::OnCommand(UINT nCommandID)
{
	switch(nCommandID)
	{
	case IDC_BTN_ABOUT:
		OnDlgAbout();
		break;
	case IDC_BTN_INSTALL:
		OnBtnInstallUpdateHackmap();
		break;
	case IDC_BTN_REMOVE:
		OnBtnRemoveHackmap();
		break;
	case IDC_BTN_EDITCONFIG:
		OnBtnEditConfig();
		break;
	case IDC_BTN_UPGRADECONFIG:
		OnBtnUpgradeConfig();
		break;
	case IDC_BTN_REVEAL_AUTOMAP_ACT:
		OnBtnRevealAutomapAct();
		break;
	case IDC_BTN_EXIT:
		SendMessage(m_hwnd, WM_COMMAND, IDOK, 0);
		break;
	default:
		m_currView->OnContextMenuCommand(nCommandID);
		break;
	}
	return FALSE;
}

void winProperties::OnDropFiles(HDROP hDrop)
{
	TCHAR szFileName[1024], *pDot;
	UINT nCount = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
	if(nCount > 0)
		//	while(nCount-- > 0)
	{
		DragQueryFile(hDrop, 0, szFileName, COUNTOF(szFileName));
		if((pDot = _tcsrchr(szFileName, _T('.'))) != NULL)
		{
			pDot++;
			if(_tcsicmp(pDot, _T("bmp")) == 0 ||
				_tcsicmp(pDot, _T("jpg")) == 0 ||
				_tcsicmp(pDot, _T("gif")) == 0 ||
				_tcsicmp(pDot, _T("ico")) == 0 ||
				_tcsicmp(pDot, _T("wmf")) == 0)
			{
				HBITMAP hbmp = NULL;
				if(m_Picture.SetImage(szFileName))
				{
					hbmp = m_Picture.GetHandle();
				}
				else if(_tcsicmp(pDot, _T("bmp")) == 0)
				{
					hbmp = (HBITMAP)LoadImage(NULL,
						szFileName,
						IMAGE_BITMAP,
						0, 0,
						LR_LOADFROMFILE);
					if(hbmp)
					{
						if(m_hBitmap)
							DeleteObject(m_hBitmap);
						m_hBitmap = hbmp;
					}
				}
				if(hbmp)
				{
					if(m_hBrush)
						DeleteObject(m_hBrush);
					m_hBrush = CreatePatternBrush(hbmp);
					::InvalidateRect(m_hwnd, NULL, TRUE);
				}
			}
			else if(_tcsicmp(pDot, _T("dll")) == 0)
			{
				DWORD pid = m_gameView.GetSelectedGameId();
				if(pid)
				{
					TCHAR szFormat[256];
					if(LoadString(IDS_WRN_LOAD_DLL, szFormat, COUNTOF(szFormat)))
					{
						TCHAR szWarning[1024];
						_stprintf(szWarning, szFormat, szFileName, pid);
						if(MessageBox(m_hwnd, szWarning, g_szAppName, MB_YESNO|MB_ICONWARNING) == IDYES)
							m_gameView.LoadModule(szFileName);
					}
				}
			}
		}
	}
	DragFinish(hDrop);
}

BOOL winProperties::OnHelpInfo(LPHELPINFO lphi)
{
	TCHAR szHelpInfo[1024];
	if(LoadString(lphi->iCtrlId, szHelpInfo, COUNTOF(szHelpInfo)-1))
	{
		HH_POPUP hPop; // HTML Help popup structure

		// Initialize structure to NULLs	
		memset(&hPop, 0, sizeof(hPop)); 

		// Set size of structure
		hPop.cbStruct         = sizeof(hPop);		

		// Yellow background color
		hPop.clrBackground    = RGB(255, 255, 208);	

		hPop.clrForeground    = -1; // Font color	
		hPop.rcMargins.left   = -1;			 
		hPop.rcMargins.bottom = -1;
		hPop.rcMargins.right  = -1;
		hPop.pt               = lphi->MousePos;	
		hPop.pszText          = szHelpInfo; // Message from String Table
		hPop.pszFont          = NULL; // Font

		HtmlHelp((HWND)lphi->hItemHandle, NULL, 
			HH_DISPLAY_TEXT_POPUP, (DWORD)&hPop);
	}
	return TRUE;
}

inline
HBRUSH winProperties::OnCtlColor(HDC hdc, HWND hwnd)
{
	SetBkMode(hdc, TRANSPARENT);
	return m_hBrush;
}

void winProperties::OnTimer(UINT nIDEvent)
{
	if(m_nIdEvent == nIDEvent)
	{
		m_gameView.OnUpdate(m_hwnd);
		if( !IsMainView() )
		{
			m_dllView.TargettingProcess(m_gameView.GetSelectedGameId());
			m_iscanView.TargettingProcess(m_gameView.GetSelectedGameId());
			m_currView->OnUpdate(m_hwnd);
		}
	}
}

void winProperties::OnNotify(int idCtrl, LPNMHDR pnmh )
{
	switch(pnmh->idFrom)
	{
	case IDC_LIST_PROCESS:
	case IDC_LIST_MODULES:
	case IDC_LIST_INTEGRITYSCAN:
		OnNotifyCurrentView(pnmh);
		break;
	case IDC_TAB_VIEW:
		OnNotifyTabConTrol(pnmh);
		break;
	}
}

void winProperties::OnNotifyCurrentView(LPNMHDR pnmh)
{
	switch( pnmh->code )
	{
	case LVN_GETDISPINFO:
		{
			LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pnmh;
			if ( !(pDispInfo->item.mask & LVIF_TEXT) ) return;
			
			m_currView->OnGetdispinfo( pDispInfo );
		}
		break;
		
	case LVN_COLUMNCLICK:
		m_currView->OnColumnclick( (NM_LISTVIEW*)pnmh );
		break;
		
	case NM_RCLICK:
		m_currView->OnRClickItem((LPNMITEMACTIVATE)pnmh);
		break;
	case NM_CLICK:
		m_currView->OnLClickItem((LPNMITEMACTIVATE)pnmh);
		break;
	case NM_DBLCLK:
		m_currView->OnDblclk((LPNMITEMACTIVATE)pnmh);
		break;
	}
}

void winProperties::OnNotifyTabConTrol(LPNMHDR pnmh)
{
	switch( pnmh->code )
	{
	case NM_CLICK:
		OnClickTabCtrl(pnmh);
		break;
	}
}

void winProperties::OnBtnInstallUpdateHackmap()
{
	m_gameView.OnInstallUpdateHackmap();
	if(m_currView == static_cast<CListViewBase*>(&m_iscanView))
		m_iscanView.OnUpdateNow();
}

void winProperties::OnBtnRemoveHackmap()
{
	m_gameView.OnRemoveHackmap();
	if(m_currView == static_cast<CListViewBase*>(&m_iscanView))
		m_iscanView.OnUpdateNow();
}

void winProperties::OnBtnEditConfig()
{
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(GetModuleHandle(NULL), szPath, COUNTOF(szPath));
	_tcscpy(GetFileNameFromPath(szPath), _T("d2hackmap.cfg"));
	if((DWORD)ShellExecute(NULL, _T("open"), szPath, NULL, NULL, SW_SHOW) <= 32)
		ShellExecute(NULL, _T("open"), _T("notepad.exe"), szPath, NULL, SW_SHOW);
}

void winProperties::OnBtnRevealAutomapAct()
{
	m_gameView.OnRevealAutomapAct();
}

void winProperties::OnClickTabCtrl(LPNMHDR pnmh)
{
	int index = TabCtrl_GetCurFocus(pnmh->hwndFrom);
	if(index != 0 && m_gameView.GetSelectedGameId() == 0)
		TabCtrl_SetCurSel(pnmh->hwndFrom, 0);
	else
		SwitchView(index);
}

void winProperties::SwitchView(int index)
{
	ASSERT( index < COUNTOF(m_views) );
	if(m_views[index] != m_currView)
	{
		ShowWindow(m_currView->GetSafeHwnd(), SW_HIDE);
		m_currView = m_views[index];
		ShowWindow(m_currView->GetSafeHwnd(), SW_SHOW);
		m_dllView.TargettingProcess(m_gameView.GetSelectedGameId());
		m_iscanView.TargettingProcess(m_gameView.GetSelectedGameId());
		m_currView->OnSwitchView();
	}
}
