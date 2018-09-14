#ifndef HACK_MAP_H
#define HACK_MAP_H

#include "memhelper.h"

#define ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))

enum DllNo {DLLNO_D2CLIENT, DLLNO_D2COMMON, DLLNO_D2GFX, DLLNO_D2WIN, DLLNO_D2LANG, DLLNO_D2CMP, DLLNO_D2MULTI, DLLNO_BNCLIENT, DLLNO_D2NET, DLLNO_STORM, DLLNO_FOG, DLLNO_D2LAUNCH, DLLNO_D2GAME, DLLNO_D2Direct3D, DLLNO_D2GDI};

enum DllBase {
	DLLBASE_D2CLIENT = 0x6FAA0000,
	DLLBASE_D2COMMON = 0x6FD40000,
	DLLBASE_D2GFX = 0x6FA70000,
	DLLBASE_D2WIN = 0x6F8A0000,
	DLLBASE_D2LANG = 0x6FC10000,
	DLLBASE_D2CMP = 0x6FDF0000,
	DLLBASE_D2MULTI = 0x6F9A0000,
	DLLBASE_BNCLIENT = 0x6FF00000,
	DLLBASE_D2NET = 0x6FC00000,
	DLLBASE_STORM = 0x6FFB0000,
	DLLBASE_FOG = 0x6FF50000,
	DLLBASE_D2GAME = 0x6FC30000,
	DLLBASE_D2LAUNCH = 0x6FA10000,
	DLLBASE_D2Direct3D = 0x6F840000,
	DLLBASE_D2GDI = 0x6F830000
};

#define DLLOFFSET(a1,b1) ((DLLNO_##a1)|(( ((b1)<0)?(b1):(b1)-DLLBASE_##a1 )<<8))

//NOTE :- reference vars buggy

#define D2FUNCPTR(d1,v1,t1,t2,o1) typedef t1 d1##_##v1##_t t2; d1##_##v1##_t *d1##_##v1 = (d1##_##v1##_t *)DLLOFFSET(d1,o1);
#define D2VARPTR(d1,v1,t1,o1)     typedef t1 d1##_##v1##_t;    d1##_##v1##_t *p_##d1##_##v1 = (d1##_##v1##_t *)DLLOFFSET(d1,o1);
#define D2ASMPTR(d1,v1,o1)        DWORD d1##_##v1 = DLLOFFSET(d1,o1);

#define D2PATCH_HELPER(x) {x.func, x.addr, x.param, x.len, x.mask, x.oldcode}

struct Patch_t{
	void (*func)(DWORD, DWORD, DWORD);
	DWORD addr;
	DWORD param;
	BYTE len;
	DWORD mask;
	BYTE *oldcode;
};

struct PatchItem_t {
	Patch_t *patch;
	int num;
	BOOL fInitOnDemand;
};

struct ConfigVar {
	char *szName1, *szName2;
	union {
		void *pVar;
		char **pszVar;
	};
	BYTE nSize, nSize2; //0 for string
	short anArrayMax[4];

};

extern HMODULE hInstDLL;

struct ToggleVar {
	BYTE flag;
	BYTE key;
	char* desc;
	void (*func)();
	BYTE _2[2];
};

struct TreeNode {
	char *key, *str;
	TreeNode *left, *right;
};


BOOL RelocD2Ptrs(DWORD* pPtrsStart, DWORD* pPtrsEnd);
DWORD RelocD2Patchs(Patch_t* pPatchStart, Patch_t* pPatchEnd);
BOOL SaveD2Patchs(Patch_t* pPatchStart, Patch_t* pPatchEnd);
BOOL InstallD2Patchs(Patch_t* pPatchStart, Patch_t* pPatchEnd);
void RemoveD2Patchs(Patch_t* pPatchStart, Patch_t* pPatchEnd);
void DeleteD2Patchs(Patch_t* pPatchStart, Patch_t* pPatchEnd);
DWORD GetDllOffset(int num);

// config stuff
void LoadConfig(ConfigVar* config, int num);
void DeleteConfig(ConfigVar* config, int num);

// misc helpers
char isquote(char c);
char *AllocStrN(char *str, int len);
void AllocStrN(char **pstr, char *str, int len);
int strtol2(int &num, char *&str, int base);
char *isnumber(char *str, int base);
void SwapInt(int &num1, int &num2);
void *memcpy2(void *dest, const void *src, size_t count);
int strnicmp2(const char *str1, const char *str2, size_t count);
char skipspace(char *&p);
wchar_t * __cdecl wsprintfW2(wchar_t *dest, char *fmt, ...);
void wcscpy2(wchar_t *dest, char *src);
wchar_t * wcsrcat(wchar_t *dest, wchar_t *src);
void trimspaces(char *str);
void trimspaces(wchar_t *str);


// file helpers
HANDLE OpenFileRead(char *filename);
HANDLE OpenFileWrite(char *filename);
DWORD WriteFile(HANDLE hFile, void *buf, DWORD len);
DWORD ReadFile(HANDLE hFile, void *buf, DWORD len);
char *ReadFileLine(char *str, int len, HANDLE hFile);
BYTE *AllocReadFile(char *filename);
char *GetMyFileNameStrrchr(char *dest, char ch);
HANDLE OpenFileReadHelper(char *postfix);


