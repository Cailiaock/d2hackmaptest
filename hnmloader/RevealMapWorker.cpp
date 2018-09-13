//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////


#include "stdhdrs.h"
#include "RevealMapWorker.h"
#include "../hackmap/d2structs.h"
#include "ModuleWalker.h"
#include "GameWalker.h"
#include "IntegrityWalker.h"
#include "AutoHandle.h"
#include "RemoteExecute.h"
#include "IntegrityWalker.h"

#define AUTO_RM_MAX_RETRY 2

#define INST_NOP 0x90
#define INST_CALL 0xe8
#define INST_JMP 0xe9
#define INST_JMPR 0xeb
#define INST_RET 0xc3

enum DllNo {DLLNO_D2CLIENT, DLLNO_D2COMMON, DLLNO_D2GFX, DLLNO_D2WIN, DLLNO_D2LANG, DLLNO_D2CMP, DLLNO_BNCLIENT, DLLNO_D2NET};

enum DllBase {
	DLLBASE_D2CLIENT = 0x6FAA0000,
	DLLBASE_D2COMMON = 0x6FD40000,
	DLLBASE_D2GFX = 0x6FA70000,
	DLLBASE_D2WIN = 0x6F8A0000,
	DLLBASE_D2LANG = 0x6FC10000,
	DLLBASE_D2CMP = 0x6FDF0000,
	DLLBASE_BNCLIENT = 0x6FF00000,
	DLLBASE_D2NET = 0x6FC00000,
};
#define DLLOFFSET(a1,b1) ((DLLNO_##a1)|(( ((b1)<0)?(b1):(b1)-DLLBASE_##a1 )<<8))
#define D2FUNCPTR(d1,v1,t1,t2,o1) typedef t1 d1##_##v1##_t t2; d1##_##v1##_t *d1##_##v1 = (d1##_##v1##_t *)DLLOFFSET(d1,o1);
#define D2VARPTR(d1,v1,t1,o1)     typedef t1 d1##_##v1##_t;    d1##_##v1##_t *p_##d1##_##v1 = (d1##_##v1##_t *)DLLOFFSET(d1,o1);
#define D2ASMPTR(d1,v1,o1)        DWORD d1##_##v1 = DLLOFFSET(d1,o1);

D2FUNCPTR(D2CLIENT, GetPlayerUnit, UnitAny * __stdcall, (), DLLBASE_D2CLIENT+0x883D0)
D2FUNCPTR(D2CLIENT, RecvCommand07, void __fastcall, (BYTE *cmdbuf), DLLBASE_D2CLIENT+0x122A0)
D2FUNCPTR(D2CLIENT, RecvCommand08, void __fastcall, (BYTE *cmdbuf), DLLBASE_D2CLIENT+0x12310)
D2FUNCPTR(D2CLIENT, RevealAutomapRoom, void __fastcall, (DrlgRoom1 *room1, DWORD clipflag, AutomapLayer *layer), DLLBASE_D2CLIENT+0x2D180)
D2FUNCPTR(D2COMMON, InitDrlgLevel, void __stdcall, (DrlgLevel *drlglevel), DLLBASE_D2COMMON+0x34C10)
D2FUNCPTR(D2COMMON, GetDrlgLevel, DrlgLevel * __stdcall, (DrlgMisc *drlgmisc, DWORD levelno), DLLBASE_D2COMMON+0x349A0)
D2VARPTR(D2CLIENT, pDrlgAct, DrlgAct *, DLLBASE_D2CLIENT+0x107984)
D2VARPTR(D2CLIENT, pAutomapLayer, AutomapLayer *, DLLBASE_D2CLIENT+0x1119A4)
D2FUNCPTR(D2CLIENT, PrintGameStringAtTopLeft, void __fastcall, (wchar_t* text, int arg2), DLLBASE_D2CLIENT+0x7C600)
D2VARPTR(D2CLIENT, PlayerUnit, UnitAny *, DLLBASE_D2CLIENT+0x11B200)
D2VARPTR(D2NET, Socket, DWORD, DLLBASE_D2NET+0xB0C0)
D2VARPTR(D2CLIENT, pszServerIp, char, DLLBASE_D2CLIENT+0xD5468)
#define OFFSETOF_D2CLIENT_InitAutomapLayer 0x2BBC4
#define OFFSETOF_D2CLIENT_InitAutomapLayer_END 0x2BC8A
#define SIZEOF_D2CLIENT_InitAutomapLayer OFFSETOF_D2CLIENT_InitAutomapLayer_END-OFFSETOF_D2CLIENT_InitAutomapLayer+2
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRevealMapWorker::CRevealMapWorker()
{

}

