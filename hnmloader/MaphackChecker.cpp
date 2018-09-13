//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#include "stdhdrs.h"
#include "MaphackChecker.h"
#include "AutoHandle.h"
#include <algorithm>

#define GAMELOOP_PATCH_OFFSET 0x9640
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMaphackChecker::CMaphackChecker()
{

}

CMaphackChecker::~CMaphackChecker()
{

}

BOOL CMaphackChecker::CheckD2CLIENT(HANDLE hProcess, CModuleWalker& dllWalker)
{
	CModuleWalker::DllInfo* pdiD2Client = dllWalker.FindModule(_T("D2CLIENT.DLL"));
	if(!pdiD2Client) return FALSE;

	DWORD cbSize;
	BYTE buf[5];
	if(!ReadProcessMemory(hProcess, (LPBYTE)(pdiD2Client->imageBase+0xADBFD+5), &buf[0], sizeof(buf), &cbSize))
		return FALSE;

	BYTE fp[] = {0xe9, 0x03, 0x01, 0x00, 0x00};
	return memcmp(buf, fp, sizeof(buf)) == 0;
}

static BYTE fp1[] = {0xEB, 0x04, 0x33, 0xC0, 0xEB, 0x69, 0xE8, 0xA1, 0xFB, 0xFF, 0xFF};
static BYTE fp2[] = {0x59, 0x89, 0x46, 0x08, 0x90, 0x8B, 0x0E, 0x51, 0x8B, 0x4E, 0xF8,
0x8B, 0xD0, 0xE8, 0xF9, 0xC7, 0xFF, 0xFF, 0x90, 0x8B};
static BYTE fp3[] = {0xE8, 0x7E, 0xFE, 0xFF, 0xFF, 0x55, 0x8B, 0xEC, 0x51, 0x51, 0xB9,
0x00, 0x02, 0xDC, 0x0A, 0xC6, 0x45, 0xF8, 0xE9, 0xC6, 0x45, 0xF9,
0x03, 0xC6, 0x45, 0xFA, 0x01, 0xC6, 0x45, 0xFB, 0x00, 0xC6, 0x45,
0xFC, 0x00, 0xE8, 0x92, 0xC2, 0xFF, 0xFF, 0x6A, 0x05, 0x8D, 0x55,
0xF8, 0x8B, 0xC8, 0xE8, 0x12, 0xC7, 0xFF, 0xFF, 0xC9, 0xC3};
struct fingerprint_t
{
	PBYTE ps;
	PBYTE pe;
};

inline BOOL CheckFingerPrint(PBYTE ps, PBYTE pe, PBYTE pfs, PBYTE pfe)
{
	PBYTE p = std::search( ps, pe, pfs, pfe );
	return p != pe && std::search( p+1, pe, pfs, pfe ) == pe;
}

BOOL CMaphackChecker::CheckForHackmap(LPBYTE ps, LPBYTE pe, HANDLE hProcess, CModuleWalker& dllWalker, CModuleWalker::DllInfo& mhDll)
{
	CModuleWalker::DllInfo* pdiD2Client = dllWalker.FindModule(_T("D2CLIENT.DLL"));
	if(!pdiD2Client) return FALSE;
	
	DWORD addr = pdiD2Client->imageBase+0x2FD6A;
	DWORD cbSize;
	BYTE buf[5];
	if(!ReadProcessMemory(hProcess, (LPBYTE)addr, &buf[0], sizeof(buf), &cbSize))
		return FALSE;

	DWORD callee = addr + *(int*)(buf+1) + 5;
	if(mhDll.imageBase < callee && mhDll.imageBase+mhDll.imageSize > callee)
		return TRUE;

	return FALSE;
}

BOOL CMaphackChecker::CheckForMaphack51Edited(LPBYTE ps, LPBYTE pe, HANDLE hProcess, CModuleWalker& dllWalker, CModuleWalker::DllInfo& mhDll)
{
	if(CheckD2CLIENT(hProcess, dllWalker))
		return TRUE;
	fingerprint_t fingerprints[] = {
		{fp1, fp1+sizeof(fp1)},
		{fp2, fp2+sizeof(fp2)},
		{fp3, fp3+sizeof(fp3)},
	};
	
	int hitcount = 0;
	for(int i = 0; i < sizeof(fingerprints)/sizeof(fingerprint_t); ++i)
	{
		if(CheckFingerPrint(ps, pe, fingerprints[i].ps, fingerprints[i].pe))
		{
			++hitcount;
			break;
		}
	}
	if(hitcount > 0)
		return TRUE;
	return FALSE;
}

HMODULE CMaphackChecker::CheckMaphack(DWORD processId, int& verinfo)
{
	verinfo = MH_NOT_LOADED;

	AutoHandle<HANDLE> hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	if(hProcess == NULL ) return NULL;
	CModuleWalker dllWalker;
	if(!dllWalker.Walk(processId)) return NULL;

	CModuleWalker::DllInfo* pdi = TestHackmapLoaded(hProcess, dllWalker);
	if(!pdi) return NULL;
	verinfo = MH_UNKNOWN;
	
	AutoHandle<LPBYTE> pAllocBase = (LPBYTE)VirtualAlloc(NULL, pdi->imageSize, MEM_COMMIT, PAGE_READWRITE);
	if(pAllocBase == NULL) return (HMODULE)pdi->imageBase;
	DWORD cbSize;
	if(!ReadProcessMemory(hProcess, (LPVOID)pdi->imageBase, pAllocBase, pdi->imageSize, &cbSize))
		return (HMODULE)pdi->imageBase;

	if(CheckForHackmap(pAllocBase, pAllocBase+pdi->imageSize, hProcess, dllWalker, *pdi))
	{
		verinfo = MH_HACKMAP;
	}
	else if(CheckForMaphack51Edited(pAllocBase, pAllocBase+pdi->imageSize, hProcess, dllWalker, *pdi))
	{
		verinfo = MH_51EDITED_MAPHACK;
	}

	return (HMODULE)pdi->imageBase;
}

CModuleWalker::DllInfo* CMaphackChecker::TestHackmapLoaded(HANDLE hProcess, CModuleWalker& dllWalker)
{
	CModuleWalker::DllInfo* pdiTarget = NULL;
	CModuleWalker::DllInfo* pdiD2Client = dllWalker.FindModule(_T("D2CLIENT.DLL"));
	if(pdiD2Client)
	{
		DWORD target = pdiD2Client->imageBase+GAMELOOP_PATCH_OFFSET; // check game loop patch location
		BYTE buf[5];
		DWORD cbSize;
		if(ReadProcessMemory(hProcess, (LPBYTE)target, buf, sizeof(buf), &cbSize))
		{
			if(buf[0] == 0xe8)
			{
				DWORD callee = (DWORD)(target + *(int*)(buf+1) + 5);
				CModuleWalker::DllInfo *pdi = dllWalker.FindModuleByAddress( callee );
				if(pdi && pdi->imageBase != (DWORD)pdiD2Client->imageBase)
					pdiTarget = pdi;
			}
		}
	}
	return pdiTarget;
}

HMODULE CMaphackChecker::TestHackmapLoaded(DWORD processId)
{
	AutoHandle<HANDLE> hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	if(hProcess == NULL ) return NULL;
	CModuleWalker dllWalker;
	if(!dllWalker.Walk(processId)) return NULL;
	
	CModuleWalker::DllInfo *pdi = TestHackmapLoaded(hProcess, dllWalker);
	return pdi ? (HMODULE)pdi->imageBase : NULL;
}
