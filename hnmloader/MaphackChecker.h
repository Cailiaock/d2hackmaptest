//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////


#ifndef __MAPHACK_CHECKER_H__
#define __MAPHACK_CHECKER_H__

#include "ModuleWalker.h"

#define MH_NOT_LOADED -1
#define MH_UNKNOWN 0
#define MH_HACKMAP 1
#define MH_51EDITED_MAPHACK 2

class CMaphackChecker  
{
public:
	CMaphackChecker();
	~CMaphackChecker();
	static HMODULE CheckMaphack(DWORD processId, int& verinfo);
	static HMODULE TestHackmapLoaded(DWORD processId);
private:
	static CModuleWalker::DllInfo* TestHackmapLoaded(HANDLE hProcess, CModuleWalker& dllWalker);
	static BOOL CheckForHackmap(LPBYTE ps, LPBYTE pe, HANDLE hProcess, CModuleWalker& dllWalker, CModuleWalker::DllInfo& mhDll);
	static BOOL CheckForMaphack51Edited(LPBYTE ps, LPBYTE pe, HANDLE hProcess, CModuleWalker& dllWalker, CModuleWalker::DllInfo& mhDll);
	static BOOL CheckD2CLIENT(HANDLE hProcess, CModuleWalker& dllWalker);
};

#endif // __MAPHACK_CHECKER_H__
