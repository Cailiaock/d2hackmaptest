//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////


#ifndef __REVEALMAP_WORKER_H__
#define __REVEALMAP_WORKER_H__

class CModuleWalker;
struct GameStat;

#define RM_SCAN_GAMEDLLS_ONLY 1
#define RM_INTEGRITY_SCAN 2
#define RM_NOWARNINGS 4
#define RM_REPORT_ISCAN_TOGAME 8
#define RM_EXTRA_INTEGRITY_CHECK 16

class CRevealMapWorker  
{
	enum {eTxtColWhite = 0, eTxtColRed = 1, eTxtColGreen = 2, eTxtColBlue = 3,
		eTxtColGold2 = 4, 
		eTxtColGold = 7, eTxtColDeepYellow = 8, eTxtColLightYellow = 9,
		eTxtColDeepGreen = 10
	};
public:
	CRevealMapWorker();
	~CRevealMapWorker();
	BOOL RevealAutomapAct(DWORD processId, HWND hwnd, DWORD flag);
	BOOL TestInGame(DWORD processId);
	BOOL AutoRevealAutomapAct(DWORD processId, HWND hwnd, GameStat& gameStat, DWORD flag);
	BOOL GetGameServerInfo(DWORD processId, HWND hwnd, DWORD& gameServerId, tstring& gameServerIp);
private:
	BOOL BefroeActing(DWORD processId, HWND hwnd, DWORD flag);
	BOOL PostD2Message(DWORD processId, HWND hwnd, DWORD flag, LPCTSTR text, BYTE col);
	BOOL PromptTombLevel(DWORD processId, HWND hwnd, DWORD flag, BYTE tomblvl);
	static BOOL GetRemoteStructureData(HANDLE hProcess, DWORD baseaddr, int* offsets, int count, void* result, DWORD nSize = 4);
	static BOOL GetPlayerUnit(HANDLE hProcess, CModuleWalker& walker, DWORD& pla);
	static BOOL GetCurrentDrlgLevel(HANDLE hProcess, DWORD pla, DWORD& result);
	static BOOL GetCurrentTombLevel(HANDLE hProcess, DWORD drlgact, void* result);
	static BOOL GetDrlgAct(HANDLE hProcess, CModuleWalker& walker, DWORD& drlgact);
	static BOOL GetGameServerIp(HANDLE hProcess, CModuleWalker& walker, DWORD& serverId, char* lpszServerIp, int size);
};

#endif // __REVEALMAP_WORKER_H__
