Patch_t a1024X768ResolutionPatchs[] = {
	{PatchCALL,   DLLOFFSET(D2COMMON, DLLBASE_D2COMMON+0x107A5),   (DWORD)HR1024X768_FixMPQ, 5, -1},
	{PatchVALUE,   DLLOFFSET(D2COMMON, DLLBASE_D2COMMON+0x107AA),  0x000CC2, 3, -1}, 

	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x81BAA),   (DWORD)HR1024X768_FixOffset_ASM, 12, -1},


	{PatchVALUE,  DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x23ED),   0x400, 2, -1},	// 6FAA23EC
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

Patch_t aSleepyPatchs1[] = {
	{PatchVALUE,  DLLOFFSET(D2WIN, DLLBASE_D2WIN+0xD071),   0x90, 1, -1},
	{PatchVALUE,  DLLOFFSET(D2WIN, DLLBASE_D2WIN+0xD072),   0x016a9090, 4, -1},
};

Patch_t aSleepyPatchs2[] = {
	{PatchJMP,    DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x150C0),   (DWORD)MySleep_ASM, 6, -1},
};

Patch_t aKeepGameWindowPatchs[] = {
	// 0x4938: push SW_MINIMIZE; push ecx; call edi;
	{PatchFILL,   D2GFX_KeepD2WindowPatch1,   4, 1, -1}, // SW_MINIMIZE

	// 0x4917: push SW_MINIMIZE; push ecx; call edi;
	{PatchFILL,   D2GFX_KeepD2WindowPatch2,   4, 1, -1}, // SW_MINIMIZE

	// 0x4978: push SW_HIDE; push edx; call ebp;
	{PatchFILL,   D2GFX_KeepD2WindowPatch3,   4, 1, -1}, // SW_HIDE
};

Patch_t aPacketRecvPatchs[] = {
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x1511A),   (DWORD)GamePacketReceivedInterceptPatch_ASM, 9, -1},
};

Patch_t aD2Patchs2[] = {
	{PatchCALL,   DLLOFFSET(D2MULTI, DLLBASE_D2MULTI+0xF77B),   (DWORD)NextGameNamePatch, 5, -1},
	{PatchCALL,   DLLOFFSET(D2MULTI, DLLBASE_D2MULTI+0xF7B6),   (DWORD)NextGamePasswordPatch, 5, -1},
	{PatchCALL,   DLLOFFSET(D2MULTI, DLLBASE_D2MULTI+0x10567),   (DWORD)NextGameNamePatch, 5, -1},
	{PatchCALL,   DLLOFFSET(D2MULTI, DLLBASE_D2MULTI+0x105A2),   (DWORD)NextGamePasswordPatch, 5, -1},
};

Patch_t aD2LocalePatchs2[] = {
	// Locale
	{PatchCALL,   DLLOFFSET(D2MULTI, DLLBASE_D2MULTI+0x6143),   (DWORD)EnterChatPatch, 5, -1},


	// Unicode CharName Support
	{PatchVALUE,   DLLOFFSET(D2LAUNCH, DLLBASE_D2LAUNCH+0xBB93),   (DWORD)UnicodeCharNameCheck, 4, -1}, // 检查UnicodeCharName的合法性
	{PatchVALUE,   DLLOFFSET(D2LAUNCH, DLLBASE_D2LAUNCH+0x14578),   (DWORD)0x6FA31044, 4, -1}, // unicode2Win替换掉Unicode::toUtf，发送0x68登录服务器报文
	{PatchVALUE,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x62026),   (DWORD)0x00000FB8, 4, -1}, // mov eax,0Fh 替换掉call    ds:?unicodenwidth@Unicode@@SIIPBDH@Z，Party窗口中文玩家名字长度计算
	{PatchVALUE,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x6202A),   (DWORD)0xF08B9000, 4, -1}, // nop 替换掉call    ds:?unicodenwidth@Unicode@@SIIPBDH@Z，Party窗口中文玩家名字长度计算
	{PatchVALUE,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x6202F),   (DWORD)0x7610FE83, 4, -1}, // cmp     esi, 10h替换掉cmp     esi, 0Bh，Party窗口中文玩家名字长度限制
	{PatchVALUE,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x62064),   (DWORD)0x6FB6DAC8, 4, -1}, // win2Unicode替换掉Unicode::toUnicode，Party窗口显示中文玩家名字
	{PatchVALUE,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x624A9),   (DWORD)0x6FB6DAC8, 4, -1}, // win2Unicode替换掉Unicode::toUnicode
	{PatchVALUE,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x2ECA7),   (DWORD)0x6FB6DAC8, 4, -1}, // win2Unicode替换掉Unicode::toUnicode，在小地图上显示玩家名字
	{PatchVALUE,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x3ACCD),   (DWORD)0x6FB6DAC8, 4, -1}, // win2Unicode替换掉Unicode::toUnicode，在组队图标里显示玩家名字

	// Unicode GameName Support
	{PatchVALUE,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x2EF56),   (DWORD)0x6FB6DAC8, 4, -1}, // win2Unicode替换掉Unicode::toUnicode，中文游戏名
};