CRevealMapWorker::~CRevealMapWorker()
{

}

struct D2Ptrs_t
{
	UnitAny* (__stdcall *D2CLIENT_GetPlayerUnit)();
	void (__fastcall *D2CLIENT_RecvCommand07)(BYTE *cmdbuf);
	void (__fastcall *D2CLIENT_RecvCommand08)(BYTE *cmdbuf);
	void (__fastcall *D2CLIENT_RevealAutomapRoom)(DrlgRoom1 *room1, DWORD clipflag, AutomapLayer *layer);
	void (__stdcall *D2COMMON_InitDrlgLevel)(DrlgLevel *drlglevel);
	DrlgLevel* (__stdcall *D2COMMON_GetDrlgLevel)(DrlgMisc *drlgmisc, DWORD levelno);
	DrlgAct** p_D2CLIENT_pDrlgAct;
	AutomapLayer** p_D2CLIENT_pAutomapLayer;
	void (__fastcall *D2CLIENT_PrintGameStringAtTopLeft)(wchar_t* text, int arg2);
};

struct RevealMapContext : public D2Ptrs_t
{
	void (*RevealAutomapLevel)(D2Ptrs_t *pctx, DrlgLevel *pDrlgLevel);
	void (*RevealAutomapRoom)(D2Ptrs_t* pctx, DrlgRoom2 *room2);
	void (__fastcall *InitAutomapLayer)(DWORD levelno, DWORD reallyInitAutomapLayer);
	void (__fastcall *fnInitAutomapLayer)(DWORD levelno);
};

void __declspec(naked) __fastcall InitAutomapLayer(DWORD levelno, DWORD fnInitAutomapLayer)
{
	__asm {
		//ecx = level no
		pushad
		xor ebp,ebp
		call edx
		popad
		ret
	}
}
DECLARE_FUNCTION_END(InitAutomapLayer)

void RevealAutomapRoom(D2Ptrs_t* pctx, DrlgRoom2 *room2)
{
	if (room2->nPresetType == 2) {
		DWORD presetno = *room2->nPresetType2No;
		if (presetno >= 38 && presetno <= 39) return; //Act 1 - Swamp Fill 1&2
		if (presetno >= 401 && presetno <= 405) return; //Act 2 - Desert Fill Bone 1&2, Desert Fill Wagon 1, Desert Fill Berms 1&2
		if (presetno == 836) return; //Act 4 - Lava X
		if (presetno == 863) return; //Act 5 - Town
	}
	if (room2->pRoom1) {
		pctx->D2CLIENT_RevealAutomapRoom(room2->pRoom1, 1, *pctx->p_D2CLIENT_pAutomapLayer);
	} else {
		BYTE cmdbuf[6];
		*(WORD *)(cmdbuf+1) = (WORD)room2->xPos;
		*(WORD *)(cmdbuf+3) = (WORD)room2->yPos;
		cmdbuf[5] = (BYTE)room2->pDrlgLevel->nLevelNo;
		pctx->D2CLIENT_RecvCommand07(cmdbuf);
		pctx->D2CLIENT_RevealAutomapRoom(room2->pRoom1, 1, *pctx->p_D2CLIENT_pAutomapLayer);
		pctx->D2CLIENT_RecvCommand08(cmdbuf);
	}
}
DECLARE_FUNCTION_END(RevealAutomapRoom)

void RevealAutomapLevel(RevealMapContext *pctx, DrlgLevel *pDrlgLevel)
{
	DrlgRoom2 *currroom = pDrlgLevel->pRoom2First;
	while (currroom) {
		pctx->RevealAutomapRoom(pctx, currroom);
		currroom = currroom->pNext;
	}
}
DECLARE_FUNCTION_END(RevealAutomapLevel)

