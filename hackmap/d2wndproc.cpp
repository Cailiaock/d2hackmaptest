#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "hackmap.h"

static HWND g_hwndD2;
static LPCSTR _HACKMAP_PROP = "HACK MAP";
static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam)
{
	char classname[20];

	GetClassName(hwnd, classname, sizeof(classname));

	if(!strcmp(classname, "Diablo II"))
	{
		DWORD pid, tid;
		tid = GetWindowThreadProcessId( hwnd, &pid );
		if( pid == GetCurrentProcessId() )
		{
			//		*(HWND*)lparam = hwnd;
			g_hwndD2 = hwnd;
			return FALSE;	// Stop enum
		}
	}

	return TRUE;	// Continue enum
}

static BOOL FindD2Window()
{
	g_hwndD2 = NULL;
	EnumWindows(EnumWindowsProc, (LPARAM)&g_hwndD2);
	//	EnumThreadWindows(GetCurrentThreadId(), EnumWindowsProc, (LPARAM)&g_hwndD2);
	return g_hwndD2 != NULL;
}

BOOL InstallWindowHook()
{
	if(g_hwndD2) return TRUE;
	if(!FindD2Window()) return FALSE;
	::SetProp(g_hwndD2, _HACKMAP_PROP, (HANDLE)hInstDLL);
	return TRUE;
}

void RemoveWindowHook()
{
	if(g_hwndD2)
	{
		::RemoveProp(g_hwndD2, _HACKMAP_PROP);
	}
}