Patch_t aD2Patchs[] = {
	{PatchFILL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x2D1CC),   INST_NOP, 2, -1}, //floors inside
	{PatchFILL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x2D210),   INST_NOP, 2, -1}, //walls inside
	{PatchFILL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x2D270),   INST_NOP, 6, -1}, //shrine distance

	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x2CD82),   (DWORD)OverrideShrinePatch_ASM, 7, 0xffffffff},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x2D2C2),   (DWORD)AddShrinePatch_ASM, 6, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x2E4E0),   (DWORD)DrawAutomapCellPatch, 5, -1},
	{PatchJMP,    DLLOFFSET(D2COMMON, DLLBASE_D2COMMON+0x2041A),   (DWORD)WeatherPatch_ASM, 5, -1},

	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x19FE1),   (DWORD)LightingPatch_ASM, 8, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x96C0),   (DWORD)ShakeScreenPatch, 5, -1},
	{PatchFILL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x172BD),   INST_NOP, 2, 0xffffffff}, //force perspective shake
	{PatchVALUE,  DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x17EB7),   INST_JMPR, 1, -1}, //force get shake
	{PatchFILL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x17DA0),   INST_NOP, 14, -1}, //kill add shake
	{PatchFILL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x2B6A0),   INST_NOP, 0x37, 0xffffffff}, //kill automap scroll

	{PatchCALL,   DLLOFFSET(D2WIN,    DLLBASE_D2WIN+0xA865),   (DWORD)LifeBarPatch_ASM, 5, -1},

	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x3E7DC),   (DWORD)ItemNamePatch_ASM, 5, -1},
	{PatchVALUE,  DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x80844),   0xe990, 2, -1}, //kill ground gold name
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x43B90),   (DWORD)OpenCubeStringPatch, 5, -1},

	// new
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0xADBFD),   (DWORD)InfravisionPatch_ASM, 0x44, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x2EC1A),   (DWORD)HostilePlayerColor_ASM, 5, -1},
	// end

	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x2E6D6),   (DWORD)MonsterBlobNamePatch_ASM, 5, -1},
	{PatchJMP,  DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x2EB79),   (DWORD)MonsterBlobColPatch_ASM, 5, -1},
	
	// not found yet