void __stdcall RemoteRevealAutomapAct(RevealMapContext *pctx)
{
	UnitAny* unit = pctx->D2CLIENT_GetPlayerUnit();
	if (!unit || !unit->pPos->pRoom1) return;
	
	DWORD currlvl = unit->pPos->pRoom1->pRoom2->pDrlgLevel->nLevelNo;
	DWORD act = 0;
	BYTE actlvls[] = {1, 40, 75, 103, 109, 133};
	do {} while (currlvl >= actlvls[++act]);
	
	for (DWORD lvl = actlvls[act-1]; lvl < actlvls[act]; lvl++) {
		DrlgLevel *pDrlgLevel = pctx->D2COMMON_GetDrlgLevel((*pctx->p_D2CLIENT_pDrlgAct)->pDrlgMisc, lvl);
		if (!pDrlgLevel->pRoom2First) {
			pctx->D2COMMON_InitDrlgLevel(pDrlgLevel);
		}
		pctx->InitAutomapLayer(lvl, (DWORD)pctx->fnInitAutomapLayer);
		pctx->RevealAutomapLevel(pctx, pDrlgLevel);
	}
	
	pctx->InitAutomapLayer(currlvl, (DWORD)pctx->fnInitAutomapLayer);
}
DECLARE_FUNCTION_END(RemoteRevealAutomapAct)

void _declspec(naked) __stdcall RevealAutomapAct2(RevealMapContext *pctx)
{
	UnitAny* unit;
	DrlgLevel *pDrlgLevel;
	DrlgRoom2 *currroom;
	DWORD currlvl, act, presetno;
	BYTE actlvls[5];
	BYTE cmdbuf[6];
	__asm      /* prolog */
	{
		push ebp;
		mov ebp, esp;
		sub esp, __LOCAL_SIZE;
	}
	
	unit = pctx->D2CLIENT_GetPlayerUnit();
	if (unit && unit->pPos->pRoom1)
	{
		currlvl = unit->pPos->pRoom1->pRoom2->pDrlgLevel->nLevelNo;
		act = 0;
		actlvls[0] = 1;
		actlvls[1] = 40;
		actlvls[2] = 75;
		actlvls[3] = 103;
		actlvls[4] = 109;
		actlvls[5] = 133;
		do {} while (currlvl >= actlvls[++act]);
		
		for (DWORD lvl = actlvls[act-1]; lvl < actlvls[act]; lvl++) {
			pDrlgLevel = pctx->D2COMMON_GetDrlgLevel((*pctx->p_D2CLIENT_pDrlgAct)->pDrlgMisc, lvl);
			if (!pDrlgLevel->pRoom2First) {
				pctx->D2COMMON_InitDrlgLevel(pDrlgLevel);
			}

			__asm {
				pushad;
				mov ecx, lvl;
				mov edx, [pctx];
				mov edx, [edx]RevealMapContext.fnInitAutomapLayer;
				xor ebp,ebp;
				call edx;
				popad;
			}

			currroom = pDrlgLevel->pRoom2First;
			while (currroom) {

				if (currroom->nPresetType == 2) {
					presetno = *currroom->nPresetType2No;
					if (presetno >= 38 && presetno <= 39) goto nextroom; //Act 1 - Swamp Fill 1&2
					if (presetno >= 401 && presetno <= 405) goto nextroom; //Act 2 - Desert Fill Bone 1&2, Desert Fill Wagon 1, Desert Fill Berms 1&2
					if (presetno == 836) goto nextroom; //Act 4 - Lava X
					if (presetno == 863) goto nextroom; //Act 5 - Town
				}
				if (currroom->pRoom1) {
					pctx->D2CLIENT_RevealAutomapRoom(currroom->pRoom1, 1, *pctx->p_D2CLIENT_pAutomapLayer);
				} else {
					*(WORD *)(cmdbuf+1) = (WORD)currroom->xPos;
					*(WORD *)(cmdbuf+3) = (WORD)currroom->yPos;
					cmdbuf[5] = (BYTE)currroom->pDrlgLevel->nLevelNo;
					pctx->D2CLIENT_RecvCommand07(cmdbuf);
					pctx->D2CLIENT_RevealAutomapRoom(currroom->pRoom1, 1, *pctx->p_D2CLIENT_pAutomapLayer);
					pctx->D2CLIENT_RecvCommand08(cmdbuf);
				}
nextroom:
				currroom = currroom->pNext;
			}
		}
		
		__asm {
			pushad;
			mov ecx, lvl;
			mov edx, [pctx];
			mov edx, [edx]RevealMapContext.fnInitAutomapLayer;
			xor ebp,ebp;
			call edx;
			popad;
		}
	}

	__asm      /* epilog */
	{
		mov esp, ebp;
		pop ebp;
		ret 4;
	}
}

