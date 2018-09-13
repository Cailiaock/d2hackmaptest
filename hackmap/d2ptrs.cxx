typedef HMODULE (__stdcall *LoadLibraryA_t)(LPCSTR);
#define pD2PtrsListStart p_D2CLIENT_pDrlgAct


D2VARPTR(D2CLIENT, pDrlgAct, DrlgAct *, DLLBASE_D2CLIENT+0x107984)

// D2GAME ptrs by marsgod
D2FUNCPTR(D2GAME, PlayerSave, void __fastcall, (GameStructInfo* ptGame,UnitAny* ptPlayer,char* ptPlayerName,DWORD unk), DLLBASE_D2GAME+0x0005A500)

// D2COMMON ptrs by marsgod
D2FUNCPTR(D2COMMON, UpdatePlayerState, void __fastcall, (), -0x2915)
D2FUNCPTR(D2COMMON, GetMinExpToLevel, int __stdcall, (DWORD charclass, DWORD level), -10628)
D2ASMPTR(D2CLIENT, DrawClient, DLLBASE_D2CLIENT+0x5F480)
D2VARPTR(D2CLIENT, Ping, DWORD, DLLBASE_D2CLIENT+0x107774)	// 6FBA7774
D2VARPTR(D2CLIENT, FPS, DWORD, DLLBASE_D2CLIENT+0xFA96C)		// 6FB9A96C
D2FUNCPTR(D2CLIENT, ToggleAutoMap, void __fastcall, (), DLLBASE_D2CLIENT+0x33840)	// 6FAD3840
D2FUNCPTR(D2COMMON, GetACTFromLevel, DWORD __stdcall, (DWORD nLevelNo), -10001)
D2FUNCPTR(D2COMMON, GetLevelRecord, DWORD __fastcall, (DWORD nLevelNo), -10010)
D2FUNCPTR(D2COMMON, GetLevelRecord34, DWORD __fastcall, (DWORD nLevelNo), -10000)
D2FUNCPTR(D2COMMON, GetLevelRecordLOSdraw98, DWORD __fastcall, (DWORD nLevelNo), -10002)
D2FUNCPTR(D2COMMON, GetLevelRecordSaveMonsters94, DWORD __fastcall, (DWORD nLevelNo), -10004)
D2FUNCPTR(D2COMMON, CalculatePetExp, DWORD __stdcall, (DWORD EXP_LVL,DWORD PetLevel), -11084)
D2FUNCPTR(D2CLIENT, RecvCommand1D1E1F, void __fastcall, (BYTE *cmdbuf), DLLBASE_D2CLIENT+0x12F10)
D2VARPTR(D2COMMON, pBeltLocationTbl, DWORD *, DLLBASE_D2COMMON+0xA9604)
D2VARPTR(D2COMMON, pInvLocationTbl, DWORD *, DLLBASE_D2COMMON+0xAA2D8)

