//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////


#ifndef __DIALOG_BASE_H__
#define __DIALOG_BASE_H__

#pragma warning( disable : 4786 )
#include <CommCtrl.h>
#include <Windowsx.h>

class CDialogBase  
{
public:
	CDialogBase(UINT nIDD);
	virtual ~CDialogBase();
	int DoModal( HINSTANCE hInstance = NULL, HWND hParent = NULL );
	HWND DoModeless( HINSTANCE hInstance = NULL, HWND hParent = NULL );
	operator const HWND() {return m_hwnd;}
	CDialogBase& operator =(CDialogBase& dlg);
protected:
	static BOOL CALLBACK DialogProcWrapper(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL InternalDialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnDialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	virtual BOOL OnOK();
	virtual BOOL OnCancel();
	virtual void OnLButtonDown(UINT, POINT&) {}
	virtual void OnLButtonUp(UINT, POINT&) {}
	virtual void OnMouseMove(UINT, POINT&) {}
	virtual void OnNotify(int idCtrl, LPNMHDR pnmh ) {}
protected:
	HWND m_hwnd;
	UINT	m_IDD;
	HMODULE m_hInstance;
	BOOL m_returncode_valid;
	BOOL m_dlgvisible;
	BOOL m_fModalDlg;
};

#endif // __DIALOG_BASE_H__
