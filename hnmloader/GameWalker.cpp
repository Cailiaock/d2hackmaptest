//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#include "stdhdrs.h"
#include "GameWalker.h"
#include "ModuleWalker.h"
#include "ModuleLoader.h"

CGameWalker::CGameWalker(void)
{
}

CGameWalker::~CGameWalker(void)
{
}

void CGameWalker::Walk()
{
	DelGameList delGames(m_games);
	Walk(delGames);
}

void CGameWalker::Walk(DelGameList& delGames)
{
	EnumWindowStruct ew = {this, &delGames};
	EnumWindows(EnumWindowsProc, (LPARAM)&ew );
}

BOOL CGameWalker::EnumWindowsProc(HWND hwnd, LPARAM lparam)
{
	EnumWindowStruct* pew = reinterpret_cast<EnumWindowStruct*>(lparam);
	return pew->walker->UpdateGameList(hwnd, *pew->delGames);
}

BOOL CGameWalker::UpdateGameList( HWND hwnd, DelGameList& delGames )
{
	TCHAR text[256];

	GetClassName(hwnd, text, COUNTOF(text)-1 );
	if( _tcsicmp(text, _T("Diablo II")) == 0 )
	{
		DWORD pid;
		GetWindowThreadProcessId(hwnd, &pid);

		if( m_games.find( pid ) == m_games.end() )
		{
			ZeroMemory( text, sizeof(text) );
			::GetWindowText( hwnd, text, COUNTOF(text)-1 );
			GameInfo& gi = m_games[pid];
			gi.processId = pid;
			gi.hwnd = hwnd;
			gi.hModuleHackmap = NULL;
			gi.mhVersionInfo = -1;
			gi.gameTitle = text;
			gi.gameServerIp = _T("");
			gi.gameServerId = 0;
			memset(&gi.gameStat, 0, sizeof(gi.gameStat) );
		}
		else
			delGames.erase(pid);
	}
	return TRUE;
}


HMODULE CGameWalker::LoadModule( DWORD pid, HWND hwnd, LPCTSTR modulePath )
{
	LPCTSTR pszPath = NULL;
	TCHAR szPath[MAX_PATH];
	if(_tcsrchr(modulePath, _T('\\')) != NULL)
		pszPath = modulePath;
	else
	{
		GetModuleFileName(GetModuleHandle(NULL), szPath, COUNTOF(szPath));
		_tcscpy(GetFileNameFromPath(szPath), modulePath);
		pszPath = szPath;
	}


	if( TestFileExist(pszPath) )
		return CModuleLoader::LoadModule( pid, hwnd, pszPath );
	else
	{
		GlobalFormatAndReportStatus(IDS_ERROR_FILE_NOTEXISTS, pszPath);
		return NULL;
	}
}

HMODULE CGameWalker::UnloadModule( DWORD pid, HWND hwnd, HMODULE hModule )
{
	return CModuleLoader::LoadModule( pid, hwnd, NULL, hModule );
}

BOOL CGameWalker::TestFileExist( LPCTSTR lpszPath )
{
	useconv_tombstr(szPath, lpszPath);

	OFSTRUCT ofs;
	HFILE h = OpenFile( szPath, &ofs, OF_EXIST );
	return h != HFILE_ERROR;
}