D2FUNCPTR(D2CLIENT, RecvCommand07, void __fastcall, (BYTE *cmdbuf), DLLBASE_D2CLIENT+0x122A0)
D2FUNCPTR(D2CLIENT, RecvCommand08, void __fastcall, (BYTE *cmdbuf), DLLBASE_D2CLIENT+0x12310)
D2FUNCPTR(D2COMMON, InitDrlgLevel, void __stdcall, (DrlgLevel *drlglevel), -0x2716)
D2FUNCPTR(D2COMMON, GetDrlgLevel, DrlgLevel * __stdcall, (DrlgMisc *drlgmisc, DWORD levelno), -0x2715)
D2FUNCPTR(D2CLIENT, RevealAutomapRoom, void __fastcall, (DrlgRoom1 *room1, DWORD clipflag, AutomapLayer *layer), DLLBASE_D2CLIENT+0x2D180)
D2VARPTR(D2CLIENT, pAutomapCellsHeader, AutomapCellNode *, DLLBASE_D2CLIENT+0x1119A0)
D2VARPTR(D2CLIENT, pAutomapLayer, AutomapLayer *, DLLBASE_D2CLIENT+0x1119A4)
D2VARPTR(D2CLIENT, fAutomapOn, DWORD, DLLBASE_D2CLIENT+0x0011A6D0)
D2VARPTR(D2CLIENT, ptAutomap, POINT, DLLBASE_D2CLIENT+0x001119C8)
D2VARPTR(D2CLIENT, ptOffset, POINT, DLLBASE_D2CLIENT+0x001119D8) // need to confirm
D2VARPTR(D2CLIENT, nPtDivisor, int, DLLBASE_D2CLIENT+0x000D7BC0)
D2FUNCPTR(D2CLIENT, GetAutomapSize, DWORD __stdcall, (), DLLBASE_D2CLIENT+0x2F820)
D2FUNCPTR(D2CLIENT, NewAutomapCell, AutomapCell * __fastcall, (), DLLBASE_D2CLIENT+0x0002BA40)
D2FUNCPTR(D2CLIENT, AddAutomapCell, void __fastcall, (AutomapCell *cell, AutomapCell **node), DLLBASE_D2CLIENT+0x0002CD50)
D2FUNCPTR(D2CLIENT, SetAutomapParty, void __fastcall, (DWORD flag), DLLBASE_D2CLIENT+0x0002B9F0)
D2FUNCPTR(D2CLIENT, SetAutomapNames, void __fastcall, (DWORD flag), DLLBASE_D2CLIENT+0x0002BA20)
D2VARPTR(D2COMMON, nWeaponsTxt, int, DLLBASE_D2COMMON+0x000AA2EC)
D2VARPTR(D2COMMON, nArmorTxt, int, DLLBASE_D2COMMON+0x000AA2F4)
D2VARPTR(D2COMMON, nUnknownVar_1, int, DLLBASE_D2COMMON+0x000AA2FC)
D2FUNCPTR(D2COMMON, GetObjectTxt, ObjectTxt * __stdcall, (DWORD objno), -0x2982)
D2FUNCPTR(D2COMMON, GetItemTxt, ItemTxt * __stdcall, (DWORD itemno), -0x2968)
D2FUNCPTR(D2COMMON, GetLevelTxt, LevelTxt * __stdcall, (DWORD levelno), -0x2987)
D2FUNCPTR(D2CLIENT, GetMonsterTxt, MonsterTxt * __fastcall, (DWORD monno), DLLBASE_D2CLIENT+0xE570)
D2FUNCPTR(D2COMMON, GetSuperuniqueTxt, SuperuniqueTxt * __stdcall, (DWORD monno), -0x29AC)
D2FUNCPTR(D2COMMON, GetTreasureClasses, TreasureClass * __stdcall, (DWORD tcno, DWORD lvlno), -10659)
D2FUNCPTR(D2COMMON, GetFunUnk_5, DWORD __stdcall, (DWORD nLevelNo), -10002)
D2FUNCPTR(D2CLIENT, GetPlayerUnit, UnitAny * __stdcall, (), DLLBASE_D2CLIENT+0x883D0)
D2FUNCPTR(D2COMMON, GetUnitStat, int __stdcall, (UnitAny *unit, DWORD statno, DWORD unk), -0x2917)
D2FUNCPTR(D2COMMON, GetUnitState, int __stdcall, (UnitAny *unit, DWORD stateno), -0x28F7)
D2FUNCPTR(D2COMMON, GetItemFlag, DWORD __stdcall, (UnitAny *item, DWORD flagmask, DWORD lineno, char *filename), -0x29D3)
D2FUNCPTR(D2CLIENT, TestPvpFlag, DWORD __fastcall, (DWORD planum1, DWORD planum2, DWORD flagmask), DLLBASE_D2CLIENT+0xAB900)
D2FUNCPTR(D2CLIENT, GetMonsterOwner, DWORD __fastcall, (DWORD monnum), DLLBASE_D2CLIENT+0x11B00)
D2FUNCPTR(D2CLIENT, GetUnitFromId, UnitAny * __fastcall, (DWORD unitid, DWORD unittype), DLLBASE_D2CLIENT+0x869F0)
D2FUNCPTR(D2CLIENT, GetNextPlayer, UnitAny * __fastcall, (UnitAny* pla), DLLBASE_D2CLIENT+0xF5B0)
D2FUNCPTR(D2CLIENT, GetInventoryId, DWORD __fastcall, (UnitAny* pla, DWORD unitno, DWORD arg3), DLLBASE_D2CLIENT+0x11920)
D2FUNCPTR(D2CLIENT, GetUnitNoFromId, DWORD __fastcall, (DWORD unitid), DLLBASE_D2CLIENT+0x11B20)
D2FUNCPTR(D2COMMON, TestFunUnk_6, DWORD __stdcall, (UnitAny *unit1, UnitAny *unit2, DWORD arg3), -0x287A)
D2FUNCPTR(D2COMMON, GetMonsterColorIndex, int __stdcall, (UnitAny *mon, int no), -0x2B09)
D2FUNCPTR(D2CLIENT, SocketProtect_ORIG, void __fastcall, (UnitAny *unit, DWORD arg2), DLLBASE_D2CLIENT+0xB14E0)
//D2ASMPTR(D2CLIENT, SocketProtect_ORIG, DLLBASE_D2CLIENT+0xB14E0)
D2FUNCPTR(D2GFX, DrawAutomapCell, void __stdcall, (CellContext *context, DWORD xpos, DWORD ypos, RECT *cliprect, DWORD bright), -0x275D)
D2FUNCPTR(D2GFX, DrawAutomapCell2, void __stdcall, (CellContext *context, DWORD xpos, DWORD ypos, DWORD bright2, DWORD bright, BYTE *coltab), -0x2758)
D2FUNCPTR(D2CMP, InitCellFile, void __stdcall, (void *cellfile, CellFile **outptr, char *srcfile, DWORD lineno, DWORD filever, char *filename), -0x2728)
D2FUNCPTR(D2CMP, DeleteCellFile, void __stdcall, (CellFile *cellfile), -0x2730)
D2FUNCPTR(D2WIN, SetTextSize, DWORD __fastcall, (DWORD size), -0x278F)
D2FUNCPTR(D2WIN, GetTextWidthFileNo, DWORD __fastcall, (wchar_t *str, DWORD* width, DWORD* fileno), -0x2793)
D2FUNCPTR(D2WIN, DrawText, void __fastcall, (wchar_t *str, int xpos, int ypos, DWORD col, DWORD unknown), -0x2785)
D2FUNCPTR(D2GFX, DrawLine, void __stdcall, (int x1, int y1, int x2, int y2, DWORD col, DWORD unknown), -0x2749)
D2FUNCPTR(D2GFX, GetScreenSize, DWORD __stdcall, (), -0x2715)
D2FUNCPTR(D2GFX, GetHwnd, HWND __stdcall, (), -0x272B)
D2VARPTR(D2CLIENT, xMapShake, int, DLLBASE_D2CLIENT+0x10F8C8)
D2VARPTR(D2CLIENT, yMapShake, int, DLLBASE_D2CLIENT+0x10C20C)
D2VARPTR(D2CLIENT, yPosition, int, DLLBASE_D2CLIENT+0x1119FC)
D2FUNCPTR(D2CLIENT, GetPlayerXOffest, int __stdcall, (), DLLBASE_D2CLIENT+0x15890)
D2FUNCPTR(D2CLIENT, GetPlayerYOffset, int __stdcall, (), DLLBASE_D2CLIENT+0x15880)
D2FUNCPTR(D2CLIENT, SetExitApp, void __fastcall, (DWORD flag), DLLBASE_D2CLIENT+0x2040)
D2FUNCPTR(D2CLIENT, GetUiVar, DWORD __fastcall, (DWORD varno), DLLBASE_D2CLIENT+0x83230)
D2FUNCPTR(D2CLIENT, SetUiVar, DWORD __fastcall, (DWORD varno, DWORD howset, DWORD unknown1), DLLBASE_D2CLIENT+0x83260)
D2FUNCPTR(D2CLIENT, PrintGameStringAtTopLeft, void __fastcall, (wchar_t* text, int arg2), DLLBASE_D2CLIENT+0x7C600)
D2FUNCPTR(D2CLIENT, PrintGameStringAtBottomLeft, void __fastcall, (wchar_t* text, int arg2), DLLBASE_D2CLIENT+0x7C950)
D2VARPTR(D2CLIENT, nQuestPage, int, DLLBASE_D2CLIENT+0x116F15)
D2FUNCPTR(D2CLIENT, GetSelectedUnit, UnitAny * __stdcall, (), DLLBASE_D2CLIENT+0x15A20)
D2VARPTR(D2CLIENT, pInSocketablePatch, UnitAny *, DLLBASE_D2CLIENT+0x1158F0)
D2VARPTR(D2CLIENT, pMonsterNameLifePatch1, UnitAny, DLLBASE_D2CLIENT+0xD9290)
D2VARPTR(D2CLIENT, pMonsterNameLifePatch2, UnitAny, DLLBASE_D2CLIENT+0xD9298)
D2FUNCPTR(D2LANG, GetLocaleText, wchar_t* __fastcall, (WORD nLocaleTxtNo), -10004)
D2FUNCPTR(D2WIN, CreateEditBox, D2EditBox* __fastcall, (DWORD style, int ypos, int xpos, DWORD arg4, DWORD arg5, DWORD arg6, DWORD arg7, DWORD arg8, DWORD arg9, DWORD size, void* buf), -10070)
D2FUNCPTR(D2WIN, DestroyEditBox, DWORD __fastcall, (D2EditBox* box), -10071)
D2FUNCPTR(D2WIN, GetEditBoxText, wchar_t* __fastcall, (D2EditBox* box), -10077)
D2FUNCPTR(D2WIN, AddEditBoxChar, DWORD __fastcall, (D2EditBox* box, BYTE keycode), -10074)
D2FUNCPTR(D2WIN, SetEditBoxText, void* __fastcall, (D2EditBox* box, wchar_t* txt), -10075)
D2FUNCPTR(D2WIN, SetEditBoxProc, void __fastcall, (D2EditBox* box, BOOL (__stdcall *FunCallBack)(D2EditBox*,DWORD,DWORD)), -10072)
D2FUNCPTR(D2WIN, SelectEditBoxText, void __fastcall, (D2EditBox* box), -10082)
D2VARPTR(D2WIN, pFocusedControl, D2EditBox*, DLLBASE_D2WIN+0x5E25C)
D2VARPTR(D2CLIENT, szLastChatMessage, wchar_t, DLLBASE_D2CLIENT+0x111A58)
D2VARPTR(D2CLIENT, nTextLength, int, DLLBASE_D2CLIENT+0x112D04)
D2FUNCPTR(D2CLIENT, GetDifficulty, BYTE __stdcall, (), DLLBASE_D2CLIENT+0xC090)
D2FUNCPTR(D2CLIENT, GetQuestInfo, void* __stdcall, (), DLLBASE_D2CLIENT+0x5BD20)
D2FUNCPTR(D2CLIENT, GetGameQuestInfo, void* __stdcall, (), DLLBASE_D2CLIENT+0x5BD30)
D2FUNCPTR(D2COMMON, GetQuestFlag, int __stdcall, (void *questinfo, DWORD quest, DWORD flag), -0x2B63)
D2FUNCPTR(D2COMMON, GetItemFromInventory, DWORD __stdcall, (UnitInventory* inv), -0x2825)
D2FUNCPTR(D2COMMON, GetNextItemFromInventory, DWORD __stdcall, (DWORD no), -0x2840)
D2FUNCPTR(D2COMMON, GetUnitFromItem, UnitAny * __stdcall, (DWORD arg1), -0x2841)
D2FUNCPTR(D2COMMON, GetCursorItem, UnitAny * __stdcall, (UnitInventory * ptInventory), -0x2816)
D2FUNCPTR(D2COMMON, GetItemValue, int __stdcall, (UnitAny * player, UnitAny * item, DWORD difficulty, void* questinfo, int value, DWORD flag), -0x2A17)
D2FUNCPTR(D2CLIENT, CalcShake, void __fastcall, (DWORD *xpos, DWORD *ypos), DLLBASE_D2CLIENT+0x17C50)
D2ASMPTR(D2CLIENT, InitAutomapLayer, DLLBASE_D2CLIENT+0x2BBC4)
D2ASMPTR(D2CLIENT, InitAutomapLayer_END, DLLBASE_D2CLIENT+0x2BC8A)
D2ASMPTR(D2CLIENT, GetPlayerStat, DLLBASE_D2CLIENT+0x309EA)
D2ASMPTR(D2CLIENT, GetPlayerStat_END, DLLBASE_D2CLIENT+0x30E8C)
D2ASMPTR(D2CLIENT, GetPlayerStatJmpTbl, DLLBASE_D2CLIENT+0x30A36)
D2ASMPTR(D2CLIENT, GetPlayerStatJmpTbl2, DLLBASE_D2CLIENT+0x30B87)
D2ASMPTR(D2CLIENT, OverrideShrinePatch_ORIG, DLLBASE_D2CLIENT+0x10F990)
D2ASMPTR(D2COMMON, GetLevelIdFromRoom, -0x2749)
D2FUNCPTR(D2NET, SendPacket, void __stdcall, (DWORD arg1, BYTE* buf, size_t len), -10005)
D2FUNCPTR(D2CLIENT, GetGameInfo, GameStructInfo* __stdcall, (), DLLBASE_D2CLIENT+0x1F70)
//D2FUNCPTR(D2CLIENT, RightClickInventoryItem, int __fastcall, (UnitAny * pla, UnitInventory * i, int x, int y, int invlocation), DLLBASE_D2CLIENT+0x48C60); 
//D2FUNCPTR(D2CLIENT, LeftClickInventoryItem, int __fastcall, (UnitAny * pla, UnitInventory * i, int x, int y, int clicktype, DWORD address, int invlocation), DLLBASE_D2CLIENT+0x475C0);
//D2FUNCPTR(D2CLIENT, LButtonDownHandler, void __stdcall, (D2MSG * ptMsg), DLLBASE_D2CLIENT+0x4BAD0);
D2FUNCPTR(D2CLIENT, RButtonUpHandler, void __stdcall, (D2MSG * ptMsg), DLLBASE_D2CLIENT+0x8E90);
//D2FUNCPTR(D2CLIENT, MouseClick, int __fastcall, (int ClickType, unsigned short MouseX, unsigned short MouseY, BYTE ClickFlag), DLLBASE_D2CLIENT+0xA9920);
D2FUNCPTR(D2CLIENT, GetUnknownFlag, DWORD __fastcall, (), DLLBASE_D2CLIENT+0xC080);
D2FUNCPTR(D2COMMON, 10409, DWORD __stdcall, (UnitAny * pla, int invlocation, DWORD unk), -10409);
D2FUNCPTR(D2COMMON, 10247, int __stdcall, (UnitInventory* inventory, UnitAny *item, DWORD u3, DWORD u4, DWORD u5, int* u6, int* ItemCount, int invlocation), -10247);
D2VARPTR(D2CLIENT, UnkU3, DWORD, DLLBASE_D2CLIENT+0xD92A0);
D2VARPTR(D2CLIENT, UnkU4, DWORD, DLLBASE_D2CLIENT+0xD92A4);
D2VARPTR(D2CLIENT, UnkU5, DWORD, DLLBASE_D2CLIENT+0xD5810);