BOOL PrepareRealInitAutomapLayer(RevealMapContext& ptrs, CRemoteExecute& execContext, CModuleWalker& walker)
{
	BYTE abInitAutomapLayer[SIZEOF_D2CLIENT_InitAutomapLayer];

	CModuleWalker::DllInfo* pdi = walker.FindModule(_T("D2CLIENT.DLL"));
	if(!pdi) return FALSE;
	LPBYTE ps = (LPBYTE)pdi->imageBase+OFFSETOF_D2CLIENT_InitAutomapLayer;
	LPBYTE pe = (LPBYTE)pdi->imageBase+OFFSETOF_D2CLIENT_InitAutomapLayer_END;


	execContext.ReadProcessMemory(ps, abInitAutomapLayer, pe-ps);
	abInitAutomapLayer[pe-ps] = INST_RET;
	
	int offsetofcall = ps-(LPBYTE)execContext.GetCurrentAvailableBase();
	for (int i = 0; i < pe-ps; i++) {
		if (abInitAutomapLayer[i] == INST_CALL) {
			// adjust call offset
			*(int*)(abInitAutomapLayer+i+1) += offsetofcall;
		}
	}
	
	*(LPVOID*)&ptrs.fnInitAutomapLayer = execContext.WriteProcessMemory(abInitAutomapLayer, sizeof(abInitAutomapLayer));

	return TRUE;
}

DWORD GetDllOffset(TCHAR *dll, int offset, CModuleWalker& walker)
{
	CModuleWalker::DllInfo* pdi = walker.FindModule(dll);
	if(pdi == NULL ) return 0;

	if (offset < 0) {
		return (DWORD)GetProcAddress((HMODULE)pdi->imageBase, (LPCSTR)-offset);
	}
	return pdi->imageBase+offset;
}

DWORD GetDllOffset(int num, CModuleWalker& walker)
{
	static TCHAR *dlls[] = {_T("D2CLIENT.DLL"), _T("D2COMMON.DLL"), _T("D2GFX.DLL"), _T("D2WIN.DLL"), _T("D2LANG.DLL"), _T("D2CMP.DLL"), _T("BNCLIENT.DLL"), _T("D2NET.DLL")};
	return GetDllOffset(dlls[num&0xff], num>>8, walker);
}

BOOL FixupD2Ptrs(D2Ptrs_t& ctx, CModuleWalker& walker)
{
	LPDWORD psrc = (LPDWORD)&D2CLIENT_GetPlayerUnit;
	LPDWORD pdst = (LPDWORD)&ctx;
	for( int i = 0; i < sizeof(D2Ptrs_t)/sizeof(DWORD); ++i, ++psrc, ++pdst)
	{
		*pdst  = GetDllOffset(*psrc, walker);
		if(*pdst == 0)
			return FALSE;
	}
	return TRUE;
}

BOOL FixupD2Ptrs(LPDWORD pdst, LPDWORD psrc, int num, CModuleWalker& walker)
{
	for( int i = 0; i < num; ++i, ++psrc, ++pdst)
	{
		*pdst  = GetDllOffset(*psrc, walker);
		if(*pdst == 0)
			return FALSE;
	}
	return TRUE;
}

BOOL PrepareRevealMapContext(RevealMapContext& ctx, CRemoteExecute& execContext, CModuleWalker& walker)
{
	DWORD codelen = FUNCTLEN(RemoteRevealAutomapAct)
				+ FUNCTLEN(RevealAutomapLevel)
				+ FUNCTLEN(RevealAutomapRoom)
				+ FUNCTLEN(InitAutomapLayer)
				+ SIZEOF_D2CLIENT_InitAutomapLayer;
	DWORD totallen = codelen + sizeof(RevealMapContext);
	if(!execContext.AllocMemory(totallen))
		return FALSE;

	*(LPVOID*)&ctx.RevealAutomapLevel
		= execContext.WriteProcessMemory(
			CRemoteExecute::GetFuncAddress(&RevealAutomapLevel),
				FUNCTLEN(RevealAutomapLevel));


	*(LPVOID*)&ctx.RevealAutomapRoom
		= execContext.WriteProcessMemory(
			CRemoteExecute::GetFuncAddress(&RevealAutomapRoom),
				FUNCTLEN(RevealAutomapRoom));

	*(LPVOID*)&ctx.InitAutomapLayer
		= execContext.WriteProcessMemory(
			CRemoteExecute::GetFuncAddress(&InitAutomapLayer),
				FUNCTLEN(InitAutomapLayer));
	
	
	return PrepareRealInitAutomapLayer(ctx, execContext, walker);
}

