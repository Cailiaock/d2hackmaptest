#ifndef MEMORY_HELPER_H
#define MEMORY_HELPER_H

//memory helpers

#define INST_NOP 0x90
#define INST_CALL 0xe8
#define INST_JMP 0xe9
#define INST_JMPR 0xeb
#define INST_RET 0xc3

#define DB(x) __asm _emit (x)
#define DW(x) __asm _emit (x)&0xff __asm _emit (x)>>8&0xff
#define DD(x) __asm _emit (x)&0xff __asm _emit (x)>>8&0xff __asm _emit (x)>>16&0xff __asm _emit (x)>>24&0xff

DWORD VirtualProtect(DWORD pAddress, DWORD len, DWORD prot);
void ReadLocalBYTES(DWORD pAddress, void *buf, int len);
void WriteLocalBYTES(DWORD pAddress, void *buf, int len);
void FillLocalBYTES(DWORD pAddress, BYTE ch, int len);
DWORD ReadLocalDWORD(DWORD pAddress);
void InterceptLocalCode2(BYTE inst, DWORD pOldCode, DWORD pNewCode, int len = 5);
void InterceptLocalCode(BYTE inst, DWORD pOldCode, DWORD pNewCode, int len = 5);

//patchs stuff
void PatchCALL(DWORD addr, DWORD param, DWORD len);
void PatchCALL2(DWORD addr, DWORD param, DWORD len);
void PatchJMP(DWORD addr, DWORD param, DWORD len);
void PatchFILL(DWORD addr, DWORD param, DWORD len);
void PatchVALUE(DWORD addr, DWORD param, DWORD len);
void PatchDATA(DWORD addr, DWORD param, DWORD len);

#endif // MEMORY_HELPER_H