// keep window
D2ASMPTR(D2GFX, KeepD2WindowPatch1, DLLBASE_D2GFX+0x4939)
D2ASMPTR(D2GFX, KeepD2WindowPatch2, DLLBASE_D2GFX+0x4D18)
D2ASMPTR(D2GFX, KeepD2WindowPatch3, DLLBASE_D2GFX+0x4F79)
D2FUNCPTR(D2CLIENT, GetGetItemFromListEntry, DWORD __stdcall, (), DLLBASE_D2CLIENT+0xCBE3E)
D2VARPTR(BNCLIENT, fnLoadLibraryA, LoadLibraryA_t, DLLBASE_BNCLIENT+0x160C4)

// Locale
D2FUNCPTR(STORM, OpenArchive, BOOL __stdcall, (LPCSTR filename, DWORD flag1, DWORD flag2, PHANDLE phArchive), -0x10A)
D2FUNCPTR(STORM, CloseArchive, void __stdcall, (HANDLE phArchive), -0xFC)
D2FUNCPTR(D2WIN, ORD_2743, DWORD __fastcall, (DWORD u1, LPCWSTR lpText, DWORD u2, DWORD u3, DWORD u4), -0x2743)
D2FUNCPTR(D2WIN, ORD_27D7, DWORD __fastcall, (LPCWSTR lpText, DWORD u1, DWORD u2), -0x27D7)
D2FUNCPTR(D2WIN, DrawText2, DWORD __fastcall, (LPWSTR lpText, int x, int y, DWORD u1, int nColor, DWORD u2), DLLBASE_D2WIN+0xA9E0)
D2FUNCPTR(D2WIN, InitializeFontTable, DWORD __fastcall, (DWORD u1, LPSTR lpText, DWORD u2, LPCSTR file, DWORD line), DLLBASE_D2WIN+0x122F8)
D2FUNCPTR(D2WIN, InitializeFont, DWORD __fastcall, (DWORD u1, LPSTR lpText, DWORD dwSize), DLLBASE_D2WIN+0x9DC0)
D2FUNCPTR(D2WIN, 6F8AB680_Func, void __fastcall, (D2EditBox *hWnd), DLLBASE_D2WIN+0x98A0)
D2FUNCPTR(D2WIN, Patch5_Func, DWORD __fastcall, (D2EditBox *hWnd, DWORD bKeyCode), DLLBASE_D2WIN+0x7AB0)
D2FUNCPTR(D2WIN, CreateFont, DWORD __fastcall, (LPCSTR lpFontPath, DWORD u1), -0x2737)
D2FUNCPTR(D2WIN, FreeFontHandle, void __fastcall, (DWORD handle), -0x2739)
D2VARPTR(D2CLIENT, InputCount, DWORD, DLLBASE_D2CLIENT+0x112D04)
D2VARPTR(BNCLIENT, ChatMessage, LPDWORD, DLLBASE_BNCLIENT+0x1D8F0)
D2VARPTR(D2WIN, FontTable, FontInfo_t, DLLBASE_D2WIN+0x5D8C0);
D2VARPTR(D2WIN, CurrentFont, DWORD, DLLBASE_D2WIN+0x1DB24);
D2FUNCPTR(FOG, FreePoolMemory, DWORD __fastcall, (DWORD handle, LPCSTR lpFile, DWORD flag, DWORD u1), -0x273B);


