// Implementation of a system tray icon & menu for WinService
#include "stdhdrs.h"
#include <tchar.h>
#include "winAbout.h"
#include <lmcons.h>
#include "winMenu.h"

#define MAIN_WINDOW_CLASS_NAME	g_szAppName _T(" Tray Icon")

// Application specific messages
// Message used for system tray notifications
#define WM_TRAYNOTIFY				WM_USER+1

// Implementation

winMenu::winMenu()
		:m_hmenu(NULL)
{
	// Initialise the properties dialog object
	if (!m_properties.Init())
	{
		PostQuitMessage(0);
		return;
	}

	// Load the icons for the tray
	m_tray_icon = LoadIcon(g_hAppInstance, MAKEINTRESOURCE(IDI_TRAY_ICON));

	// Load the popup menu
	m_hmenu = LoadMenu(g_hResInstance, MAKEINTRESOURCE(IDR_TRAYMENU));
}


BOOL winMenu::Init()
{
	// Create a dummy window to handle tray icon messages
	WNDCLASSEX wndclass;

	wndclass.cbSize			= sizeof(wndclass);
	wndclass.style			= 0;
	wndclass.lpfnWndProc	= winMenu::WndProc;
	wndclass.cbClsExtra		= 0;
	wndclass.cbWndExtra		= 0;
	wndclass.hInstance		= g_hAppInstance;
	wndclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName	= (const TCHAR *) NULL;
	wndclass.lpszClassName	= MAIN_WINDOW_CLASS_NAME;
	wndclass.hIconSm		= LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wndclass);

	
	m_hwnd = CreateWindow(MAIN_WINDOW_CLASS_NAME,
				MAIN_WINDOW_CLASS_NAME,
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				200, 200,
				NULL,
				NULL,
				g_hAppInstance,
				NULL);
	if (m_hwnd == NULL)
	{
		PostQuitMessage(0);
		return FALSE;
	}

	// Install the tray icon!
	AddTrayIcon();

	// Timer to trigger icon updating
	SetTimer(m_hwnd, 1, 5000, NULL);

	// record which client created this window
	SetWindowLong(m_hwnd, GWL_USERDATA, (LONG) this);


	// display main dialog
	PostMessage(m_hwnd, WM_COMMAND, ID_PROPERTIES, 0);

	return TRUE;
}

winMenu::~winMenu()
{
	// Remove the tray icon
	DelTrayIcon();
	
	// Destroy the loaded menu
	if (m_hmenu != NULL)
		DestroyMenu(m_hmenu);
}

void
winMenu::AddTrayIcon()
{
	SendTrayMsg(NIM_ADD, m_tray_icon);
}

void
winMenu::DelTrayIcon()
{
	SendTrayMsg(NIM_DELETE, m_tray_icon);
}


void
winMenu::FlashTrayIcon(DWORD /*flash*/)
{
	SendTrayMsg(NIM_MODIFY, m_tray_icon);
}

void
winMenu::SendTrayMsg(DWORD msg, HICON hIcon)
{
	// Create the tray icon message
	m_nid.hWnd = m_hwnd;
	m_nid.cbSize = sizeof(m_nid);
	m_nid.uID = IDI_TRAY_ICON;			// never changes after construction
	m_nid.hIcon = hIcon;
	m_nid.uFlags = NIF_ICON | NIF_MESSAGE;
	m_nid.uCallbackMessage = WM_TRAYNOTIFY;

	// Use resource string as tip if there is one
	if (LoadString(IDS_VERSION_INFO, m_nid.szTip, COUNTOF(m_nid.szTip)-1))
	{
	    m_nid.uFlags |= NIF_TIP;
	}

	// Send the message
	if (Shell_NotifyIcon(msg, &m_nid))
	{
		// Set the enabled/disabled state of the menu items

		EnableMenuItem(m_hmenu, ID_PROPERTIES, MF_ENABLED);
		EnableMenuItem(m_hmenu, ID_CLOSE, MF_ENABLED);
	}
}

// Process window messages
LRESULT CALLBACK winMenu::WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// This is a static method, so we don't know which instantiation we're 
	// dealing with. We use Allen Hadden's (ahadden@taratec.com) suggestion 
	// from a newsgroup to get the pseudo-this.
	winMenu *_this = reinterpret_cast<winMenu*>( GetWindowLong(hwnd, GWL_USERDATA) );

	switch (iMsg)
	{
		// Every five seconds, a timer message causes the icon to update
	case WM_TIMER:
		// Update the icon
		_this->FlashTrayIcon(0);
		break;

	case WM_COMMAND:
		// User has clicked an item on the tray menu
		switch (LOWORD(wParam))
		{
		case ID_PROPERTIES:
			// Show the properties dialog, unless it is already displayed
			if(_this->m_properties.DoModal( g_hResInstance, NULL ) == IDOK)
				PostQuitMessage(0);
			break;

		case ID_ABOUT:
			{
			// Show the About box
			HBRUSH hBrush = NULL;
			HBITMAP hBitmap = LoadBitmap(g_hAppInstance, MAKEINTRESOURCE(IDB_BACKGROUND));
			if(hBitmap)	hBrush = CreatePatternBrush(hBitmap);
			winAbout about;
			about.SetBGBrush(hBrush);
			about.DoModal( g_hResInstance, hwnd );
			if(hBrush)	DeleteObject(hBrush);
			if(hBitmap)	DeleteObject(hBitmap);
			}
			break;

		case ID_CLOSE:
			PostMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		
		default:
			break;
		}
		return 0;

	case WM_TRAYNOTIFY:
		// User has clicked on the tray icon or the menu
		{
			// Get the submenu to use as a pop-up menu
			HMENU submenu = GetSubMenu(_this->m_hmenu, 0);

			// What event are we responding to, RMB click?
			if (lParam==WM_RBUTTONUP)
			{
				if (submenu)
				{
					// Make the first menu item the default (bold font)
					SetMenuDefaultItem(submenu, 0, TRUE);
					
					// Get the current cursor position, to display the menu at
					POINT mouse;
					GetCursorPos(&mouse);

					// There's a "bug"
					// (Microsoft calls it a feature) in Windows 95 that requires calling
					// SetForegroundWindow. To find out more, search for Q135788 in MSDN.
					//
					SetForegroundWindow(_this->m_nid.hWnd);

					// Display the menu at the desired position
					TrackPopupMenu(submenu,
							0, mouse.x, mouse.y, 0,
							_this->m_nid.hWnd, NULL);
					PostMessage(_this->m_nid.hWnd, WM_NULL, 0, 0);
				}
			}
			
			// Or was there a LMB double click?
			else if (lParam==WM_LBUTTONDBLCLK)
			{
				// double click: execute first menu item
				SendMessage(_this->m_nid.hWnd,
						WM_COMMAND, 
						GetMenuItemID(submenu, 0),
						0);
			}

			return 0;
		}

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	// Message not recognised
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

