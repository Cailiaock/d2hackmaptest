//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#pragma once

#pragma warning(disable:4786)
#include <map>
#include "tstring.h"
#include "AutoList.h"

struct GameStat {
	BYTE curractlvl;
	BYTE actlvls[5];
	BYTE tomblvl;
	BYTE tombbosslvl;
	BYTE iscan;
};

class CGameWalker
{
public:
	struct GameInfo {
		DWORD		processId;
		HWND		hwnd;
		HMODULE		hModuleHackmap;
		int			mhVersionInfo;
		DWORD		gameServerId;
		tstring		gameServerIp;
		tstring		gameTitle;
		GameStat	gameStat;
	};
	typedef std::map<DWORD, GameInfo> GameList;
	typedef AutoList<GameList> DelGameList;
	GameList m_games;
public:
	CGameWalker(void);
	~CGameWalker(void);
	void Walk();
	void Walk(DelGameList& delGames);

	// helpers
	static HMODULE LoadModule( DWORD pid, HWND hwnd, LPCTSTR modulePath );
	static HMODULE UnloadModule( DWORD pid, HWND hwnd, HMODULE hModule );

	// details
private:
	struct EnumWindowStruct
	{
		CGameWalker* walker;
		DelGameList* delGames;
	};
	static BOOL WINAPI EnumWindowsProc(HWND hwnd, LPARAM lparam);
	BOOL UpdateGameList( HWND hwnd, DelGameList& delGames );
	static BOOL TestFileExist( LPCTSTR lpszPath );
};