D2ASMPTR(D2CLIENT, D2EndOfPtr, 0xeeeeeeee)


/*
// exist but not use
D2FUNCPTR(D2COMMON, GetObjgroupTxt, ObjgroupTxt * __stdcall, (DWORD objno), -0x2983)
// end

// not found yet
//D2FUNCPTR(D2WIN, GetTextWidth, int __fastcall, (wchar_t *str), 0x6F8AC7C0)
// end

// not exist in 1.10
D2ASMPTR(D2CLIENT, DrawItemBlobPatch_ORIG, 0x6FAC84B9)
// end

// replace by other stuff
D2FUNCPTR(D2COMMON, GetMonsterTxt, MonsterTxt * __stdcall, (DWORD monno), -0x2956)
D2VARPTR(D2COMMON, aTreasureClasses, TreasureClass *, 0x6FDE1C14)
// end
*/

#define pD2PtrsListEnd D2CLIENT_D2EndOfPtr

#define D2CLIENT_pDrlgAct (*p_D2CLIENT_pDrlgAct)

#define D2CLIENT_pAutomapLayer (*p_D2CLIENT_pAutomapLayer)
#define D2CLIENT_pAutomapCellsHeader (*p_D2CLIENT_pAutomapCellsHeader)
#define D2CLIENT_fAutomapOn (*p_D2CLIENT_fAutomapOn)
#define D2CLIENT_ptAutomap (*p_D2CLIENT_ptAutomap)
#define D2CLIENT_ptOffset (*p_D2CLIENT_ptOffset)