//	{PatchCALL,   DLLOFFSET(D2CLIENT, 0x6FB4BAEC),   (DWORD)HideItemsPatch_ASM, 6, -1},
//	{PatchVALUE,  DLLOFFSET(D2CLIENT, 0x6FAC88B0),   (DWORD)MonsterBlobColPatch_ASM, 4, 0xffff},
//	{PatchVALUE,  DLLOFFSET(D2CLIENT, 0x6FAC88BC),   (DWORD)MonsterBlobColPatch_ASM, 4, 0xffff},
//	{PatchCALL,   DLLOFFSET(D2CLIENT, 0x6FAC8795),   (DWORD)MonsterDieingPatch_ASM, 5, -1},
//	{PatchCALL,   DLLOFFSET(D2CLIENT, 0x6FAC889C),   (DWORD)PlayerBlobColPatch_ASM, 6, 0xffffffff},
//	{PatchCALL,   DLLOFFSET(D2CLIENT, 0x6FAC86FA),   (DWORD)MixedBlobColPatch_ASM, 6, -1},
	// end

	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x2E6A5),   (DWORD)DrawObjectBlobPatch, 5, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x2E818),   (DWORD)DrawPlayerBlobPatch, 5, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x2E6B8),   (DWORD)DrawMonsterBlobPatch_ASM, 5, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x2E631),   (DWORD)DrawItemBlobPatch_ASM, 6, -1},
	{PatchCALL2,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x2EE2A),   (DWORD)DrawPlayerBlobPatch2_ASM, 0x3B, -1},

	// new
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x2DEC7),   (DWORD)RoomPatch_ASM, 6, -1},
	// end

	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0xA54AB),   (DWORD)MonsterDeathPatch_ASM, 5, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0xA6326),   (DWORD)MonsterDescCommaPatch1_ASM, 5, 0xffffff},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0xA63FC),   (DWORD)MonsterDescCommaPatch2_ASM, 5, 0xffffff},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x8105B),   (DWORD)MonsterDescCommaPatch3_ASM, 6, 0xffffff},
	
	// new
	{PatchCALL,   DLLOFFSET(D2WIN, DLLBASE_D2WIN+0xAC60),   (DWORD)MonsterLifebarNamePatch_ASM, 6, -1},
	{PatchCALL,   DLLOFFSET(D2WIN, DLLBASE_D2WIN+0xACD0),   (DWORD)MonsterLifebarNamePatch_ASM, 6, -1},
	// end


	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x40144),   (DWORD)ViewInventoryPatch1, 5, -1}, //draw equip items
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x40109),   (DWORD)GetViewingInventoryPlayer, 5, -1}, //draw inv items
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x482CE),   (DWORD)GetViewingInventoryPlayer, 5, -1}, //desc item
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x41EFF),   (DWORD)ViewInventoryPatch3_ASM, 5, -1}, //test has item

	
	// new added by sting
	// stats
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x2FD6A),   (DWORD)GetViewingStatsPlayer, 5, -1}, //draw inv items
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x30248),   (DWORD)GetViewingStatsPlayer, 5, -1}, //draw inv items
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x30729),   (DWORD)GetViewingStatsPlayer, 5, -1}, //draw inv items
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x30A95),   (DWORD)GetViewingStatsPlayer, 5, -1}, //draw inv items
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0xADA8E),   (DWORD)GetViewingStatsPlayer, 5, -1}, //draw inv items
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x310E7),   (DWORD)GetViewingStatsPlayer, 5, -1}, //draw inv items

	//skill
/*	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x76C07),   (DWORD)GetViewingSkillsPlayer, 5, -1}, //draw inv items
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x763B7),   (DWORD)GetViewingSkillsPlayer, 5, -1}, //draw inv items
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x76416),   (DWORD)GetViewingSkillsPlayer, 5, -1}, //draw inv items
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x76A57),   (DWORD)GetViewingSkillsPlayer, 5, -1}, //draw inv items
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x768AF),   (DWORD)GetViewingSkillsPlayer, 5, -1}, //draw inv items
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x76967),   (DWORD)GetViewingSkillsPlayer, 5, -1}, //draw inv items
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x76981),   (DWORD)GetViewingSkillsPlayer, 5, -1}, //draw inv items
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x76A08),   (DWORD)GetViewingSkillsPlayer, 5, -1}, //draw inv items

	// pet
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x4D5E8),   (DWORD)GetViewingPet, 5, -1}, //draw inv items
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x4D317),   (DWORD)GetViewingPet, 5, -1}, //draw inv items
*/	// end


	// new
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x47116),   (DWORD)ViewInventoryPatch4_ASM, 5, -1}, //test has item
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x41890),   (DWORD)SocketViewtablePatch_ASM, 5, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x4302E),   (DWORD)SocketViewtablePatch2_ASM, 5, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x51CBF),   (DWORD)ItemBasicStatPatch_ASM, 5, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0xA9920),   (DWORD)OutTownSelectPatch1_ASM, 5, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x8875A),   (DWORD)OutTownSelectPatch2_ASM, 6, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x88874),   (DWORD)OutTownSelectPatch3_ASM, 6, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x471AD),   (DWORD)SocketProtectPatch1_ASM, 6, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x47BEC),   (DWORD)SocketProtectPatch2_ASM, 6, -1},
	// end

	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x64C27),   (DWORD)CowLevelQuestPatch_ASM, 6, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x80B91),   (DWORD)PermShowItemsPatch_ASM, 6, 0xffffffff},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x820D1),   (DWORD)PermShowItemsPatch_ASM, 6, 0xffffff},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x88A27),   (DWORD)PermShowItemsPatch_ASM, 5, -1},

	// new
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x7F9B3),   (DWORD)UnknownPatch5, 5, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x7C496),   (DWORD)UnknownPatch6, 5, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x2E21F),   (DWORD)GameTimePatch, 5, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x328BC),   (DWORD)InputLinePatch_ASM, 5, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x323BF),   (DWORD)InputLinePatch2_ASM, 5, -1},
	{PatchVALUE,  DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x36316), 0x2EB58, 3, -1},
	{PatchVALUE,  DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x363D6), 0x2EB58, 3, -1},
	// end


	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x9640),   (DWORD)GameLoopPatch_ASM, 6, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0xB528),   (DWORD)GameEndPatch_ASM, 5, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x35A07),   (DWORD)KeydownPatch_ASM, 7, -1},
	{PatchVALUE,  DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x33430), (DWORD)"<KittyNet>: Correct usage is: %s", 4, 0xffff},
	{PatchVALUE,  DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x33599), (DWORD)"<KittyNet>: Correct usage is: %s (name) (message)", 4, 0xffff},
	
	// new
	{PatchJMP,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x30F8A),   (DWORD)ViewPlayerStatPatch_ASM, 6, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x5F102),   (DWORD)PermShowOrbPatch_ASM, 5, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x5F1E6),   (DWORD)PermShowOrbPatch2_ASM, 0x0B, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x5F1F1),   (DWORD)PermShowOrbPatch3_ASM, 6, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x5E6F8),   (DWORD)GameTimeClockPatch_ASM, 5, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x5F02C),   (DWORD)GameTimeClockPatch2_ASM, 6, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x7C64C),   (DWORD)MessageLogPatch1_ASM, 5, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x7C969),   (DWORD)MessageLogPatch2_ASM, 5, -1},
	// end

	// not exist