enum TStatType {
			Strength,
      Energy,
      Dexterity,
      Vitality,
      StatPoints,
      SkillPoints,
      Life,
      MaxLife,
      Mana,
      MaxMana,
      Stamina,
      MaxStamina,
      Level,
      Experience,
      Gold,
      GoldBank,
      DefensePercent,
      MaxDamagePercent,
      MinDamagePercent,
      ToHit,
      ToBlock,
      MinDamage,
      MaxDamage,
      SecondaryMinDamage,
      SecondaryMaxDamage,
      DamagePercent,
      ManaRecovery,
      ManaRecoveryBonus,
      StaminaRecoveryBonus,
      LastExperience,
      NextExperience,
      ArmorClass,
      ArmorClassVsMissile,
      ArmorClassVsMelee,
      DamageReduction,
      MagicDamageReduction,
      DamageResist,
      MagicResist,
      MaxMagicResist,
      FireResist,
      MaxFireResist,
      LightResist,
      MaxLightResist,
      ColdResist,
      MaxColdResist,
      PoisonResist,
      MaxPoisonResist,
      DamageAura,
      FireMinDamage,
      FireMaxDamage,
      LightMinDamage,
      LightMaxDamage,
      MagicMinDamage,
      MagicMaxDamage,
      ColdMinDamage,
      ColdMaxDamage,
      ColdLength,
      PoisonMinDamage,
      PoisonMaxDamage,
      PoisonLength,
      LifeDrainMinDamage,
      LifeDrainMaxDamage,
      ManaDrainMinDamage,
      ManaDrainMaxDamage,
      StamDrainMinDamage,
      StamDrainMaxDamage,
      StunLength,
      VelocityPercent,
      AttackRate,
      OtherAnimRate,
      Quantity,
      Value,
      Durability,
      MaxDurability,
      LifeRegen,
      MaxDurabilityPercent,
      MaxLifePercent,
      MaxManaPercent,
      AttackerTakesDamage,
      GoldFind,
      MagicFind,
      Knockback,
      TimeDuration,
      ClassSkillsBonus,
      UnsentParam1,
      AddExperience,
      HealAfterKill,
      ReducedPrices,
      DoubleHerbDuration,
      LightRadius,
      LightColor,
      LowerRequirementsPercent,
      LowerLevelRequirement,
      FasterAttackRate,
      LowerLevelRequirementPercent,
      LastBlockFrame,
      FasterMoveVelocity,
      NonClassSkill,
      State,
      FasterHitRecovery,
      MonsterPlayerCount,
      SkillPoisonOverrideLength,
      FasterBlockRate,
      SkillBypassUndead,
      SkillBypassDemons,
      FasterCastRate,
      SkillBypassBeasts,
      SingleSkill,
      RestInPeace,
      CurseResistance,
      PoisonLengthReduction,
      NormalDamage,
      Howl,
      HitBlindsTarget,
      DamageToMana,
      IgnoreTargetDefense,
      FractionalTargetAC,
      PreventHeal,
      HalffReezeDuration,
      ToHitPercent,
      DamageTargetAC,
      DemonDamagePercent,
      UndeadDamagepercent,
      DemonToHit,
      UndeadToHit,
      Throwable,
      ElementalSkillBonus,
      AllSkillsBonus,
      AttackerTakesLightingDamage,
      IronMaidenLevel,
      LifeTapLevel,
      ThornsPercent,
      BoneArmor,
      BoneArmorMax,
      Freeze,
      OpenWounds,
      CrushingBlow,
      KickDamage,
      ManaAfterKill,
      HealAfterDemonKill,
      ExtraBlood,
      DeadlyStrike,
      AbsorbFirePercent,
      AbsorbFire,
      AbsorbLightingPercent,
      AbsorbLight,
      AbsorbMagicPercent,
      AbsorbMagic,
      AbsorbColdPercent,
      AbsorbCold,
      Slow,
      Aura,
      Indesctructible,
      CannotBeFrozen,
      StaminaDrainPercent,
      Reanimate,
      Pierce,
      MagicArrow,
      ExplosiveArrow,
      ThrowMinDamage,
      ThrowMaxDamage,
      SkillHandOfAthena,
      SkillStaminaPercent,
      SkillPassiveStaminaPercent,
      SkillConcentration,
      SkillEnchant,
      SkillPierce,
      SkillConviction,
      SkillChillingArmor,
      SkillFrenzy,
      SkillDecrepify,
      SkillArmorPercent,
      Alignment,
      Target0,
      Target1,
      GoldLost,
      ConversionLevel,
      ConversionMaxHP,
      UnitDoOverlay,
      AttackVsMonsterType,
      DamageVsMonsterType,
      Fade,
      ArmorOverridePercent,
      Unused183,
      Unused184,
      Unused185,
      Unused186,
      Unused187,
      SkillTabBonus,
      Unused189,
      Unused190,
      Unused191,
      Unused192,
      Unused193,
      Sockets,
      SkillOnAttack,
      SkillOnKill,
      SkillOnDeath,
      SkillOnStriking,
      SkillOnLevelUp,
      Unused200,
      SkillOnGetHit,
      Unused202,
      Unused203,
      ChargedSkill,
      Unused204,
      Unused205,
      Unused206,
      Unused207,
      Unused208,
      Unused209,
      Unused210,
      Unused211,
      Unused212,
      ArmorPerLevel,
      ArmorPercentPerLevel,
      LifePerLevel,
      ManaPerLevel,
      MaxDamagePerLevel,
      MaxDamagePercentPerLevel,
      StrengthPerLevel,
      DexterityPerLevel,
      EnergyPerLevel,
      VitalityPerLevel,
      ToHitPerLevel,
      ToHitPercentPerLevel,
      ColdDamageMaxPerLevel,
      FireDamageMaxPerLevel,
      LightningDamageMaxPerLevel,
      PoisonDamageMaxPerLevel,
      ResistColdPerLevel,
      ResistFirePerLevel,
      ResistLightningPerLevel,
      ResistPoisonPerLevel,
      AbsorbColdPerLevel,
      AbsorbFirePerLevel,
      AbsorbLightningPerLevel,
      AbsorbPoisonPerLevel,
      ThornsPerLevel,
      GoldFindPerLevel,
      MagicFindPerLevel,
      RegenStaminaPerLevel,
      StaminaPerLevel,
      DamageDemonPerLevel,
      DamageUndeadPerLevel,
      ToHitDemonPerLevel,
      ToHitUndeadPerLevel,
      CrushingBlowPerLevel,
      OpenWoundsPerLevel,
      KickDamagePerLevel,
      DeadlyStrikePerLevel,
      FindGemsPerLevel,
      ReplenishDurability,
      ReplenishQuantity,
      ExtraStack,
      FindItem,
      SlashDamage,
      SlashDamagePercent,
      CrushDamage,
      CrushDamagePercent,
      ThrustDamage,
      ThrustDamagePercent,
      AbsorbSlash,
      AbsorbCrush,
      AbsorbThrust,
      AbsorbSlashPercent,
      AbsorbCrushPercent,
      AbsorbThrustPercent,
      ArmorByTime,
      ArmorPercentByTime,
      LifeByTime,
      ManaByTime,
      MaxDamageByTime,
      MaxDamagePercentByTime,
      StrengthByTime,
      DexterityByTime,
      EnergyByTime,
      VitalityByTime,
      ToHitByTime,
      ToHitPercentByTime,
      ColdMaxDamageByTime,
      FireMaxDamageByTime,
      LightningMaxDamageByTime,
      PoisonMaxDamageByTime,
      ResistColdByTime,
      ResistFireByTime,
      ResistLightningByTime,
      ResistPoisonByTime,
      AbsorbColdByTime,
      AbsorbFireByTime,
      AbsorbLightningByTime,
      AbsorbPoisonByTime,
      FindGoldByTime,
      FindMagicByTime,
      RegenStaminaByTime,
      StaminaByTime,
      DamageDemonByTime,
      DamageUndeadByTime,
      ToHitDemonByTime,
      ToHitUndeadByTime,
      CrushingBlowByTime,
      OpenWoundsByTime,
      KickDamageByTime,
      DeadlyStrikeByTime,
      FindGemsByTime,
      PierceCold,
      PierceFire,
      PierceLightning,
      PiercePoison,
      DamageVsMonster,
      DamagePercentVsMonster,
      ToHitVsMonster,
      ToHitPercentVsMonster,
      DefenseVsMonster,
      DefensePercentVsMonster,
      FireLength,
      BurningMin,
      BurningMax,
      ProgressiveDamage,
      ProgressiveSteal,
      ProgressiveOther,
      ProgressiveFire,
      ProgressiveCold,
      ProgressiveLightning,
      ExtraCharges,
      ProgressiveToHit,
      PoisonCount,
      DamageFramerate,
      PierceIdx,
      PassiveFireMastery,
      PassiveLightningMastery,
      PassiveColdMastery,
      PassivePoisonMastery,
      PassiveFirePierce,
      PassiveLightningPierce,
      PassiveColdPierce,
      PassivePoisonPierce,
      PassiveCriticalStrike,
      PassiveDodge,
      PassiveAvoid,
      PassiveEvade,
      PassiveWarmth,
      PassiveMasteryMeleeToHit,
      PassiveMasteryMeleeDamage,
      PassiveMasteryMeleeCritical,
      PassiveMasteryThrowToHit,
      PassiveMasteryThrowDamage,
      PassiveMasteryThrowCritical,
      PassiveWeaponBlock,
      PassiveSummon_resist,
      ModifierListSkill,
      ModifierListLevel,
      LastSentLifePercent,
      SourceUnitType,
      SourceUnitID,
      ShortParam1,
      QuestItemDifficulty,
      PassiveMagicMastery,
    	PassiveMagicPierce
    	};

#endif // HACK_MAP_H