#define D2CLIENT_xMapShake (*p_D2CLIENT_xMapShake)
#define D2CLIENT_yMapShake (*p_D2CLIENT_yMapShake)

#define D2COMMON_nWeaponsTxt (*p_D2COMMON_nWeaponsTxt)
#define D2COMMON_nArmorTxt (*p_D2COMMON_nArmorTxt)

#define D2CLIENT_nQuestPage (*p_D2CLIENT_nQuestPage)
#define D2CLIENT_Ping (*p_D2CLIENT_Ping)
#define D2CLIENT_FPS (*p_D2CLIENT_FPS)
#define D2COMMON_pBeltLocationTbl (*p_D2COMMON_pBeltLocationTbl)
#define D2COMMON_pInvLocationTbl (*p_D2COMMON_pInvLocationTbl)

#define D2COMMON_aTreasureClasses (*p_D2COMMON_aTreasureClasses)
#define D2CLIENT_nTextLength (*p_D2CLIENT_nTextLength)
#define D2CLIENT_yPosition (*p_D2CLIENT_yPosition)
#define D2CLIENT_pInSocketablePatch (*p_D2CLIENT_pInSocketablePatch)
#define D2CLIENT_nPtDivisor (*p_D2CLIENT_nPtDivisor)
#define D2WIN_pFocusedControl (*p_D2WIN_pFocusedControl)
#define D2NET_Socket (*p_D2NET_Socket)
#define D2WIN_FontTable (p_D2WIN_FontTable)
#define D2WIN_CurrentFont (*p_D2WIN_CurrentFont)
