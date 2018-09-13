//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "hackmap.h"


DWORD GetDllOffset(char *dll, int offset)
{
	HMODULE hmod = GetModuleHandle(dll);
	if (!hmod)
		hmod = LoadLibrary(dll);
	if (!hmod) return 0;
	if (offset < 0) {
		return (DWORD)GetProcAddress(hmod, (LPCSTR)-offset);
	}
	return ((DWORD)hmod)+offset;
}

DWORD GetDllOffset(int num)
{
	static char *dlls[] = {"D2CLIENT.DLL", "D2COMMON.DLL", "D2GFX.DLL", "D2WIN.DLL", "D2LANG.DLL", "D2CMP.DLL", "D2MULTI.DLL", "BNCLIENT.DLL", "D2NET.DLL", "STORM.DLL", "FOG.DLL", "D2LAUNCH.DLL", "D2GAME.DLL", "D2Direct3D.dll", "D2GDI.dll"};
	return GetDllOffset(dlls[num&0xff], num>>8);
}

BOOL RelocD2Ptrs(DWORD* pPtrsStart, DWORD* pPtrsEnd)
{
	DWORD *p = pPtrsStart, ret = TRUE;
	do {
		if (!(*p = GetDllOffset(*p))) ret = FALSE;
	} while (++p <= pPtrsEnd);
	return ret;
}

DWORD RelocD2Patchs(Patch_t* pPatchStart, Patch_t* pPatchEnd)
{
	Patch_t* p = pPatchStart;

	DWORD chksum = 0;
	do {
		p->addr = GetDllOffset(p->addr);
		if (p->addr) chksum ^= ReadLocalDWORD(p->addr) & p->mask;
	} while (++p < pPatchEnd);
	return chksum;
}

BOOL SaveD2Patchs(Patch_t* pPatchStart, Patch_t* pPatchEnd)
{
	Patch_t* p = pPatchStart;
	
	do {
		p->oldcode = new BYTE[p->len];
		ReadLocalBYTES(p->addr, p->oldcode, p->len);
	} while (++p < pPatchEnd);
	return TRUE;
}

BOOL InstallD2Patchs(Patch_t* pPatchStart, Patch_t* pPatchEnd)
{
	Patch_t* p = pPatchStart;
	
	do {
		if (p->func)
			p->func(p->addr, p->param, p->len);
	} while (++p < pPatchEnd);
	return TRUE;
}

void RemoveD2Patchs(Patch_t* pPatchStart, Patch_t* pPatchEnd)
{
	Patch_t* p = pPatchStart;

	do {
		if (p->oldcode) WriteLocalBYTES(p->addr, p->oldcode, p->len);
	} while (++p < pPatchEnd);
}

void DeleteD2Patchs(Patch_t* pPatchStart, Patch_t* pPatchEnd)
{
	Patch_t* p = pPatchStart;

	do {
		delete p->oldcode;
	} while (++p < pPatchEnd);
}


extern BOOL InstallPatchs();
extern void RemovePatchs();

//main
HMODULE hInstDLL;
BOOL WINAPI DllMain(HINSTANCE hInstDLL2, DWORD dwReason, LPVOID pReserved)
{
	switch (dwReason) {
		case DLL_PROCESS_ATTACH:
			hInstDLL = hInstDLL2;
			DisableThreadLibraryCalls(hInstDLL);
			return InstallPatchs();

		case DLL_PROCESS_DETACH:
			if (!pReserved)
				RemovePatchs();
			break;
	}
	return TRUE;
}

#define D2LOADER_PLUGIN

#ifdef D2LOADER_PLUGIN


extern "C" PVOID __stdcall QueryInterface() //NOTE :- needs to be exported
{
	char filename[MAX_PATH];
	GetModuleFileName(hInstDLL, filename, MAX_PATH);
	LoadLibrary(filename);
	return NULL;
}

#endif
