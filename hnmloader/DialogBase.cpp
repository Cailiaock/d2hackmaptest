//////////////////////////////////////////////////////////////////////
// DialogBase.cpp: implementation of the CDialogBase class.
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "DialogBase.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDialogBase::CDialogBase(UINT nIDD)
			:m_IDD(nIDD)
{
	m_hwnd = NULL;
	m_dlgvisible = FALSE;
	m_fModalDlg = TRUE;
}

CDialogBase::~CDialogBase()
{

}

inline CDialogBase& CDialogBase::operator =(CDialogBase& dlg)
{
	m_hwnd = dlg.m_hwnd;
	m_IDD = dlg.m_IDD;
	m_hInstance = dlg.m_hInstance;
	m_returncode_valid = dlg.m_returncode_valid;
	m_dlgvisible = dlg.m_dlgvisible;
	m_fModalDlg = dlg.m_fModalDlg;
	return *this;
}

BOOL CDialogBase::DoModal( HINSTANCE hInstance, HWND hParent )
{
	if(m_hwnd)
	{
		ShowWindow(m_hwnd, SW_SHOW);
		ShowWindow(m_hwnd, SW_RESTORE);
		return FALSE;
	}
	m_fModalDlg = TRUE;
	m_hInstance = hInstance;
	if( m_hInstance == NULL )
		m_hInstance = ::GetModuleHandle(NULL);

	return DialogBoxParam(m_hInstance,
		MAKEINTRESOURCE(m_IDD),
		hParent,
		(DLGPROC) CDialogBase::DialogProcWrapper,
		(LPARAM) this);
}

HWND CDialogBase::DoModeless( HINSTANCE hInstance, HWND hParent )
{
	m_fModalDlg = FALSE;
	m_hInstance = hInstance;
	if( m_hInstance == NULL )
		m_hInstance = ::GetModuleHandle(NULL);

	// Now, if the dialog is not already displayed, show it!
	if (!m_dlgvisible)
	{
		m_hwnd = ::CreateDialogParam( m_hInstance,
					MAKEINTRESOURCE(m_IDD), 
					hParent,
					(DLGPROC) CDialogBase::DialogProcWrapper,
					(LPARAM)this);
		if( m_hwnd != NULL )
		{
			::ShowWindow( m_hwnd, SW_SHOW );
		}
	}
	return m_hwnd;
}

BOOL CALLBACK CDialogBase::DialogProcWrapper(HWND hwnd,
						  UINT uMsg,
						  WPARAM wParam,
						  LPARAM lParam )
{
	// We use the dialog-box's USERDATA to store a _this pointer
	// This is set only once WM_INITDIALOG has been recieved, though!
	CDialogBase *_this = reinterpret_cast<CDialogBase*>(GetWindowLong(hwnd, GWL_USERDATA));
	if( uMsg == WM_INITDIALOG )
	{
		_this = reinterpret_cast<CDialogBase*>(lParam);
		_this->m_hwnd = hwnd;
	}
	if( _this != NULL )
		return _this->InternalDialogProc( uMsg, wParam, lParam );
	return 0;
}

BOOL CDialogBase::InternalDialogProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		{
			// Retrieve the Dialog box parameter and use it as a pointer
			// to the calling vncProperties object
			SetWindowLong(m_hwnd, GWL_USERDATA, (LPARAM)this);
			// Bring window to top
			SetForegroundWindow(m_hwnd);
			if(!OnInitDialog())
			{
				m_returncode_valid = TRUE;
				EndDialog(m_hwnd, IDOK);
				m_dlgvisible = FALSE;
				return FALSE;
			}

			m_dlgvisible = TRUE;

			return FALSE;
		}

	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			OnLButtonDown( wParam, pt );
		}
		break;

	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			OnLButtonUp( wParam, pt );
		}
		break;

	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			OnMouseMove( wParam, pt );
		}
		break;

	case WM_CLOSE:
		if( !m_fModalDlg )
			return ::DestroyWindow( m_hwnd );
		break;

	case WM_DESTROY:
		if( !m_fModalDlg )
		{
			SetWindowLong(m_hwnd, GWL_USERDATA, (LPARAM)0);
			delete this;
		}
		m_hwnd = NULL;
		return TRUE;

	case WM_NOTIFY:
		OnNotify( (int)wParam, (LPNMHDR)lParam );
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			m_returncode_valid = TRUE;
			if( OnOK() )
			{
				EndDialog(m_hwnd, IDOK);
				m_dlgvisible = FALSE;
				PostMessage( m_hwnd, WM_CLOSE, 0, 0 );
			}
			return TRUE;
		case IDCANCEL:
			m_returncode_valid = TRUE;
			if( OnCancel() )
			{
				EndDialog(m_hwnd, IDCANCEL);
				m_dlgvisible = FALSE;
			}
			return TRUE;
		}
	}
	return OnDialogProc(uMsg, wParam, lParam);
}

BOOL CDialogBase::OnDialogProc(UINT, WPARAM, LPARAM)
{
	return 0;
}

BOOL CDialogBase::OnInitDialog()
{
	return TRUE;
}

BOOL CDialogBase::OnOK()
{
	return TRUE;
}

BOOL CDialogBase::OnCancel()
{
	return TRUE;
}
