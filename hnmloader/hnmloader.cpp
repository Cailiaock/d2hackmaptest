//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#include "stdhdrs.h"
#include <crtdbg.h>
#include "winMenu.h"
#include "ConfigLoader.h"

int WinAppMain();
BOOL isNT();

HINSTANCE	g_hAppInstance = NULL;
HINSTANCE	g_hResInstance = NULL;
BOOL g_isNTPlatform = isNT();


static BOOL isNT()
{
	
	OSVERSIONINFO osversioninfo;
	osversioninfo.dwOSVersionInfoSize = sizeof(osversioninfo);
	
	// Get the current OS version
	if (!GetVersionEx(&osversioninfo))
		return FALSE;
	return osversioninfo.dwPlatformId == VER_PLATFORM_WIN32_NT;
}

static void SetWorkDirectory()
{
	TCHAR szPath[MAX_PATH];
	::GetModuleFileName( NULL, szPath, COUNTOF(szPath)-1 );
	*_tcsrchr(szPath, _T('\\')) = _T('\0');
	SetCurrentDirectory(szPath);
}

static void SetCrtDebugFlag()
{
#ifdef _DEBUG
	// Get current flag
	int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );

	// Turn on leak-checking bit
	tmpFlag |= _CRTDBG_LEAK_CHECK_DF;

	// Set flag to the new value
	_CrtSetDbgFlag( tmpFlag );
#endif
}

static BOOL IsLangIDMatched(LPTSTR lpIDS)
{
	TCHAR szDelimiter[] = _T(";, \t\r\n");
	TCHAR szNumber[] = _T("0123456789");
	LPTSTR p, ps, pe;

	if(lpIDS == NULL) return FALSE;

	WORD wLocalLangID = LOWORD(GetUserDefaultLCID());
	ps = lpIDS;
	pe = ps+_tcslen(ps);

	while(ps && ps < pe)
	{
		ps = _tcspbrk(ps, szNumber);
		if(ps)
		{
			p = _tcspbrk(ps, szDelimiter);
			if(p) *p = _T('\0');
			if(wLocalLangID == _ttoi(ps))
				return TRUE;
			if(!p) return FALSE;
			ps = p+1;
		}
	}
	return FALSE;
}

static HINSTANCE LoadLocaleResource()
{
	WIN32_FIND_DATA fld;
	HANDLE h = FindFirstFile(_T("*.lng"), &fld);
	HMODULE hModule = NULL;
	TCHAR szLangID[256];

	if(h != INVALID_HANDLE_VALUE)
	{
		do
		{
			hModule = LoadLibrary(fld.cFileName);
			if(hModule)
			{
				LoadString(hModule, IDS_LANG_IDS, szLangID, COUNTOF(szLangID)-1);
				if(IsLangIDMatched(szLangID))
					break;

				FreeLibrary(hModule);
				hModule = NULL;
			}
		}while(FindNextFile(h, &fld));
		FindClose(h);
	}
	return hModule;
}

static void LoadConfig()
{
	__try {
		char szConfigFileName[MAX_PATH];
		GetModuleFileNameA(GetModuleHandle(NULL), szConfigFileName, COUNTOF(szConfigFileName));
		strcpy(strrchr(szConfigFileName, '.')+1, "ini");
		g_config.LoadConfig(szConfigFileName);
	} __except(EXCEPTION_EXECUTE_HANDLER){
	}
}

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE /*hPrevInstance*/,
#ifdef UNICODE
				   PSTR lpszCmdLine,
#else
				   PSTR szCmdLine,
#endif
				   int /*iCmdShow*/)
{
	SetWorkDirectory();
	SetCrtDebugFlag();

	// Save the application instance and main thread id
	g_hAppInstance = hInstance;

	return WinAppMain();
}

// This is the main routine for WinService when running as an application
// (under Windows 95 or Windows NT)
// Under NT, WinService can also run as a service.  The WinServiceMain routine,
// defined in the WinService header, is used instead when running as a service.

int WinAppMain()
{
	// Load local Resource if exist, for further localization support
	if((g_hResInstance = LoadLocaleResource()) == NULL)
		g_hResInstance = g_hAppInstance; // Use default

	winMenu menu;
	if( !menu.Init() )
	{
		MessageBox(NULL, _T("Initialization failed."), g_szAppName, MB_OK);
		PostQuitMessage(0);
	}

	LoadConfig();

	// Now enter the message handling loop until told to quit!
	MSG msg;
	while (GetMessage(&msg, NULL, 0,0) )
	{
		TranslateMessage(&msg);  // convert key ups and downs to chars
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}
