// This class handles creation of a system-tray icon & menu

class winMenu;

#ifndef _WIN_MENU
#define _WIN_MENU

#include "winProperties.h"
#include <ShellAPI.h>

// The tray menu class itself
class winMenu
{
public:
	winMenu();
	~winMenu();
	BOOL Init();
protected:
	// Tray icon handling
	void AddTrayIcon();
	void DelTrayIcon();
	void FlashTrayIcon(DWORD flash);
	void SendTrayMsg(DWORD msg, HICON hIcon);

	// Message handler for the tray window
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

	// Fields
protected:
	// Properties object for this server
	winProperties	m_properties;

	HWND			m_hwnd;
	HMENU			m_hmenu;
	NOTIFYICONDATA	m_nid;

	// The icon handles
	HICON			m_tray_icon;
};


#endif // _WIN_MENU