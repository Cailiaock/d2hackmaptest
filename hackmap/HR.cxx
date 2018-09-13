d2ptrs.cxx:
D2VARPTR(D2COMMON, pBeltLocationTbl, DWORD *, DLLBASE_D2COMMON+0xA9604)
D2VARPTR(D2COMMON, pInvLocationTbl, DWORD *, DLLBASE_D2COMMON+0xAA2D8)

d2patchs.cxx:
Patch_t a1024X768ResolutionPatchs[] = {
	{PatchCALL,   DLLOFFSET(D2COMMON, DLLBASE_D2COMMON+0x107A5),   (DWORD)HR1024X768_FixMPQ, 5, -1},
	{PatchVALUE,   DLLOFFSET(D2COMMON, DLLBASE_D2COMMON+0x107AA),  0x000CC2, 3, -1}, 

	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x81BAA),   (DWORD)HR1024X768_FixOffset_ASM, 12, -1},


	{PatchVALUE,  DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x23ED),   0x0400, 2, -1},	// 6FAA23EC
	{PatchVALUE,  DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x23F7),   0x0300, 2, -1}, 
	
	// 修改D2Direct3D，使在全屏模式下显示正确的1024X768
	{PatchVALUE,  DLLOFFSET(D2Direct3D, DLLBASE_D2Direct3D+0x205D),   0x0400, 2, -1}, 
	{PatchVALUE,  DLLOFFSET(D2Direct3D, DLLBASE_D2Direct3D+0x2067),   0x0300, 2, -1}, 
	{PatchVALUE,  DLLOFFSET(D2Direct3D, DLLBASE_D2Direct3D+0x2CDD),   0x0400, 2, -1}, 
	{PatchVALUE,  DLLOFFSET(D2Direct3D, DLLBASE_D2Direct3D+0x2CE7),   0x0300, 2, -1}, 
	{PatchVALUE,  DLLOFFSET(D2Direct3D, DLLBASE_D2Direct3D+0x5370),   0x0400, 2, -1}, 

	{PatchVALUE,  DLLOFFSET(D2GDI, DLLBASE_D2GDI+0x118F),   0x0400, 2, -1}, // 6F831189
	{PatchVALUE,  DLLOFFSET(D2GDI, DLLBASE_D2GDI+0x1199),   0x0300, 2, -1},
	{PatchVALUE,  DLLOFFSET(D2GDI, DLLBASE_D2GDI+0x25EC),   0x0400, 2, -1}, // 6F8325E6
	
	{PatchVALUE,  DLLOFFSET(D2GFX, DLLBASE_D2GFX+0x4536),   0x0400, 2, -1},	// 6FA74532
	{PatchVALUE,  DLLOFFSET(D2GFX, DLLBASE_D2GFX+0x453B),   0x0300, 2, -1},  
	{PatchVALUE,  DLLOFFSET(D2GFX, DLLBASE_D2GFX+0x4B61),   0x0400, 2, -1}, // 6FA74B5D
	{PatchVALUE,  DLLOFFSET(D2GFX, DLLBASE_D2GFX+0x4B66),   0x0300, 2, -1}, 
};


hackmap.cpp:
/*
高分辨率支持640X480->1024X768
*/


DWORD HR1024X768_FixMPQ()
{
	// 修正1024X768下腰带中物品的位置，参考belts.txt，修正第一部分（用于640X480分辨率）
	DWORD *BeltTbl = D2COMMON_pBeltLocationTbl;
	DWORD *p;
	int i,j;
	
	p = BeltTbl;
	for (i=0;i<7;i++)	// 一共需要修正7条记录
	{
		p++;p++;	// 跳过最前面的两个DWORD
		for (j=0;j<16;j++)
		{
			if (*p != 0) (*p) = (*p) + 194;
			p++;
			if (*p != 0) (*p) = (*p) + 194;
			p++;
			if (*p != 0) (*p) = (*p) + 285;
			p++;
			if (*p != 0) (*p) = (*p) + 285;
			p++;
		}
	}
	
	// 修正1024X768下身上物品的位置，参考invertory.txt，修正第一部分（用于640X480分辨率）
	DWORD *InvTbl = D2COMMON_pInvLocationTbl;
	
	p = InvTbl;
	for (i=0;i<16;i++)	// 一共需要修正16条记录
	{
		for (j=0;j<12;j++)
		{
			if (*p != -1 && i<5) (*p) = (*p) + 384;
			p++;
			if (*p != -1 && i<5) (*p) = (*p) + 384;
			p++;
			if (*p != -1) (*p) = (*p) + 143;
			p++;
			if (*p != -1) (*p) = (*p) + 143;
			p++;
			p++;
		}
	}
	return 0;
}

void __declspec(naked) HR1024X768_FixOffset_ASM()
{
	__asm {
		push eax
		mov eax,0x6FBBA748	// XOffset
		mov dword ptr [eax],0
		mov eax,0x6FBBA74C	// YOffset
		mov dword ptr [eax],-145
		pop eax
		ret
	}
}