//	{PatchVALUE,  DLLOFFSET(D2CLIENT, 0x6FACC77E+1), (DWORD)"<MouseNet>: ", 4, 0xffff},
	// end


	// new features, added by sting
	{NULL,   D2CLIENT_InitAutomapLayer_END,   1, 1, -1},

	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0xB6D82),   (DWORD)PassingActBugFixPatch_ASM, 6, -1}, //draw inv items
	{PatchFILL,   DLLOFFSET(D2WIN, DLLBASE_D2WIN+0x7424), INST_JMPR, 1, -1}, // copy&paste bugfix
	// 0x33, 0x4A for detection request?

	// added by marsgod
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x81FF7),   (DWORD)DrawClientPatch_ASM, 5, -1}, // DrawClientPatch
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x12FDE),   (DWORD)UpdateExp_ASM, 5, -1}, // just got a update player state packet

	// right click swap
	// trade 6FAEABE6
//	{PatchVALUE,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0xD5844),   (DWORD)RButtonUpHandlerPatch, 4, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x8E90),   (DWORD)RButtonUpHandlerPatch_ASM, 10, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x4ABE6),   (DWORD)RButtonInTradePatch_ASM, 5, -1},
//	{PatchVALUE,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0xEEB9C),   (DWORD)LButtonDownHandlerPatch, 4, -1},

#ifndef IMPK_USE_ONLY
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x13768),   (DWORD)ULCPatch_ASM, 6, -1}, // ULCPatch
#endif
};

Patch_t aD2LocalePatchs[] = {
	{PatchFILL,   DLLOFFSET(D2WIN, DLLBASE_D2WIN+0xA0DD), INST_JMPR, 1, -1}, // in channel
	{PatchFILL,   DLLOFFSET(D2WIN, DLLBASE_D2WIN+0xA0CE), INST_NOP, 2, -1},
	{PatchCALL,   DLLOFFSET(D2WIN, DLLBASE_D2WIN+0xAD69),   (DWORD)D2WIN_DrawText2Patch, 5, -1},
	{PatchCALL,   DLLOFFSET(D2WIN, DLLBASE_D2WIN+0x9F62),   (DWORD)D2Win_InitializeFontTablePatch, 5, -1},
	{PatchCALL,   DLLOFFSET(D2WIN, DLLBASE_D2WIN+0xA04D),   (DWORD)D2Win_InitializeFontPatch, 5, -1},
	{PatchCALL,   DLLOFFSET(D2WIN, DLLBASE_D2WIN+0x9CE7),   (DWORD)D2Win_InitializeFontPatch, 5, -1},
	{PatchFILL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x7E1B4), INST_JMPR, 1, -1}, // in game
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x32995),   (DWORD)D2LocaleGameChat, 5, -1}, // in game chat
	{PatchCALL,   DLLOFFSET(D2WIN, DLLBASE_D2WIN+0xB2F0),   (DWORD)IsPritableCharacterPatch_ASM, 5, -1}, // for traditionnal chinese

	{PatchCALL,   DLLOFFSET(D2WIN, DLLBASE_D2WIN+0xDAB0),   (DWORD)ChannelEnterCharPatch, 5, -1},
