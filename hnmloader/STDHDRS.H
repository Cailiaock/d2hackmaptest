//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#ifndef STD_HEADER_H
#define STD_HEADER_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <cassert>
#include "resource.h"

#pragma warning( disable : 4786 )
#include "tstring.h"

#ifndef COUNTOF
#define COUNTOF(x)	sizeof((x)) / sizeof(x[0])
#endif

#ifndef OFFSETOF
// calculate the offset of members in structure
#define OFFSETOF( type, offset) ( (DWORD)&((type*)0)->offset )
#endif

#define mymbstowcs(wcstr, cb_wcstr, mbstr, cb_mbstr)	\
	MultiByteToWideChar(CP_ACP,					\
				0,										\
				mbstr,									\
				cb_mbstr,								\
				wcstr,									\
cb_wcstr)

#define mymbstowcs2(wcstr, mbstr) mymbstowcs(wcstr, COUNTOF(wcstr), mbstr, -1)

#define mywcstombs(mbstr, cb_mbstr, wcstr, cb_wcstr)	\
	WideCharToMultiByte(CP_ACP,					\
	0,										\
	wcstr,									\
	cb_wcstr,								\
	mbstr,									\
	cb_mbstr,								\
	NULL,									\
	NULL)

#define mywcstombs2(mbstr, wcstr) mywcstombs(mbstr, COUNTOF(mbstr), wcstr, -1)

#ifdef UNICODE
	#define useconv_towcstr(wcstr, tstr) \
		LPCWSTR wcstr = tstr;
	#define useconv_tombstr(mbstr, tstr) \
		char mbstr[MAX_PATH]; \
		mywcstombs2(mbstr, tstr);
#else // multibytes
	#define useconv_towcstr(wcstr, tstr) \
		WCHAR wcstr[MAX_PATH]; \
		mymbstowcs2(wcstr, tstr);
	#define useconv_tombstr(mbstr, tstr) \
		LPCSTR mbstr = tstr;
#endif // UNICODE

#define GetFileNameFromPath(path) (_tcsrchr(path, _T('\\'))+1)
#define ASSERT		assert

#define g_szAppName _T("Hackmap")
extern void GlobalReportStatus(UINT nIdString);
extern void __cdecl GlobalFormatAndReportStatus(UINT nIdString, ...);
extern HWND GlobalGetMainHwnd();
extern HINSTANCE	g_hAppInstance;
extern HINSTANCE	g_hResInstance;
extern BOOL g_isNTPlatform;
inline int LoadString(UINT uID, LPTSTR lpBuffer, int nBufferMax)
{
	return LoadString(g_hResInstance, uID, lpBuffer, nBufferMax)
		|| (g_hResInstance != g_hAppInstance ? LoadString(g_hAppInstance, uID, lpBuffer, nBufferMax) : 0);
}

#endif // STD_HEADER_H