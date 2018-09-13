//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "memhelper.h"

//memory helpers

DWORD VirtualProtect(DWORD pAddress, DWORD len, DWORD prot)
{
	DWORD oldprot = 0;
	VirtualProtect((void *)pAddress, len, prot, &oldprot);
	return oldprot;
}

void ReadLocalBYTES(DWORD pAddress, void *buf, int len)
{
	ReadProcessMemory(GetCurrentProcess(), (void *)pAddress, buf, len, 0);
}

void WriteLocalBYTES(DWORD pAddress, void *buf, int len)
{
	DWORD oldprot = VirtualProtect(pAddress, len, PAGE_EXECUTE_READWRITE);
	WriteProcessMemory(GetCurrentProcess(), (void *)pAddress, buf, len, 0);
	VirtualProtect(pAddress, len, oldprot);
}

void FillLocalBYTES(DWORD pAddress, BYTE ch, int len)
{
	BYTE *buf1 = new BYTE[len];
	memset(buf1, ch, len);
	WriteLocalBYTES(pAddress, buf1, len);
	delete buf1;
}

DWORD ReadLocalDWORD(DWORD pAddress)
{
	DWORD val = 0;
	ReadLocalBYTES(pAddress, &val, 4);
	return val;
}

void InterceptLocalCode2(BYTE inst, DWORD pOldCode, DWORD pNewCode, int len)
{
	BYTE *buf1 = new BYTE[len];
	buf1[0] = inst;
	*(DWORD *)(buf1+1) = pNewCode;
	memset(buf1+5, INST_NOP, len-5);
	WriteLocalBYTES(pOldCode, buf1, len);
	delete buf1;
}

void InterceptLocalCode(BYTE inst, DWORD pOldCode, DWORD pNewCode, int len)
{
	pNewCode -= (pOldCode+5);
	InterceptLocalCode2(inst, pOldCode, pNewCode, len);
}


//patchs stuff

void PatchCALL(DWORD addr, DWORD param, DWORD len)
{
	InterceptLocalCode(INST_CALL, addr, param, len);
}

void PatchCALL2(DWORD addr, DWORD param, DWORD len)
{
	InterceptLocalCode(INST_CALL, addr, param, len);
	if (len > 7)
	{
		BYTE buf[] = {INST_JMPR, (BYTE)(len-7)};
		WriteLocalBYTES(addr+5, buf, sizeof(buf));
	}
}

void PatchJMP(DWORD addr, DWORD param, DWORD len)
{
	InterceptLocalCode(INST_JMP, addr, param, len);
}

void PatchFILL(DWORD addr, DWORD param, DWORD len)
{
	FillLocalBYTES(addr, (BYTE)param, len);
}

void PatchVALUE(DWORD addr, DWORD param, DWORD len)
{
	WriteLocalBYTES(addr, &param, len);
}

void PatchDATA(DWORD addr, DWORD param, DWORD len)
{
	WriteLocalBYTES(addr, (void*)param, len);
}