//	{PatchCALL,   DLLOFFSET(D2WIN, DLLBASE_D2WIN+0xDB80),   (DWORD)ChannelEnterCharPatch2, 5, -1},
	{PatchJMP,   DLLOFFSET(D2LANG, DLLBASE_D2LANG+0x1BD0),   (DWORD)D2Lang_Win2Unicode, 5, -1},
	{PatchJMP,   DLLOFFSET(D2LANG, DLLBASE_D2LANG+0x1C20),   (DWORD)D2Lang_Unicode2Win, 5, -1},
	{PatchJMP,   DLLOFFSET(D2LANG, DLLBASE_D2LANG+0x1B30),   (DWORD)D2Lang_Utf82Unicode, 5, -1},
	{PatchCALL,   DLLOFFSET(BNCLIENT, DLLBASE_BNCLIENT+0x63B6),   (DWORD)D2MultiByteFixPatch_ASM, 6, -1}, // for /w *acc msg text

	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x7C386),   (DWORD)DrawTextInGameWithMyFont, 5, -1}, // Main InGame Screen Text
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x32450),   (DWORD)DrawTextWithMyFont, 5, -1}, // Chat Input Box
	{PatchCALL,   DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0x50170),   (DWORD)DrawTextInGameWithMyFont, 5, -1}, // Log Message List Box
	{PatchCALL,   DLLOFFSET(D2WIN, DLLBASE_D2WIN+0x114F4),   (DWORD)DrawTextInChannelWithMyFont, 5, -1}, // Town Hall Channel
	{PatchCALL,   DLLOFFSET(D2WIN, DLLBASE_D2WIN+0x8624),   (DWORD)DrawTextWithMyFont, 5, -1}, // Channel Input Box
};

#define RECVCMD_OFFSET(n) DLLOFFSET(D2CLIENT, DLLBASE_D2CLIENT+0xD6270+12*(n))
// 6FB76270 + 12*n
Patch_t aD2PacketHandlerPatchs[] = {
	//!!CAUTION: the order must not be changed.
#ifndef IMPK_USE_ONLY
	{PatchVALUE, RECVCMD_OFFSET(0x5A), (DWORD)RecvCommand_5A_Patch_ASM, 4, 0xffff},
#endif
	{PatchVALUE, RECVCMD_OFFSET(0x5D), (DWORD)RecvCommand_5D_Patch_ASM, 4, 0xffff},
	{PatchVALUE, RECVCMD_OFFSET(0x89), (DWORD)RecvCommand_89_Patch_ASM, 4, 0xffff},
};

PatchItem_t aD2PatchItems[] = {
	{aD2Patchs, ARRAYSIZE(aD2Patchs)},
	{aSleepyPatchs1, ARRAYSIZE(aSleepyPatchs1)},
	{aSleepyPatchs2, ARRAYSIZE(aSleepyPatchs2),1},
	{aD2PacketHandlerPatchs, ARRAYSIZE(aD2PacketHandlerPatchs)},
	{a1024X768ResolutionPatchs, ARRAYSIZE(a1024X768ResolutionPatchs)},
	{aKeepGameWindowPatchs, ARRAYSIZE(aKeepGameWindowPatchs), 1},
	{aPacketRecvPatchs, ARRAYSIZE(aPacketRecvPatchs), 1},
	{aD2LocalePatchs, ARRAYSIZE(aD2LocalePatchs), 1},
//	{aD2Patchs2, ARRAYSIZE(aD2Patchs2), 1},
//	{aD2LocalePatchs2, ARRAYSIZE(aD2LocalePatchs2), 1},
};