BOOL CRevealMapWorker::BefroeActing(DWORD processId, HWND hwnd, DWORD flag)
{
	TCHAR szErrInfo[512];
	if(!TestInGame(processId))
	{
		if(!(flag&RM_NOWARNINGS))
		{
			if(LoadString(IDS_ERR_MUST_INGAME, szErrInfo, COUNTOF(szErrInfo)))
				MessageBox(GlobalGetMainHwnd(), szErrInfo, g_szAppName, MB_OK|MB_ICONERROR );
		}
		return FALSE;
	}
	
	if(flag & RM_INTEGRITY_SCAN)
	{
		BOOL fClean;
		CIntegrityWalker integrityWalker;
		if(!integrityWalker.IntegrityCheck(processId, flag&RM_SCAN_GAMEDLLS_ONLY, fClean))
		{
			if(flag&RM_NOWARNINGS) return FALSE;
			if(LoadString(IDS_WRN_ISCAN_FAILED, szErrInfo, COUNTOF(szErrInfo)))
			{
				if(MessageBox(GlobalGetMainHwnd(), szErrInfo, g_szAppName, MB_YESNO|MB_ICONWARNING) == IDNO)
					return FALSE;
			}
		}
		if(!fClean)
		{
			if(flag&RM_NOWARNINGS) return FALSE;
			if(LoadString(IDS_WRN_INTEGRITY_BROKEN, szErrInfo, COUNTOF(szErrInfo)))
			{
				if(MessageBox(GlobalGetMainHwnd(), szErrInfo, g_szAppName, MB_YESNO|MB_ICONWARNING) == IDNO)
					return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL CRevealMapWorker::RevealAutomapAct(DWORD processId, HWND hwnd, DWORD flag)
{
	if(!BefroeActing(processId, hwnd, flag)) return FALSE;

	CRemoteExecute execContext;
	if(!execContext.OpenProcess(processId, hwnd, REXEC_NT_PREFER|REXEC_FREE_MEM_AFTER_CALL|REXEC_USE_WINDOW_HOOK))
		return FALSE;

	CModuleWalker walker;
	if(!walker.Walk(processId)) return FALSE;

	RevealMapContext context;
	if(!FixupD2Ptrs(context, walker))
		return FALSE;

	if(!PrepareRevealMapContext(context, execContext, walker))
		return FALSE;

	DWORD retval;
	return execContext.Execute(
		(CRemoteExecute::entry_t)CRemoteExecute::GetFuncAddress(&RemoteRevealAutomapAct),
		FUNCTLEN(RemoteRevealAutomapAct),
		&context, sizeof(context),
		retval);
}

BOOL CRevealMapWorker::TestInGame(DWORD processId)
{
	CModuleWalker walker;
	if(walker.Walk(processId))
	{
		DWORD pla=0;
		AutoHandle<HANDLE> hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
		return hProcess != NULL && GetPlayerUnit(hProcess, walker, pla) && pla;
	}
	return FALSE;
}

BOOL CRevealMapWorker::GetPlayerUnit(HANDLE hProcess, CModuleWalker& walker, DWORD& pla)
{
	DWORD addr = GetDllOffset((int)D2CLIENT_GetPlayerUnit, walker);
	if(!addr) return FALSE;
	static int offsets[] = {1, 0};
	return GetRemoteStructureData(hProcess, addr, offsets, COUNTOF(offsets), &pla);
}

BOOL CRevealMapWorker::GetDrlgAct(HANDLE hProcess, CModuleWalker& walker, DWORD& drlgact)
{
	DWORD addr = GetDllOffset((int)p_D2CLIENT_pDrlgAct, walker);
	if(!addr) return FALSE;
	static int offsets[] = {0};
	return GetRemoteStructureData(hProcess, addr, offsets, COUNTOF(offsets), &drlgact);
}

BOOL CRevealMapWorker::GetGameServerIp(HANDLE hProcess, CModuleWalker& walker, DWORD& serverId, char* lpszServerIp, int size)
{
	static int offsets[] = {0};
	DWORD addr = GetDllOffset((int)p_D2NET_Socket, walker);
	if(!addr) return FALSE;
	if(GetRemoteStructureData(hProcess, addr, offsets, COUNTOF(offsets), &serverId))
	{
		if(!serverId) return TRUE;
		addr = GetDllOffset((int)p_D2CLIENT_pszServerIp, walker);
		return addr && GetRemoteStructureData(hProcess, addr, offsets, COUNTOF(offsets), lpszServerIp, size);
	}
	return FALSE;
}

BOOL CRevealMapWorker::GetRemoteStructureData(HANDLE hProcess, DWORD baseaddr, int* offsets, int count, void* result, DWORD nSize)
{
	for(int i = 0; i < count-1; i++)
	{
		if(!baseaddr || !::ReadProcessMemory(hProcess, (LPVOID)(baseaddr+offsets[i]), &baseaddr, sizeof(DWORD), NULL ))
			return FALSE;
	}
	return baseaddr && ::ReadProcessMemory(hProcess, (LPVOID)(baseaddr+offsets[i]), result, nSize, NULL );
}

BOOL CRevealMapWorker::GetCurrentDrlgLevel(HANDLE hProcess, DWORD pla, DWORD& result)
{
	static int offsets[] = {OFFSETOF(UnitAny,pPos), 7*sizeof(DWORD), OFFSETOF(DrlgRoom1,pRoom2), OFFSETOF(DrlgRoom2,pDrlgLevel), OFFSETOF(DrlgLevel,nLevelNo) };
	return GetRemoteStructureData(hProcess, pla, offsets, COUNTOF(offsets), &result);
}

BOOL CRevealMapWorker::GetCurrentTombLevel(HANDLE hProcess, DWORD drlgact, void* result)
{
	static int offsets[] = {OFFSETOF(DrlgAct,pDrlgMisc), OFFSETOF(DrlgMisc,nStaffTombLvl)};
	return GetRemoteStructureData(hProcess, drlgact, offsets, COUNTOF(offsets), result, 8);
}

BOOL CRevealMapWorker::AutoRevealAutomapAct(DWORD processId, HWND hwnd, GameStat& gameStat, DWORD flag)
{
	CModuleWalker walker;
	if(!walker.Walk(processId)) return FALSE;
	AutoHandle<HANDLE> hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	if( hProcess == NULL ) return FALSE;

	DWORD pla;
	if(!GetPlayerUnit(hProcess, walker, pla) || !pla)
	{
		memset(&gameStat, 0, sizeof(gameStat));
		return TRUE;
	}

	DWORD currlvl;
	if(!GetCurrentDrlgLevel(hProcess, pla, currlvl))
		return FALSE;

	DWORD act = 0;
	static BYTE actlvls[] = {1, 40, 75, 103, 109, 133};
	do {} while (currlvl >= actlvls[++act]);
	gameStat.curractlvl = (BYTE)act;

	BOOL fReported = FALSE;

	if(currlvl == 0x2e && gameStat.tomblvl == 0)
	{
		DWORD drlgact;
		struct tombstruct {
			DWORD tomblvl;
			DWORD tombbosslvl;
		};
		tombstruct tomb;
		if(GetDrlgAct(hProcess, walker, drlgact) && GetCurrentTombLevel(hProcess, drlgact, &tomb))
		{
			if(PromptTombLevel(processId, hwnd, flag, (BYTE)tomb.tomblvl))
			{
				fReported = TRUE;
				gameStat.tomblvl = (BYTE)tomb.tomblvl;
				gameStat.tombbosslvl = (BYTE)tomb.tombbosslvl;
			}
		}
	}
	
	if(gameStat.actlvls[act-1] < AUTO_RM_MAX_RETRY)
	{
		if(RevealAutomapAct(processId, hwnd, flag))
		{
			fReported = TRUE;
			gameStat.actlvls[act-1] = AUTO_RM_MAX_RETRY+1;
		}
		else
		{
			gameStat.actlvls[act-1]++;
		}
	}

	if(!fReported && gameStat.iscan != act && (flag&(RM_EXTRA_INTEGRITY_CHECK|RM_REPORT_ISCAN_TOGAME)))
	{
		BOOL fClean;
		CIntegrityWalker integrityWalker;
		if(integrityWalker.IntegrityCheck(processId, flag&RM_SCAN_GAMEDLLS_ONLY, fClean) && !fClean)
		{
			TCHAR text[512];
			SYSTEMTIME t;
			GetLocalTime(&t);
			_stprintf(text, _T("<Hackmap at %.2d:%.2d:%.2d>: Warning!! Integrity scaner has detected your game code is not clean."), t.wHour, t.wMinute, t.wSecond);
			if(PostD2Message(processId, hwnd, RM_NOWARNINGS,
				text, eTxtColRed))
				gameStat.iscan = (BYTE)act;
		}
	}

	return TRUE;
}

struct D2MessageContext : public D2Ptrs_t
{
	wchar_t* text;
	BYTE col;
};

void __stdcall RemotePostD2Message(D2MessageContext *pctx)
{
	pctx->D2CLIENT_PrintGameStringAtTopLeft(pctx->text, pctx->col);
}
DECLARE_FUNCTION_END(RemotePostD2Message)

BOOL CRevealMapWorker::PostD2Message(DWORD processId, HWND hwnd, DWORD flag, LPCTSTR text, BYTE col)
{
	if(!BefroeActing(processId, hwnd, flag)) return FALSE;

	CRemoteExecute execContext;
	if(!execContext.OpenProcess(processId, hwnd, REXEC_NT_PREFER|REXEC_FREE_MEM_AFTER_CALL|REXEC_USE_WINDOW_HOOK))
		return FALSE;
	
	CModuleWalker walker;
	if(!walker.Walk(processId)) return FALSE;
	
	D2MessageContext context;
	if(!FixupD2Ptrs(context, walker))
		return FALSE;

	size_t len = _tcslen(text)+1;
	
#ifdef UNICODE
	LPCWSTR wtext = text;
#else
	LPWSTR wtext = new wchar_t[len];
	mymbstowcs(wtext, len, text, len);
#endif

	DWORD totallen = sizeof(D2MessageContext) + (len+10)*sizeof(wchar_t) + FUNCTLEN(RemotePostD2Message);
	if(!execContext.AllocMemory(totallen))
		return FALSE;

	context.text = (wchar_t*)execContext.WriteProcessMemory((LPVOID)wtext, (len)*sizeof(wchar_t));
	context.col = col;
	if(!context.text)
		return FALSE;

#ifndef	UNICODE
	delete [] wtext;
#endif

	DWORD retval;
	return execContext.Execute(
		(CRemoteExecute::entry_t)CRemoteExecute::GetFuncAddress(&RemotePostD2Message),
		FUNCTLEN(RemotePostD2Message),
		&context, sizeof(context),
		retval);
}

BOOL CRevealMapWorker::PromptTombLevel(DWORD processId, HWND hwnd, DWORD flag, BYTE tomblvl)
{
	TCHAR text[100];
	static BYTE tomblvls[] = {0x45, 0x44, 0x43, 0x42, 0x46, 0x47, 0x48};
	for(int i = 0; i < COUNTOF(tomblvls); i++)
	{
		if(tomblvls[i] == tomblvl)
		{
			i += 9;
			_stprintf(text, _T("<Hackmap>: True Tal Rasha's Tomb is at %d o'clock"), i < 13 ? i : i-12);
			return PostD2Message(processId, hwnd, flag, text, eTxtColLightYellow);
		}
	}
	return FALSE;
}

BOOL CRevealMapWorker::GetGameServerInfo(DWORD processId, HWND hwnd, DWORD& gameServerId, tstring& gameServerIp)
{
	DWORD sock = 0;
	CModuleWalker walker;
	if(walker.Walk(processId))
	{
		char szGameServerIp[0x20];
		AutoHandle<HANDLE> hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
		if(hProcess && GetGameServerIp(hProcess, walker, sock, szGameServerIp, sizeof(szGameServerIp)))
		{
			if(sock == 0)
			{
				gameServerIp = _T("");
			}
			else if(sock != gameServerId)
			{
#ifdef UNICODE
				WCHAR szServerIp[MAX_PATH];
				mymbstowcs2(szServerIp, szGameServerIp);
#else
				LPCSTR szServerIp = szGameServerIp;
#endif
				gameServerIp = szServerIp;
			}
			gameServerId = sock;
			return TRUE;
		}
	}
	return FALSE;
}
