//config stuff

ConfigVar aConfigVars[] = {
	{"LowCPUUsageToggle", 0,      &tToggleSleepy, 1, 2, 1},
	{"ShowHighestResistToggle", 0,      &tToggleTrueResist, 1, 2, 1},
	{"AreaLevelToggle", 0,        &tToggleAreaLevel, 1, 2, 1},
	{"ULCMaskToggle", 0,        &tToggleULCMask, 1, 2, 1},
	{"ShowPacketToggle", 0,        &tToggleShowPacket, 1, 2, 1},
	{"BugKMToggle", 0,        &tToggleBugKM, 1, 2, 1},
	{"BugKDToggle", 0,        &tToggleBugKD, 1, 2, 1},
	{"BugKBToggle", 0,        &tToggleBugKB, 1, 2, 1},
	
	{"AutoMapToggle", 0,        &tToggleAutoMap, 1, 2, 1},
	{"AutoPartyToggle", 0,        &tToggleAutoParty, 1, 2, 1},
	{"ShowPingToggle", 0,        &tToggleShowPing, 1, 2, 1},
	{"ShowExpToggle", 0,        &tToggleShowExp, 1, 2, 1},
	{"ShowPETExpToggle", 0,        &tToggleShowPetExp, 1, 2, 1},
	{"RevealActAutomapKey", 0,          &vkRevealAct, 1, 1, 1},
	{"RevealLevelAutomapKey", 0,        &vkRevealLevel, 1, 1, 1},
	{"CenterMapScrollKey", 0,           &vkCenterMap, 1, 1, 1},
	{"QuickExitGameKey", 0,             &vkExitGame, 1, 1, 1},
	{"QuickNextGameKey", 0,             &vkQuickNextGame, 1, 1, 1},
	{"QuickBackToTownKey", 0,				&vkBackToTown, 1, 1, 1},
	{"ViewEquipmentKey", 0,             &viewingInventory.vkKeyCode, 1, 1, 1},

	// new, added by sting
	{"FirstPlayerStatKey", 0,			&vkFirstPlayerStat, 1, 1, 1},
	{"NextPlayerStatKey", 0,			&vkNextPlayerStat, 1, 1, 1},
	{"PreviousPlayerStatKey", 0,		&vkPrevPlayerStat, 1, 1, 1},
	{"ViewPlayerStatsKey", 0,			&viewingStats.vkKeyCode, 1, 1, 1},
//	{"ViewPlayerSkillsKey", 0,			&viewingSkills.vkKeyCode, 1, 1, 1},
//	{"ViewPlayerPetKey", 0,				&viewingPet.vkKeyCode, 1, 1, 1},
//	{"ViewPlayerQuestInfoKey", 0,		&viewingQuestInfo.vkKeyCode, 1, 1, 1},
	{"ExtraworkDllAction", 0,			&fExtraworkDllAction, 1, 1, 1},
	{"VersionCheckingDllAction", 0,			&fVersionCheckingDllAction, 1, 1, 1},
	{"AutoNextGameName", 0,				&fAutoNextGameName, 1, 1, 1},
	{"AutoNextGamePassword", 0,				&fAutoNextGamePassword, 1, 1, 1},
	{"AutoRevealAct", 0,				&fAutoRevealAct, 1, 1, 1},
	{"LogInGameMessage", 0,				&fLogInGameMessage, 1, 1, 1},
	{"MessageLogToggle", 0,				&fLogInGameMessage, 1, 1, 1},
	{"LocaleMPQ", 0,					&szLocaleMPQ, 0, 1, 1},
	{"LocalizationSupport", 0,				&fLocalizationSupport, 1, 1, 1},
	{"RightClickSwapToggle", 0,				&tToggleRightClickSwap, 1, 2, 1},
	{"UseCustomFontToggle", 0,				&tToggleUseCustomFont, 1, 2, 1},
	{"LayerLevelNoToggle", 0,				&tToggleLayerLevelNo, 1, 2, 1},
	// end
	
	{"WeatherToggle", "FullVisualsToggle", &tToggleFullVisuals, 1, 2, 1},
	// new
	{"InfravisionToggle", "FullVisualsToggle", &tToggleInfravision, 1, 2, 1},
	{"LightRadiuToggle", "FullVisualsToggle", &tToggleLightRadiu, 1, 2, 1},
	{"ScreenShakeToggle", "FullVisualsToggle", &tToggleScreenshake, 1, 2, 1},
	{"HiddenCorpseToggle", "FullVisualsToggle", &tToggleHiddenCorpse, 1, 2, 1},
	// end

	{"AutomapMonstersToggle", 0,        &tToggleAutomapMonsters, 1, 2, 1},
	{"AutomapLevelNamesToggle", 0,      &tToggleAutomapLevelNames, 1, 2, 1},
	{"AutomapChestsToggle", 0,          &tToggleAutomapChests, 1, 2, 1},

	
	{"AutomapItemsToggle", 0,           &tToggleAutomapItems, 1, 2, 1},

	// new
	{"AutomapCorpseToggle", 0,           &tToggleAutomapCorpse, 1, 2, 1},
	{"AutomapActiveRoomToggle", 0,       &tToggleAutomapActiveRoom, 1, 2, 1},
	{"AutomapReadyRoomToggle", 0,       &tToggleAutomapReadyRoom, 1, 2, 1},
	// end

	{"HiddenItemsToggle", 0,            &tToggleHiddenItems, 1, 2, 1},
	{"ScrollMapToggle", 0,              &tToggleScrollMap, 1, 2, 1},

	// new
	{"AutomapScreenAreaToggle", 0,              &tToggleAutomapScreenArea, 1, 2, 1},
	// end

	// not exist
	{"AutomapMissilesToggle", 0,        &tToggleAutomapMissiles, 1, 2, 1},
	// end

	{"MonsterTCToggle", 0,              &tToggleMonsterTC, 1, 2, 1},

	// new
	{"MonsterLevelToggle", 0,              &tToggleMonsterLevel, 1, 2, 1},
	{"MonsterResistsToggle", 0,              &tToggleMonsterResists, 1, 2, 1},
	// end

	{"PermShowItemsToggle", 0,          &tTogglePermShowItems, 1, 2 ,1},

	// new
	{"GameTimeToggle", 0,          &tToggleGameTime, 1, 2 ,1},
	{"ClockToggle", 0,          &tToggleClock, 1, 2 ,1},
	{"PermShowOrbsToggle", 0,          &tTogglePermShowOrb, 1, 2 ,1},
	{"InputLineToggle", 0,          &tToggleInputLine, 1, 2 ,1},
	{"OutTownSelectToggle", 0,          &tToggleOutTownSelect, 1, 2 ,1},
	// end

	{"AutomapPartyDefault", 0,          &fAutomapPartyDefault, 1, 1, 1},
	{"AutomapNamesDefault", 0,          &fAutomapNamesDefault, 1, 1, 1},

	{"MiniShrinesToggle", 0,            &tToggleMiniShrine, 1, 2, 1},
	{"LevelNameColour", 0,              &nCaveNameTextCol, 1, 1, 1},
	{"StaffTombLevelDesc", 0,           &szStaffTombLvlDesc, 0, 1, 1},
	{"DiagonalScrollToggle", 0,         &tToggleDiagonalScroll, 1, 2, 1},
	{"MapScrollSpeed", 0,               &nMapScrollSpeed, 1, 1, 1},
	{"LifeBarTransparency", 0,          &nLifeBarTrans, 1, 1, 1},
	{"LifeBarColour", 0,                &nLifeBarColour, 1, 1, 1},
	{"SocketProtectToggle", 0,          &tToggleSocketProtect, 1, 2, 1},

	// new
	{"ChickenLifeToggle", 0,          &tToggleChickenLife, 1, 2, 1},
	{"ChickenHostileToggle", 0,          &tToggleChickenHostile, 1, 2, 1},
	{"ChickenHostileNearbyToggle", 0,          &tToggleChickenHostileNearby, 1, 2, 1},
	{"ChickenLife", 0,          &nChickenLife, 4, 1, 1},
	{"ChickenHostileLife", 0,          &nChickenHostileLife, 4, 1, 1},
	{"ChickenHostileNearbyLife", 0,          &nChickenHostileNearbyLife, 4, 1, 1},
	{"ChickenLifePercent", 0,          &nChickenLifePercent, 4, 1, 1},
	{"ChickenHostileLifePercent", 0,          &nChickenHostileLifePercent, 4, 1, 1},
	{"ChickenHostileNearbyLifePercent", 0,          &nChickenHostileNearbyLifePercent, 4, 1, 1},
	// end

	{"UnitBlobFiles", 0,                &apUnitBlobFiles, 0, 1, ARRAYSIZE(apUnitBlobFiles)},
	{"PlayerBlobFile", 0,               &apUnitBlobFiles[0], 0, 1, 1},
	{"MonsterBlobFile", 0,              &apUnitBlobFiles[1], 0, 1, 1},
	{"ObjectBlobFile", 0,               &apUnitBlobFiles[2], 0, 1, 1},
	{"MissileBlobFile", 0,              &apUnitBlobFiles[3], 0, 1, 1},
	{"ItemBlobFile", 0,                 &apUnitBlobFiles[4], 0, 1, 1},

	// new
	{"AutomapActiveRoomColour", 0,            &anAutomapActiveRoomColors, 1, 1, ARRAYSIZE(anAutomapActiveRoomColors)},
	{"AutomapReadyRoomColour", 0,            &anAutomapReadyColors, 1, 1, ARRAYSIZE(anAutomapReadyColors)},
	{"AutomapScreenAreaColour", 0,            &anAutomapScreenColors, 1, 1, ARRAYSIZE(anAutomapScreenColors)},
	// end

	{"ClosedChestColour", 0,            &anClosedChestColors, 1, 1, ARRAYSIZE(anClosedChestColors)}, 
	{"LockedChestColour", 0,            &anLockedChestColors, 1, 1, ARRAYSIZE(anLockedChestColors)},

	{"HostileMissileColour", 0,         &anHostileMissileColors, 1, 1, ARRAYSIZE(anHostileMissileColors)},
	{"GuidedMissileColour", 0,          &anGuidedMissileColors, 1, 1, ARRAYSIZE(anGuidedMissileColors)},
	{"TracerMissileColour", 0,          &anTracerMissileColors, 1, 1, ARRAYSIZE(anTracerMissileColors)},
	{"OtherMissileColour", 0,          &anOtherMissileColors, 1, 1, ARRAYSIZE(anOtherMissileColors)},
	{"MissileColours", 0,               &anMissileColours, 1, 1, ARRAYSIZE(anMissileColours)},

	// new
	{"MissileCorpse", 0,               &afMissileCorpses, 1, 1, ARRAYSIZE(afMissileCorpses)},
	{"EtherealItemPrefix", 0,               &szEtherealItemPrefix, 0, 1, 1},
	{"EtherealItemPostfix", 0,               &szEtherealItemPostfix, 0, 1, 1},
	// end

	{"RuneNumbersToggle", 0,            &tToggleRuneNumbers, 1, 2, 1},
	{"SocketNumbersToggle", 0,          &tToggleSocketNumbers, 1, 2, 1},

	// new
	{"ItemLevelsToggle", 0,          &tToggleItemLevel, 1, 2, 1},
	{"ItemValueToggle", 0,          &tToggleItemValue, 1, 2, 1},
	{"ItemValueNpc", 0,          &nItemValueNpc, 4, 1, 1},
	{"ItemIndexsToggle", 0,          &tToggleItemIndex, 1, 2, 1},
	{"UnitNumbersToggle", 0,          &tToggleUnitNumber, 1, 2, 1},
	{"ViewSocketablesToggle", 0,          &tToggleViewSocketable, 1, 2, 1},
	{"ItemBasicStatsToggle", 0,          &tToggleItemBasicStat, 1, 2, 1},
	// end

	{"QuestItemColour", 0,              &nQuestItemColour, 1, 1, 1},

	// new
	{"HiddenItemLevel", 0,              &nHiddenItemLevel, 1, 1, 1},
	// end

	// new, added by sting
	{"KeepGameWindowToggle", 0,         &tToggleKeepGameWindow, 1, 2, 1},
	{"MinimapToggle", 0,				&tToggleMinimap, 1, 2, 1},
	{"MinimapCells", 0,					(void*)-1, 1, 1, {{nMaxMinimapLevelNo}, {nMaxMinimapCellNo}} },
//	{"MinimapCellColour", 0,	        &nMinimapCellCol, 1, 1, 1},
	{"MinimapCellColour", 0,	        &nMinimapCellCol[0], 1, 1, 1},
	{"MinimapCellColour2", 0,	        &nMinimapCellCol[1], 1, 1, 1},
	{"MinimapSize", 0,			        &nMinimapSize, 1, 1, 1},
	{"EnterGameSound", 0,			     &fEnterGameSound, 1, 1, 1},
	{"ServerIpToggle", 0,			     &tToggleServerIp, 1, 2, 1},

#ifdef ENABLE_CATCH_EXECEPTION
	// for debug only
	{"CrashDumpFile", 0,			     &nDumpType, 4, 1, 1},
	// end
#endif
	
	// not exist
	{"RareItemColour", 0,               (char *)anItemColours+5*2*2, 1, 2, {{ARRAYSIZE(anItemColours)}, {8}, {2}, {7}} },
	{"UniqueItemColour", 0,             (char *)anItemColours+6*2*2, 1, 2, {{ARRAYSIZE(anItemColours)}, {8}, {2}, {7}} },
	// end

	{"ItemColours", 0,                  &anItemColours, 1, 2, {{ARRAYSIZE(anItemColours)}, {8}, {2}, {7} } }, // 0x0BB8, 8, 2, 0x4007
	{"WeaponColours", 0,                &anItemWeaponCols, 1, 2, {{ARRAYSIZE(anItemWeaponCols)}, {8}, {2}, {7} } }, // 0x3E8, 8, 2, 0x4007
	{"ArmorColours", 0,                 &anItemArmorCols, 1, 2, {{ARRAYSIZE(anItemArmorCols)}, {8}, {2}, {7} } }, // 0x3E8, 8, 2, 0x4007
	{"MiscItemColours", 0,              &anItemMiscCols, 1, 2, {{ARRAYSIZE(anItemMiscCols)}, {8}, {2}, {7} } }, // 0x3E8, 8, 2, 0x4007

	{"CharmColours", 0,                 &anItemMiscCols[95], 1, 2, {{3}, {8}, {2}, {7}} }, // 0x3, 8, 2, 0x4007
	{"RuneColours", 0,                  &anRuneColours, 1, 2, ARRAYSIZE(anRuneColours)}, // 0x64
	{"RejuvPotColours", 0,              &anItemMiscCols[7], 1, 2,  {{2}, {8}, {2}, {7}} },
	{"HealingPotColours", 0,            &anItemMiscCols[79], 1, 2, {{5}, {8}, {2}, {7}} },
	{"ManaPotColours", 0,               &anItemMiscCols[84], 1, 2, {{5}, {8}, {2}, {7}} },
	{"AmethystColours", "GemColours",   &anItemMiscCols[49], 1, 2, {{5}, {8}, {2}, {7}} },
	{"TopazColours", "GemColours",      &anItemMiscCols[54], 1, 2, {{5}, {8}, {2}, {7}} },
	{"SapphireColours", "GemColours",   &anItemMiscCols[59], 1, 2, {{5}, {8}, {2}, {7}} },
	{"EmeraldColours", "GemColours",    &anItemMiscCols[64], 1, 2, {{5}, {8}, {2}, {7}} },
	{"RubyColours", "GemColours",       &anItemMiscCols[69], 1, 2, {{5}, {8}, {2}, {7}} },
	{"DiamondColours", "GemColours",    &anItemMiscCols[74], 1, 2, {{5}, {8}, {2}, {7}} },
	{"SkullColours", "GemColours",      &anItemMiscCols[89], 1, 2, {{5}, {8}, {2}, {7}} },

	{"GoodGoldNumber", 0,               &nGoldGoodNum, 4, 1, 1},
	{"GoodGoldColour", 0,               &nGoldGoodCol, 1, 2, 1},
	{"PoorGoldColour", 0,               &nGoldPoorCol, 1, 2, 1},

	{"MonsterDescColour", 0,            &nMonsterTextCol, 1, 1, 1},
	{"MonsterDeathAnims", 0,            &afMonsterDeathAnims, 1, 1, ARRAYSIZE(afMonsterDeathAnims)},
	{"MonsterColours", 0,               &anMonsterColours, 1, 1, ARRAYSIZE(anMonsterColours)},
	// new
	{"MonsterCorpseColour", 0,           &anMonsterCorpseColors, 1, 1, ARRAYSIZE(anMonsterCorpseColors)},
	{"DangerousMonster", 0,              &anDangerousMonster, 1, 2, ARRAYSIZE(anDangerousMonster)},
	{"DangerousMonsterChickenToggle", 0, &tToggleChickenDangerousMonster, 1, 2, 1},
	{"DangerousMonsterAction", 0,		 &fDangerousMonsterAction, 1, 1, 1},

	// end

	{"NormalMonsterColour", 0,          &anMonsterNormalColors, 1, 1, ARRAYSIZE(anMonsterNormalColors)},
	{"BossMonsterColour", 0,            &anMonsterBossColors, 1, 1, ARRAYSIZE(anMonsterBossColors)},
	{"MinionMonsterColour", 0,          &anMonsterMinionColors, 1, 1, ARRAYSIZE(anMonsterMinionColors)},
	{"ChampionMonsterColour", 0,        &anMonsterChampColors, 1, 1, ARRAYSIZE(anMonsterChampColors)},
	{"Act5BarbarianColour", 0,          &anMonsterColours[522], 1, 1, 1},
	{"NeutralPlayerColour", 0,          &anNeutralPlayerColors, 1, 1, ARRAYSIZE(anNeutralPlayerColors)},
	{"HostilePlayerColour", 0,          &anHostilePlayerColors, 1, 1, ARRAYSIZE(anHostilePlayerColors)},

	// new
	{"NormalCorpseColour", 0,          &anNormalCorpseColors, 1, 1, ARRAYSIZE(anNormalCorpseColors)},
	{"BossCorpseColour", 0,          &anBossCorpseColors, 1, 1, ARRAYSIZE(anBossCorpseColors)},
	{"MinionCorpseColour", 0,          &anMinionCorpseColors, 1, 1, ARRAYSIZE(anMinionCorpseColors)},
	{"ChampionCorpseColour", 0,          &anChampionCorpseColors, 1, 1, ARRAYSIZE(anChampionCorpseColors)},
	// end

	{"EnchantmentDescs", 0,             &aszEnchantDescs, 0, 1, ARRAYSIZE(aszEnchantDescs)},
	{"AuraDescs", 0,                    &aszAuraDescs, 0, 1, ARRAYSIZE(aszAuraDescs)},
	{"ImmunityDescs", 0,                &aszImmuneDescs, 0, 1, ARRAYSIZE(aszImmuneDescs)},

	{"ExtraStrongDesc", 0,              &aszEnchantDescs[5], 0, 1, 1},
	{"ExtraFastDesc", 0,                &aszEnchantDescs[6], 0, 1, 1},
	{"CursedDesc", 0,                   &aszEnchantDescs[7], 0, 1, 1},
	{"MagicResistantDesc", 0,           &aszEnchantDescs[8], 0, 1, 1},
	{"FireEnchantedDesc", 0,            &aszEnchantDescs[9], 0, 1, 1},
	{"ChampionDesc", 0,                 &aszEnchantDescs[16], 0, 1, 1},
	{"LightningEnchantedDesc", "LightEnchantedDesc",       &aszEnchantDescs[17], 0, 1, 1},
	{"ColdEnchantedDesc", 0,            &aszEnchantDescs[18], 0, 1, 1},
	{"ThiefDesc", 0,                    &aszEnchantDescs[24], 0, 1, 1},
	{"ManaBurnDesc", 0,                 &aszEnchantDescs[25], 0, 1, 1},
	{"TeleportationDesc", 0,            &aszEnchantDescs[26], 0, 1, 1},
	{"SpectralHitDesc", 0,              &aszEnchantDescs[27], 0, 1, 1},
	{"StoneSkinDesc", 0,                &aszEnchantDescs[28], 0, 1, 1},
	{"MultipleShotsDesc", 0,            &aszEnchantDescs[29], 0, 1, 1},
	{"GhostlyDesc", 0,                  &aszEnchantDescs[36], 0, 1, 1},
	{"FanaticDesc", 0,                  &aszEnchantDescs[37], 0, 1, 1},
	{"PossessedDesc", 0,                &aszEnchantDescs[38], 0, 1, 1},
	{"BerserkerDesc", 0,                &aszEnchantDescs[39], 0, 1, 1},

	{"MightAuraDesc", 0,                &aszAuraDescs[0], 0, 1, 1},
	{"HolyFireAuraDesc", 0,             &aszAuraDescs[1], 0, 1, 1},
	{"BlessedAimAuraDesc", 0,           &aszAuraDescs[2], 0, 1, 1},
	{"HolyFreezeAuraDesc", 0,           &aszAuraDescs[3], 0, 1, 1},
	{"HolyShockAuraDesc", 0,            &aszAuraDescs[4], 0, 1, 1},
	{"ConvictionAuraDesc", 0,           &aszAuraDescs[5], 0, 1, 1},
	{"FanaticismAuraDesc", 0,           &aszAuraDescs[6], 0, 1, 1},

	{"PhysicalImmunityDesc", 0,         &aszImmuneDescs[0], 0, 1, 1},
	{"MagicImmunityDesc", 0,            &aszImmuneDescs[1], 0, 1, 1},
	{"FireImmunityDesc", 0,             &aszImmuneDescs[2], 0, 1, 1},
	{"LightningImmunityDesc", 0,        &aszImmuneDescs[3], 0, 1, 1},
	{"ColdImmunityDesc", 0,             &aszImmuneDescs[4], 0, 1, 1},
	{"PoisonImmunityDesc", 0,           &aszImmuneDescs[5], 0, 1, 1},
};
