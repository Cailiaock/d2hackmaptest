//-------------------------------------------------------------------------
// Based on the source reversed from MH 5.1s sting-edited version
// 2003- sting - stingxp@yahoo.com
//
// The part for localization support based on d2locale plugin for v1.09d
// Authors(Battlecn.net ID): 
//......
//Neo
//Kant
//-------------------------------------------------------------------------

//Mousepad's Diablo II Maphack v4.6c (v1.09d)
//Derivative works to be distributed for free with full source and full credits (ie dont blame me)
//?2001-2 mousepad - mousepad@forward.to

/*
Baal释放5小队： 4D 01 B2 00 00 00 1E 01 00 00 01 F2 3A A0 13 00 00
Baal转身下去：5D 24 00 03 00 00 
KD打开最后的封印： 89 0C

ULC火风暴屏蔽：检测0x99报文，一旦发现使用197技能（火风暴）则转为普通攻击的动画，哈哈
*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <wchar.h>
#include <math.h>
#include "d2structs.h"
#include "hackmap.h"

#define ENABLE_CATCH_EXECEPTION

#ifdef ENABLE_CATCH_EXECEPTION
#include "exception.cxx"
#endif

DWORD GetCurrentAct(DrlgLevel* currlvl);
DrlgLevel *GetUnitDrlgLevel(UnitAny *unit);
int __stdcall UnicodeCharNameCheck(void *unk1, void *unk2, char *lpCharName);
HIMC hIMC;

char *szProgTitle = "D2hackmap (sting version) V1.16 modified by SJ";
void KeepGameWindow();
void ToggleSleepy();

BYTE fSleepy = 0;


#include "d2ptrs.cxx"
#include "d2helpers.cxx"
#include "hnmtoggles.cxx"

DWORD HR1024X768_FixMPQ();
void HR1024X768_FixOffset_ASM();

//#define TWO_LINE_DISPLAY 1

BYTE BugKD = 0;
BYTE BugKB = 0;
BYTE BugKM = 0;
DWORD BugKDTimer,BugKBTimer,BugKMTimer;
DWORD BugMsgTimer;
BYTE BugMsgShowed=0;

BYTE BugKDTimerSet[10],BugKBTimerSet[10],BugKMTimerSet[10];
BYTE ForceBugKDQuit = 0;
DWORD ForceBugKDQuitTimer;
BYTE ForceBugKDQuitTimerSet[10];

int currentgameexp = 0;
int Pet_currentgameexp = 0;
int PET_EXP_LVL=100;

struct I386Registers
{
  DWORD edi;
  DWORD esi;
  DWORD ebp;
  DWORD esp;
  DWORD ebx;
  DWORD edx;
  DWORD ecx;
  DWORD eax;
};

struct D2ScreenHookStruct
{
  BYTE show;
  int x,y;
  wchar_t msg[512];
  BYTE color;
  BYTE size;
} D2ScreenHook[10];

/*
D2ScreenHook[0] : 显示+Exp，获得的EXP
D2ScreenHook[1] : 显示Lvl以及总的EXP
D2ScreenHook[2] : 显示Ping
D2ScreenHook[3] : 显示FPS
D2ScreenHook[4] : 显示BugMessage
D2ScreenHook[5] : 显示PET EXP百分比
*/

inline BYTE GetGameMode()
{
  GameStructInfo *gi = D2CLIENT_GetGameInfo();
  return gi->nExpansion;
}

inline DrlgLevel *GetPlayerDrlgLevel()
{
  return D2CLIENT_GetPlayerUnit()->pPos->pRoom1->pRoom2->pDrlgLevel;
}

inline BYTE GetAreaLevel()
{
  LevelTxt *Lvl=D2COMMON_GetLevelTxt(GetPlayerDrlgLevel()->nLevelNo);
  return GetGameMode()?Lvl->nMonLvEx[D2CLIENT_GetDifficulty()]:Lvl->nMonLv[D2CLIENT_GetDifficulty()];   
}

void __declspec(naked) MySleep_ASM()
{
  __asm
  {
    jz  orig_code
    mov ebp,6FAB50C6h
    jmp ebp
orig_code:
    pop edi
    pop esi
    pop ebp
    pop ebx
    add esp,54h

    test ebx,ebx
    jnz over
    inc ebx
over:   
    ret
  }
}

void __declspec(naked) ULCPatch_ASM()
{
    __asm {
      cmp [tToggleULCMask.flag],0
      je      org_code
      movzx   ecx,byte ptr [esi+6]
      cmp     ecx,197
      jne     org_code
      mov     byte ptr [esi+6],0  ; modify the firestorm effect to normal attack effect
org_code:
      mov     cx, [esi+0Eh]
      xor     edx, edx
      ret;
  }
}

void CheckBugQuest()
{
  // 检查是否指定的任务在当前游戏是BUG任务，如果是，则设置Quest标志，否则清除Quest标志
  // BugKB: quest=40
  // BugKD: quest=26
  // BugKM: quest=22
  int d1,d2,d3;
  
  BugKD = 0;
  BugKB = 0;
  BugKM = 0;
  
//  DWORD myCurrentAct = GetCurrentAct(GetUnitDrlgLevel(D2CLIENT_GetPlayerUnit()));
  

  if (tToggleBugKD.flag)
  {
    d1 = D2COMMON_GetQuestFlag(D2CLIENT_GetQuestInfo(), 26, 0);
    d3 = D2COMMON_GetQuestFlag(D2CLIENT_GetGameQuestInfo(), 26, 15);
    
    if ((d1 == 0) && (d3 == 0)) // 玩家还没完成指定任务，且当前游戏可以完成
      BugKD = 1;
  }
  
  if (tToggleBugKB.flag)
  {
    d1 = D2COMMON_GetQuestFlag(D2CLIENT_GetQuestInfo(), 40, 0);
    d3 = D2COMMON_GetQuestFlag(D2CLIENT_GetGameQuestInfo(), 40, 15);

    if ((d1 == 0) && (d3 == 0)) // 玩家还没完成指定任务，且当前游戏可以完成
      BugKB = 1;
  }

  if (tToggleBugKM.flag)
  {
    d1 = D2COMMON_GetQuestFlag(D2CLIENT_GetQuestInfo(), 22, 0);
    d3 = D2COMMON_GetQuestFlag(D2CLIENT_GetGameQuestInfo(), 22, 15);

    if ((d1 == 0) && (d3 == 0)) // 玩家还没完成指定任务，且当前游戏可以完成
      BugKM = 1;
  }
}

BYTE fLogInGameMessage = 0;

void LogExitMessage()
{
  char asc_buf[0x400];
  char asc_buf1[0x400];
  GameStructInfo *gi = NULL;

  if (fLogInGameMessage==0) return;
  
  UnitAny *ptPlayer = D2CLIENT_GetPlayerUnit();
  int exp = D2COMMON_GetUnitStat(ptPlayer, STAT_EXP, 0);

  sprintf(asc_buf1,"--EXP gained[%d]--\r\n",exp - currentgameexp);

  // I just exit a game, log it.
  gi = D2CLIENT_GetGameInfo();
  if (gi)
    sprintf(asc_buf,"----------------------------%s(*%s) exit game------------------------------\r\n",gi->szCharName,gi->szAccountName);

  SYSTEMTIME t;
  char timebuf[256];
  GetLocalTime(&t);
  sprintf(timebuf,"[%04X][%04d-%02d-%02d %02d:%02d:%02d]",GetCurrentProcessId(),t.wYear,t.wMonth,t.wDay,t.wHour, t.wMinute, t.wSecond );

  HANDLE hFile = OpenFileWrite("d2messages.log");
  SetFilePointer(hFile, 0, 0, FILE_END);
  WriteFile(hFile,timebuf,strlen(timebuf));
  WriteFile(hFile,asc_buf1,strlen(asc_buf1));
  WriteFile(hFile,timebuf,strlen(timebuf));
  WriteFile(hFile,asc_buf,strlen(asc_buf));
  CloseHandle(hFile);
}
void UpdateExp();

void InitScreenHook()
{
  for (int i=0; i < ARRAYSIZE(D2ScreenHook); i++)
  {
    D2ScreenHook[i].show = 0;
    D2ScreenHook[i].size = 1;
  }
  
  // 显示BugKD、BugKM、BugKB的消息
  D2ScreenHook[4].x = 30;
  D2ScreenHook[4].y = 150;
  D2ScreenHook[4].color = 8;
  D2ScreenHook[4].size = 2;
}

void DisplayGameInfo()
{
	
#ifdef TWO_LINE_DISPLAY
  D2ScreenHook[0].show = tToggleShowExp.flag;
#endif

  D2ScreenHook[1].show = tToggleShowExp.flag;
  
  D2ScreenHook[5].show = tToggleShowPetExp.flag;
  
  if (tToggleShowPing.flag)
  {
    D2ScreenHook[2].show = 1;
    D2ScreenHook[2].x = D2GetScreenSizeX() - 70;
    D2ScreenHook[2].y = D2GetScreenSizeY() - 30;

    DWORD myPing;
    myPing = D2CLIENT_Ping;
    if (myPing >= 700)
      D2ScreenHook[2].color = 1;
    else
      D2ScreenHook[2].color = 2;
    wsprintfW2(D2ScreenHook[2].msg, "%d",myPing);

    D2ScreenHook[3].show = 1;
    D2ScreenHook[3].x = 65;
    D2ScreenHook[3].y = D2GetScreenSizeY() - 30;
    D2ScreenHook[3].color = 2;
    wsprintfW2(D2ScreenHook[3].msg, "%d",D2CLIENT_FPS);
  }
  else
  {
    D2ScreenHook[2].show = 0;
    D2ScreenHook[3].show = 0;
  }
  
  for (int i=0; i < ARRAYSIZE(D2ScreenHook); i++)
  {
    if (D2ScreenHook[i].show)
      {
        DWORD oldfont = D2WIN_SetTextSize(1);
        D2WIN_SetTextSize( D2ScreenHook[i].size);
        D2WIN_DrawText(D2ScreenHook[i].msg, D2ScreenHook[i].x, D2ScreenHook[i].y, D2ScreenHook[i].color, 0);
        D2WIN_SetTextSize(oldfont);
      }
  }
}

void __declspec(naked) DrawClientPatch_ASM()
{
    __asm {
    call D2CLIENT_DrawClient      ; orig code
    call DisplayGameInfo
    ret;
  }
}

void InitExp()
{
  UnitAny *ptPlayer = D2CLIENT_GetPlayerUnit();
  currentgameexp = D2COMMON_GetUnitStat(ptPlayer, STAT_EXP, 0);

#ifdef TWO_LINE_DISPLAY
  D2ScreenHook[0].show = tToggleShowExp.flag;
  D2ScreenHook[0].x = 120;
//  D2ScreenHook[0].x = 50;
  D2ScreenHook[0].color = 0;
#endif

  D2ScreenHook[1].show = tToggleShowExp.flag;
  D2ScreenHook[1].x = 120;
  D2ScreenHook[1].color = 0;

  UnitAny *ptPET = D2CLIENT_GetUnitFromId(D2CLIENT_GetInventoryId(ptPlayer, 7, 0), 1);
  Pet_currentgameexp = D2COMMON_GetUnitStat(ptPET, STAT_EXP, 0);
  D2ScreenHook[5].x = D2GetScreenSizeX() - 340;

  int PET_LVL = D2COMMON_GetUnitStat(ptPET, STAT_LEVEL, 0)+1;
  int PET_NEXTEXP = D2COMMON_GetUnitStat(ptPET, STAT_NEXTEXP, 0);
  if (PET_LVL>=1 && PET_NEXTEXP > 0)
    PET_EXP_LVL=PET_NEXTEXP/(PET_LVL*PET_LVL*(PET_LVL+1));
  UpdateExp();
}

void UpdateExp()
{
  char temp_s[512];
  UnitAny *ptPlayer = D2CLIENT_GetPlayerUnit();
  int exp = D2COMMON_GetUnitStat(ptPlayer, STAT_EXP, 0);
  DWORD lvl = D2COMMON_GetUnitStat(ptPlayer, STAT_LEVEL, 0);
  if (lvl == 0) return;
  DWORD player_class = ptPlayer->nTxtFileNo;
  int nextlvlexp = D2COMMON_GetMinExpToLevel(player_class, lvl);
  int currentlvlexp = 0;
  if (lvl>1)
  {
    currentlvlexp = D2COMMON_GetMinExpToLevel(player_class, lvl-1);
  }
  
  int totalexpneedtoupgrade = nextlvlexp - currentlvlexp;
  int currentlvlgainedexp = exp - currentlvlexp;
  int currentgamegainedexp = exp - currentgameexp;
  if (currentgamegainedexp > currentlvlgainedexp) currentgamegainedexp = currentlvlgainedexp;

#ifdef  TWO_LINE_DISPLAY
  sprintf(temp_s,"+Exp:[%.2f%%%%]%d", ((float)(currentgamegainedexp)*100)/((float)totalexpneedtoupgrade),currentgamegainedexp);
  wsprintfW2(D2ScreenHook[0].msg, temp_s);
  D2ScreenHook[0].y = D2GetScreenSizeY()-65;
#endif

#ifdef TWO_LINE_DISPLAY
  sprintf(temp_s,"Lvl%d:[%.2f%%%%]%d / %d", lvl,((float)currentlvlgainedexp*100)/((float)totalexpneedtoupgrade), currentlvlgainedexp,totalexpneedtoupgrade);
#else
  sprintf(temp_s,"EXP:[%.2f%%%%][%.2f%%%%]%d", ((float)currentlvlgainedexp*100)/((float)totalexpneedtoupgrade),((float)(currentgamegainedexp)*100)/((float)totalexpneedtoupgrade), currentgamegainedexp);
#endif
//  sprintf(temp_s,"%d", currentgamegainedexp);
  wsprintfW2(D2ScreenHook[1].msg, temp_s);
  D2ScreenHook[1].y = D2GetScreenSizeY()-50;//550;

  // PET exp stuff

  UnitAny *ptPET = D2CLIENT_GetUnitFromId(D2CLIENT_GetInventoryId(ptPlayer, 7, 0), 1);
  if (ptPET == NULL) return;
  int Pet_exp = D2COMMON_GetUnitStat(ptPET, STAT_EXP, 0);
  DWORD Pet_lvl = D2COMMON_GetUnitStat(ptPET, STAT_LEVEL, 0);
  if (Pet_lvl == 0 || Pet_lvl >= 99)  return;
  
  int Pet_nextlvlexp = PET_EXP_LVL*(Pet_lvl+1)*(Pet_lvl+1)*(Pet_lvl+2);
  int Pet_currentlvlexp = PET_EXP_LVL*Pet_lvl*Pet_lvl*(Pet_lvl+1);
  
  int Pet_totalexpneedtoupgrade = Pet_nextlvlexp - Pet_currentlvlexp;
  int Pet_currentlvlgainedexp = Pet_exp - Pet_currentlvlexp;
  int Pet_currentgamegainedexp = Pet_exp - Pet_currentgameexp;
  if (Pet_currentgamegainedexp > Pet_currentlvlgainedexp) Pet_currentgamegainedexp = Pet_currentlvlgainedexp;

//  sprintf(temp_s,"PET:%d %d %d ",PET_EXP_LVL,Pet_currentlvlexp,Pet_nextlvlexp);
  sprintf(temp_s,"PET[%02d]:[%.2f%%%%][%.2f%%%%]%d", Pet_lvl,((float)Pet_currentlvlgainedexp*100)/((float)Pet_totalexpneedtoupgrade),((float)(Pet_currentgamegainedexp)*100)/((float)Pet_totalexpneedtoupgrade), Pet_currentgamegainedexp);
  wsprintfW2(D2ScreenHook[5].msg, temp_s);
  D2ScreenHook[5].y = D2GetScreenSizeY()-50;//550;
}

void __declspec(naked) UpdateExp_ASM()
{
  DWORD return_address;
    __asm {
    pop return_address
    call D2COMMON_UpdatePlayerState
    push return_address
    cmp [tToggleShowExp.flag],0
    je not_show
    pushad
    call UpdateExp
    popad
not_show:
    ret
  }
}


/*
Auto Party Staff:
Packet Type 0x07
Player To Player Relations
Lengh = 40

5A 07 02 1B B1 2E 14 05 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
(BYTE) Packet Type (0x07)
(BYTE) Action Type
(DWORD) Players ID
(BYTE) Player's Action
(NULLSTRAND[16]) Blank
(NULLSTRAND[16]) Blank

The server will send you this packet to notify you of any changes that a D2 client would use to set up partys/hostile players/looting etc.
This packet is a very important packet when it comes to party relations and should be the packet you take note of when adding party support into your bot.
Action Type 0x00 - Nutural
Player's Actions:
0x04 = Player has removed Hostile

Action Type 0x02 - teaming
Player's Actions:
0x05 = Player Is Asking To Party With You
0x07 = New Player Has Joined Party
0x08 = Your Now In This Players party?

Action Type 0x04 - friendly
Player's Actions:
0x02 = Player has given you loot

Action Type 0x08 - nasty
Player's Actions:
0x03 = Player has gone hostile

Action Type 0x09 - remove
Player's Actions:
0x09 = Player has left your party
0x06 = Player has removed party invite
0x0B = Player has removed Loot


0x5E
Party Request's

5E 08 3E 82 EB 2B 
(BYTE) Action type
(DWORD) Player ID (Of The Other Persion)

Action Type's
0x06 = Invite A Player To Party With You
0x07 = Cancel A Party Invite To Another Player
0x08 = Accept A Party Invite From Another Player


*/

//5A 07 02 1B B1 2E 14 05 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 

void __stdcall AutoParty(BYTE *aPacket)
{
  
  if (aPacket[1] == 0x07 && aPacket[2] == 0x02 && aPacket[7] == 0x05 && tToggleAutoParty.flag)
  {
    // Player Is Asking To Party With You
    // Player ID = aPacket[3~6]
    // Send packet back: 5E 08 xx xx xx xx
    BYTE acceptInvite[6] = {0x5E,0x08};
    acceptInvite[2] = aPacket[3];
    acceptInvite[3] = aPacket[4];
    acceptInvite[4] = aPacket[5];
    acceptInvite[5] = aPacket[6];

    D2NET_SendPacket(0,acceptInvite,sizeof(acceptInvite));
  }
}

void __declspec(naked) RecvCommand_5A_Patch_ASM()
{
  __asm
  {
    //esi = packet first byte(packet type)
    //edx = packet length
    //ecx,edi = cmdbuf
    pushad
    push ecx;
    call AutoParty;
    popad
    push 0x6FAB3930
    ret
  }
}

void __stdcall BugKBProtect(BYTE *aPacket)
{
  /*
  Baal转身下去：5D 24 00 03 00 00 
  */
  if (aPacket[1] == 0x24 && aPacket[2] == 0x00 && aPacket[3] == 0x03 && tToggleBugKB.flag>=1 && BugKB==1)
  {
      DWORD act = GetCurrentAct(GetUnitDrlgLevel(D2CLIENT_GetPlayerUnit()))-1;
      DWORD nCurrentAct = act < 0 ? 0 : act;
      if (nCurrentAct == 4)
      {
        D2CLIENT_PrintGameStringAtTopLeft(L"BugKB: Force quit because some guys try to kill my lovely Baal", 8);
        D2ExitGame(0);
      }
  }
}

void __declspec(naked) RecvCommand_5D_Patch_ASM()
{
  __asm
  {
    //esi = packet first byte(packet type)
    //edx = packet length
    //ecx,edi = cmdbuf
    pushad
    push ecx;
    call BugKBProtect
    popad
    push 0x6FAB3E00
    ret
  }
}

void __stdcall BugKDProtect(BYTE *aPacket)
{
  /*
  KD打开最后的封印： 89 0C
  */
  if (aPacket[1] == 0x0C && tToggleBugKD.flag >= 1 && BugKD==1)
  {
      DWORD act = GetCurrentAct(GetUnitDrlgLevel(D2CLIENT_GetPlayerUnit()))-1;
      DWORD nCurrentAct = act < 0 ? 0 : act;
      if (nCurrentAct == 3)
      {
        D2CLIENT_PrintGameStringAtTopLeft(L"BugKD: Force quit because some guys try to kill my lovely Diablo", 8);
        ForceBugKDQuit=1;
      }
  }
}

void __declspec(naked) RecvCommand_89_Patch_ASM()
{
  __asm
  {
    //esi = packet first byte(packet type)
    //edx = packet length
    //ecx,edi = cmdbuf
    pushad
    push ecx;
    call BugKDProtect
    popad
    push 0x6FAB4340
    ret
  }
}

void __fastcall PacketHandler(DWORD len, BYTE *cmdbuf)
{
  char buf[1024];
  char tempbuf[10];
  wchar_t temp[2048];

  buf[0] = 0;
  strcpy(buf,"->");

  // 0x4D
  // 0x7A
  // 0x94
  // 0xA7
  if (len<100 && tToggleShowPacket.flag)
  {
    for (unsigned int i=0;i<len;i++)
    {
      sprintf(tempbuf,"%02X ",cmdbuf[i]);
      strcat(buf,tempbuf);
    }

    wsprintfW2(temp, buf);
    D2CLIENT_PrintGameStringAtTopLeft(temp, 0);
//    strcat(buf,"\n");
//    OutputDebugString(buf);
  }
//  else
//  {
//    sprintf(buf,"Long Packet(%02X,%d)",cmdbuf[0],len);
//    wsprintfW2(temp, buf);
//    D2CLIENT_PrintGameStringAtTopLeft(temp, 0);
//  }
}


void __declspec(naked) PacketHandler_ASM()
{
  // esi = cmdbuf
  // ecx = len
  __asm {
    
    pushad
    mov edx,esi
    call PacketHandler
    popad
    mov edx,esi
    mov ecx,ebp
    call eax
    mov eax,0x6FAB56B9
    jmp eax
  }
}

void __declspec(naked) PacketHandler1_ASM()
{
  // ebx = packnum
  // ebp = cmdbuf
  // eax = len
  __asm {
    
    pushad
    mov     ecx, 0x6FB76274[edx*4]
    mov edx,ebp
    call PacketHandler
    popad
    mov     eax, 0x6FB76274[edx*4]
    push 0x6FAB512A
    ret
  }
}

//automap stuff
BOOL fAutomapLayerInitialized;
void __declspec(naked) __fastcall InitAutomapLayer1(DWORD levelno)
{
  __asm {
    //ecx = level no
    mov eax,D2CLIENT_InitAutomapLayer_END
    mov byte ptr [eax],INST_RET
    pushad
    xor ebp,ebp
    call D2CLIENT_InitAutomapLayer
    popad
    mov byte ptr [eax],0x8b
    ret
  }
}

void InitAutomapLayer(DWORD levelno)
{
  if (!fAutomapLayerInitialized) {
    fAutomapLayerInitialized = VirtualProtect(D2CLIENT_InitAutomapLayer_END, 1, PAGE_EXECUTE_READWRITE);
  }
  InitAutomapLayer1(levelno);
}


DWORD GetPresetUnitObjCell(DWORD objno, DWORD levelno)
{
  if (objno == 397) return 318; //chests
  if (objno == 371) return 301;//318; //countess chest
  if (objno == 152) return 300; //orifice
  if (objno == 460) return 1468; //drehya = frozen anya
  if ((objno == 402) && (levelno == 46)) return 0; //canyon/arcane waypoint
  if ((objno == 267) && (levelno != 75) && (levelno != 103)) return 0; //bank not act3,4 town
  if (objno == 462) return 1472; //nihlathak
  if ((objno == 376) && (levelno == 107)) return 745; // hell forge

  //FIXME
  if (objno >= 574) { //random
    static WORD rndobjcells[9] = { //rnd obj no, new obj no, shrine min, shrine max (not inclusive)
      CELLNO_MYSHRINES+0,  //574 = 310 (shrine), 2,  6
      CELLNO_MYSHRINES+7,  //575 = 310 (shrine), 7,  7
      CELLNO_MYSHRINES+0,  //576 = 310 (shrine), 8,  11
      CELLNO_MYSHRINES+12, //575 = 310 (shrine), 12, 12
      CELLNO_MYSHRINES+0,  //578 = 310 (shrine), 1,  5
      CELLNO_MYSHRINES+14, //579 = 310 (shrine), 14, 14
      0, //580 = chest
      0, //581 = chest
      0, //582 = arcane symbol
    };
    return rndobjcells[objno-574];
  }
  ObjectTxt *obj = D2COMMON_GetObjectTxt(objno);
  DWORD cellno = obj->nAutoMap;
  if (cellno == CELLNO_SHRINE) {
    static WORD parm0cells[4] = {
      CELLNO_MYSHRINES+0, //0 = ??
      CELLNO_MYSHRINES+2, //1 = health
      CELLNO_MYSHRINES+3, //2 = mana
      CELLNO_MYSHRINES+0, //3 = magic
    };
    cellno = (obj->nSubClass & 1) ? parm0cells[obj->nParm0] : 0;
  }
  return cellno;
}

DrlgRoom2 *GetRoomTileOtherRoom2(DrlgRoom2 *room2, DWORD roomtileno)
{
  RoomTile *roomtile = room2->pRoomTiles;
  while (roomtile) {
    if (*roomtile->nNum == roomtileno) {
      return roomtile->pRoom2;
    }
    roomtile = roomtile->pNext;
  }
  return 0;
}


void AddMinimapCell(AutomapCell *cell, DrlgLevel *pDrlgLevel);
void AddAutomapRoomCell(int xpos, int ypos, int cellno, DrlgRoom2 *room2, int xoffset = 0, int yoffset = 0)
{
  AutomapCell *cell = D2CLIENT_NewAutomapCell();

  cell->nCellNo = cellno;
  xpos = (room2->xPos*5)+xpos;
  ypos = (room2->yPos*5)+ypos;
  UnitAny* pla = D2CLIENT_GetPlayerUnit();
  cell->xPixel = (((xpos-ypos)*16)/10)+1+xoffset;
  cell->yPixel = (((ypos+xpos)*8)/10)-3+yoffset;
  D2CLIENT_AddAutomapCell(cell, &D2CLIENT_pAutomapLayer->pObjects);

  //if(room2->pDrlgLevel->nLevelNo == 107)
  //{
  //  char szLevelName[1024];
  //  LevelTxt* lt = D2COMMON_GetLevelTxt(cellno ? cellno : -cellno);
  //  if(!lt)
  //  {
  //    sprintf(szLevelName, "cellno = %d\n", cellno > 0 ? cellno : -cellno);
  //    OutputDebugString(szLevelName);
  //  }
  //}

  AddMinimapCell(cell, room2->pDrlgLevel);
}

void ResetAutomapCellNo2(AutomapCell* cell)
{
  if (cell->nCellNo >= CELLNO_MYSHRINES && cell->nCellNo < CELLNO_MYSHRINES+23) {
    cell->nCellNo = CELLNO_SHRINE;
  } else if ((short)cell->nCellNo < 0) {
    cell->nCellNo = 0;
  }
}

void ResetAutomapCellNo(AutomapCell* cell)
{
  if (cell) {
    ResetAutomapCellNo(cell->pLess);
    ResetAutomapCellNo2(cell);
    ResetAutomapCellNo(cell->pMore);
  }
}

void ResetAutomapCell()
{
  for (AutomapCellNode *cellnode = D2CLIENT_pAutomapCellsHeader; cellnode != NULL; cellnode = cellnode->pNext) {
    if (cellnode->pAutomapCell) {
      ResetAutomapCellNo(cellnode->pAutomapCell->pLess);
      ResetAutomapCellNo2(cellnode->pAutomapCell);
      ResetAutomapCellNo(cellnode->pAutomapCell->pMore);
    }
  }
}

void RevealAutomapRoom2(DrlgRoom2 *room2)
{
  D2CLIENT_RevealAutomapRoom(room2->pRoom1, 1, D2CLIENT_pAutomapLayer);

  PresetUnit *unit = room2->pPresetUnits;
  while (unit) {
    int cellno = 0, xoffset = 0, yoffset = 0;
    switch (unit->nUnitType) {
      case UNITNO_MONSTER:
        if (unit->nTxtFileNo == 256) cellno = 300; //izual
//        else if (unit->nTxtFileNo == 745) cellno = unit->nTxtFileNo; //Hephasto The Armorer at Hell Forge
        break;

      case UNITNO_OBJECT:
        cellno = GetPresetUnitObjCell(unit->nTxtFileNo, room2->pDrlgLevel->nLevelNo);
        break;

      case UNITNO_ROOMTILE:
        DrlgRoom2 *otherroom2 = GetRoomTileOtherRoom2(room2, unit->nTxtFileNo);
        if (otherroom2) {
          //change pos here because better for minimap
          cellno = -(int)(otherroom2->pDrlgLevel->nLevelNo);
          xoffset = +8;
          yoffset = -28;
        }
        break;
    }
    if (cellno) {
      AddAutomapRoomCell(unit->xPos, unit->yPos, cellno, room2, xoffset, yoffset);
    }
    unit = unit->pNext;
  }
}

void RevealAutomapRoom(DrlgRoom2 *room2)
{
  if (room2->nPresetType == 2) {
    DWORD presetno = *room2->nPresetType2No;
    if (presetno >= 38 && presetno <= 39) return; //Act 1 - Swamp Fill 1&2
    if (presetno >= 401 && presetno <= 405) return; //Act 2 - Desert Fill Bone 1&2, Desert Fill Wagon 1, Desert Fill Berms 1&2
    if (presetno == 836) return; //Act 4 - Lava X
    if (presetno == 863) return; //Act 5 - Town
  }
  if (room2->pRoom1) {
    RevealAutomapRoom2(room2);
  } else {
    BYTE cmdbuf[6];
    *(WORD *)(cmdbuf+1) = (WORD)room2->xPos;
    *(WORD *)(cmdbuf+3) = (WORD)room2->yPos;
    cmdbuf[5] = (BYTE)room2->pDrlgLevel->nLevelNo;
    D2CLIENT_RecvCommand07(cmdbuf);
    RevealAutomapRoom2(room2);
    D2CLIENT_RecvCommand08(cmdbuf);
  }
}

void RevealAutomapLevel(DrlgLevel *pDrlgLevel)
{
  DrlgRoom2 *currroom = pDrlgLevel->pRoom2First;
  while (currroom) {
    RevealAutomapRoom(currroom);
    currroom = currroom->pNext;
  }
}

DrlgLevel *GetUnitDrlgLevel(UnitAny *unit)
{
  if (unit && unit->pPos->pRoom1) {
    return unit->pPos->pRoom1->pRoom2->pDrlgLevel;
  }
  return NULL;
}

void RevealAutomapLevel()
{
  RevealAutomapLevel(GetUnitDrlgLevel(D2CLIENT_GetPlayerUnit()));
}

static BYTE actlvls[] = {1, 40, 75, 103, 109, 137};
DWORD GetCurrentAct(DrlgLevel* currlvl)
{
  if (!currlvl) return 0;

  DWORD l = currlvl->nLevelNo;
  DWORD act = 0;
  do {} while (l >= actlvls[++act]);
  return act;
}
BYTE afRevealedActs[5];

void RevealAutomapAct()
{
  DWORD currlvl = GetUnitDrlgLevel(D2CLIENT_GetPlayerUnit())->nLevelNo;
  DWORD act = 0;
  do {} while (currlvl >= actlvls[++act]);
  
  for (DWORD lvl = actlvls[act-1]; lvl < actlvls[act]; lvl++) {
    DrlgLevel *pDrlgLevel = D2COMMON_GetDrlgLevel(D2CLIENT_pDrlgAct->pDrlgMisc, lvl);
    if (!pDrlgLevel) break;
    if (!pDrlgLevel->pRoom2First) {
      D2COMMON_InitDrlgLevel(pDrlgLevel);
    }
    InitAutomapLayer(lvl);
    RevealAutomapLevel(pDrlgLevel);
  }

  InitAutomapLayer(currlvl);
  afRevealedActs[act-1] = 1;
}

void __fastcall OverrideShrinePatch(AutomapCell *newnode, AutomapCell *currnode)
{
  if ((newnode->nCellNo < CELLNO_MYSHRINES) || (newnode->nCellNo >= (CELLNO_MYSHRINES+NUMOF_SHRINES))) return;
  if (currnode->nCellNo >= newnode->nCellNo) return;
  currnode->fSaved = newnode->fSaved;
  currnode->nCellNo = newnode->nCellNo;
}

void __declspec(naked) OverrideShrinePatch_ASM()
{
  __asm {
//esi = ptr curr node
//ecx = ptr new node
    pushad
    mov edx,esi
    call OverrideShrinePatch
    popad
//original code
    mov edx,D2CLIENT_OverrideShrinePatch_ORIG
    mov edx,[4*eax+edx]
    ret
  }
}

//shrines stuff

CellFile *apMyShrineCells[2][NUMOF_SHRINES];
CellFile *pWaypointCell;
CellFile *pMinimapObjectCell;
void InitShrines()
{
  char filename[MAX_PATH];
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < NUMOF_SHRINES; j++) {
      wsprintf(GetMyFileNameStrrchr(filename, '\\'), "MINISHRINE%.2d.BMP"+(i?0:4), j);
      apMyShrineCells[i][j] = InitCellFile(LoadBmpCellFile(filename));
    }
  }
  wsprintf(GetMyFileNameStrrchr(filename, '\\'), "WAYPOINT.BMP");
  pWaypointCell = InitCellFile(LoadBmpCellFile(filename));
  wsprintf(GetMyFileNameStrrchr(filename, '\\'), "BLOBCELL.BMP");
  pMinimapObjectCell = InitCellFile(LoadBmpCellFile(filename));
  if(!pMinimapObjectCell) {
    wsprintf(GetMyFileNameStrrchr(filename, '\\'), "BLOBDOT.BMP");
    pMinimapObjectCell = InitCellFile(LoadBmpCellFile(filename));
  }
}

void DeleteShrines()
{
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < NUMOF_SHRINES; j++) {
      DeleteCellFile(apMyShrineCells[i][j]);
    }
  }
  DeleteCellFile(pWaypointCell);
  DeleteCellFile(pMinimapObjectCell);
}

void __declspec(naked) AddShrinePatch_ASM()
{
  __asm {
//esi = ptr unit object
//edi = ptr object.txt
//return edx = cell no
//    mov edx,[edi]ObjectTxt.nAutoMap //original code
    mov edx,[edi+0x1BC]
    cmp edx,CELLNO_SHRINE
    jne notshrine
    xor edx,edx
//    test [edi]ObjectTxt.nSubClass,1
    test [edi+0x166],1
    je notshrine
//    mov eax,[esi]UnitAny.pObjectData
    mov eax,[esi+0x14]
    movzx edx,[eax]ObjectData.nShrineNo
    add edx,CELLNO_MYSHRINES
notshrine:
    ret
  }
}

//draw automap cave names/shrines

BYTE nCaveNameTextCol = 0;

BYTE *szStaffTombLvlDesc;

void DrawAutomapCellHint(int cellno, DWORD xpos, DWORD ypos);

void __stdcall DrawAutomapCellPatch(CellContext *pCellContext, DWORD xpos, DWORD ypos, RECT *cliprect, DWORD bright)
{
  int cellno = pCellContext->nCellNo;
  //if(cellno)
  //{
  //  char szLevelName[1024];
  //  LevelTxt* lt = D2COMMON_GetLevelTxt(cellno ? cellno : -cellno);
  //  if(!lt)
  //  {
  //    sprintf(szLevelName, "cellno = %d\n", cellno ? cellno : -cellno);
  //    OutputDebugString(szLevelName);
  //  }
  //}
  if (cellno < 0) {
    if (tToggleAutomapLevelNames.flag) {
      wchar_t temp[1024];
      wcscpy(temp, D2COMMON_GetLevelTxt(-cellno)->szName);
      if (szStaffTombLvlDesc && (-cellno == (int)D2CLIENT_pDrlgAct->pDrlgMisc->nStaffTombLvl))
        ConvertToD2String(temp+wcslen(temp), szStaffTombLvlDesc, 0, &temp[ARRAYSIZE(temp)-1]);
      D2DrawCenterText(6, temp, xpos, ypos, (nCaveNameTextCol > 12) ? 0 : nCaveNameTextCol);
    }
    return;
  }
  if (cellno == CELLNO_WAYPOINT && pWaypointCell != 0)
  {
    pCellContext->nCellNo = 0;
    pCellContext->pCellFile = pWaypointCell;
    xpos += (D2CLIENT_GetAutomapSize() ? 4 : 8)-(pWaypointCell->cells[0]->width/2);
    ypos += (D2CLIENT_GetAutomapSize() ? 4 : 0);
  }
  if (((cellno -= CELLNO_MYSHRINES) >= 0) && (cellno < NUMOF_SHRINES)) {
    CellFile *myshrine = apMyShrineCells[D2CLIENT_GetAutomapSize() & tToggleMiniShrine.flag][cellno];
    if (myshrine) {
      pCellContext->pCellFile = myshrine;
      pCellContext->nCellNo = 0;
      xpos += (D2CLIENT_GetAutomapSize() ? 4 : 8)-(myshrine->cells[0]->width/2);
    } else {
      pCellContext->nCellNo = CELLNO_SHRINE;
    }
  }
  D2GFX_DrawAutomapCell(pCellContext, xpos, ypos, cliprect, bright);
}

//visuals stuff

void __declspec(naked) WeatherPatch_ASM()
{
  __asm {
    cmp [tToggleFullVisuals.flag],0
    je rainold
    xor al,al
rainold:
    ret 0x04
  }
}

void __declspec(naked) LosDrawPatch_ASM()
{
  __asm {
    cmp [tToggleFullVisuals.flag],0
    je losdrawold
    xor eax,eax
losdrawold:
    ret 0x04
  }
}

void __declspec(naked) LightingPatch_ASM()
{
  __asm {
//edx = ptr red
//[esp+0x0c] = ptr green
//[esp+0x10] = ptr blue
//return eax = intensity
    cmp [tToggleFullVisuals.flag],0
    je lightold

    mov eax,0xFF
    mov [edx],al
    mov esi,[esp+0x0c]
    mov [esi],al
    mov esi,[esp+0x10]
    mov [esi],al
    pop esi
    pop esi
    ret 0x08
lightold:
//original code
    push ecx
    mov esi, edx
    call D2COMMON_GetLevelIdFromRoom
    ret
  }
}

//map scroll stuff (also visuals shake)

int xMapOffset = 0, yMapOffset = 0;

BYTE nMapScrollSpeed = 8;

void __fastcall ShakeScreenPatch(DWORD *xpos, DWORD *ypos)
{
  D2CLIENT_CalcShake(xpos, ypos);
  if (tToggleScreenshake.flag) {
    D2CLIENT_xMapShake = 0;
    D2CLIENT_yMapShake = 0;
  }
  if (tToggleScrollMap.flag) {
    D2CLIENT_xMapShake += xMapOffset;
    D2CLIENT_yMapShake += yMapOffset;
  }

  *xpos += D2CLIENT_xMapShake;
  *ypos += D2CLIENT_yMapShake;
}

void ScrollMap(char xinc, char yinc)
{
  if (tToggleScrollMap.flag) {
    xMapOffset += xinc*nMapScrollSpeed;
    yMapOffset += yinc*nMapScrollSpeed;
  } else if (D2CLIENT_fAutomapOn) {
    D2CLIENT_ptAutomap.x += xinc*8;
    D2CLIENT_ptAutomap.y += yinc*8;
  }
}

void CenterMap()
{
  xMapOffset = 0;
  yMapOffset = 0;
}

//lifebar stuff

BYTE nLifeBarTrans = -1, nLifeBarColour = -1;

void __declspec(naked) LifeBarPatch_ASM()
{
  __asm {
    pop eax
//originalish code
    movzx edx,[nLifeBarTrans]
    cmp dl,-1
    jne nodefault1
    mov dl,1
nodefault1:
    push edx
    mov dl,[nLifeBarColour]
    cmp dl,-1
    jne nodefault2
    mov dl,5
nodefault2:
    push edx
    sub edi,ecx
    jmp eax
  }
}

//shared item vars

char nQuestItemColour = -1;
char nUnk1ItemColour = 1;
int nItemValueNpc = 0x201;
char nMonsterColor = -1;
typedef char ItemColours[8][2][7][2];

static union {
  ItemColours anItemColours[3000];
  struct {
    ItemColours anItemWeaponCols[1000];
    ItemColours anItemArmorCols[1000];
    ItemColours anItemMiscCols[1000];
  };
};

char anRuneColours[100][2];

int nGoldGoodNum = 0; char nGoldGoodCol[2] = {-1,-1}, nGoldPoorCol[2] = {-1,-1};

void InitItemColours()
{
}

int GetItemColourIndex(int pos)
{
  return (pos < D2COMMON_nWeaponsTxt) ? pos :
  ((pos -= D2COMMON_nWeaponsTxt) < D2COMMON_nArmorTxt) ? pos+ARRAYSIZE(anItemWeaponCols) :
  pos - D2COMMON_nArmorTxt + ARRAYSIZE(anItemWeaponCols) + ARRAYSIZE(anItemArmorCols);
}

ItemColours *GetItemColour2(int pos)
{
  return (pos < D2COMMON_nWeaponsTxt) ? &anItemWeaponCols[pos] :
    ((pos -= D2COMMON_nWeaponsTxt) < D2COMMON_nArmorTxt) ? &anItemArmorCols[pos] :
    &anItemMiscCols[pos - D2COMMON_nArmorTxt];
}

char *GetItemColour(UnitAny *item)
{
  ItemTxt *itemtxt = D2COMMON_GetItemTxt(item->nTxtFileNo);

  if (itemtxt->dwCode == D2TXTCODE('gld ')) {
    return (D2COMMON_GetUnitStat(item, STAT_GOLD, 0) >= nGoldGoodNum) ? nGoldGoodCol : nGoldPoorCol;
  }

  if (itemtxt->nType == 74) {
    DWORD runeno = (itemtxt->szCode[1]-'0')*10+(itemtxt->szCode[2]-'0')-1;
    if (runeno < ARRAYSIZE(anRuneColours)) return anRuneColours[runeno];
  }

  int index = GetItemColourIndex(item->nTxtFileNo);
  if(index > ARRAYSIZE(anItemColours)-1 ) index = ARRAYSIZE(anItemColours)-1;
  int socknum = D2COMMON_GetUnitStat(item, STAT_NUMSOCKETS, 0);
  if( socknum > 6) socknum = 6;
  
  return anItemColours[index][(item->pItemData->nQuality-1)&7][!!D2COMMON_GetItemFlag(item, ITEMFLAG_ETHEREAL, 0, "?")][socknum];
}

//items name stuff
BYTE *szEtherealItemPrefix = NULL;
BYTE *szEtherealItemPostfix = NULL;
void __fastcall ItemNamePatch(wchar_t *name, UnitAny *item)
{
  wchar_t temp[0x40];
  int itemno = item->nTxtFileNo, quality = item->pItemData->nQuality, col = -1, bktnum = -1;

  ItemTxt *itemtxt = D2COMMON_GetItemTxt(itemno);

  trimspaces(name);

  col = GetItemColour(item)[0];

  if ((col == -1) && itemtxt->fQuest && (itemtxt->dwCode != D2TXTCODE('leg '))) col = nQuestItemColour;

  if (D2COMMON_GetItemFlag(item, STAT_UNK1, 0, "?")) col = nUnk1ItemColour;

  // gold
  if (itemtxt->dwCode == D2TXTCODE('gld ')) {
    // FIXME: name+0x40
    wcsrcat(name, wsprintfW2(temp, "%d ", D2COMMON_GetUnitStat(item, STAT_GOLD, 0)));
  }

  // ethereal
  if (D2COMMON_GetItemFlag(item, ITEMFLAG_ETHEREAL, 0, "?"))
  {
    if (szEtherealItemPrefix)
    {
      ConvertToD2String(temp, szEtherealItemPrefix, col, &temp[ARRAYSIZE(temp)-1]);
      wcsrcat(name, temp);
    }
    if (szEtherealItemPostfix)
      ConvertToD2String(name+wcslen(name), szEtherealItemPostfix, col, name+128);
  }
  
  // rune numbers
  if (tToggleRuneNumbers.flag && (itemtxt->nType == 74)) {
    bktnum = (itemtxt->szCode[1]-'0')*10+(itemtxt->szCode[2]-'0');
    wsprintfW2(name+wcslen(name), " (%d)", bktnum);
  }

  // socket number
  if (tToggleSocketNumbers.flag && D2COMMON_GetItemFlag(item, ITEMFLAG_SOCKETED, 0, "?")) {
    wsprintfW2(name+wcslen(name), " (%d)", D2COMMON_GetUnitStat(item, STAT_NUMSOCKETS, 0));
  }

  // item level
  if (tToggleItemLevel.flag && (itemtxt->dwCode != D2TXTCODE('gld '))) {
    wsprintfW2(name+wcslen(name), " (%ld)", item->pItemData->nItemLevel);
  }


  // item value
  if (tToggleItemValue.flag && (itemtxt->fQuest == 0) && (itemtxt->dwCode != D2TXTCODE('gld ')) ) {
    if (!D2CLIENT_GetUiVar(0x0C)) {
      int value = D2COMMON_GetItemValue( D2CLIENT_GetPlayerUnit(), item, D2CLIENT_GetDifficulty(), D2CLIENT_GetQuestInfo(), nItemValueNpc, 1);
      wsprintfW2(name+wcslen(name), " ($%d)", value);
    }
  }

  // unit number
  if (tToggleUnitNumber.flag) {
    wsprintfW2(name+wcslen(name), " [0x%X]", item->nUnitId);
  }

  // item index
  if (tToggleItemIndex.flag) {
    wsprintfW2(name+wcslen(name), " [i%d]", GetItemColourIndex(item->nTxtFileNo)+1);
  }

  if (col != -1) ColourD2String(name, col);

}

void __declspec(naked) ItemNamePatch_ASM()
{
  __asm {
//ebx = ptr unit item
//ecx = ptr item name string
    mov edx,ebx
    jmp ItemNamePatch
  }
}


//hide items stuff

BYTE __fastcall HideItemsPatch(UnitAny *unit)
{
  if (!tToggleHiddenItems.flag && (unit->nUnitType == UNITNO_ITEM)) {
    return (GetItemColour(unit)[1] == -2);
  }
  return 0;
}

void __declspec(naked) HideItemsPatch_ASM()
{
  __asm {
//esi = ptr unit
    mov [esi+0xec],eax //original code
    mov ecx,esi
    call HideItemsPatch
    test al,al
    je showitem
    add dword ptr [esp],-0x29
showitem:
    ret
  }
}

//monsters automap blob stuff

#define _0 -2
#define _1 -1

BYTE anMonsterColours[1000] = {
  _1, //0 Skeleton
  _1, //1 Returned
  _1, //2 BoneWarrior
  _1, //3 BurningDead
  _1, //4 Horror
  _1, //5 Zombie
  _1, //6 HungryDead
  _1, //7 Ghoul
  _1, //8 DrownedCarcass
  _1, //9 PlagueBearer
  _1, //10 Afflicted
  _1, //11 Tainted
  _1, //12 Misshapen
  _1, //13 Disfigured
  _1, //14 Damned
  _1, //15 FoulCrow
  _1, //16 BloodHawk
  _1, //17 BlackRaptor
  _1, //18 CloudStalker
  _1, //19 Fallen
  _1, //20 Carver
  _1, //21 Devilkin
  _1, //22 DarkOne
  _1, //23 WarpedFallen
  _1, //24 Brute
  _1, //25 Yeti
  _1, //26 Crusher
  _1, //27 WailingBeast
  _1, //28 GargantuanBeast
  _1, //29 SandRaider
  _1, //30 Marauder
  _1, //31 Invader
  _1, //32 Infidel
  _1, //33 Assailant
  _1, //34 unused, Gorgon
  _1, //35 unused, StoneStalker
  _1, //36 unused, SerpentQueen
  _1, //37 unused, StygianWatcher
  _1, //38 Ghost
  _1, //39 Wraith
  _1, //40 Specter
  _1, //41 Apparition
  _1, //42 DarkShape
  _1, //43 DarkHunter
  _1, //44 VileHunter
  _1, //45 DarkStalker
  _1, //46 BlackRogue
  _1, //47 FleshHunter
  _1, //48 DuneBeast
  _1, //49 RockDweller
  _1, //50 JungleHunter
  _1, //51 DoomApe
  _1, //52 TempleGuard
  _1, //53 MoonClan
  _1, //54 NightClan
  _1, //55 BloodClan
  _1, //56 HellClan
  _1, //57 DeathClan
  _1, //58 FallenShaman
  _1, //59 CarverShaman
  _1, //60 DevilkinShaman
  _1, //61 DarkShaman
  _1, //62 WarpedShaman
  _1, //63 QuillRat
  _1, //64 SpikeFiend
  _1, //65 ThornBeast
  _1, //66 RazorSpine
  _1, //67 JungleUrchin
  _1, //68 SandMaggot
  _1, //69 RockWorm
  _1, //70 Devourer
  _1, //71 GiantLamprey
  _1, //72 WorldKiller
  _1, //73 TombViper
  _1, //74 ClawViper
  _1, //75 Salamander
  _1, //76 PitViper
  _1, //77 SerpentMagus
  _1, //78 SandLeaper
  _1, //79 CaveLeaper
  _1, //80 TombCreeper
  _1, //81 TreeLurker
  _1, //82 RazorPitDemon
  _1, //83 Huntress
  _1, //84 SaberCat
  _1, //85 NightTiger
  _1, //86 HellCat
  _1, //87 Itchies
  _1, //88 BlackLocusts
  _1, //89 PlagueBugs
  _1, //90 HellSwarm
  _1, //91 DungSoldier
  _1, //92 SandWarrior
  _1, //93 Scarab
  _1, //94 SteelWeevil
  _1, //95 AlbinoRoach
  _1, //96 DriedCorpse
  _1, //97 Decayed
  _1, //98 Embalmed
  _1, //99 PreservedDead
  _1, //100 Cadaver
  _1, //101 HollowOne
  _1, //102 Guardian
  _1, //103 Unraveler
  _1, //104 Horadrim Ancient
  _1, //105 Baal Subject Mummy
  _1, //106 unused, DamnedHorde
  _1, //107 unused, TwistedHorde
  _1, //108 unused, WickedHorde
  _1, //109 unused, UnholyHorde
  _1, //110 CarrionBird
  _1, //111 UndeadScavenger
  _1, //112 HellBuzzard
  _1, //113 WingedNightmare
  _1, //114 Sucker
  _1, //115 Feeder
  _1, //116 BloodHook
  _1, //117 BloodWing
  _1, //118 Gloam
  _1, //119 SwampGhost
  _1, //120 BurningSoul
  _1, //121 BlackSoul
  _1, //122 Arach
  _1, //123 SandFisher
  _1, //124 PoisonSpinner
  _1, //125 FlameSpider
  _1, //126 SpiderMagus
  _1, //127 ThornedHulk
  _1, //128 BrambleHulk
  _1, //129 Thrasher
  _1, //130 Spikefist
  _1, //131 GhoulLord
  _1, //132 NightLord
  _1, //133 DarkLord
  _1, //134 BloodLord
  _1, //135 Banished
  _1, //136 DesertWing
  _1, //137 Fiend
  _1, //138 Gloombat
  _1, //139 BloodDiver
  _1, //140 DarkFamiliar
  _1, //141 RatMan
  _1, //142 Fetish
  _1, //143 Flayer
  _1, //144 SoulKiller
  _1, //145 StygianDoll
  _0, //146 DeckardCain
  _0, //147 Gheed
  _0, //148 Akara
  _0, //149 dummy, chicken
  _0, //150 Kashya
  _0, //151 dummy, rat
  _0, //152 Dummy, RogueTown
  _0, //153 Dummy, HellMeteor
  _0, //154 Charsi
  _0, //155 Warriv
  _1, //156 Andariel
  _0, //157 dummy, Smallbird
  _0, //158 dummy, Largebird
  _0, //159 dummy, Bat
  _1, //160 DarkRanger
  _1, //161 VileArcher
  _1, //162 DarkArcher
  _1, //163 BlackArcher
  _1, //164 FleshArcher
  _1, //165 DarkSpearwoman
  _1, //166 VileLancer
  _1, //167 DarkLancer
  _1, //168 BlackLancer
  _1, //169 FleshLancer
  _1, //170 SkeletonArcher
  _1, //171 ReturnedArcher
  _1, //172 BoneArcher
  _1, //173 BurningDeadArcher
  _1, //174 HorrorArcher
  _0, //175 Warriv
  _0, //176 Atma
  _0, //177 Drognan
  _0, //178 Fara
  _0, //179 dummy, Cow
  _1, //180 SandMaggotYoung
  _1, //181 RockWormYoung
  _1, //182 DevourerYoung
  _1, //183 GiantLampreyYoung
  _1, //184 WorldKillerYoung
  _0, //185 dummy, Camel
  _1, //186 Blunderbore
  _1, //187 Gorbelly
  _1, //188 Mauler
  _1, //189 Urdar
  _1, //190 SandMaggotEgg
  _1, //191 RockWormEgg
  _1, //192 DevourerEgg
  _1, //193 GiantLampreyEgg
  _1, //194 WorldKillerEgg
  _0, //195 dummy, Act2Male
  _0, //196 Dummy, Act2Female
  _0, //197 dummy, Act2Child
  _0, //198 Greiz
  _0, //199 Elzix
  _0, //200 Geglash
  _0, //201 Jerhyn
  _0, //202 Lysander
  _0, //203 Dummy, Act2Guard
  _0, //204 dummy, Act2Vendor1
  _0, //205 dummy, Act2Vendor2
  _1, //206 FoulCrowNest
  _1, //207 BloodHawkNest
  _1, //208 BlackVultureNest
  _1, //209 CloudStalkerNest
  _0, //210 Meshif
  _1, //211 Duriel
  _1, //212 Undead RatMan
  _1, //213 Undead Fetish
  _1, //214 Undead Flayer
  _1, //215 Undead SoulKiller
  _1, //216 Undead StygianDoll
  _1, //217 unused, DarkGuard
  _1, //218 unused, DarkKnight
  _1, //219 unused, BloodGuard
  _1, //220 unused, BloodKnight
  _1, //221 unused, DarkPaladin
  _1, //222 unused, BloodMage
  _1, //223 unused, Demonist
  _1, //224 unused, BlackMagus
  _1, //225 unused, Diabolist
  _1, //226 unused, DeathMage
  _0, //227 Maggot
  _1, //228 MummyGenerator
  _1, //229 Radament
  _1, //230 unused, FireBeast
  _1, //231 unused, IceGlobe
  _1, //232 unused, LightningBeast
  _1, //233 unused, PoisonOrb
  _1, //234 FlyingScimitar
  _1, //235 Zakarumite
  _1, //236 Faithful
  _1, //237 Zealot
  _1, //238 Sexton
  _1, //239 Cantor
  _1, //240 Heirophant
  _1, //241 Heirophant
  _1, //242 Mephisto
  _1, //243 Diablo
  _0, //244 DeckardCain
  _0, //245 DeckardCain
  _0, //246 DeckardCain
  _1, //247 Swamp Dweller
  _1, //248 Bog Creature
  _1, //249 Slime Prince
  _1, //250 Summoner
  _0, //251 tyrael
  _0, //252 asheara
  _0, //253 hratli
  _0, //254 alkor
  _0, //255 ormus
  _1, //256 izual
  _0, //257 halbu
  _1, //258 WaterWatcherLimb
  _1, //259 RiverStalkerLimb
  _1, //260 StygianWatcherLimb
  _1, //261 WaterWatcherHead
  _1, //262 RiverStalkerHead
  _1, //263 StygianWatcherHead
  _0, //264 meshif
  _0, //265 DeckardCain
  _0, //266 navi (flavie)
  _1, //267 Bloodraven
  _0, //268 Dummy, bug
  _0, //269 Dummy, scorpion
  _1, //270 RogueScout
  _1, //271 Dummy, RogueHireling (act1 hireling)
  _0, //272 Dummy, RogueTownShoot
  _1, //273 GargoyleTrap
  _1, //274 ReturnedMage
  _1, //275 BoneMage
  _1, //276 BurningDeadMage
  _1, //277 HorrorMage
  _1, //278 RatManShaman
  _1, //279 FetishShaman
  _1, //280 FlayerShaman
  _1, //281 SoulKillerShaman
  _1, //282 StygianDollShaman
  _0, //283 larva
  _1, //284 SandMaggotQueen
  _1, //285 RockWormQueen
  _1, //286 DevourerQueen
  _1, //287 GiantLampreyQueen
  _1, //288 WorldKillerQueen
  _1, //289 ClayGolem
  _1, //290 BloodGolem
  _1, //291 IronGolem
  _1, //292 FireGolem
  _0, //293 Dummy, Familiar
  _0, //294 Dummy, Act3Male
  _1, //295 NightMarauder
  _0, //296 Dummy, Act3Female
  _0, //297 Natalya
  _1, //298 FleshSpawner
  _1, //299 StygianHag
  _1, //300 Grotesque
  _1, //301 FleshBeast
  _1, //302 StygianDog
  _1, //303 GrotesqueWyrm
  _1, //304 Groper
  _1, //305 Strangler
  _1, //306 StormCaster
  _1, //307 Corpulent
  _1, //308 CorpseSpitter
  _1, //309 MawFiend
  _1, //310 DoomKnight
  _1, //311 AbyssKnight
  _1, //312 OblivionKnight
  _1, //313 QuillBear
  _1, //314 SpikeGiant
  _1, //315 ThornBrute
  _1, //316 RazorBeast
  _1, //317 GiantUrchin
  _0, //318 Dummy, Snake
  _0, //319 Dummy, Parrot
  _0, //320 Dummy, Fish
  _0, //321 Dummy, Evil Hole
  _0, //322 Dummy, Evil Hole
  _0, //323 Dummy, Evil Hole
  _0, //324 Dummy, Evil Hole
  _0, //325 Dummy, Evil Hole
  _0, //326 a trap, trapfirebolt
  _0, //327 a trap
  _0, //328 a trap (act2 wall, act3 totem)
  _0, //329 a trap, trappoison
  _0, //330 a trap, trapchainlightning
  _0, //331 Kaelan
  _0, //332 Dummy, InvisoSpawner
  _1, //333 unused, DiabloClone
  _1, //334 SuckerNest
  _1, //335 FeederNest
  _1, //336 BloodHookNest
  _1, //337 BloodWingNest
  _1, //338 Guard (act2 hireling)
  _0, //339 Dummy, MiniSpider
  _1, //340 x, PrisonFL
  _1, //341 x, PrisonFR
  _1, //342 x, PrisonBL
  _1, //343 x, PrisonBR
  _0, //344 Dummy, Bonewall*
  _1, //345 Council Member
  _1, //346 Council Member
  _1, //347 Council Member
  _1, //348 Turret1
  _1, //349 Turret2
  _1, //350 Turret3
  _0, //351 Hydra1
  _0, //352 Hydra2
  _0, //353 Hydra3
  _1, //354 a trap, pillar
  _0, //355 Dummy, seven tombs
  _1, //356 Dopplezon
  _1, //357 Valkyrie
  _0, //358 Dummy, Act2Hireling
  _0, //359 Iron Wolf (act3 town+hireling)*
  _1, //360 Balrog
  _1, //361 PitLord
  _1, //362 VenomLord
  _1, //363 NecroSkeleton (necro skel)
  _1, //364 NecroMage (necro mage)
  _1, //365 Griswold
  _0, //366 compellingorb
  _0, //367 tyrael
  _0, //368 youngdiablo
  _0, //369 a trap, nova
  _1, //370 Dummy, SpiritMummy
  _1, //371 LightningSpire
  _1, //372 FireTower
  _1, //373 Slinger
  _1, //374 SpearCat
  _1, //375 NightSlinger
  _1, //376 HellSlinger
  _0, //377 Dummy, Boba Fett
  _0, //378 Dummy, Darth Maul
  _1, //379 ReturnedMage
  _1, //380 BoneMage
  _1, //381 BaalColdMage
  _1, //382 HorrorMage
  _1, //383 ReturnedMage
  _1, //384 BoneMage
  _1, //385 BurningDeadMage
  _1, //386 HorrorMage
  _1, //387 ReturnedMage
  _1, //388 BoneMage
  _1, //389 BurningDeadMage
  _1, //390 HorrorMage
  _1, //391 Hell Bovine
  _0, //392 x, window right (act4 window)*
  _0, //393 x, window left (act4 window)*
  _1, //394 SpearCat
  _1, //395 NightSlinger
  _1, //396 RatMan
  _1, //397 Fetish
  _1, //398 Flayer
  _1, //399 SoulKiller
  _1, //400 StygianDoll
  _0, //401 Dummy, Mephisto Spirit
  _1, //402 The Smith
  _0, //403 TrappedSoul*
  _0, //404 TrappedSoul*
  _0, //405 Jamella
  _0, //406 Izual's Ghost
  _1, //407 RatMan
  _0, //408 Malachai?
  _1, //409 The Feature Creep

  _0, //410 Wake of Destruction
  _0, //411 Charged Bolt Sentry
  _0, //412 Lightning Sentry
  _0, //413 Blade Creeper (blade sentinel)
  _0, //414 Invis Pet*
  _0, //415 Inferno Sentry
  _0, //416 Death Sentry
  _1, //417 Shadow Warrior
  _1, //418 Shadow Master
  _1, //419 Druid Hawk*
  _1, //420 Druid Spirit Wolf
  _1, //421 Druid Fenris (dire wolf)
  _1, //422 Spirit of Barbs
  _1, //423 Heart of Wolverine
  _1, //424 Oak Sage
  _1, //425 Druid Plague Poppy (poison creeper)*
  _1, //426 Druid Cycle of Life (carrion vine)*
  _1, //427 Vine Creature (solar creeper)*
  _1, //428 Druid Bear
  _1, //429 Eagle
  _1, //430 Wolf
  _1, //431 Bear
  _0, //432 Barricade Door*
  _0, //433 Barricade Door*
  _0, //434 Prison Door*
  _0, //435 Barricade Tower*
  _1, //436 RotWalker
  _1, //437 ReanimatedHorde
  _1, //438 ProwlingDead
  _1, //439 UnholyCorpse
  _1, //440 DefiledWarrior
  _1, //441 Siege Beast
  _1, //442 CrushBiest
  _1, //443 BloodBringer
  _1, //444 GoreBearer
  _1, //445 DeamonSteed
  _1, //446 SnowYeti1
  _1, //447 SnowYeti2
  _1, //448 SnowYeti3
  _1, //449 SnowYeti4
  _1, //450 WolfRider1
  _1, //451 WolfRider2
  _1, //452 WolfRider3
  _1, //453 Minionexp
  _1, //454 Slayerexp
  _1, //455 IceBoar
  _1, //456 FireBoar
  _1, //457 HellSpawn
  _1, //458 IceSpawn
  _1, //459 GreaterHellSpawn
  _1, //460 GreaterIceSpawn
  _1, //461 FanaticMinion
  _1, //462 BerserkSlayer
  _1, //463 ConsumedFireBoar
  _1, //464 ConsumedIceBoar
  _1, //465 FrenziedHellSpawn
  _1, //466 FrenziedIceSpawn
  _1, //467 InsaneHellSpawn
  _1, //468 InsaneIceSpawn
  _1, //469 Succubusexp
  _1, //470 VileTemptress
  _1, //471 StygianHarlot
  _1, //472 Hell Temptress
  _1, //473 Blood Temptress
  _1, //474 Dominus
  _1, //475 VileWitch
  _1, //476 StygianFury
  _1, //477 Blood Witch
  _1, //478 Hell Witch
  _1, //479 OverSeer
  _1, //480 Lasher
  _1, //481 OverLord
  _1, //482 BloodBoss
  _1, //483 HellWhip
  _1, //484 MinionSpawner
  _1, //485 MinionSlayerSpawner
  _1, //486 MinionIce/fireBoarSpawner
  _1, //487 MinionIce/fireBoarSpawner
  _1, //488 Minionice/hellSpawnSpawner
  _1, //489 MinionIce/fireBoarSpawner
  _1, //490 MinionIce/fireBoarSpawner
  _1, //491 Minionice/hellSpawnSpawner
  _1, //492 Imp1
  _1, //493 Imp2
  _1, //494 Imp3
  _1, //495 Imp4
  _1, //496 Imp5
  _1, //497 CatapultS
  _1, //498 CatapultE
  _1, //499 CatapultSiege
  _1, //500 CatapultW
  _1, //501 Frozen Horror1
  _1, //502 Frozen Horror2
  _1, //503 Frozen Horror3
  _1, //504 Frozen Horror4
  _1, //505 Frozen Horror5
  _1, //506 Blood Lord1
  _1, //507 Blood Lord2
  _1, //508 Blood Lord3
  _1, //509 Blood Lord4
  _1, //510 Blood Lord5
  _0, //511 Larzuk
  _0, //512 Drehya
  _0, //513 Malah
  _0, //514 Nihlathak Town
  _0, //515 Qual-Kehk
  _0, //516 Catapult Spotter S (target?)
  _0, //517 Catapult Spotter E (target?)
  _0, //518 Catapult Spotter Siege (target?)
  _0, //519 Catapult Spotter W
  _0, //520 DeckardCain
  _0, //521 tyrael
  0xcb, //522 Act 5 Combatant (barb fighter)*
  _0, //523 Act 5 Combatant*
  _0, //524 Barricade Wall Right*
  _0, //525 Barricade Wall Left*
  _1, //526 Nihlathak outside town
  _0, //527 Drehya outside town
  _1, //528 Evil hut
  _1, //529 Death Mauler1
  _1, //530 Death Mauler2
  _1, //531 Death Mauler3
  _1, //532 Death Mauler4
  _1, //533 Death Mauler5
  _0, //534 POW (captured barb)*
  _0, //535 Act 5 Townguard
  _0, //536 Act 5 Townguard
  _0, //537 Ancient Statue 1
  _0, //538 Ancient Statue 2
  _0, //539 Ancient Statue 3
  _1, //540 Ancient Barbarian 1
  _1, //541 Ancient Barbarian 2
  _1, //542 Ancient Barbarian 3
  _0, //543 Baal Throne
  _1, //544 Baal Crab
  _0, //545 Baal Taunt (invis follower)
  _1, //546 Putrid Defiler1
  _1, //547 Putrid Defiler2
  _1, //548 Putrid Defiler3
  _1, //549 Putrid Defiler4
  _1, //550 Putrid Defiler5
  _1, //551 Pain Worm1
  _1, //552 Pain Worm2
  _1, //553 Pain Worm3
  _1, //554 Pain Worm4
  _1, //555 Pain Worm5
  _0, //556 Bunny
  _1, //557 Council Member
  _1, //558 VenomLord
  _0, //559 Baal Crab to Stairs
  _1, //560 Act 5 Hireling 1hs
  _1, //561 Act 5 Hireling 2hs (act5 hireling)
  _1, //562 Baal Tentacle
  _1, //563 Baal Tentacle
  _1, //564 Baal Tentacle
  _1, //565 Baal Tentacle
  _1, //566 Baal Tentacle
  _0, //567 Injured Barbarian 1 (town barb)
  _0, //568 Injured Barbarian 2 (town barb)
  _0, //569 Injured Barbarian 3 (town barb)
  _1, //570 Baal Crab Clone
  _1, //571 Baals Minion
  _1, //572 Baals Minion
  _1, //573 Baals Minion
  _0, //574 Worldstone Effect
  _1, //575
  _1, //576
  _1, //577
  _1, //578
  _1, //579
  _1, //580
  _1, //581
  _1, //582
  _1, //583
  _1, //584
  _1, //585
  _1, //586
  _1, //587
  _1, //588
  _1, //589
  _1, //590
  _1, //591
  _1, //592
  _1, //593
  _1, //594
  _1, //595
  _1, //596
  _1, //597
  _1, //598
  _1, //599
  _1, //600
  _1, //601
  _1, //602
  _1, //603
  _1, //604
  _1, //605
  _1, //606
  _1, //607
  _1, //608
  _1, //609
  _1, //610
  _1, //611
  _1, //612
  _1, //613
  _1, //614
  _1, //615
  _1, //616
  _1, //617
  _1, //618
  _1, //619
  _1, //620
  _1, //621
  _1, //622
  _1, //623
  _1, //624
  _1, //625
  _1, //626
  _1, //627
  _1, //628
  _1, //629
  _1, //630
  _1, //631
  _1, //632
  _1, //633
  _1, //634
  _1, //635
  _1, //636
  _1, //637
  _1, //638
  _1, //639
  _1, //640
  _1, //641
  _1, //642
  _1, //643
  _1, //644
  _1, //645
  _1, //646
  _1, //647
  _1, //648
  _1, //649
  _1, //650
  _1, //651
  _1, //652
  _1, //653
  _1, //654
  _1, //655
  _1, //656
  _1, //657
  _1, //658
  _1, //659
  _1, //660
  _1, //661
  _1, //662
  _1, //663
  _1, //664
  _1, //665
  _1, //666
  _1, //667
  _1, //668
  _1, //669
  _1, //670
  _1, //671
  _1, //672
  _1, //673
  _1, //674
  _1, //675
  _1, //676
  _1, //677
  _1, //678
  _1, //679
  _1, //680
  _1, //681
  _1, //682
  _1, //683
  _1, //684
  _1, //685
  _1, //686
  _1, //687
  _1, //688
  _1, //689
  _1, //690
  _1, //691
  _1, //692
  _1, //693
  _1, //694
  _1, //695
  _1, //696
  _1, //697
  _1, //698
  _1, //699
  _1, //700
  _1, //701
  _1, //702
  _1, //703
};

#undef _0
#undef _1

int nCurrentAct;
BYTE anLockedChestColors[] = {9,9,9,9,9};
BYTE anClosedChestColors[] = {9,9,9,9,9};
BYTE anHostileMissileColors[] = {0x62,0x62,0x62,0x62,0x62};
BYTE anGuidedMissileColors[] = {0x5B,0x5B,0x5B,0x5B,0x5B};
BYTE anTracerMissileColors[] = {0xFF,0xFF,0xFF,0xFF,0xFF};
BYTE anOtherMissileColors[] = {0xFF,0xFF,0xFF,0xFF,0xFF};


BYTE anMissileColours[1000];

//mixed automap blob stuff

int IsObjectChest(int objno)
{
  ObjectTxt *obj = D2COMMON_GetObjectTxt(objno);
  return (obj->nSelectable0 && (
    (obj->nOperateFn == 1) || //bed, undef grave, casket, sarc
    (obj->nOperateFn == 3) || //basket, urn, rockpile, trapped soul
    (obj->nOperateFn == 4) || //chest, corpse, wooden chest, buriel chest, skull and rocks, dead barb
    (obj->nOperateFn == 5) || //barrel
    (obj->nOperateFn == 7) || //exploding barrel
    (obj->nOperateFn == 14) || //loose bolder etc....*
    (obj->nOperateFn == 19) || //armor stand
    (obj->nOperateFn == 20) || //weapon rack
    (obj->nOperateFn == 33) || //writ
    (obj->nOperateFn == 48) || //trapped soul
    (obj->nOperateFn == 51) || //stash
    (obj->nOperateFn == 68)    //evil urn
    ));
}

BYTE __fastcall MixedBlobColPatch(UnitAny *unit)
//note will use bad text colour for stash if dont return -1
{
  if (unit->nUnitType == UNITNO_OBJECT) {
    if (tToggleAutomapChests.flag && IsObjectChest(unit->nTxtFileNo) && !unit->eMode) {
      return (unit->pObjectData->fChestLocked) ? anLockedChestColors[nCurrentAct] : anClosedChestColors[nCurrentAct];
    }
  } else if (unit->nUnitType == UNITNO_MISSILE) {
    if (tToggleAutomapMissiles.flag && !(unit->fFlags0&1)) { // FIXME fFlag0--> test fFlag0, 1
      if (anMissileColours[unit->nTxtFileNo] != (BYTE)-2) return anMissileColours[unit->nTxtFileNo];
      if (unit->nOwnerType == UNITNO_PLAYER) {
        if (unit->nOwnerId == D2CLIENT_GetPlayerUnit()->nUnitId) {
          return anTracerMissileColors[nCurrentAct];
        }
        if (D2CLIENT_TestPvpFlag(unit->nOwnerId, D2CLIENT_GetPlayerUnit()->nUnitId, 8)) {
          return (unit->pPos->pTargetUnit == D2CLIENT_GetPlayerUnit()) ? anGuidedMissileColors[nCurrentAct] : anHostileMissileColors[nCurrentAct];
        } 
      }
      return anOtherMissileColors[nCurrentAct];
    }
  } else if (unit->nUnitType == UNITNO_ITEM) {
    if (tToggleAutomapItems.flag) return GetItemColour(unit)[1];
  }
  return -1;
}



BYTE anMonsterNormalColors[] = {0x62,0x62,0x62,0x62,0x62};
BYTE anMonsterBossColors[] = {0x5b,0x5b,0x5b,0x5b,0x5b};
BYTE anMonsterMinionColors[] = {0x5b,0x5b,0x5b,0x5b,0x5b};
BYTE anMonsterChampColors[] = {0x5b,0x5b,0x5b,0x5b,0x5b};
BYTE anNeutralPlayerColors[] = {0x62,0x62,0x62,0x62,0x62};
BYTE anHostilePlayerColors[] = {0x5b,0x5b,0x5b,0x5b,0x5b};
BYTE anNormalCorpseColors[] = {8,8,8,8,8};
BYTE anBossCorpseColors[] = {8,8,8,8,8};
BYTE anMinionCorpseColors[] = {8,8,8,8,8};
BYTE anChampionCorpseColors[] = {8,8,8,8,8};

BYTE anMonsterCorpseColors[1000];
struct DangerousMonster {
  BYTE type;
  char stat;
}anDangerousMonster[1000];
BYTE fGameShutdow;

void ChickenQuit(wchar_t* quitinfo, int no = 8)
{
  if (fGameShutdow == 0) {
    D2CLIENT_PrintGameStringAtTopLeft(quitinfo, no);
    D2ExitGame(0);
    fGameShutdow = 1;
  }
}

BYTE __fastcall MonsterBlobColPatch2(UnitAny *mon)
{
  if (tToggleAutomapMonsters.flag) {
    if (anMonsterColours[mon->nTxtFileNo] != (BYTE)-1) return anMonsterColours[mon->nTxtFileNo];
    if (mon->pMonsterData->fChamp & 1) return anMonsterChampColors[nCurrentAct];
    if (mon->pMonsterData->fMinion & 1) return anMonsterMinionColors[nCurrentAct];
    if (mon->pMonsterData->fBoss & 1) return anMonsterBossColors[nCurrentAct];
    return anMonsterNormalColors[nCurrentAct];
  }
  return -2;
}

BYTE __fastcall MonsterBlobColPatch(UnitAny *mon)
{
  if (mon->nUnitType == UNITNO_MONSTER) {
    if (mon->eMode && mon->eMode != 0x0C) {
      return MonsterBlobColPatch2(mon);
    }
    if (tToggleAutomapCorpse.flag) {
      int index = D2COMMON_GetMonsterColorIndex(mon, 0);
      if (index) {
        if (anMonsterCorpseColors[index] == (BYTE)-1) {
          if (mon->pMonsterData->fChamp & 1) return anChampionCorpseColors[nCurrentAct];
          if (mon->pMonsterData->fMinion & 1) return anMinionCorpseColors[nCurrentAct];
          if (mon->pMonsterData->fBoss & 1) return anBossCorpseColors[nCurrentAct];
          return anMonsterNormalColors[nCurrentAct];
        }
        return anMonsterCorpseColors[index];
      }
    }
  }
  return MixedBlobColPatch(mon);
}

void __declspec(naked) MonsterBlobColPatch_ASM()
{
  __asm {
//esi = ptr unit monster
//ebp = ptr colour
//return eax = draw flag
    mov ecx,esi
    call MonsterBlobColPatch
    mov [ebp],al
    sub al,-4
    setb al
    movzx eax,al
    pop esi
    pop ebp
    add esp, 0x0C
    ret
  }
}

void __declspec(naked) MonsterDieingPatch_ASM()
{
  __asm {
//esi = ptr unit monster
//return eax = skip flag
    cmp [esi]UnitAny.eMode,0 //monster mode = Death/DT
    je monsterdead
    test eax,0x200000 //original code
    ret
monsterdead:
    or al,1
    ret
  }
}

//players automap blob stuff


BYTE __fastcall PlayerBlobColPatch(UnitAny *pla)
{
  return D2CLIENT_TestPvpFlag(pla->nUnitId, D2CLIENT_GetPlayerUnit()->nUnitId, 8) ? anHostilePlayerColors[nCurrentAct] : anNeutralPlayerColors[nCurrentAct];
}

void __declspec(naked) PlayerBlobColPatch_ASM()
{
  __asm {
//esi = ptr unit player
//return cl = colour
    mov ecx,esi
    call PlayerBlobColPatch
    mov cl,al
    ret
  }
}

//monsters automap name stuff

BYTE *aszEnchantDescs[50];
BYTE *aszAuraDescs[7];
BYTE *aszImmuneDescs[6];

BYTE nMonsterTextCol = 1;

void __fastcall MonsterBlobNamePatch(UnitAny *mon, int xpos, int ypos)
{
  wchar_t temp[1024], *p = temp;
  BYTE txtcol = (nMonsterTextCol > 12) ? 0 : nMonsterTextCol;
  if (mon->eMode && mon->eMode != 0x0C && D2CLIENT_GetMonsterOwner(mon->nUnitId) == -1) {
    if (mon->pMonsterData->fBoss & 1) {
      int aura = 0, enchno;
      for (int i = 0; i < 9; i++) {
        enchno = mon->pMonsterData->anEnchants[i];
        if (enchno == 0) break;
        if (enchno == 30) aura = 1;
        if (enchno < 32 && aszEnchantDescs[enchno]) {
          p = ConvertToD2String(p, aszEnchantDescs[enchno], txtcol, &temp[ARRAYSIZE(temp)-1]);
        }
      }

      if (aura) {
        //NOTE :- testing if monster effected by aura, NOT source of aura
        static BYTE states[7] = {0x21, 0x23, 0x28, 0x2b, 0x2e, 0x1c, 0x31};
        for (int i = 0; i < ARRAYSIZE(states); i++) {
          if (aszAuraDescs[i] && D2COMMON_GetUnitState(mon, states[i])) {
            p = ConvertToD2String(p, aszAuraDescs[i], txtcol, &temp[ARRAYSIZE(temp)-1]);
          }
        }
      }
    }

    static BYTE stats[6] = {0x24, 0x25, 0x27, 0x29, 0x2b, 0x2d};
    for (int i = 0; i < ARRAYSIZE(stats); i++) {
      if (aszImmuneDescs[i] && D2COMMON_GetUnitStat(mon, stats[i], 0) >= 100) {
        //FIXME
        p = ConvertToD2String(p, aszImmuneDescs[i], txtcol, &temp[ARRAYSIZE(temp)-1]);
      }
    }
  }
  
  if (p > temp)
    D2DrawCenterText(6, temp, xpos, ypos-10, txtcol);
}

void __declspec(naked) MonsterBlobNamePatch_ASM()
{
  __asm {
//esi = ptr unit monster
//edi = x pos
//ebp = y pos
    mov ecx,esi
    mov edx,edi
    push ebp
    call MonsterBlobNamePatch
    mov ecx, [esi+4]
//original code
    jmp D2CLIENT_GetMonsterTxt
  }
}



void __declspec(naked) MixedBlobColPatch_ASM()
{
  __asm {
//esi = ptr unit
    mov ecx,esi
    call MixedBlobColPatch
    cmp al,-1
    jne gotcol
    cmp [esi]UnitAny.nUnitType, UNITNO_OBJECT
    je defcol
    dec al //now -2
gotcol:
    pop ecx //return
//originalish code
    mov [ebx],al
    sub al,-2
    movzx eax,al
    pop edi
    pop esi
    pop ebx
defcol:
    ret
  }
}

//blob draw stuff

char *apUnitBlobFiles[6];

CellFile *apUnitBlobCells[6];
void InitBlobs()
{
  for (int i = 0; i < ARRAYSIZE(apUnitBlobCells); i++) {
    char filename[MAX_PATH];
    if (apUnitBlobFiles[i]) wsprintf(GetMyFileNameStrrchr(filename, '\\'), "%s.BMP", apUnitBlobFiles[i]);
    apUnitBlobCells[i] = apUnitBlobFiles[i] ? InitCellFile(LoadBmpCellFile(filename)) : 0;
  }
}

void DeleteBlobs()
{
  for (int i = 0; i < ARRAYSIZE(apUnitBlobCells); i++) {
    DeleteCellFile(apUnitBlobCells[i]);
  }
}

void DrawAutomapCell2(CellFile *cellfile, int xpos, int ypos, BYTE col)
{
  CellContext ct;
  memset(&ct, 0, sizeof(ct));
  if (!(ct.pCellFile = cellfile)) return;

  xpos -= (cellfile->cells[0]->width/2);
  ypos += (cellfile->cells[0]->height/2);

  int xpos2 = xpos - cellfile->cells[0]->xoffs, ypos2 = ypos - cellfile->cells[0]->yoffs;
  if ((xpos2 >= D2GetScreenSizeX()) || ((xpos2 + (int)cellfile->cells[0]->width) <= 0) || (ypos2 >= D2GetScreenSizeY()) || ((ypos2 + (int)cellfile->cells[0]->height) <= 0)) return;

  static BYTE coltab[2][256];//, tabno = 0, lastcol = 0;
  if (!coltab[0][1]) for (int k = 0; k < 255; k++) coltab[0][k] = coltab[1][k] = k;
  cellfile->mylastcol = coltab[cellfile->mytabno ^= (col != cellfile->mylastcol)][255] = col;

  D2GFX_DrawAutomapCell2(&ct, xpos, ypos, -1, 5, coltab[cellfile->mytabno]);
}

void __fastcall DrawAutomapBlob(int xpos, int ypos, DWORD col, DWORD unittype)
{
  if (D2CLIENT_GetAutomapSize()) {
    xpos -= 1;
    ypos += 5;
  }
  if (!apUnitBlobCells[unittype]) {
    static char blines[][2] = {0,-2, 4,-4, 8,-2, 4,0, 8,2, 4,4, 0,2, -4,4, -8,2, -4,0, -8,-2, -4,-4, 0,-2};
    for (int i = 0; i < ARRAYSIZE(blines)-1; i++) {
      D2GFX_DrawLine(xpos+blines[i][0], ypos+blines[i][1], xpos+blines[i+1][0], ypos+blines[i+1][1], col, 0xff);
    }
  } else {
    DrawAutomapCell2(apUnitBlobCells[unittype], xpos, ypos, (BYTE)col);
  }
}

void __fastcall DrawObjectBlobPatch(int xpos, int ypos, DWORD col)
{
  DrawAutomapBlob(xpos, ypos, col, UNITNO_OBJECT);
}

void __fastcall DrawPlayerBlobPatch(int xpos, int ypos, DWORD col)
{
  DrawAutomapBlob(xpos, ypos, col, UNITNO_PLAYER);
}

void __declspec(naked) DrawMonsterBlobPatch_ASM()
{
  __asm {
    push UNITNO_MONSTER
    push [esp+8]
    call DrawAutomapBlob
    ret 4
  }
}

void __declspec(naked) DrawItemBlobPatch_ASM()
{
  __asm {
//eax = unit type-2
//edi = x pos
//ebp = y pos
//[esp+0x14] = col
    test eax,eax
    jne notunit2
//original code
    ret
notunit2:
    add eax,2
    push eax
    push dword ptr [esp+0x18]
    mov ecx,edi
    mov edx,ebp
    call DrawAutomapBlob
    add dword ptr [esp], 0x61
    ret
  }
}

void __declspec(naked) DrawPlayerBlobPatch2_ASM()
{
  __asm {
//dl = col
//ecx = x pos
//eax = y pos
    push UNITNO_PLAYER
    push edx
    mov edx,eax
    call DrawAutomapBlob
//    add dword ptr [esp], 0x36
    ret
  }
}

BYTE __fastcall HostilePlayerPatch(UnitAny *unit)
{
  return D2CLIENT_TestPvpFlag(unit->nUnitId, D2CLIENT_GetPlayerUnit()->nUnitId, 8)
    ? anHostilePlayerColors[nCurrentAct]
    : anNeutralPlayerColors[nCurrentAct];
}

void __declspec(naked) HostilePlayerColor_ASM()
{
  __asm {
    mov ecx, esi
    call HostilePlayerPatch
    mov cl, al
    ret
  }
}

//monster stuff

BYTE afMonsterCorpses[1000];
BYTE afMissileCorpses[1000];
BYTE afMonsterDeathAnims[1000];

BYTE nHiddenItemLevel;
wchar_t * __fastcall OpenCubeStringPatch(wchar_t *str, DWORD col)
{
  return ColourD2String(str, (nQuestItemColour != -1) ? nQuestItemColour : col);
}


BOOL __fastcall InfravisionPatch3(UnitAny *unit)
{
  switch(unit->nUnitType) {
  case UNITNO_PLAYER:
    if (!unit->eMode || unit->eMode == 0x11) return FALSE;
    break;
  case UNITNO_MONSTER:
    if (!unit->eMode || unit->eMode == 0x0C) return FALSE;
    break;
  case UNITNO_ITEM:
    break;
  default:
    return FALSE;
  }
  DrlgLevel *curlvl = GetUnitDrlgLevel(unit);
  if (curlvl && !D2COMMON_GetFunUnk_5(curlvl->nLevelNo)) return FALSE;
  return D2COMMON_TestFunUnk_6(D2CLIENT_GetPlayerUnit(), unit, 2);
}

BOOL __fastcall InfravisionPatch2(UnitAny *unit)
{
  BYTE c;
  switch(unit->nUnitType) {
  case UNITNO_MONSTER:
    if (!tToggleHiddenCorpse.flag && unit->eMode == 0x0C) {
      return afMonsterCorpses[unit->nTxtFileNo] == 0;
    }
    break;
  case UNITNO_MISSILE:
    if (!tToggleHiddenCorpse.flag && (unit->fFlags0)) {
      return afMissileCorpses[unit->nTxtFileNo] == 0;
    }
    break;
  case UNITNO_ITEM:
    c = (BYTE)GetItemColour(unit)[1];
    if (c == 0xFC) return TRUE;
    if (!tToggleHiddenItems.flag) {
      if (c == 0xFD)
        return unit->pItemData->nItemLevel == nHiddenItemLevel;
      else
        return c == 0xFE;
    }
    break;
  }
  return FALSE;
}

BYTE abGetPlayerStat[0x6FAD0E8C-0x6FAD09EA+100];
DWORD anGetPlayerStatJmpTbl[11];
DWORD anGetPlayerStatJmpTbl2[9];
BOOL PrepareGetPlayerStat()
{
  LPBYTE ps = (LPBYTE)D2CLIENT_GetPlayerStat;
  LPBYTE pe = (LPBYTE)D2CLIENT_GetPlayerStat_END;

  BYTE prelog[] = {0x81, 0xEC, 0xB0, 0x03, 0x00, 0x00,
           0x89, 0x4c, 0x24, 0x14,
           0x89, 0x54, 0x24, 0x18,};
  BYTE epilog[] = {0x8b, 0xc6,
           0x81, 0xc4, 0xbc, 0x03, 0x00, 0x00, 0xc3};
  memcpy(anGetPlayerStatJmpTbl, *(DWORD**)D2CLIENT_GetPlayerStatJmpTbl, sizeof(anGetPlayerStatJmpTbl));
  int offsetjmptbl = (PBYTE)D2CLIENT_GetPlayerStatJmpTbl-ps;
  memcpy(anGetPlayerStatJmpTbl2, *(DWORD**)D2CLIENT_GetPlayerStatJmpTbl2, sizeof(anGetPlayerStatJmpTbl2));
  int offsetjmptbl2 = (PBYTE)D2CLIENT_GetPlayerStatJmpTbl2-ps;

  memcpy(abGetPlayerStat, prelog, sizeof(prelog));
  memcpy(abGetPlayerStat+sizeof(prelog)+(pe-ps), epilog, sizeof(epilog));
  memcpy(abGetPlayerStat+sizeof(prelog), ps, pe-ps);

  int offsetofcall = ps-(LPBYTE)abGetPlayerStat-sizeof(prelog);
  for (int i = 0; i < pe-ps; i++) {
    if (abGetPlayerStat[i] == INST_CALL && abGetPlayerStat[i+1] != INST_CALL) {
      // adjust call offset
      *(int*)(abGetPlayerStat+i+1) += offsetofcall;
    }
  }
  for (int i = 0; i < ARRAYSIZE(anGetPlayerStatJmpTbl); i++) {
    anGetPlayerStatJmpTbl[i] += abGetPlayerStat+sizeof(prelog) - ps;
  }
  *(DWORD*)(abGetPlayerStat+sizeof(prelog)+offsetjmptbl) = (DWORD)anGetPlayerStatJmpTbl;
  for (int i = 0; i < ARRAYSIZE(anGetPlayerStatJmpTbl2); i++) {
    anGetPlayerStatJmpTbl2[i] += abGetPlayerStat+sizeof(prelog) - ps;
  }
  *(DWORD*)(abGetPlayerStat+sizeof(prelog)+offsetjmptbl2) = (DWORD)anGetPlayerStatJmpTbl2;

  return TRUE;
}

 int __declspec(naked) __fastcall GetPlayerStat2(UnitAny *pla, int statindex)
 {
   __asm
   {
     pushad;
     lea eax, abGetPlayerStat;
     call eax;
     mov [esp]I386Registers.eax, eax;
     popad;
     ret;
   }
 }

inline
BOOL TestMonsterOnScreen(UnitAny *mon, UnitAny *pla)
{
  return abs((int)mon->pPos->xpos2-(int)pla->pPos->xpos2) < D2GetScreenSizeX()/2
    && abs((int)mon->pPos->ypos2-(int)pla->pPos->ypos2) < (D2GetScreenSizeY()-48)/2;
}

void BackToTown();
BYTE fBackToTown;
BYTE fDangerousMonsterAction;
BYTE fDangerousMonsterActive;
BYTE fGetPlayerStatPrepared;
void CheckPlayerStat(UnitAny *mon)
{
  if (!fGetPlayerStatPrepared) {
    fGetPlayerStatPrepared = PrepareGetPlayerStat();
  }
  UnitAny *pla = D2CLIENT_GetPlayerUnit();
  static BYTE statindexs[] = {0x0e, 0x0f, 0x10, 0x11};
//  static BYTE statindexs[] = {0x0e, 0x10, 0x0f, 0x11};
  if (anDangerousMonster[mon->nTxtFileNo].type && anDangerousMonster[mon->nTxtFileNo].type < ARRAYSIZE(statindexs)+1 && TestMonsterOnScreen(mon, pla)) {
    int stat = GetPlayerStat2(pla, statindexs[anDangerousMonster[mon->nTxtFileNo].type-1]);
    if (anDangerousMonster[mon->nTxtFileNo].stat <= stat) {
      if (fDangerousMonsterActive == 0) {
        fDangerousMonsterActive = 1;
        if (fDangerousMonsterAction == 2) {
          D2CLIENT_PrintGameStringAtTopLeft(L"<Hackmap>: player's resistance below chicken threshold, returning town.", 8);
          BackToTown();
        } else if (fDangerousMonsterAction == 1) {
          ChickenQuit(L"<Hackmap>: player's resistance below chicken threshold, exiting game.");
        }
      }
    }
  }
}

BOOL TestPlayerInTown(UnitAny *unit)
{
  DrlgLevel* curlvl = GetUnitDrlgLevel(unit);
  return curlvl ?
    curlvl->nLevelNo == 1 // act1
    || curlvl->nLevelNo == 40 // act2
    || curlvl->nLevelNo == 75 // act3
    || curlvl->nLevelNo == 103  // act4
    || curlvl->nLevelNo == 109  // act5
    : FALSE;
}

BOOL __fastcall InfravisionPatch(UnitAny *unit)
{
  if (tToggleChickenDangerousMonster.flag && unit->nUnitType == UNITNO_MONSTER && unit->eMode && unit->eMode != 0x0C) {
    if (!TestPlayerInTown(D2CLIENT_GetPlayerUnit())) {
      CheckPlayerStat(unit);
    } else if (fDangerousMonsterActive) {
      fDangerousMonsterActive = 0;
    }
  }
  if (InfravisionPatch2(unit)) return TRUE;
  return !tToggleInfravision.flag ? InfravisionPatch3(unit) : FALSE;
}

void __declspec(naked) InfravisionPatch_ASM()
{
  __asm {
    mov ecx, esi
    call InfravisionPatch
    add dword ptr [esp], 0x108
    ret
  }
}

BYTE anAutomapActiveRoomColors[] = {0x16, 0x16, 0x16, 0x16, 0x16};
BYTE anAutomapReadyColors[] = {7 ,7, 7, 7, 7};
BYTE anAutomapScreenColors[] = {0x16, 0x16, 0x16, 0x16, 0x16};

void ScreenToAutomap(POINT* pos, int x, int y)
{
  pos->x = (x-y)/2/D2CLIENT_nPtDivisor-D2CLIENT_ptOffset.x+8;
  pos->y = (x+y)/4/D2CLIENT_nPtDivisor-D2CLIENT_ptOffset.y-8;
  if (D2CLIENT_GetAutomapSize()) {
    --pos->x;
    pos->y += 5;
  }
}

void ScreenToAutomapRelative(POINT* pos, int x, int y)
{
  x += D2CLIENT_GetPlayerXOffest();
  y += D2CLIENT_GetPlayerYOffset();
  ScreenToAutomap(pos, y*2+x, y*2-x);
}

inline void D2GetPlayerPosition(POINT* ptPlayer)
{
  ScreenToAutomapRelative(ptPlayer, D2GetScreenSizeX()/2, (D2GetScreenSizeY()-48)/2);
}

void RoomPatch2(DrlgRoom2 *pRoom2, DWORD col)
{
  POINT ptTopLeft, ptTopRight, ptBottomLeft, ptBottomRight;
  ScreenToAutomap(&ptTopLeft, pRoom2->xPos*160, pRoom2->yPos*160);
  ScreenToAutomap(&ptTopRight, (pRoom2->xPos+pRoom2->xPos1)*160, pRoom2->yPos*160);
  ScreenToAutomap(&ptBottomLeft, pRoom2->xPos*160, (pRoom2->yPos+pRoom2->yPos1)*160);
  ScreenToAutomap(&ptBottomRight, (pRoom2->xPos+pRoom2->xPos1)*160, (pRoom2->yPos+pRoom2->yPos1)*160);

  D2GFX_DrawLine(ptTopLeft.x, ptTopLeft.y, ptTopRight.x, ptTopRight.y, col, -1);
  D2GFX_DrawLine(ptBottomLeft.x, ptBottomLeft.y, ptBottomRight.x, ptBottomRight.y, col, -1);
  D2GFX_DrawLine(ptTopLeft.x, ptTopLeft.y, ptBottomLeft.x, ptBottomLeft.y, col, -1);
  D2GFX_DrawLine(ptTopRight.x, ptTopRight.y, ptBottomRight.x, ptBottomRight.y, col, -1);
}

void DrawMinimapLevel(int lvlno);
void RoomPatch()
{
  if (tToggleAutomapReadyRoom.flag) {
    DrlgRoom1 *pRoom1 = D2CLIENT_pDrlgAct->pRoom1;
    while (pRoom1) {
      RoomPatch2(pRoom1->pRoom2, anAutomapReadyColors[nCurrentAct]);
      pRoom1 = pRoom1->pNext;
    }
  }
  if (tToggleAutomapActiveRoom.flag) {
    DrlgRoom1 *pRoom1 = D2CLIENT_GetPlayerUnit()->pPos->pRoom1;
    if (pRoom1->nRoomsNear > 0) {
      for (DWORD i = 0; i < pRoom1->nRoomsNear; ++i) {
        RoomPatch2(pRoom1->paRoomsNear[i]->pRoom2,
            anAutomapActiveRoomColors[nCurrentAct]);
      }
    }
  }
  if (tToggleAutomapScreenArea.flag) {
    POINT ptTopLeft, ptTopRight, ptBottomLeft, ptBottomRight;
    ScreenToAutomapRelative(&ptTopLeft, 0, 0);
    ScreenToAutomapRelative(&ptTopRight, D2GetScreenSizeX(), 0);
    ScreenToAutomapRelative(&ptBottomLeft, 0, D2GetScreenSizeY()-48);
    ScreenToAutomapRelative(&ptBottomRight, D2GetScreenSizeX(), D2GetScreenSizeY()-48);
    D2GFX_DrawLine(ptTopLeft.x, ptTopLeft.y, ptTopRight.x, ptTopRight.y, anAutomapScreenColors[nCurrentAct], -1);
    D2GFX_DrawLine(ptBottomLeft.x, ptBottomLeft.y, ptBottomRight.x, ptBottomRight.y, anAutomapScreenColors[nCurrentAct], -1);
    D2GFX_DrawLine(ptTopLeft.x, ptTopLeft.y, ptBottomLeft.x, ptBottomLeft.y, anAutomapScreenColors[nCurrentAct], -1);
    D2GFX_DrawLine(ptTopRight.x, ptTopRight.y, ptBottomRight.x, ptBottomRight.y, anAutomapScreenColors[nCurrentAct], -1);
  }

  if (tToggleMinimap.flag) {
    DrawMinimapLevel(GetUnitDrlgLevel(D2CLIENT_GetPlayerUnit())->nLevelNo);
  }
}

void __declspec(naked) RoomPatch_ASM()
{
  __asm {
    call RoomPatch
    mov ecx, p_D2CLIENT_pAutomapLayer
    mov ecx, [ecx]
    ret
  }
}

void __declspec(naked) MonsterDeathPatch_ASM()
{
  __asm {
    cmp [afMonsterDeathAnims+eax],0
    je noanim
    cmp eax,0x100 //original code
    ret
noanim:
    pop eax
    ret 4
  }
}

//monster select name stuff

struct TCTallys {
  int groupnos[5];
  int runeno;
};

BYTE anMonsterTCs[1000];
BYTE anSuperuniqueTCs[100];
int numMonsterTC;
int numSuperuniqueTC;

void GetHighestTC(int tcno, TCTallys *tallys)
{
  TreasureClass *tc = D2COMMON_GetTreasureClasses(tcno, 0);
  if (!tc) return;

  for (int i = 0; i < (int)tc->numitems; i++) {
    if (tc->items[i].tcflag) {
      if (tc->items[i].thingno <= 160) {
        int groupno = (tc->items[i].thingno-1)/32;
        int levno = (((tc->items[i].thingno-1)%32)+1)*3;
        if (levno > tallys->groupnos[groupno]) tallys->groupnos[groupno] = levno;
      } else {
        GetHighestTC(tc->items[i].thingno, tallys);
      }
    } else {
      ItemTxt *itemtxt = D2COMMON_GetItemTxt(tc->items[i].thingno);
      if (itemtxt && itemtxt->nType == 74) {
        int runeno = (itemtxt->szCode[1]-'0')*10+(itemtxt->szCode[2]-'0');
        if (runeno > tallys->runeno) tallys->runeno = runeno;
      }
    }
  }
}

void InitTCList()
{
	int x = 0;
	int y = 0;
  MonsterTxt *mon;
  // i < ARRAYSIZE(anMonsterTCs)
  for (int i = 0; mon = D2CLIENT_GetMonsterTxt(i); i++) {
    TCTallys tallys;
    memset(&tallys, 0, sizeof(tallys));
    for (int j = 0; j < 4; j++) {
      int tcno = mon->tcs[D2CLIENT_GetDifficulty()][j];
      GetHighestTC(tcno, &tallys);
    }
    anMonsterTCs[i] = tallys.groupnos[1];
	x = i;
  }
  SuperuniqueTxt *sup;
  // j < ARRAYSIZE(anSuperuniqueTCs)
  for (int j = 0; sup = D2COMMON_GetSuperuniqueTxt(j); j++) {
    TCTallys tallys;
    memset(&tallys, 0, sizeof(tallys));
    int tcno = sup->tcs[D2CLIENT_GetDifficulty()];
    GetHighestTC(tcno, &tallys);
    anSuperuniqueTCs[j] = tallys.groupnos[1];
	y = j;
  }
  numMonsterTC = x;
  numSuperuniqueTC = y;
}

void __fastcall MonsterLifebarNamePatch1(UnitAny *unit, wchar_t *str)
{
  if (tToggleMonsterTC.flag) {
    WORD uniqueno = unit->pMonsterData->pEnchants->uniqueno + 67;
    wsprintfW2(str+wcslen(str)-1, "(tc%d)  ", (uniqueno < 67) ? anSuperuniqueTCs[uniqueno] : anMonsterTCs[unit->nTxtFileNo]);
  }
}

void GetMonsterLevelTC(wchar_t *str, UnitAny *unit, DWORD no)
{
  if (tToggleMonsterLevel.flag) {
    int lvl = D2COMMON_GetUnitStat(unit, STAT_LEVEL, 0);
    if (unit->pMonsterData->fBoss & 1) {
      lvl += 3;
      PBYTE p = unit->pMonsterData->anEnchants;
      for (int i = 0; *p && i < ARRAYSIZE(unit->pMonsterData->anEnchants); ++i, ++p) {
        if (*p == 16 || (*p >= 36 && *p <= 38)) {
          --lvl;
        }
      }
    }
    wsprintfW2(str+wcslen(str), " (l%d)", lvl);
  }
  if (tToggleMonsterTC.flag) {
    BYTE tc;
    if (unit->pMonsterData->fNormal & 1) {
      tc = anSuperuniqueTCs[unit->pMonsterData->uniqueno];
    } else {
      tc = anMonsterTCs[unit->nTxtFileNo];
    }
    if (tc) wsprintfW2(str+wcslen(str), " (tc%d)", tc);
  }
}

void GetMonsterResists(wchar_t *str, UnitAny *unit, DWORD no)
{
  if (tToggleMonsterResists.flag) {
    int dr = D2COMMON_GetUnitStat(unit, STAT_DAMAGE_REDUCED, 0);
    int mr = D2COMMON_GetUnitStat(unit, STAT_MAGIC_RESIST, 0);
    int fr = D2COMMON_GetUnitStat(unit, STAT_FIRE_RESIST, 0);
    int lr = D2COMMON_GetUnitStat(unit, STAT_LIGHTING_RESIST, 0);
    int cr = D2COMMON_GetUnitStat(unit, STAT_COLD_RESIST, 0);
    int pr = D2COMMON_GetUnitStat(unit, STAT_POSION_RESIST, 0);
    if (dr| mr | fr | lr | cr | pr) {
      wsprintfW2(str+wcslen(str), " (Res:");
      if (dr) wsprintfW2Colour(str+wcslen(str), 7, " D%d", dr); // Golden
      if (mr) wsprintfW2Colour(str+wcslen(str), 8, " M%d", mr); // Deep yellow
      if (fr) wsprintfW2Colour(str+wcslen(str), 1, " F%d", fr); // Red
      if (lr) wsprintfW2Colour(str+wcslen(str), 9, " L%d", lr); // Light yellow
      if (cr) wsprintfW2Colour(str+wcslen(str), 3, " C%d", cr); // Blue
      if (pr) wsprintfW2Colour(str+wcslen(str), 2, " P%d", pr); // Green
      wsprintfW2(str+wcslen(str), ")");
    }
  }
}

UnitAny* FindUnitFromUnitNo(DWORD unitno)
{
  UnitAny *unit = NULL;
  while (unit=D2CLIENT_GetNextPlayer(unit)) {
    if (unit->eMode == unitno) break;
  }
  return unit;
}

BYTE fViewSocketableOn;
BYTE fViewSock_MonLifebarFlag;
BYTE fMonsterLifebarNameOn;
wchar_t szMonsterLifebarName[0x400];
wchar_t szMonsterDesc[0x400*2];
wchar_t* __fastcall MonsterLifebarNamePatch(wchar_t *oldname, DWORD* textsize, DWORD* size, DWORD flag)
{
  if (!oldname) {
    fMonsterLifebarNameOn = 0;
    szMonsterLifebarName[0] = 0;
    return NULL;
  }
  fViewSock_MonLifebarFlag = 1;
  if (!fViewSocketableOn) {
    wcscpy(szMonsterLifebarName, oldname);
  } else {
    wcsrcat(szMonsterLifebarName, L"\n\n");
    wcsrcat(szMonsterLifebarName, oldname);
    if (p_D2CLIENT_pMonsterNameLifePatch1->nTxtFileNo < D2GetTextFileNo(szMonsterLifebarName)) {
      *size = D2GetTextFileNo(szMonsterLifebarName) + p_D2CLIENT_pMonsterNameLifePatch2->nTxtFileNo;
    } else {
      *size = p_D2CLIENT_pMonsterNameLifePatch1->nTxtFileNo;
    }
  }
  UnitAny *unit = D2CLIENT_GetSelectedUnit();
  if (!fMonsterLifebarNameOn && unit) {
    DWORD orilen = wcslen(szMonsterLifebarName);
    DWORD trimlen = orilen;
    if (trimlen > 0) {
      wchar_t *p = &szMonsterLifebarName[trimlen-1];
      while (trimlen > 0 && *p == L' ') {
        -- trimlen;
        *p-- = 0;
      }
    }
//    if (unit->nUnitType == UNITNO_MONSTER)
//      wsprintfW2(szMonsterLifebarName+wcslen(szMonsterLifebarName), " [%d,%d]", unit->nTxtFileNo,unit->nUnitId);
    if (unit->nUnitType == UNITNO_MONSTER && !(D2CLIENT_GetMonsterTxt(unit->nTxtFileNo)->flag1b & 2) && D2CLIENT_GetMonsterOwner(unit->nUnitId) == -1) {
      GetMonsterLevelTC(szMonsterLifebarName, unit, 0);
      GetMonsterResists(szMonsterLifebarName, unit, 0);
    }
    if (unit->nUnitType == UNITNO_ITEM && tToggleUnitNumber.flag) {
      DWORD no;
      if (unit->nUnitType == UNITNO_PLAYER) {
        UnitAny *unit2 = FindUnitFromUnitNo(unit->nUnitId);
        if (unit2) no = unit2->unkNo;
      }
      wsprintfW2(szMonsterLifebarName+wcslen(szMonsterLifebarName), " [0x%X,%d]", unit->nUnitId, no ? no : unit->nTxtFileNo);
    }
    DWORD l = orilen - trimlen;
    while (l-- > 0) {
      wcscat(szMonsterLifebarName, L" ");
    }
  }
  if (wcslen(szMonsterLifebarName) >= ARRAYSIZE(szMonsterLifebarName)) {
    wcscpy(szMonsterLifebarName, &szMonsterDesc[wcslen(szMonsterLifebarName)]);
  }
  if (!flag) {
    *textsize += D2GetTextWidth(oldname)/2 - D2GetTextWidth(szMonsterLifebarName)/2;
  }
  fMonsterLifebarNameOn = 1;
  return szMonsterLifebarName;
}

void __declspec(naked) MonsterLifebarNamePatch_ASM()
{
  __asm {
    push edx
    mov edx, esp
    push [esp+0x14]
    lea eax, [esp+0x10]
    push eax
    call MonsterLifebarNamePatch
    mov ecx, eax
    pop edx     // restore edx
    pop eax     // return address
    push ebx
    push esi
    mov esi, ecx
    mov ebx, edx
    jmp eax
  }
}

wchar_t szMonsterDescComma[] = L", ";

void __declspec(naked) MonsterDescCommaPatch1_ASM()
{
  __asm {
    mov eax,offset szMonsterDescComma
    ret
  }
}

void __declspec(naked) MonsterDescCommaPatch2_ASM()
{
  __asm {
    mov edx,offset szMonsterDescComma
    ret
  }
}

void __fastcall MonsterDescCommaPatch3(wchar_t *str, MonsterTxt *montxt)
{
  if (montxt->flag == 0x1506) {
    wcscat(str, szMonsterDescComma);
  }
}

void __declspec(naked) MonsterDescCommaPatch3_ASM()
{
  __asm {
//ecx = ptr string
//ebx = ptr monstats.txt
    mov edx,ebx
    call MonsterDescCommaPatch3
    mov cx, [ebx+8]
    ret
  }
}


//view inventory stuff

class ViewingPlayerInfo
{
public:
  void OpenView();
  void ViewLoop();
  ViewingPlayerInfo():nPlayerId(-1),nUnitType(-1),nUnitNo(-1),vkKeyCode(-1){}
  BOOL TestUnit(UnitAny *unit);
  virtual void CreateUI() = 0;
  virtual void DestroyUI() = 0;
  virtual DWORD GetUI() = 0;
private:
  friend UnitAny * __fastcall ViewingGetUnit(ViewingPlayerInfo* vpi, UnitAny *unitdef);
private:
  DWORD nPlayerId;
  DWORD nUnitType;
  DWORD nUnitNo;
public:
  BYTE  vkKeyCode;
};

BOOL ViewingTestMonster(UnitAny *mon)
{
  DWORD flag = D2CLIENT_GetMonsterTxt(mon->nTxtFileNo)->flag1;
  return (flag&0x1000000) && !(flag&0x200);
}

BOOL ViewingPlayerInfo::TestUnit(UnitAny *unit)
{
  if (unit && unit->pInventory) {
    if (unit->nUnitType == UNITNO_PLAYER) {
      if (vkKeyCode != (BYTE)-1 && unit->eMode) {
        return unit->eMode != 0x11;
      }
    } else if (unit->nUnitType == UNITNO_MONSTER){
      if (vkKeyCode != (BYTE)-1 && unit->eMode && unit->eMode != 0x0C) {
        return ViewingTestMonster(unit) != 0;
      }
    }
  }
  return FALSE;
}


UnitAny * __fastcall ViewingGetUnit(ViewingPlayerInfo* vpi, UnitAny *unitdef)
{
  UnitAny *unitpla = D2CLIENT_GetUnitFromId(vpi->nPlayerId, vpi->nUnitType);
  if (unitpla) {
    if (vpi->nUnitNo != -1) {
      UnitAny *unitmon = D2CLIENT_GetUnitFromId(D2CLIENT_GetInventoryId(unitpla, vpi->nUnitNo, 0), UNITNO_MONSTER);
      return unitmon ? unitmon : unitdef;
    }
    return unitpla;
  }
  return unitdef;
}

class ViewingInventoryInfo : public ViewingPlayerInfo
{
public:
  virtual void CreateUI() { D2CLIENT_SetUiVar(UIVAR_INVENTORY, 0, 1); }
  virtual void DestroyUI() { D2CLIENT_SetUiVar(UIVAR_INVENTORY, 1, 1); }
  virtual DWORD GetUI() { return D2CLIENT_GetUiVar(UIVAR_INVENTORY); }
};


void ViewingPlayerInfo::OpenView()
{
  UnitAny *unitpla = D2CLIENT_GetSelectedUnit();
  if (!unitpla || !TestUnit(unitpla)) return;
  
  CreateUI();

  if (unitpla->nUnitType == UNITNO_MONSTER && (nPlayerId = D2CLIENT_GetMonsterOwner(unitpla->nUnitId)) != -1) {
    nUnitType = 0;
    nUnitNo = D2CLIENT_GetUnitNoFromId(unitpla->nUnitId);
  } else {
    nUnitType = unitpla->nUnitType;
    nUnitNo = -1;
    nPlayerId = unitpla->nUnitId;
  }
}

void ViewingPlayerInfo::ViewLoop()
{
  if (nPlayerId != -1) {
    if (!GetUI() || !TestUnit(ViewingGetUnit(this, NULL))) {
      nPlayerId = -1;
      if (GetUI()) {
        CreateUI();
      }
    }
  }
}

class ViewingStatsInfo : public ViewingPlayerInfo
{
public:
  virtual void CreateUI() { D2CLIENT_SetUiVar(UIVAR_STATS, 0, 1); }
  virtual void DestroyUI() { D2CLIENT_SetUiVar(UIVAR_STATS, 1, 1); }
  virtual DWORD GetUI() { return D2CLIENT_GetUiVar(UIVAR_STATS); }
};

class ViewingSkillsInfo : public ViewingPlayerInfo
{
public:
  virtual void CreateUI() { D2CLIENT_SetUiVar(UIVAR_SKILLS, 2, 1); }
  virtual void DestroyUI() { D2CLIENT_SetUiVar(UIVAR_SKILLS, 1, 1); }
  virtual DWORD GetUI() { return D2CLIENT_GetUiVar(UIVAR_SKILLS); }
};

class ViewingQuestInfo : public ViewingPlayerInfo
{
public:
  virtual void CreateUI() { D2CLIENT_SetUiVar(UIVAR_QUEST, 0, 1); }
  virtual void DestroyUI() { D2CLIENT_SetUiVar(UIVAR_QUEST, 1, 1); }
  virtual DWORD GetUI() { return D2CLIENT_GetUiVar(UIVAR_QUEST); }
};

class ViewingPetInfo : public ViewingPlayerInfo
{
public:
  virtual void CreateUI() { D2CLIENT_SetUiVar(UIVAR_PET, 2, 1); }
  virtual void DestroyUI() { D2CLIENT_SetUiVar(UIVAR_PET, 1, 1); }
  virtual DWORD GetUI() { return D2CLIENT_GetUiVar(UIVAR_PET); }
};



ViewingInventoryInfo viewingInventory;
ViewingStatsInfo viewingStats;
ViewingSkillsInfo viewingSkills;
ViewingPetInfo viewingPet;
ViewingQuestInfo viewingQuestInfo;

ViewingPlayerInfo* viewingList[] = {
  &viewingInventory,
  &viewingStats,
//  &viewingSkills,
//  &viewingPet,
//  &viewingQuestInfo,
};

wchar_t* __fastcall GetUnitText(wchar_t* str, UnitAny* unit)
{
  *str = 0;
  if (unit->nUnitType == UNITNO_PLAYER){
    return wsprintfW2(str, "%s", unit->pPlayerData->szName);
  }
  wchar_t* p = D2LANG_GetLocaleText( D2CLIENT_GetMonsterTxt(unit->nTxtFileNo)->nLocaleTxtNo );
  UnitAny *unit2 = D2CLIENT_GetUnitFromId( D2CLIENT_GetMonsterOwner(unit->nUnitId), 0);
  if (unit2 && unit2 != D2CLIENT_GetPlayerUnit()) {
    return wsprintfW2(str, "%s's %S", unit2->pPlayerData->szName, p);
  }
  return wsprintfW2(str, "%S", p);
}


DWORD __fastcall D2SetTextSize( int pos, wchar_t *str, int count, DWORD maxlen)
{
  static BYTE nTextSize[] = {6,0,7,2,6,1,-1,5,0,-1,-1,0,-1,0,0,0};
  DWORD oldsize = D2WIN_SetTextSize(pos);
  if (count) {
    do {
      D2WIN_SetTextSize(pos);
    } while ( count-- && ((pos=nTextSize[pos]) != -1) && D2GetTextWidth(str) > maxlen);
  }
  while (D2GetTextWidth(str) > maxlen) {
    str[wcslen(str)-1] = 0;
    maxlen -= 8;
  }
  wcscat(str, L"...");
  return D2WIN_SetTextSize(oldsize);
}

UnitAny *ViewInventoryPatch1()
{
  UnitAny *unitpla = ViewingGetUnit(&viewingInventory,D2CLIENT_GetPlayerUnit());
  if (unitpla != D2CLIENT_GetPlayerUnit()) {
    wchar_t temp[80];
    wchar_t *p = GetUnitText(temp, unitpla);
    D2DrawCenterText(D2SetTextSize(1, p, -1, sizeof(temp)), p, D2GetScreenOffsX()+484, D2GetScreenOffsY()+236, 0);
  }
  return unitpla;
}

UnitAny* GetViewingInventoryPlayer()
{
  return ViewingGetUnit(&viewingInventory,D2CLIENT_GetPlayerUnit());
}

UnitAny* GetViewingStatsPlayer()
{
  return ViewingGetUnit(&viewingStats,D2CLIENT_GetPlayerUnit());
}

UnitAny* GetViewingSkillsPlayer()
{
  return ViewingGetUnit(&viewingSkills,D2CLIENT_GetPlayerUnit());
}

UnitAny* GetViewingPet()
{
  return ViewingGetUnit(&viewingPet,D2CLIENT_GetPlayerUnit());
}

UnitAny* GetViewingQuestInfo()
{
  return ViewingGetUnit(&viewingQuestInfo,D2CLIENT_GetPlayerUnit());
}


void __declspec(naked) ViewInventoryPatch3_ASM()
{
  __asm {
    call D2CLIENT_GetPlayerUnit
    cmp edx, eax
    jnz viewinvold
    //edx = default unit
    lea ecx, viewingInventory;
    call ViewingGetUnit
    mov edx,eax
viewinvold:
//original code
    mov ecx,0x17
    ret
  }
}

void __declspec(naked) SocketProtectPatch_ASM()
{
  __asm {
    call D2CLIENT_GetPlayerUnit
    mov ecx, eax
    push 0x13
    pop edx
    jmp D2CLIENT_SocketProtect_ORIG
  }
}

DWORD ViewInventoryPatch4()
{
  if (ViewingGetUnit(&viewingInventory, NULL)) {
    SocketProtectPatch_ASM();
    return 1;
  }
  return 0;
}

void __declspec(naked) ViewInventoryPatch4_ASM()
{
  __asm {
    mov ecx, ebx
    call ViewInventoryPatch4
    test eax, eax
    jz oldcode
    add dword ptr [esp], -0xC8
    ret 4
oldcode:
    push [esp+4]
    call D2COMMON_GetCursorItem
    ret 4
  }
}

void __fastcall SocketViewtableCallRoutine(UnitAny *unit, DWORD arg, void (__fastcall *func)(UnitAny*, DWORD))
{
  __asm mov eax, 0xB1F8
  func(unit, arg);
}

void __fastcall SocketViewtablePatch(UnitAny *u, DWORD arg, void (__fastcall *func)(UnitAny*, DWORD))
{
  fViewSock_MonLifebarFlag = 0;
  SocketViewtableCallRoutine(u, arg, func);
  if (fViewSock_MonLifebarFlag && tToggleViewSocketable.flag) {
    fViewSocketableOn = 1;
    UnitAny *unit = D2CLIENT_pInSocketablePatch;
    if (unit && unit->pInventory) {
      DWORD n = D2COMMON_GetItemFromInventory(unit->pInventory);
      while (n) {
        D2CLIENT_pInSocketablePatch = D2COMMON_GetUnitFromItem(n);
        SocketViewtableCallRoutine(unit, 1, func);
        n = D2COMMON_GetNextItemFromInventory(n);
      }
    }
    fViewSocketableOn = 0;
    D2CLIENT_pInSocketablePatch = unit;
  }
}

void __declspec(naked) SocketViewtablePatch_ASM()
{
  __asm {
    call SocketViewtablePatch
    ret
  }
}

void __declspec(naked) SocketViewtablePatch2_ASM()
{
  __asm {
    cmp fViewSocketableOn, 0
    jz outcode
    add dword ptr [esp], 0x6E
outcode:
    mov ecx, 0xF9E
    ret
  }
}

void __declspec(naked) ItemBasicStatPatch_ASM()
{
  __asm {
    cmp [tToggleItemBasicStat.flag], 0
    jz callcode
    xor eax, eax
    jmp quitcode
callcode:
    push dword ptr [esp+4]
    call D2COMMON_GetItemFromInventory
quitcode:
    ret 4
  }
}


BYTE fPlayerInTown;
void __declspec(naked) OutTownSelectPatch1_ASM()
{
  __asm {
    pop eax
    push [esp+8]
    push [esp+8]
    push offset outcode
    sub esp, 0x28
    push ebx
    push esi
    mov fPlayerInTown, 1
    jmp eax
outcode:
    mov fPlayerInTown, 0
    ret 8
  }
}

int __fastcall OutTownSelectPatch2(UnitAny *unit)
{
  if (!fPlayerInTown && viewingInventory.TestUnit(unit)) {
    if (tToggleOutTownSelect.flag || TestPlayerInTown(unit))
      return 1;
  }
  return -1;
}

void __declspec(naked) OutTownSelectPatch2_ASM()
{
  __asm {
    shr eax, 0x15
    and eax, 1
    jnz donothing
    mov ecx, esi
    call OutTownSelectPatch2;
    cmp al, 0xFF
    jz quitcode
    add dword ptr [esp], 0x10F
    mov ebx, eax
donothing:
    ret
quitcode:
    xor eax, eax
    ret
  }
}

void __declspec(naked) OutTownSelectPatch3_ASM()
{
  __asm {
    cmp fPlayerInTown, 0
    jz outcode
    mov ecx, esi
    call InfravisionPatch3
    cmp al, 1
    ret
outcode:
    shr eax, 7
    and eax, 1
    ret
  }
}

void __declspec(naked) SocketProtectPatch1_ASM()
{
  __asm {
    cmp tToggleSocketProtect.flag, 0
    jz outcode
    call SocketProtectPatch_ASM
    add dword ptr [esp], 0x42
    ret
outcode:
    mov ebx, [esp+0x1C]
    test ebx, ebx
    ret
  }
}

void __declspec(naked) SocketProtectPatch2_ASM()
{
  __asm {
    cmp tToggleSocketProtect.flag, 0
    jz outcode
    call SocketProtectPatch_ASM
    add dword ptr [esp], 0x35
    ret
outcode:
    mov eax, [esp+0x58]
    test eax, eax
    ret
  }
}

//misc patchs stuff

BYTE fAutomapPartyDefault = 1, fAutomapNamesDefault = 1;

void __fastcall CowLevelQuestPatch(int ypos, int xpos)
{
  int QuesttPage = D2CLIENT_nQuestPage;
  if (QuesttPage==0) {
  	// 检查ACT1杀牛王任务是否是BUG？
    int cow = D2COMMON_GetQuestFlag(D2CLIENT_GetQuestInfo(), 4, 10);
    if (cow)
    {
    	if (D2GetScreenSizeX()==800)
    		D2DrawCenterText(8, L"X", D2GetScreenOffsX()+300, D2GetScreenOffsY()+50, 1);
    	else
    		D2DrawCenterText(8, L"X", 300, 142+50, 1);
    }
  }
  else
  if (QuesttPage==1) {
  	
  	// 检查ACT2杀大虫任务是否是BUG？
    int dru = D2COMMON_GetQuestFlag(D2CLIENT_GetQuestInfo(), 14, 5);
    if (dru)
    {
    	if (D2GetScreenSizeX()==800)
    		D2DrawCenterText(8, L"X", D2GetScreenOffsX()+300, D2GetScreenOffsY()+50, 1);
    	else
    		D2DrawCenterText(8, L"X", 300, 142+50, 1);
    }
  }
}

void __declspec(naked) CowLevelQuestPatch_ASM()
{
  __asm {
    call CowLevelQuestPatch
//original code
    cmp esi,0x116
    ret
  }
}

BOOL PermShowItemsPatch()
{
  return tTogglePermShowItems.flag || D2CLIENT_GetUiVar(0x0d);
}

void __declspec(naked) PermShowItemsPatch_ASM()
{
  __asm {
    call PermShowItemsPatch
    test eax,eax
    ret
  }
}

BOOL UnknownPatch5()
{
  static BYTE nums[] = {1, 2, 3, 4, 5, 9, 0x0B, 0x0F, 0x16, 0x21};
  for( int i = 0; i < ARRAYSIZE(nums); ++i) {
    if (D2CLIENT_GetUiVar(nums[i]))
      return TRUE;
  }
  return FALSE;
}

DWORD __fastcall UnknownPatch6(DWORD no)
{
  if (D2GFX_GetScreenSize()) {
    return D2CLIENT_GetUiVar(no);
  }
  return 0;
}


//game init stuff

BYTE fGameInitDone = 0;
DWORD nTickCount;

void SaveTickCount()
{
  nTickCount = GetTickCount();
}

void __fastcall GameTimePatch(DWORD size)
{
#define ShowScreenX D2GetScreenSizeX()-16
  wchar_t buf[512];
  int ypos = D2CLIENT_yPosition;
  if (tToggleGameTime.flag) {
    DWORD secs = (GetTickCount() - nTickCount)/1000;
    wsprintfW2(buf,"%.2d:%.2d:%.2d", secs/3600, (secs/60)%60, secs%60);
    D2DrawCenterText( 8, buf, ShowScreenX, ypos, 4, 0);
    ypos += 16;
  }
  if (tToggleClock.flag) {

    SYSTEMTIME t;
    GetLocalTime(&t);

    wsprintfW2(buf, "%.2d:%.2d:%.2d", t.wHour, t.wMinute, t.wSecond );
    D2DrawCenterText( 8, buf, ShowScreenX, ypos, 4, 0);
    ypos += 16;
  }
  
  if (tToggleLayerLevelNo.flag) {
    wsprintfW2(buf, "Layer No: %d", D2CLIENT_pAutomapLayer->nLayerNo);
    D2DrawCenterText( 8, buf, ShowScreenX, ypos, 4, 0);
    ypos += 16;

    wsprintfW2(buf, "Level No: %d", GetUnitDrlgLevel(D2CLIENT_GetPlayerUnit())->nLevelNo);
    D2DrawCenterText( 8, buf, ShowScreenX, ypos, 4, 0);
    ypos += 16;
  }

  if (tToggleAreaLevel.flag)
  {
    wsprintfW2(buf, "Area Level: %d", GetAreaLevel() );
    D2DrawCenterText( 8, buf, ShowScreenX, ypos, 4, 0);
    ypos += 16;
  }

  GameStructInfo *gi = D2CLIENT_GetGameInfo();
  if(tToggleServerIp.flag && *gi->szGameServerIp)
  {
    wsprintfW2(buf, "%s", gi->szGameServerIp);
    D2DrawCenterText( 8, buf, ShowScreenX, ypos, 4, 0);
  }
  D2WIN_SetTextSize(size);
}

void ResetGameShutdownFlag()
{
  fGameShutdow = 0;
}

UnitAny* GetNearbyUnit(DWORD unitid, DWORD unittype)
{
  UnitAny *unit = D2CLIENT_GetUnitFromId(unitid, unittype);
  if (unit) {
    DrlgRoom1 *room1 = D2CLIENT_GetPlayerUnit()->pPos->pRoom1;
    DWORD nRoomsNear = room1->nRoomsNear;
    if (nRoomsNear > 0) {
      DrlgRoom1 **pp = room1->paRoomsNear;
      for (DWORD n = 0; n < nRoomsNear; ++n, ++pp) {
        if (unit->pPos->pRoom1 == *pp)
          return unit;
      }
    }
  }
  return NULL;
}

unsigned int nChickenLife;
unsigned int nChickenHostileLife;
unsigned int nChickenHostileNearbyLife;
unsigned int nChickenLifePercent;
unsigned int nChickenHostileLifePercent;
unsigned int nChickenHostileNearbyLifePercent;
void ChickenLife()
{
  if (fGameShutdow || TestPlayerInTown(D2CLIENT_GetPlayerUnit())) return;
  unsigned int life = D2COMMON_GetUnitStat(D2CLIENT_GetPlayerUnit(), STAT_LIFE, 0);
  if (!life) return;
  life >>= 8; // actual life
  unsigned int maxlife = D2COMMON_GetUnitStat(D2CLIENT_GetPlayerUnit(), STAT_MAXLIFE, 0);
  maxlife >>= 8;
  
  if (tToggleChickenLife.flag && ((nChickenLife && life <= nChickenLife) || (nChickenLifePercent && life*100 <= maxlife*nChickenLifePercent) )) {
    ChickenQuit(L"<Hackmap>: Life below chicken threshold, exiting game.");
  } else {
    for (UnitAny *unit = D2CLIENT_GetNextPlayer(NULL); unit; unit = D2CLIENT_GetNextPlayer(unit)) {
      if (D2CLIENT_TestPvpFlag(unit->eMode, D2CLIENT_GetPlayerUnit()->nUnitId, 8)) {
        if (tToggleChickenHostile.flag && ((nChickenHostileLife && life <= nChickenHostileLife) || (nChickenHostileLifePercent && life*100 <= maxlife*nChickenHostileLifePercent)) ) {
          ChickenQuit(L"<Hackmap>: Life below chicken hostile threshold, exiting game.");
          break;
        }
        if (tToggleChickenHostileNearby.flag && ((nChickenHostileNearbyLife && life <= nChickenHostileNearbyLife) || (nChickenHostileNearbyLifePercent && life*100 <= maxlife*nChickenHostileNearbyLifePercent)) && GetNearbyUnit(unit->nOwnerId, 0)) {
          ChickenQuit(L"<Hackmap>: Life below chicken hostile nearby threshold, exiting game.");
          break;
        }
      }
    }
  }
}

D2EditBox* pD2WinEditBox;
void CheckD2WinEditBox()
{
  if (pD2WinEditBox) {
    if (D2CLIENT_GetUiVar(5)) {
      if( *(DWORD*)p_D2CLIENT_szLastChatMessage != 0xAD5AFFFF) {
        D2WIN_DestroyEditBox(pD2WinEditBox);
        pD2WinEditBox = NULL;
        D2WIN_pFocusedControl = NULL;
      }
      D2WIN_SetTextSize(1);
    }
  }
}

BOOL __cdecl InputLinePatch(BYTE keycode)
{
	if (keycode==VK_ESCAPE || keycode==VK_RETURN)
	{
		hIMC = ImmAssociateContext(D2GFX_GetHwnd(), NULL); 
	}

   if (tToggleInputLine.flag && keycode != VK_ESCAPE) {
    if (keycode != VK_RETURN) return TRUE;
    if (pD2WinEditBox) {
      wcscpy(p_D2CLIENT_szLastChatMessage, D2WIN_GetEditBoxText(pD2WinEditBox));
    }
  }
  return FALSE;
}

void __declspec(naked) InputLinePatch_ASM()
{
  __asm {
    mov ebx, eax
    push ecx
    push [ecx+8]
    call InputLinePatch
    test eax, eax
    pop eax
    pop ecx
    jz quitcode
    add dword ptr [esp], 0x4DC
quitcode:
    ret
  }
}

BOOL __stdcall EditBoxCallBack(D2EditBox* hwnd, DWORD arg2, DWORD arg3)
{
  return TRUE;
}

BOOL __cdecl InitD2EditBox()
{
  if (!tToggleInputLine.flag) return FALSE;
  if(!pD2WinEditBox) {
    static DWORD dws[] = {0x0D, 0};
    pD2WinEditBox = D2WIN_CreateEditBox(0x83,
      D2GetScreenSizeY()-58, D2GetScreenSizeX()-266,
      0x2D,
      0, 0, 0, 0, 0,
      sizeof(dws), &dws
      );
    D2WIN_pFocusedControl = pD2WinEditBox;
    if (pD2WinEditBox) {
      D2WIN_SetEditBoxProc(pD2WinEditBox, &EditBoxCallBack);
      D2WIN_SetEditBoxText(pD2WinEditBox, p_D2CLIENT_szLastChatMessage);
    }
    *(DWORD*)p_D2CLIENT_szLastChatMessage = 0x0AD5AFFFF;
  }
  if (pD2WinEditBox)
    (pD2WinEditBox->func)(pD2WinEditBox);
  return TRUE;
}

void __declspec(naked) InputLinePatch2_ASM()
{
  __asm {
    call InitD2EditBox
    test eax, eax
    jz quitcode
    add dword ptr [esp], 0x1F1
quitcode:
    mov ecx, p_D2CLIENT_szLastChatMessage
    ret
  }
}

void __declspec(naked) PermShowOrbPatch_ASM()
{
  __asm {
    cmp [tTogglePermShowOrb.flag], 0
    jz nextcode
    add dword ptr [esp], 0x28
    ret
nextcode:
    call D2GetScreenSizeY
    ret
  }
}

void __declspec(naked) PermShowOrbPatch2_ASM()
{
  __asm {
    and word ptr [esp+0x18], 0
    ret
  }
}

void __declspec(naked) PermShowOrbPatch3_ASM()
{
  __asm {
    cmp [tTogglePermShowOrb.flag], 0
    jz nextcode
    add dword ptr [esp], 0x2D
    ret
nextcode:
    call D2GetScreenSizeX
    mov ecx, eax
    call D2GetScreenSizeY
    ret
  }
}

void __declspec(naked) GameTimeClockPatch()
{
  __asm {
    mov al, tToggleGameTime.flag
    mov cl, tToggleClock.flag
    neg al
    sbb eax, eax
    and eax, 0x10
    neg cl
    sbb ecx, ecx
    and ecx, 0x10
    add eax, ecx
    mov ecx, p_D2CLIENT_yPosition // FIXME
    mov ecx, [ecx]
    lea eax, [eax+ecx-0x10]
    ret
  }
}

void __declspec(naked) GameTimeClockPatch_ASM()
{
  __asm {
    call GameTimeClockPatch
    mov edi, eax
    ret
  }
}

void __declspec(naked) GameTimeClockPatch2_ASM()
{
  __asm {
    call GameTimeClockPatch
    add eax, 0x46
    add esi, eax
    ret
  }
}

void __fastcall D2LogMessage(wchar_t* wstr)
{
  char asc_buf[0x400];
  wchar_t buf[0x400];
  wchar_t *p = buf;

  // strip the D2 color code
  for (wchar_t *pw = wstr;*pw; ++pw) {
    if (*pw == 0xFF)
    {
      pw+=2;
      continue;
    }
    else {
      *p++ = *pw;
    }
  }
  *p++ = '\r';
  *p++ = '\n';
  *p = '\0';

  if (wcslen(buf) == 2)
  {
    // blank message , I just enter a game, log the game name and password.
    GameStructInfo *gi = D2CLIENT_GetGameInfo();
    if(*gi->szGameName) {
      if (*gi->szGamePassword)
        sprintf(asc_buf,"----------------------------%s(*%s) enter game[%s] %s/%s------------------------------\r\n",gi->szCharName,gi->szAccountName,gi->szRealmName,gi->szGameName,gi->szGamePassword);
      else
        sprintf(asc_buf,"----------------------------%s(*%s) enter game[%s] %s------------------------------\r\n",gi->szCharName,gi->szAccountName,gi->szRealmName,gi->szGameName);
    }
  }
	else
	{
		WideCharToMultiByte(CP_ACP, 0, buf, -1, asc_buf, 0x400, NULL, NULL);
	}

  SYSTEMTIME t;
  char timebuf[256];
  GetLocalTime(&t);
  sprintf(timebuf,"[%04X][%04d-%02d-%02d %02d:%02d:%02d]",GetCurrentProcessId(),t.wYear,t.wMonth,t.wDay,t.wHour, t.wMinute, t.wSecond );

  HANDLE hFile = OpenFileWrite("d2messages.log");
  SetFilePointer(hFile, 0, 0, FILE_END);
  WriteFile(hFile,timebuf,strlen(timebuf));
  WriteFile(hFile,asc_buf,strlen(asc_buf));
  CloseHandle(hFile);

}

void __declspec(naked) MessageLogPatch1_ASM()
{
  __asm {
    cmp [fLogInGameMessage],0
    je oldcode
    mov ecx, esi
    call D2LogMessage
oldcode:
    mov ecx, 0x0D
    jmp D2WIN_SetTextSize
  }
}

void __declspec(naked) MessageLogPatch2_ASM()
{
  __asm {
    cmp [fLogInGameMessage],0
    je oldcode
    mov ecx, ebp
    call D2LogMessage
oldcode:
    mov ecx, 0x0D
    jmp D2WIN_SetTextSize
  }
}

char szGameName[32];
wchar_t wszGameName[32];

void D2QuickNextGame()
{
  char *nptr = szGameName+strlen(szGameName)-1;
  char *endptr = nptr+1;
  while (nptr >= szGameName && (*nptr == '0' || isdigit(*nptr))) {
    --nptr;
  }
  if (nptr < endptr-1) {
    long n = atol(++nptr);
    char temp[32];
    sprintf(temp, "%d", ++n);
    char *nptr2 = endptr-strlen(temp);
    if (nptr < nptr2) nptr = nptr2;
    strcpy(nptr, temp);
  }

	MultiByteToWideChar(
				CP_ACP,         // code page
				0,         // character-type options
				LPCSTR(szGameName), // string to map
				strlen(szGameName),       // number of bytes in string
				wszGameName,  // wide-character buffer
				32       // size of buffer
				);
    
  D2ExitGame(0);
}

BYTE fAutoNextGameName = 1;
BYTE fAutoNextGamePassword;
wchar_t wszGamePassword[32];
void __fastcall NextGameNamePatch(D2EditBox* box, BOOL (__stdcall *FunCallBack)(D2EditBox*,DWORD,DWORD))
{
  if (!fAutoNextGameName) return;

  D2WIN_SetEditBoxText(box, wszGameName);
  D2WIN_SelectEditBoxText(box);
  // original code
  D2WIN_SetEditBoxProc(box, FunCallBack);
}
void __fastcall NextGamePasswordPatch(D2EditBox* box, BOOL (__stdcall *FunCallBack)(D2EditBox*,DWORD,DWORD))
{
  if (!fAutoNextGamePassword) return;

  D2WIN_SetEditBoxText(box, wszGamePassword);
  D2WIN_SelectEditBoxText(box);
  // original code
  D2WIN_SetEditBoxProc(box, FunCallBack);
}

void SaveGameName()
{
  GameStructInfo *gi = D2CLIENT_GetGameInfo();
  if(*gi->szGameName) {
    strcpy(szGameName, gi->szGameName);
    wcscpy2(wszGamePassword, gi->szGamePassword);

		// 支持汉字游戏名
		MultiByteToWideChar(
					CP_ACP,         // code page
					0,         // character-type options
					LPCSTR(szGameName), // string to map
					strlen(szGameName),       // number of bytes in string
					wszGameName,  // wide-character buffer
					32       // size of buffer
					);
    
  }
}

void SetCurrentPlayerStat(int inc);
void ResetMinimapCells();
void ResetPacketRecvIntercept();
void InstallPatchs2();
BYTE fEnterGameSound;
BYTE fAutoRevealAct;

BYTE LastACT;
DWORD CurrentSecond;

void GameLoopPatch()
{
  wchar_t temp[512];
  if (!fGameInitDone) {

    hIMC = ImmAssociateContext(D2GFX_GetHwnd(), NULL);

    InstallPatchs2();

    fGameInitDone = 1;
    fSleepy = 0;
#ifdef ENABLE_CATCH_EXECEPTION
    SetDbgUnexpectedHandler(TRUE);
#endif

    wsprintfW2(temp, "<Hackmap>: %s installed.", szProgTitle);
    D2CLIENT_PrintGameStringAtTopLeft(temp, 0);

    InitScreenHook();
    InitExp();
    InitShrines();
    InitItemColours();
    InitBlobs();
    InitTCList();
    ResetMinimapCells();
    if(fEnterGameSound)
      MessageBeep(MB_ICONEXCLAMATION);

    SaveTickCount();
    SaveGameName();
    ResetGameShutdownFlag();
    SetCurrentPlayerStat(0);
    memset(afRevealedActs, 0, sizeof(afRevealedActs));
    if (tToggleAutoMap.flag)
      D2CLIENT_ToggleAutoMap();
    CheckBugQuest();  // Bug Quest Stuff

    CurrentSecond = GetTickCount()/1000;
    BugKDTimer = CurrentSecond; memset(BugKDTimerSet,0,10*sizeof(BYTE));
    BugKBTimer = CurrentSecond; memset(BugKBTimerSet,0,10*sizeof(BYTE));
    BugKMTimer = CurrentSecond; memset(BugKMTimerSet,0,10*sizeof(BYTE));
    BugMsgTimer = CurrentSecond;
    LastACT  = 0;
    ForceBugKDQuit=0;
    ForceBugKDQuitTimer = CurrentSecond; memset(ForceBugKDQuitTimerSet,0,10*sizeof(BYTE));

    // 对于1.11MOD的版本，act5最后的LEVEL=136，而对于1.10的版本，act5最后的LEVEL=132
    // 根据实际的情况进行修正
    if (D2COMMON_GetLevelRecord(136)==0) actlvls[5] = 133;
    	
  }
  
  // Sleepy check
  if (fSleepy != tToggleSleepy.flag)
  {
    ToggleSleepy();
  }

  for (int i = 0; i < ARRAYSIZE(viewingList); i++) {
    viewingList[i]->ViewLoop();
  }

  if(fBackToTown && TestPlayerInTown(D2CLIENT_GetPlayerUnit()))
  {
    ResetPacketRecvIntercept();
  }
  
  ChickenLife();
  CheckD2WinEditBox();
  DWORD act = GetCurrentAct(GetUnitDrlgLevel(D2CLIENT_GetPlayerUnit()))-1;
  nCurrentAct = act < 0 ? 0 : act;
  if (fAutoRevealAct && !afRevealedActs[nCurrentAct]) {
    RevealAutomapAct();
  }
  
  // Bug Quest Stuff
  CurrentSecond = GetTickCount()/1000;
  if (nCurrentAct!=2) { BugKMTimer = CurrentSecond; memset(BugKMTimerSet,0,10*sizeof(BYTE));}
  if (nCurrentAct!=3) { BugKDTimer = CurrentSecond; memset(BugKDTimerSet,0,10*sizeof(BYTE));}
  if (nCurrentAct!=4) { BugKBTimer = CurrentSecond; memset(BugKBTimerSet,0,10*sizeof(BYTE));}

  if (tToggleBugKM.flag == 1 && BugKM==1 && nCurrentAct == 2)
  {
    DWORD mysecs = CurrentSecond - BugKMTimer;
    if (mysecs > 9)
    {
      // Force quit game
      D2ExitGame(0);
    }
    else
    {
      if (BugKMTimerSet[mysecs] == 0)
      {
        BugKMTimerSet[mysecs] = 1;
        wsprintfW2(temp, "BugKM: Will quit in %d secs", 10-mysecs);
        D2CLIENT_PrintGameStringAtTopLeft(temp, 8);
      }
    }
  }

  if (tToggleBugKD.flag == 1 && BugKD==1 && nCurrentAct == 3)
  {
    DWORD mysecs = CurrentSecond - BugKDTimer;
    if (mysecs > 9)
    {
      // Force quit game
      D2ExitGame(0);
    }
    else
    {
      if (BugKDTimerSet[mysecs] == 0)
      {
        BugKDTimerSet[mysecs] = 1;
        wsprintfW2(temp, "BugKD: Will quit in %d secs", 10-mysecs);
        D2CLIENT_PrintGameStringAtTopLeft(temp, 8);
      }
    }
  }
  
  
  if (tToggleBugKB.flag == 1 && BugKB==1 && nCurrentAct == 4)
  {
    DWORD mysecs = CurrentSecond - BugKBTimer;
    if (mysecs > 9)
    {
      // Force quit game
      D2ExitGame(0);
    }
    else
    {
      if (BugKBTimerSet[mysecs] == 0)
      {
        BugKBTimerSet[mysecs] = 1;
        wsprintfW2(temp, "BugKB: Will quit in %d secs", 10-mysecs);
        D2CLIENT_PrintGameStringAtTopLeft(temp, 8);
      }
    }
  }
  
  DWORD mysecs1,mysecs2;
  // 如果切换了场景，则重新设置BugMsg计数器
  if (LastACT != nCurrentAct)
  {
    BugMsgTimer = CurrentSecond;
    LastACT = nCurrentAct;
  }
    
  mysecs1 = CurrentSecond - BugMsgTimer;
  
  if ((ForceBugKDQuit==1) || (ForceBugKDQuit == 2 && tToggleBugKD.flag == 0))
  {
    ForceBugKDQuitTimer = CurrentSecond;
    memset(ForceBugKDQuitTimerSet,0,10*sizeof(BYTE));
    ForceBugKDQuit = 2;
    mysecs2 = 0;
  }
  
  if (ForceBugKDQuit == 2 && tToggleBugKD.flag)
  {
      mysecs2 = CurrentSecond - ForceBugKDQuitTimer;
      if (mysecs2 < 5)
      {
        if (ForceBugKDQuitTimerSet[mysecs2] == 0)
        {
          wsprintfW2(temp, "BugKD: Will quit in %d secs", 5-mysecs2);
          D2CLIENT_PrintGameStringAtTopLeft(temp, 8);
          ForceBugKDQuitTimerSet[mysecs2] = 1;
        }
      }
      else
      {
        // Force quit game
        D2ExitGame(0);
      }
  }


  if (mysecs1 > 1 && BugMsgShowed == 0)
  {
    if (tToggleBugKB.flag == 1 && BugKB==1)
    {
      D2CLIENT_PrintGameStringAtTopLeft(L"Warning! No BugKB Game! I will quit when some bad guys try to kill lovely Baal!", 8);
    }
    
    if (tToggleBugKD.flag == 1 && BugKD==1)
    {
      D2CLIENT_PrintGameStringAtTopLeft(L"Warning! No BugKD Game! I will quit when some bad guys try to kill lovely Diablo!", 8);
    }
    BugMsgShowed = 1;
  }
  // Bug Quest Stuff
  if (tToggleBugKM.flag == 2 && BugKM==1 && nCurrentAct == 2)
  {
    D2ScreenHook[4].show = 1;
    D2ScreenHook[4].color = 1;
    wsprintfW2(D2ScreenHook[4].msg, "BUGKM!!! NO PROTECT!! Please QUIT!");
  }
  else
  if (tToggleBugKD.flag == 2 && BugKD==1 && nCurrentAct == 3 && mysecs1 <= 5)
  {
    D2ScreenHook[4].show = 1;
    wsprintfW2(D2ScreenHook[4].msg, "BUGKD!!! PROTECT ON.");
    D2ScreenHook[4].color = 8;
  }
  else
  if (tToggleBugKB.flag == 2 && BugKB==1 && nCurrentAct == 4 && mysecs1 <= 5)
  {
    D2ScreenHook[4].show = 1;
    wsprintfW2(D2ScreenHook[4].msg, "BUGKB!!! PROTECT ON.");
    D2ScreenHook[4].color = 8;
  }
  else
    D2ScreenHook[4].show = 0;
}

void __declspec(naked) GameLoopPatch_ASM()
{
  __asm {
    pop eax
    sub esp, 0x24
    push edi
    mov edi, ecx
    push eax
    jmp GameLoopPatch
  }
}

void GameEndPatch2()
{
  if (pD2WinEditBox) {
    *(DWORD*)p_D2CLIENT_szLastChatMessage = 0;
    wchar_t *p = wcscpy(p_D2CLIENT_szLastChatMessage, D2WIN_GetEditBoxText(pD2WinEditBox));
    D2CLIENT_nTextLength = wcslen(p);
    D2WIN_DestroyEditBox(pD2WinEditBox);
    pD2WinEditBox = NULL;
    D2WIN_pFocusedControl = NULL;
  }
}

void GameEndPatch()
{
  if (fGameInitDone) {
    fGameInitDone = 0;

    ImmAssociateContext(D2GFX_GetHwnd(), hIMC);
    DeleteShrines();
    DeleteBlobs();
    LogExitMessage();
    GameEndPatch2();
  }
}

void __declspec(naked) GameEndPatch_ASM()
{
  __asm {
    call GameEndPatch
//original code
    mov ecx,0xb4
    ret
  }
}

struct PlayerStat {
  DWORD stat;
  char* desc;
  char* Suffix;
  int color;
}aPlayerStats[] = {
  {STAT_MAGIC_FIND, "MF  " , "%",2},
  {STAT_EXTRA_GOLD, "EG  " , "%",7},
  {STAT_FCR, "FCR " , "%",0},
  {STAT_FHR, "FHR " , "%",0},
  {STAT_IAS, "IAS " , "%",0},
  {STAT_CB, "CB  " , "%",0},
  {STAT_OW, "OW  " , "%",0},

  {STAT_FRW, "FRW " , "%",0},
  {STAT_FBR, "FBR " , "%",0},
  {STAT_PLR, "PLR " , "%",0},
  {STAT_DAMAGE_REDUCED, "DR  " , "%",0},
  {STAT_MAGIC_DAMAGE_REDUCED, "MDR ",0},
  {STAT_LIFE_LEECH, "LL  " , "%",0},
  {STAT_MANA_LEECH, "LM  " , "%",0},
  {STAT_MANA_AFTER_EACH_KILL, "EK  ",0},
  {STAT_DAMAGE_TAKEN_GOES_TO_MANA, "DTM " , "%",0},
  
  {STAT_FIRE_ABSORB,     "Fire Absorb    " , "%",0},
  {STAT_LIGHTING_ABSORB, "Lighting Absorb" , "%",0},
  {STAT_COLD_ABSORB,     "Cold Absorb    " , "%",0},
  {STAT_DEADLY_STRIKE,   "Deadly Strike  ", "%",0},
};

BYTE PAGESHOWNUMS = 7;
BYTE vkFirstPlayerStat = -1;
BYTE vkNextPlayerStat = -1;
BYTE vkPrevPlayerStat = -1;

int nCurrentPlayerStat = 0;
void SetCurrentPlayerStat(int inc)
{
  if (!inc) nCurrentPlayerStat = 0;
  else {
    nCurrentPlayerStat += inc;
    if (nCurrentPlayerStat < 0 ) nCurrentPlayerStat = ARRAYSIZE(aPlayerStats)-1;
    else if (nCurrentPlayerStat >= ARRAYSIZE(aPlayerStats)) nCurrentPlayerStat = 0;
  }
}

void PrintPlayerStat(int statno, int xoffset, int yoffset,char* desc,char* Suffix,int col=0)
{
  wchar_t temp[256];
  UnitAny *unitpla = ViewingGetUnit(&viewingStats, D2CLIENT_GetPlayerUnit());
  int pla = D2COMMON_GetUnitStat(unitpla, statno, 0);
  UnitAny *unit = D2CLIENT_GetUnitFromId(D2CLIENT_GetInventoryId(unitpla, 7, 0), 1);
  int pet = D2COMMON_GetUnitStat(unit, statno, 0);
  wsprintfW2(temp, "%s: %3d%s %3d%s", desc,pla,Suffix,pet,Suffix);
  D2DrawText( 13, temp, D2GetScreenOffsX()+xoffset, D2GetScreenOffsY()+yoffset, col);
}

DWORD myGetUnitBaseStat(UnitAny *ptUnit,int statno)
{
  int i;
  struct StatsList *ptStatsList;
  struct Stats *ptStats;
  
  ptStatsList = ptUnit->ptStatsList->pNext;
  while (ptStatsList)
  {
      ptStats = ptUnit->ptStatsList->sStats.pStats;
      for (i=0;i<ptUnit->ptStatsList->sStats.nUsed;i++)
      {
        if (ptStats[i].StatID == statno)
          return ptStats[i].Value;
      }
      ptStatsList = ptStatsList->pNext;
  }
  return 0;
}

void PrintPlayerBaseStat(int statno, int xoffset, int yoffset, int col=0)
{
  wchar_t temp[256];
  int pla = myGetUnitBaseStat(D2CLIENT_GetPlayerUnit(), statno);
  wsprintfW2(temp, "[%3d]", pla);
  D2DrawText( 13, temp, D2GetScreenOffsX()+xoffset, D2GetScreenOffsY()+yoffset, col);
}


int resistoffset[2][3]={{0,-20,-50},{0,-40,-100}};
inline int GetResistOffset()
{
  return resistoffset[GetGameMode()][D2CLIENT_GetDifficulty()];
}

void PrintPlayerRESIST(int statno, int xoffset, int yoffset,char* Suffix,int col=0,int size=6)
{
  wchar_t temp[256];
  UnitAny *unitpla = ViewingGetUnit(&viewingStats, D2CLIENT_GetPlayerUnit());
  int pla = D2COMMON_GetUnitStat(unitpla, statno, 0)+GetResistOffset();
  if ( pla < -100 ) pla = -100 ; 
  wsprintfW2(temp, "%3d%s", pla,Suffix);
  D2DrawText( size, temp, D2GetScreenOffsX()+xoffset, D2GetScreenOffsY()+yoffset, col);
}

int CheckNewStats()
{
  UnitAny *unitpla = ViewingGetUnit(&viewingStats, D2CLIENT_GetPlayerUnit());
  return D2COMMON_GetUnitStat(unitpla, 4, 0);
}

void D2QuickNextGame();
//keydown stuff

void __fastcall KeydownPatch(BYTE keycode, BYTE repeat)
{
	if (keycode==VK_RETURN)
	{
		 //进入聊天，开启输入法
		 ImmAssociateContext(D2GFX_GetHwnd(), hIMC);
	}

  if ((keycode == (BYTE)-1) || !keycode) return;

  if (keycode == VK_LEFT) ScrollMap(-2, 0);
  if (keycode == VK_RIGHT) ScrollMap(+2, 0);
  if (keycode == VK_UP) ScrollMap(0, -2);
  if (keycode == VK_DOWN) ScrollMap(0, +2);
  if (tToggleDiagonalScroll.flag) {
    if (keycode == VK_HOME) ScrollMap(-2, -1);
    if (keycode == VK_END) ScrollMap(-2, +1);
    if (keycode == VK_PRIOR) ScrollMap(+2, -1);
    if (keycode == VK_NEXT) ScrollMap(+2, +1);
  }

  if (repeat) return;

  if (keycode == vkRevealAct) RevealAutomapAct();
  if (keycode == vkRevealLevel) RevealAutomapLevel();
  if (keycode == vkCenterMap) CenterMap();
  if (keycode == vkExitGame) D2ExitGame(0);
  if (keycode == vkQuickNextGame) D2QuickNextGame();
  if (keycode == vkBackToTown || fBackToTown == 2) {
    fBackToTown = 0;
    BackToTown();
  }

  if (keycode == vkFirstPlayerStat) SetCurrentPlayerStat(0);
  if (keycode == vkNextPlayerStat) SetCurrentPlayerStat(1);
  if (keycode == vkPrevPlayerStat) SetCurrentPlayerStat(-1);
  if (keycode == vkAutoDropItem)
  {
	  UnitAny *pla = D2CLIENT_GetPlayerUnit();
	  UnitAny *item = D2COMMON_GetCursorItem(pla->pInventory);
	  D2CLIENT_PrintGameStringAtTopLeft(L"卧槽，我要扔东西了", 8);
  }
  for (int i = 0; i < ARRAYSIZE(viewingList); i++) {
    if (keycode == viewingList[i]->vkKeyCode)
      viewingList[i]->OpenView();
  }
  

  char buf[512];
  buf[0] = '\0';
  ToggleVar *tv = &pTogglesListStart;
  while (tv <= &pTogglesListEnd) {
    if (keycode == tv->key) {
      
      if (tv->flag != 0)
      {
        tv->_2[0] = tv->flag;
        tv->flag = 0;
      }
      else
      {
        if (tv->_2[0] == 0)
        {
          tv->flag = 1;
        }
        else
        {
          tv->_2[1] = tv->_2[0];
          tv->_2[0] = tv->flag;
          tv->flag = tv->_2[1];
        }
      }
      if (tv->func) (tv->func)();
      if (tv->desc)
        wsprintfA(buf+strlen(buf), ", %s %s", tv->desc, tv->flag ? "on" : "off");
    }
    tv++;
  }
  if (buf[0] != '\0') {
    wchar_t wbuf[512];
    D2CLIENT_PrintGameStringAtBottomLeft(wsprintfW2(wbuf, "<Hackmap>: %s.", &buf[2]), 0);
  }
}

void __declspec(naked) KeydownPatch_ASM()
{
  __asm {
//edi = ptr to (hwnd, msg, wparam, lparam)
    mov cl,[edi+0x08] //nVirtKey (wparam)
    mov dl,[edi+0x0c+3] //lKeyData (lparam)
    and dl,0x40 //bit 30 of lKeyData (lparam)
    call KeydownPatch
//original code
    test byte ptr [edi+0x0c+3],0x40 //bit 30 of lKeyData (lparam)
    ret
  }
}

//int statn = -2;
int GetPlayerStat(UnitAny *unit)
{
  if (!unit) return 0;

  return D2COMMON_GetUnitStat(unit, aPlayerStats[nCurrentPlayerStat].stat, 0);
  
  // old MP code
//  int n1 = D2COMMON_GetUnitStat(unit, 0xF0, 0);
//  int n2 = D2COMMON_GetUnitStat(unit, STAT_LEVEL, 0);
//  int n3 = D2COMMON_GetUnitStat(unit, STAT_MAGIC_FIND, 0);
//  return n1*n2+n3;
}

void ViewPlayerStatPatch()
{
  int i,j;
  int offset=0;
  int startx=340;
  int newstats = CheckNewStats();
  
  int ScreenResolution = (D2GetScreenSizeX()==800) ? 800 : 1024;
  
  if (ScreenResolution == 800)
  {
  	offset=0;
  	startx=340;
  	PAGESHOWNUMS=7;
  	if (newstats) startx=200;
  }
  else
  {
  	offset=0;
  	startx=480;
  	PAGESHOWNUMS=7;
  	if (newstats) startx=340;
  }
  
  for ( i = nCurrentPlayerStat*PAGESHOWNUMS ,j=0; i<(nCurrentPlayerStat+1)*PAGESHOWNUMS && i<ARRAYSIZE(aPlayerStats) ; i++,j++ )
  {

    if (j==2 && newstats)
    {
    	if (ScreenResolution==800) offset = 150;
    	else
    		offset = 152;
    }
    PrintPlayerStat( aPlayerStats[i].stat,16,startx+j*13+offset,aPlayerStats[i].desc,aPlayerStats[i].Suffix,aPlayerStats[i].color);
  }
  
  if (j==0)nCurrentPlayerStat = 0;
  if (tToggleTrueResist.flag){
  	if (ScreenResolution==800)
  	{
  		offset=23;
  		startx=350;
  	}
  	else
  	{
    	offset = 23;
  		startx=490;
    }
    //show true resist
    PrintPlayerRESIST(STAT_FIRE_RESIST     , 175 , startx , "" , 1 , 13 );
    PrintPlayerRESIST(STAT_COLD_RESIST     , 175 , startx+offset , "" , 3 , 13 );
    PrintPlayerRESIST(STAT_LIGHTING_RESIST , 175 , startx+offset*2 , "" , 9 , 13 );
    PrintPlayerRESIST(STAT_POSION_RESIST   , 175 , startx+offset*3 , "" , 12 , 13 );
  }

	if (ScreenResolution==800)
		startx=115;
	else
		startx=257;
  PrintPlayerBaseStat( STAT_STRENGTH,78,startx);
  PrintPlayerBaseStat( STAT_DEXTERITY,78,startx+65);
  PrintPlayerBaseStat( STAT_VITALITY,78,startx+150);
  PrintPlayerBaseStat( STAT_ENERGY,78,startx+210);
}

void __declspec(naked) ViewPlayerStatPatch_ASM()
{
  __asm {
    call ViewPlayerStatPatch
    add esp, 0x3B0
    ret
  }
}

void __declspec(naked) PassingActBugFixPatch_ASM()
{
    __asm {
    test eax, eax;
    jnz oldcode;
    add [esp], 0x0F;
    ret;
oldcode:
    mov ecx, esi;
    mov edi, eax;
    mov edx, ecx;
    ret;
  }
}
extern DWORD __stdcall GlobalGetOriginalData(DWORD);


void __declspec(naked) AntiDetectionPatch64_1_ASM()
{
  __asm {
    pushad;
    push eax;
    call GlobalGetOriginalData;
    mov [esp]I386Registers.eax, eax;
    popad;
    mov [ebp-0x20], eax; // old code
    ret;
  }
}

void __declspec(naked) AntiDetectionPatch64_2_ASM()
{
  __asm {
    pushad
    push ecx;
    call GlobalGetOriginalData;
    mov [esp]I386Registers.eax, eax;
    popad;
    mov ecx, eax;
    mov [ebp-0x24], ecx; // old code
    ret;
  }
}

void __declspec(naked) AntiDetectionPatch65_1_ASM()
{
  __asm {
    pushad;
    push eax;
    call GlobalGetOriginalData;
    mov [ebp-0x20], eax; // old code

    call D2CLIENT_GetGetItemFromListEntry;
    push eax;
    call GlobalGetOriginalData;
    mov [esp]I386Registers.eax, eax;
    popad;
    mov edi, eax;
    ret;
  }
}

void __declspec(naked) AntiDetectionPatch65_2_ASM()
{
  __asm {
    pushad;
    push eax;
    call GlobalGetOriginalData;
    mov [esp]I386Registers.eax, eax;
    popad;
    mov esi, eax;
    mov [ebp-0x1c], eax;
    ret;
  }
}

void __declspec(naked) AntiDetectionPatch65_3_ASM()
{
  __asm {
    pushad;
    push eax;
    call GlobalGetOriginalData;
    mov [esp]I386Registers.eax, eax;
    popad;
    mov [ebp-0x24], eax; // old code
    ret;
  }
}

BYTE fVersionCheckingDllAction;
BYTE fExtraworkDllAction = 1;
HMODULE __stdcall SuspiciousDLLPatch(BYTE flag, LPCSTR lpDllFileName)
{
  if(fGameInitDone)
    D2CLIENT_PrintGameStringAtTopLeft(L"WARNING!! Suspicious DLL is loading.", 8);

  if (flag == 0)
    return (*p_BNCLIENT_fnLoadLibraryA)(lpDllFileName);
  if(fGameInitDone)
    D2CLIENT_PrintGameStringAtTopLeft(L"Hackmap is going to unload itself.", 8);
  if (flag == (BYTE)-1)
  {
    char szPath[MAX_PATH];
    strcpy(szPath, lpDllFileName);
    strcat(szPath, ".bak");
    CopyFileA(lpDllFileName, szPath, FALSE);
  }
  return 0;
}

HMODULE __declspec(naked) __stdcall SuspiciousDLLPatch_ASM()
{
  __asm
  {
    push [esp+4];
    push eax;
    call SuspiciousDLLPatch;
    test eax, eax;
    jz unloadmyself;
    ret 4;
unloadmyself:
    add dword ptr [esp], 1; // adjust return address
    push hInstDLL;      // parameter of FreeLibrary
    // d2loader patch LoadLibraryA in BNCLIENT.DLL for his purpose.
    // we must let d2loader to load dll in case you are using it.
    mov eax, dword ptr [p_BNCLIENT_fnLoadLibraryA];
    push dword ptr [eax]; // return address of FreeLibrary
    push FreeLibrary;   // function to call
    ret;          // call FreeLibrary to unload myself
  }
}

HMODULE __declspec(naked) __stdcall VersionCheckingPatch_ASM(LPCSTR lpExtraWorkFileName)
{
  __asm
  {
    movsx eax, fVersionCheckingDllAction;
    jmp SuspiciousDLLPatch_ASM;
  }
}

HMODULE __declspec(naked) __stdcall ExtraWorkPatch_ASM(LPCSTR lpExtraWorkFileName)
{
  __asm
  {
    movsx eax, fExtraworkDllAction;
    jmp SuspiciousDLLPatch_ASM;
  }
}


void __fastcall GamePacketReceivedIntercept(BYTE* aPacket, DWORD aLength)
{
  if(fBackToTown && aLength > 6 && *(WORD*)&aPacket[0] == 0x251 && *(WORD*)&aPacket[6] == 0x3B)
  {
    ResetPacketRecvIntercept();
    BYTE castMove[9] = {0x13};
    *(DWORD*)&castMove[1] = 2;
    *(DWORD*)&castMove[5] = *(DWORD*)&aPacket[2]; // portal ID
    D2NET_SendPacket(0, castMove, sizeof(castMove));
  }
}

//////////////////////////////////////////////////////////////////////
// GamePacketReceivedSTUB()
// -------------------------------------------------------------------
// ebx = packet number
// ebp = pointer to packet    (Needed)
// edi = length
// eax = length
// ecx = length         (Needed)
// edx = ebx * 3        (Needed)
//////////////////////////////////////////////////////////////////////
void __declspec(naked) GamePacketReceivedInterceptPatch_ASM()
{
  __asm {
    // original code
    and ebx, 0xFF;
    lea edx, [ebx+ebx*2];
    pushad;
    // Call our clean c function
    mov edx, ecx;
    mov ecx, ebp;
    call GamePacketReceivedIntercept;
    // Return to game
    popad;
    add dword ptr[esp], 4;
    ret;
  }
}

BYTE fChangingAct;
void __declspec(naked) ChangeActPatch1_ASM()
{
  __asm {
    pop eax;
    push esi;
    xor esi, esi;
    cmp fChangingAct, 0;
    mov fChangingAct, 0;
    jmp eax;
  }
}

void __declspec(naked) ChangeActPatch2_ASM()
{
  __asm {
    mov fChangingAct, 1;
    ret;
  }
}


// Locale

#define   KEY_CODE_COPY     0x03
#define   KEY_CODE_PASTE      0x16
#define   KEY_CODE_CUT      0x18
#define   KEY_CODE_ENTER      '\r'

#define EDITBOX_MAX_CHAR_COUNT    196
void D2UnicodeFix(LPWSTR lpText, DWORD * dwPos)
{
  DWORD len, i, j;
  WORD  char1, char2;
  WORD  uchar;
  BYTE  mbchar[2];
  DWORD dwOldPos;
  DWORD dwNewPos;

  if (!lpText) return;
  len = wcslen(lpText);
  if (dwPos) dwOldPos = *dwPos;
  else dwOldPos = 0;
  if (len >= 2) {
    dwNewPos = -1;
    for (i=0, j = 0; i < len; i++) {
      char1 = lpText[i];
      char2 = lpText[i+1];
      if (i == dwOldPos) {
        dwNewPos = j;
      }
      //a non-english character
      if (!HIBYTE(char1) && (char1) & 0x80 && char1 != 0xff && LOBYTE(char2)) {
        mbchar[0] = LOBYTE(char1);
        mbchar[1] = LOBYTE(char2);
        if (MultiByteToWideChar(CP_ACP, 0, (char *)mbchar, sizeof(mbchar), LPWSTR(&uchar), 1)) {
          lpText[j++] = uchar;
          i++;
          if (i == dwOldPos) {
            dwNewPos = j-1;
          }
        } else {
          lpText[j++] = char1;
        }
      } else {
        lpText[j++] = char1;
      }
    }
    if (dwNewPos == (DWORD)-1) {
      dwNewPos = j;
    }
    lpText[j] = 0;
  } else {
    dwNewPos = dwOldPos;
  }
  if (dwPos) *dwPos = dwNewPos;
  return;
}

void D2UnicodeFix2(LPWSTR lpText)
{
  if (lpText) {
    size_t LEN = wcslen(lpText);
    for (size_t i = 0; i < LEN; i++)
    {
      if (lpText[i] == 0xf8f5) // Unicode 'y'
        lpText[i] = 0xff; // Ansi 'y'
    }
  }
}

int MyMultiByteToWideChar(
            UINT CodePage,         // code page
            DWORD dwFlags,         // character-type options
            LPCSTR lpMultiByteStr, // string to map
            int cbMultiByte,       // number of bytes in string
            LPWSTR lpWideCharStr,  // wide-character buffer
            int cchWideChar        // size of buffer
            )
{
  int r = MultiByteToWideChar(CodePage, dwFlags, lpMultiByteStr, cbMultiByte, lpWideCharStr, cchWideChar);
  D2UnicodeFix2(lpWideCharStr);
  return r;
}

void EditDeleteSelect(D2EditBox *hWnd)
{
  DWORD dwStart;
  DWORD len, i, j;

  if (hWnd->dwSelectStart == (DWORD)-1) return;
  if (hWnd->dwSelectStart == hWnd->dwSelectEnd) return;
  if (hWnd->dwSelectStart > hWnd->dwSelectEnd) {
    dwStart = hWnd->dwSelectEnd;
    len = hWnd->dwSelectStart - dwStart;
  } else {
    dwStart = hWnd->dwSelectStart;
    len = hWnd->dwSelectEnd - dwStart;
  }
  hWnd->dwSelectStart = (DWORD)-1;
  hWnd->dwSelectEnd = 0;
  i = dwStart;
  j = dwStart + len;
  while (1) {
    if (!hWnd->text[j]) break;
    hWnd->text[i++] = hWnd->text[j++];
  }
  hWnd->text[i] = 0;
  if (hWnd->dwCursorPos > dwStart && hWnd->dwCursorPos < dwStart + len) {
    hWnd->dwCursorPos = dwStart;
  } else if (hWnd->dwCursorPos >= dwStart+len) {
    hWnd->dwCursorPos -= len;
  }
  return;
}

void EditInsertChar(D2EditBox *hWnd, BYTE bKeyCode)
{
  DWORD len;
  LPWSTR  lpText;
  DWORD i;

  lpText = (LPWSTR)(hWnd->text);
  len = wcslen(lpText);
  if (hWnd->dwCursorPos > len) {
    hWnd->dwCursorPos = len;
  }
  for (i=len; i >= hWnd->dwCursorPos && (i >= 0); i--) {
    lpText[i+1] = lpText[i];
  }
  lpText[hWnd->dwCursorPos] = (WORD)bKeyCode;
  hWnd->dwCursorPos++;
  D2UnicodeFix(hWnd->text, &hWnd->dwCursorPos);
  D2WIN_6F8AB680_Func(hWnd);
  return;
}
HANDLE hD2LocaleMPQ;
BOOL FontFilePatch(LPSTR lpText)
{
  if (!hD2LocaleMPQ) return FALSE;
  static  char const * font_replace_list[] = { 
    "fontingamechat", 
      "fontformal10", 
      NULL 
  };
//  static  char const * font_target_path = "data\\local\\font\\chi";
  static  char const * font_target_path = "data\\local\\font\\d2locale";
  static  char const * font_tbl_suffix = ".tbl";
  char  name[MAX_PATH];
  char  * p;
  BOOL  tbl;
  int i;

  if (!lpText) return FALSE;
  p = strrchr(lpText, '\\');
  if (p) {
    strcpy(name, p+1);
  }
  p = strstr(name, font_tbl_suffix);
  if (p) {
    tbl = TRUE;
    *p = 0;
  } else {
    tbl = FALSE;
  }
  for (i=0; font_replace_list[i]; i++) {
    if (!lstrcmpi(name, font_replace_list[i])) {
      sprintf (name, "%s\\%s", font_target_path, font_replace_list[i]);
      if (tbl) {
        strcat(name, font_tbl_suffix); 
      }
      strcpy(lpText, name);
      return TRUE;
    }
  }
  return FALSE;
}

DWORD __fastcall D2WIN_DrawText2Patch(LPWSTR lpText, int x, int y, DWORD u1, int nColor, DWORD u2)
{
//  LPCWSTR lpVerText = L"v 1.10";
//  if (!wcscmp(lpText, lpVerText)) {
//    LPWSTR lpMyText = L"sting's hackmap loaded";
//    D2WIN_DrawText2(lpMyText, x, y-15, u1, nColor, u2);
//  }

  if (tToggleUseCustomFont.flag)
    D2UnicodeFix(lpText, NULL);
  return D2WIN_DrawText2(lpText, x, y, u1, nColor, u2);
}

BYTE bInitMyFont;
BYTE fInstallPatchs2;
FontInfo_t myFont;
BOOL InitializeMyFont()
{
  if (!myFont.hFontHandle) {
    const DWORD selFont = 13;
    DWORD oldfont = D2WIN_SetTextSize(1);
    FontInfo_t tmpFont;
    memcpy(&tmpFont, &D2WIN_FontTable[selFont], sizeof(FontInfo_t));
    memset(&D2WIN_FontTable[selFont], 0, sizeof(FontInfo_t));
    bInitMyFont=1;
    D2WIN_SetTextSize(selFont);
    bInitMyFont=0;
    memcpy(&myFont, &D2WIN_FontTable[selFont], sizeof(FontInfo_t));
    memcpy(&D2WIN_FontTable[selFont], &tmpFont, sizeof(FontInfo_t));
    D2WIN_SetTextSize(oldfont);
  }
  return myFont.hFontHandle != 0;
}
void ReleaseMyFont()
{
  if (myFont.hFontHandle) {
    D2WIN_FreeFontHandle(myFont.hFontHandle);
    FOG_FreePoolMemory(myFont.hMemHandle1, __FILE__, 0x135, 0);
    memset(&myFont, 0, sizeof(myFont));
  }
}

DWORD __fastcall D2Win_InitializeFontTablePatch(DWORD u1, LPSTR lpText, DWORD u2, LPCSTR file, DWORD line)
{
  if (bInitMyFont)
    FontFilePatch(lpText);
  return D2WIN_InitializeFontTable(u1, lpText, u2, file, line);
}


DWORD __fastcall D2Win_InitializeFontPatch(int fontnum, LPSTR lpText, DWORD dwSize)
{
  if(!fInstallPatchs2) {
    InstallPatchs2();
  }

  DWORD ret;
  ret = D2WIN_InitializeFont(fontnum, lpText, dwSize);
  if (bInitMyFont)
    FontFilePatch(lpText);
  return ret;
}


void __fastcall DrawTextWithMyFont(wchar_t * pszwText, int x, int y, int nColor, int unknown)
{
  if (tToggleUseCustomFont.flag && (myFont.hFontHandle || InitializeMyFont())) {
    FontInfo_t tmpFont;
    memcpy(&tmpFont, &D2WIN_FontTable[D2WIN_CurrentFont], sizeof(FontInfo_t));
    memcpy(&D2WIN_FontTable[D2WIN_CurrentFont], &myFont, sizeof(FontInfo_t));
    D2WIN_DrawText(pszwText, x, y, nColor, unknown);
    memcpy(&D2WIN_FontTable[D2WIN_CurrentFont], &tmpFont, sizeof(FontInfo_t));
  }
  else
    D2WIN_DrawText(pszwText, x, y, nColor, unknown);
}

inline
BOOL IsInGameChatting(wchar_t *pszwText)
{
  return pszwText && ((pszwText[0] == 0xff && pszwText[1] == L'c' && pszwText[2] == L'4' && wcschr(pszwText, L':') != NULL) ||
    wcsstr(pszwText, L"whisper"));
}

inline
BOOL IsInGamePrivateMessage(wchar_t *pszwText)
{
  wchar_t *pe = wcschr(pszwText, L':');
  if (pe > pszwText && *(--pe) == L')') {
    --pe;
    for (wchar_t *ps = pszwText; ps < pe; ps++) {
      if (ps[0] == L'(' && ps[1] == L'*')
        return TRUE;
    }
  }
  return FALSE;
}

void __fastcall DrawTextInGameWithMyFont(wchar_t * pszwText, int x, int y, int nColor, int unknown)
{
  if (tToggleUseCustomFont.flag && (IsInGameChatting(pszwText) || (nColor==2&&IsInGamePrivateMessage(pszwText)) ))
    DrawTextWithMyFont(pszwText, x, y, nColor, unknown);
  else
    D2WIN_DrawText(pszwText, x, y, nColor, unknown);
}

void __fastcall DrawTextInChannelWithMyFont(wchar_t * pszwText, int x, int y, int nColor, int unknown)
{
  if (x==0x1c && pszwText && (pszwText[3] == L'<' || wcsstr(pszwText, L"whisper")))
    DrawTextWithMyFont(pszwText, x, y, nColor, unknown);
  else
    D2WIN_DrawText(pszwText, x, y, nColor, unknown);
}

DWORD __fastcall ChannelEnterCharPatch(D2EditBox *hWnd, BYTE bKeyCode)
{
  DWORD ret;
  LPWSTR  lpText;
  DWORD i, j, len;
  WORD  char1, char2;
  WORD  uchar;
  BYTE  mbchar[2];
  DWORD dwPos;
  HGLOBAL hGlobal;
  LPSTR  lpStr;

  lpText = (LPWSTR)(hWnd->text);
  len = wcslen(lpText);
  if (!len) memset( hWnd->text, 0, sizeof(hWnd->text) );
  if (hWnd->dwCursorPos > len) hWnd->dwCursorPos = len;
  if (hWnd->dwCursorPos > 0) {
    char1 = lpText[hWnd->dwCursorPos-1];
    if (!HIBYTE(char1) && (char1) & 0x80 && char1 != 0xff) {
      mbchar[0] = LOBYTE(char1);
      mbchar[1] = bKeyCode;
      if (MultiByteToWideChar(CP_ACP, 0, (char *)mbchar, sizeof(mbchar), LPWSTR(&uchar), 1)) {
        lpText[hWnd->dwCursorPos-1] = uchar;
        return TRUE;
      }
    }
  }
  if (bKeyCode == KEY_CODE_PASTE) {
    ret = D2WIN_AddEditBoxChar(hWnd, bKeyCode);
    dwPos = hWnd->dwCursorPos;
    D2UnicodeFix(lpText, &dwPos);
    hWnd->dwCursorPos = dwPos;
    return ret;
  } else if (bKeyCode == KEY_CODE_COPY || bKeyCode == KEY_CODE_CUT) {
    DWORD dwStart;
    DWORD n;

    if (hWnd->dwSelectStart == (DWORD)-1) return FALSE;
    if (hWnd->dwSelectStart == hWnd->dwSelectEnd) return FALSE;
    if (hWnd->dwSelectStart > hWnd->dwSelectEnd) {
      dwStart = hWnd->dwSelectEnd;
      len = hWnd->dwSelectStart - dwStart;
    } else {
      dwStart = hWnd->dwSelectStart;
      len = hWnd->dwSelectEnd - dwStart;
    }
    if (!OpenClipboard(NULL)) return FALSE;
    EmptyClipboard();
    hGlobal = GlobalAlloc(GMEM_MOVEABLE, (len+1) * 2); 
    if (hGlobal && (lpStr = (LPSTR)GlobalLock(hGlobal))) {
      n = WideCharToMultiByte(CP_ACP, 0, lpText + dwStart, len, lpStr, (len+1)*2, NULL, NULL);
      lpStr[n] = 0;
      GlobalUnlock(hGlobal);
      SetClipboardData(CF_TEXT, hGlobal);
    }
    CloseClipboard();
    if (hGlobal) GlobalFree(hGlobal);
    if (bKeyCode == KEY_CODE_CUT) EditDeleteSelect(hWnd);
    return TRUE;
  }
  if (bKeyCode >= ' ') {
    LPWSTR lpTmp,lpSave;
    DWORD dwNewPos;

    EditDeleteSelect(hWnd);
    /*
    EditInsertChar(hWnd, bKeyCode);
    return TRUE;
    */
		lpTmp = (LPWSTR)malloc((len +2) * sizeof(WORD));
		lpSave = (LPWSTR)malloc((len +2) * sizeof(WORD));
		memset(lpTmp,0,(len +2) * sizeof(WORD));
		memset(lpSave,0,(len +2) * sizeof(WORD));
		memcpy(lpSave,lpText,len*sizeof(WORD));
		dwNewPos = (DWORD)-1;

		ret=1;
		
		i = hWnd->dwCursorPos;
		if ( i > 0)
			memcpy(lpTmp,lpText,(hWnd->dwCursorPos)*2);
		
		if (i>=len)
		{
			ret = D2WIN_AddEditBoxChar(hWnd, bKeyCode);
			lpTmp[i] = bKeyCode;
			hWnd->dwCursorPos = i+1;
		}
		else
		{
			char1 = lpText[i];
			if (!HIBYTE(char1) && (char1) & 0x80 && char1 != 0xff)
			{
				mbchar[0] = LOBYTE(char1);
				mbchar[1] = bKeyCode;
				if (MultiByteToWideChar(CP_ACP, 0, (char *)mbchar, sizeof(mbchar), LPWSTR(&uchar), 1))
					lpTmp[i] = uchar;
			}
			else
			{
				ret = D2WIN_AddEditBoxChar(hWnd, bKeyCode);
				lpTmp[i] = bKeyCode;
				hWnd->dwCursorPos = i+1;
			}
		}
		
		if (i < len)
			memcpy(&lpTmp[i+1],&lpSave[i],(len-i)*2);
		
		
		wcscpy(lpText, lpTmp);
		free(lpTmp);
		free(lpSave);
/*

    lpTmp = (LPWSTR)malloc((len +1) *2 * sizeof(WORD));
    dwNewPos = (DWORD)-1;
    for (i=j=0; i < len; i++) {
      if (i == hWnd->dwCursorPos) {
        dwNewPos = j;
      }
      if (j >= EDITBOX_MAX_CHAR_COUNT) break;
      uchar = lpText[i];
      char2 = HIBYTE(uchar);
      if (char2) {
        if (WideCharToMultiByte(CP_ACP, 0, &uchar, 1, (char *)mbchar, 2, NULL, NULL)) {
          lpTmp[j++] = mbchar[0];
          lpTmp[j++] = mbchar[1];
        } else {
          lpTmp[j++] = uchar;
        }
      } else {
        lpTmp[j++] = uchar;
      }
    }
    if (dwNewPos == (DWORD)-1) {
      dwNewPos = j;
    }
    hWnd->dwCursorPos = dwNewPos;
    lpTmp[j] = 0;
    wcscpy(lpText, lpTmp);

    free(lpTmp);
    ret = D2WIN_AddEditBoxChar(hWnd, bKeyCode);
    if ((bKeyCode & 0x80) && (bKeyCode != 0xff)) {
      if (hWnd->dwCursorPos) {
        char1 = hWnd->text[hWnd->dwCursorPos-1];
        if (!char1) char1 = bKeyCode;
        hWnd->text[hWnd->dwCursorPos-1] = 0xff;
        D2UnicodeFix(hWnd->text, &hWnd->dwCursorPos);
        hWnd->text[hWnd->dwCursorPos-1] = char1;
        D2WIN_6F8AB680_Func(hWnd);
        return ret;
      }
    }
    D2UnicodeFix(hWnd->text, &hWnd->dwCursorPos);
*/
    D2WIN_6F8AB680_Func(hWnd);
    return ret;
  }
  ret = D2WIN_AddEditBoxChar(hWnd, bKeyCode);
  return ret;
}

DWORD __fastcall ChannelEnterCharPatch2(D2EditBox *hWnd, BYTE bKeyCode)
{
  DWORD ret;
  //LPWSTR  lpText;
  //DWORD i, len;
  //LPSTR lpTmp;

  //lpText = (LPWSTR)(hWnd->text);
  //len = wcslen(lpText);
  //if (bKeyCode == KEY_CODE_ENTER) {
  //  lpTmp = (LPSTR)malloc((len +1) *2 * sizeof(WORD));
  //  if (WideCharToMultiByte(CP_ACP, 0, lpText, len+1, (LPSTR)lpTmp, (len+1)*2, NULL, NULL)) {
  //    len = strlen(lpTmp);
  //    if (len >= EDITBOX_MAX_CHAR_COUNT ) len = EDITBOX_MAX_CHAR_COUNT;
  //    for (i=0; i < len; i++) {
  //      lpText[i] = (BYTE)lpTmp[i];
  //    }
  //    lpText[i] = 0;
  //  }
  //  free(lpTmp);
  //}
  ret = D2WIN_Patch5_Func(hWnd, bKeyCode);
  return ret;
}

LPWSTR __fastcall D2Lang_Win2Unicode2(LPWSTR lpUnicodeStr, LPCSTR lpWinStr, DWORD dwBufSize)
{
  char szStackTmp[1024];
  size_t len = strlen(lpWinStr);
  char *p2, *pHeapTmp;
  if (len < sizeof(szStackTmp)) {
    p2 = szStackTmp;
    pHeapTmp = NULL;
  } else {
    pHeapTmp = new char[len+1];
    p2 = pHeapTmp;
  }

  for (LPCSTR p = lpWinStr; *p; ++p) {
    if ((BYTE)*p == 0xc3) {
      *p2++ = *(++p)+0x40;
    } else if ((BYTE)*p == 0xc2) {
      *p2++ = *(++p);
    } else {
      *p2++ = *p;
    }
  }
  *p2 = '\0';

  MultiByteToWideChar(CP_ACP, 0, pHeapTmp ? pHeapTmp : szStackTmp, -1, lpUnicodeStr, dwBufSize);
  if (pHeapTmp) delete [] pHeapTmp;
  return lpUnicodeStr;
}

LPWSTR __fastcall D2Lang_Win2Unicode(LPWSTR lpUnicodeStr, LPCSTR lpWinStr, DWORD dwBufSize)
{
  MyMultiByteToWideChar(CP_ACP, 0, lpWinStr, -1, lpUnicodeStr, dwBufSize);
  return lpUnicodeStr;
}

LPCSTR __fastcall D2Lang_Unicode2Win(LPSTR lpWinStr, LPWSTR lpUnicodeStr, DWORD dwBufSize)
{
  WideCharToMultiByte(CP_ACP, 0, lpUnicodeStr, -1, lpWinStr, dwBufSize, NULL, NULL);
  return lpWinStr;
}

LPWSTR __fastcall D2Lang_Utf82Unicode(LPWSTR lpUnicodeStr, LPCSTR lpWinStr, DWORD dwBufSize)
{
  MultiByteToWideChar(CP_ACP, 0, lpWinStr, -1, lpUnicodeStr, dwBufSize);
  return lpUnicodeStr;
}

void __declspec(naked) IsPritableCharacterPatch_ASM()
{
  __asm {
    test cl, 0x80;
    jz oldcode;
    cmp cl, 0xff;
    jz oldcode;
    pop eax; // adjust esp
    mov eax, 1;
    ret;
oldcode:
    xor ax, ax;
    mov al, cl;
    ret;
  }
}

DWORD __fastcall D2LocaleGameChat(LPWSTR lpText, DWORD u1, DWORD u2)
{
  DWORD len;
  WORD  char1, char2;
  WORD  uchar;
  BYTE  mbchar[2];

  if (lpText) {
    len = wcslen(lpText);
    if (len >= 2) {
      char1 = lpText[len-2];
      char2 = lpText[len-1];
      //a non-english character
      if (!HIBYTE(char1) && (char1) & 0x80 && char1 != 0xff && LOBYTE(char2)) {
        mbchar[0] = LOBYTE(char1);
        mbchar[1] = LOBYTE(char2);
        if (MultiByteToWideChar(CP_ACP, 0, (char *)mbchar, sizeof(mbchar), LPWSTR(&uchar), 1)) {
          lpText[len-2] = uchar;
          lpText[len-1] = 0;
          (*p_D2CLIENT_InputCount)--;
        }
      }
    }
  }
  return D2WIN_ORD_27D7(lpText,u1,u2);
}

DWORD  __fastcall EnterChatPatch(DWORD u1, LPWSTR lpText, DWORD u2, DWORD u3, DWORD u4)
{
  D2UnicodeFix(lpText, NULL);
  return D2WIN_ORD_2743(u1, lpText, u2, u3, u4);
}

void __stdcall D2MultiByteFix(LPSTR str)
{
  struct tagChat {
    DWORD _1[7];
    char txt[1];
  };
  tagChat *pc = (tagChat*)str;

  char *p2 = pc->txt + strlen(pc->txt) + 1;
  for (LPCSTR p = p2; *p; ++p) {
    if ((BYTE)*p == 0xc3) {
      *p2++ = *(++p)+0x40;
    } else if ((BYTE)*p == 0xc2) {
      *p2++ = *(++p);
    } else {
      *p2++ = *p;
    }
  }
  *p2 = '\0';
}

void __declspec(naked) D2MultiByteFixPatch_ASM()
{
  __asm {
    mov eax, [p_BNCLIENT_ChatMessage];
    mov [eax], ebp;
    push ebp;
    call D2MultiByteFix;
    ret;
  }
}

struct InventoryLocation {
  int invlocation;
  short xstart;
  short ystart;
  BYTE xmaxcell;
  BYTE ymaxcell;
  BYTE xresolution;
  BYTE yresolution;
};

inline BOOL PtInRect(const InventoryLocation& loc, short x, short y)
{
  if (x > loc.xstart && x < loc.xstart+loc.xresolution*loc.xmaxcell) {
    if (y > loc.ystart && y < loc.ystart+loc.yresolution*loc.ymaxcell) {
      return TRUE;
    }
  }
  return FALSE;
}

inline
InventoryLocation* GetInVentoryLocationInfo(short x, short y, int loctype)
{
  static InventoryLocation locations[] = {
    {0, 420, 315, 10, 4, 29, 29}, // Inventory
    {4, 155, 142, 6, 8, 29, 29}, // Stash
    {3, 210, 210, 3, 4, 29, 29}, // Cube
    {4, 165, 345, 6, 4, 29, 29}, // Stash_Classic
    {2, 110, 325, 10, 4, 29, 29}, // Trade 1 for others trade
  };
  return (x < D2GetScreenSizeX()/2) ? &locations[0] : &locations[loctype];
}

void __stdcall RButtonUpHandlerPatch(D2MSG *pmsg)
{
  if (tToggleRightClickSwap.flag) {
    int loctype = 0;
    if (D2CLIENT_GetUiVar(UIVAR_STASH))
    {
      if (GetGameMode())
        loctype = 1;
      else
        loctype = 3;
    }
    else if (D2CLIENT_GetUiVar(UIVAR_CUBE))
      loctype = 2;
    else if (D2CLIENT_GetUiVar(UIVAR_PPLTRADE))
    {
      loctype = 4;
    }
    if (loctype) {
      UnitAny *pla = D2CLIENT_GetPlayerUnit();
      UnitAny *item = D2COMMON_GetCursorItem(pla->pInventory);
      if (item && item->pItemData->nLocation2 == 0xff) {
        InventoryLocation *invloc = GetInVentoryLocationInfo(pmsg->xpos, pmsg->ypos, loctype);
        if (invloc) {
          DWORD u5 = D2COMMON_10409(pla, invloc->invlocation, D2CLIENT_GetUnknownFlag());
          for (int x = 0; x < invloc->xmaxcell; ++x) {
            for (int y = 0; y < invloc->ymaxcell; ++y) {
              int xpos = invloc->xstart + x*invloc->xresolution;
              int ypos = invloc->ystart + y*invloc->yresolution;
              int u6 = 0;
              int count = 0;
              // reset mouse point
              SendMessage(pmsg->hwnd, WM_MOUSEMOVE, pmsg->wParam, MAKELONG(xpos, ypos));
              D2COMMON_10247(pla->pInventory, item, *p_D2CLIENT_UnkU3, *p_D2CLIENT_UnkU4, u5, &u6, &count, invloc->invlocation);
              if (!count) {
                // insert item
                SendMessage(pmsg->hwnd, WM_LBUTTONDOWN, pmsg->wParam, MAKELONG(xpos, ypos));
                SendMessage(pmsg->hwnd, WM_LBUTTONUP, pmsg->wParam, MAKELONG(xpos, ypos));
                x = y = 1000; // break;
              }
            }
          }
          // restore
          SendMessage(pmsg->hwnd, WM_MOUSEMOVE, pmsg->wParam, pmsg->lParam);
        }
      }
    }
  }
  // old handler
//  D2CLIENT_RButtonUpHandler(pmsg);
}

void __declspec(naked) RButtonUpHandlerPatch_ASM()
{
  __asm {
    push [esp+8];
    call RButtonUpHandlerPatch;
//oldcode:
    mov eax, p_D2CLIENT_UnkU5;
    mov [eax], 0x10;
    add dword ptr[esp], 5;
    ret;
  }
}

void __declspec(naked) RButtonInTradePatch_ASM()
{
  __asm {
    push [esp+8];
    call RButtonUpHandlerPatch
    pop eax
    push 0x6FB6022A;
    ret
  }
}
//void __stdcall LButtonDownHandlerPatch(D2MSG *pmsg)
//{
//  D2CLIENT_LButtonDownHandler(pmsg);
//}

#pragma warning(disable : 4786 )
#include <bitset>
#include <map>
#include <vector>
#include <algorithm>
struct MinimapCell {
  short xpos;
  short ypos;
  short cellno;
};
typedef std::vector<MinimapCell> LevelMinimapObjectList;
typedef std::map<int, LevelMinimapObjectList> MinimapObjectList;
MinimapObjectList anMinimapCells;
const int nMaxMinimapLevelNo = 140;
const int nMaxMinimapCellNo = 2000;
//BYTE nMinimapCellCol = 12;// green:129, yellow:12;
BYTE nMinimapCellCol[2]={0x84,12};
BYTE nMinimapSize = 1;
std::bitset<nMaxMinimapLevelNo*nMaxMinimapCellNo> anMinimapCellMasks;



void ResetMinimapCells()
{
  anMinimapCells.clear();
  if( nMinimapSize == 0 || nMinimapSize > 8 )
    nMinimapSize = 1;
}

BOOL TestMinimapCell(int cellno, int lvlno, DrlgLevel *pDrlgLevel)
{
  return (lvlno == 46 && pDrlgLevel->pDrlgMisc->nStaffTombLvl == cellno)
    || (lvlno == pDrlgLevel->pDrlgMisc->nStaffTombLvl && cellno == 300)
    || anMinimapCellMasks.test((lvlno-1)*nMaxMinimapCellNo+(cellno-1));
}

void AddMinimapCell(AutomapCell *cell, DrlgLevel *pDrlgLevel)
{
  int cellno = (short)cell->nCellNo;
  int lvlno = pDrlgLevel->nLevelNo;
  if (lvlno == 45) lvlno = 44; // Change Valley of Snakes to Lost City

  if (cellno < 0 ) cellno = -cellno;
  if (cellno > 0 &&
    cellno <= nMaxMinimapCellNo &&
    lvlno > 0 &&
    lvlno <= nMaxMinimapLevelNo &&
    TestMinimapCell(cellno, lvlno, pDrlgLevel))
//    anMinimapCellMasks.test((lvlno-1)*nMaxMinimapCellNo+(cellno-1)))
    {
      if (lvlno == 4 && cellno == 314) { // cairn stone in stoney field
        MinimapObjectList::iterator it = anMinimapCells.find(lvlno);
        if (it != anMinimapCells.end()) {
          LevelMinimapObjectList::iterator oit = (*it).second.begin();
          LevelMinimapObjectList::iterator oend = (*it).second.end();
          for (; oit != oend; ++oit) {
            if ((*oit).cellno == cellno) return;
          }
        }
      }
      MinimapCell minicell = {(short)cell->xPixel, (short)cell->yPixel, (short)cellno};
      anMinimapCells[lvlno].push_back(minicell);
    }
}

inline void CalcTargetPosition(POINT *ptPlayer, POINT *ptObject, POINT *ptLine)
{
  int xDistance = ptObject->x-ptPlayer->x;
  int yDistance = ptObject->y-ptPlayer->y;
  double xyDistance = sqrt((double)xDistance*xDistance + yDistance*yDistance);
  ptLine->x = (LONG)(ptPlayer->x + 8*nMinimapSize*xDistance/xyDistance);
  ptLine->y = (LONG)(ptPlayer->y + 8*nMinimapSize*yDistance/xyDistance);
}

void DrawMinimapCell(int xpos, int ypos, BYTE col)
{
  POINT ptObj;
  ScreenToAutomapRelative(&ptObj, xpos+D2CLIENT_ptAutomap.x, ypos+D2CLIENT_ptAutomap.y);
  if (pMinimapObjectCell) {
    DrawAutomapCell2(pMinimapObjectCell, ptObj.x, ptObj.y, col);
  }
  else {
    wchar_t temp[20] = {L'*'};
    ColourD2String(temp, col);
    D2DrawCenterText(6, temp, ptObj.x, ptObj.y, col);
  }

  POINT ptPlayer, ptLine;
  D2GetPlayerPosition(&ptPlayer);
  if(ptPlayer.x != ptObj.x && ptPlayer.y != ptObj.y) {
    CalcTargetPosition(&ptPlayer, &ptObj, &ptLine);
    D2GFX_DrawLine(ptPlayer.x, ptPlayer.y, ptLine.x, ptLine.y, col, -1);
  }
}

void ReCalcCoordinate(int& x, int& y)
{
  int x0 = D2GetScreenSizeX()/2;
  int y0 = (D2GetScreenSizeY()-48)/2;
  int x2 = x + (nMinimapSize-1)*(x-x0);
  if (x != x0)
    y = y0 + (x2-x0)*(y-y0)/(x-x0);
  else
    y = y + (nMinimapSize-1)*(y-y0);
  x = x2;
}

void DrawMinimapLevel(int lvlno)
{
  MinimapObjectList::const_iterator cit = anMinimapCells.find(lvlno);
  if( cit != anMinimapCells.end() )
  {
    const LevelMinimapObjectList& lvlobjs = cit->second;
    int i, c;
    int xpos, ypos;
    c = lvlobjs.size();
    for( i = 0; i < c; i++ )
    {
      const MinimapCell& cell = lvlobjs[i];
      xpos = cell.xpos*10/D2CLIENT_nPtDivisor-D2CLIENT_ptOffset.x;
      ypos = cell.ypos*10/D2CLIENT_nPtDivisor-D2CLIENT_ptOffset.y;
      ReCalcCoordinate(xpos, ypos);
      DrawMinimapCell(xpos, ypos, nMinimapCellCol[(c-i+1)%2]);
    }
  }
}
char *szLocaleMPQ;
BYTE fLocalizationSupport = 1;

#include "hnmconfig.cxx"
#include "d2patchs.cxx"

void KeepGameWindow()
{
  if(tToggleKeepGameWindow.flag) {
    InstallD2Patchs(aKeepGameWindowPatchs, aKeepGameWindowPatchs+ARRAYSIZE(aKeepGameWindowPatchs));
  } else {
    RemoveD2Patchs(aKeepGameWindowPatchs, aKeepGameWindowPatchs+ARRAYSIZE(aKeepGameWindowPatchs));
  }
}

void ToggleSleepy()
{
  if (tToggleSleepy.flag)
    InstallD2Patchs(aSleepyPatchs2, aSleepyPatchs2+ARRAYSIZE(aSleepyPatchs2));
  else
    RemoveD2Patchs(aSleepyPatchs2, aSleepyPatchs2+ARRAYSIZE(aSleepyPatchs2));
  fSleepy = tToggleSleepy.flag;
}

void ResetPacketRecvIntercept()
{
  fBackToTown = 0;
  RemoveD2Patchs(aPacketRecvPatchs, aPacketRecvPatchs+ARRAYSIZE(aPacketRecvPatchs));
}

void BackToTown()
{
  UnitAny *pla = D2CLIENT_GetPlayerUnit();
  if(TestPlayerInTown(pla)) return;

  fBackToTown = 1;
  InstallD2Patchs(aPacketRecvPatchs, aPacketRecvPatchs+ARRAYSIZE(aPacketRecvPatchs));

  BYTE castTP1[9] = {0x3C};
  *(DWORD*)&castTP1[1] = 0xDC;
  *(DWORD*)&castTP1[5] = 0xFFFFFFFF;
  D2NET_SendPacket(0, castTP1, sizeof(castTP1));
  
  BYTE castTP2[5] = {0x0C};
  *(WORD*)&castTP2[1] = pla->pPos->targetx;
  *(WORD*)&castTP2[3] = pla->pPos->targety;
  D2NET_SendPacket(0, castTP2, sizeof(castTP2));
}

void InitVars()
{
  memset(afMonsterCorpses, 1, sizeof(afMonsterCorpses));
  memset(afMissileCorpses, 1, sizeof(afMissileCorpses));
  memset(afMonsterDeathAnims, 1, sizeof(afMonsterDeathAnims));
  memset(anMonsterCorpseColors, 1, sizeof(anMonsterCorpseColors));
  memset(anItemColours, -1, sizeof(anItemColours));
  memset(anRuneColours, -1, sizeof(anRuneColours));
  memset(anMissileColours, -1, sizeof(anMissileColours));
}

PBYTE InitMinimapCellsConfig()
{
  PBYTE pMiniCells = (PBYTE)VirtualAlloc(NULL, anMinimapCellMasks.size(), MEM_COMMIT, PAGE_READWRITE);
  if (pMiniCells) {
    for (int i = 0; i < ARRAYSIZE(aConfigVars); i++) {
      if (!stricmp(aConfigVars[i].szName1, "MinimapCells") ) {
        aConfigVars[i].pVar = pMiniCells;
//        for (int j = 0; j < nMaxMinimapLevelNo-1; j++) {
//          *(pMiniCells+j*nMaxMinimapCellNo+j+1) = 1;
//        }
        return pMiniCells;
      }
    }
  }
  VirtualFree(pMiniCells, 0, MEM_RELEASE);
  return NULL;
}

void InitMiniCells(PBYTE pMiniCells)
{
  PBYTE ps = pMiniCells;
  PBYTE pe = pMiniCells+anMinimapCellMasks.size();
  while (ps < pe) {
    ps = std::find(ps, pe, 1);
    if (ps != pe) {
      anMinimapCellMasks.set(ps-pMiniCells);
      ++ps;
    }
  }
}


DWORD __declspec(naked) __fastcall CastAddress(DWORD addr)
{
  __asm
  {
    mov edx, ecx;
    and edx, 0xFFFF;
    xor eax, eax;
    shr ecx, 0x10;
    mov ax, cx;
    xor eax, edx;
    shl eax, 0x10;
    or eax, edx;
    ret;
  }
}
void TestAntiDetectionRoutine64()
{
  DWORD* pgdwDetectResult1 = (DWORD*)0x6FBBC244;
  DWORD* pgdwDetectResult2 = (DWORD*)0x6FBBC248;
  DWORD* pgdwGameType = (DWORD*)0x6FBA7960;
  DWORD oldgt = *pgdwGameType;
  *pgdwGameType = 2;
  
  DWORD exec = CastAddress(0x6FF160C4);
  void (*fnCheckDetectResult)() = (void (*)())0x6FB4F2D0;

  for( int i = 0; i < ARRAYSIZE(aD2Patchs); i++)
  {
    DWORD mixaddr = CastAddress(aD2Patchs[i].addr);
    *pgdwDetectResult1 = mixaddr;
    *pgdwDetectResult2 = exec;
    fnCheckDetectResult();
  }
  *pgdwGameType = oldgt;
}

void TestAntiDetectionRoutine65()
{
  DWORD* gdwCheckResultFlag2 = (DWORD*)0x6FBBD470;
  DWORD* pgdwGameType = (DWORD*)0x6FBA7960;
  DWORD oldgt = *pgdwGameType;
  *pgdwGameType = 2;
  *gdwCheckResultFlag2 = 0x4000;
  
  void (*fnCheckDetectResult)() = (void (*)())0x6FB527A0;
  
  fnCheckDetectResult();
  *pgdwGameType = oldgt;
}

void TestAntiDetectionRoutines()
{
  TestAntiDetectionRoutine65();

  // verify patchs
  DWORD olddd, newdd;
  for( int i = 0; i < ARRAYSIZE(aD2Patchs); i++)
  {
    if(aD2Patchs[i].len >= 4)
      olddd = *(DWORD*)aD2Patchs[i].oldcode;
    else
    {
      olddd = *(DWORD*)aD2Patchs[i].addr;
      memcpy(&olddd, aD2Patchs[i].oldcode, aD2Patchs[i].len );
    }
    newdd = GlobalGetOriginalData(aD2Patchs[i].addr);
    if(newdd != olddd)
      __asm int 3;
  }
}

BOOL InitD2LocaleMPQ()
{
  char  tmp[MAX_PATH];
  char * filename = szLocaleMPQ ? szLocaleMPQ : "D2Locale.MPQ";
  char *lpFile = filename;

  BOOL fFileNameOnly = strrchr(filename, '\\') == NULL;
  if (fFileNameOnly) {
    GetModuleFileName( NULL, tmp, MAX_PATH);
    strcpy( strrchr( tmp,'\\')+1, filename);
    lpFile = tmp;
  }
  if (!STORM_OpenArchive( lpFile, 0x10000, 2, &hD2LocaleMPQ) ||!hD2LocaleMPQ) {
    if (fFileNameOnly) {
      strcpy(GetMyFileNameStrrchr( tmp, '\\'), filename);
      lpFile = tmp;
    }
    if (!STORM_OpenArchive( lpFile, 0x10000, 2, &hD2LocaleMPQ) || !hD2LocaleMPQ) {
      return FALSE;
    }
  }

  return hD2LocaleMPQ != NULL;
}

void RemoveD2LocaleMPQ()
{
  if (hD2LocaleMPQ) {
    STORM_CloseArchive(hD2LocaleMPQ);
    hD2LocaleMPQ = NULL;
  }
}

BOOL InstallPatchs()
{
  if (!RelocD2Ptrs((DWORD *)&pD2PtrsListStart, (DWORD *)&pD2PtrsListEnd))
    return FALSE;

  int i;
  DWORD chksum = 0;
  for (i = 0; i < ARRAYSIZE(aD2PatchItems); i++) {
    chksum += RelocD2Patchs(aD2PatchItems[i].patch, aD2PatchItems[i].patch+aD2PatchItems[i].num);
  }

#ifdef _DEBUG
  static DWORD chksumfix = chksum;
#else
//  if (chksum != 0xf92f7e6d) return FALSE;
#endif

  InitVars();

  PBYTE p = InitMinimapCellsConfig();
  if (!p) return FALSE;

  __try {
    LoadConfig(aConfigVars,ARRAYSIZE(aConfigVars));
    InitMiniCells(p);
  }__finally {
    VirtualFree(p, 0, MEM_RELEASE);
  }

  for(i = 0; i < ARRAYSIZE(aD2PatchItems); i++) {
    SaveD2Patchs(aD2PatchItems[i].patch, aD2PatchItems[i].patch+aD2PatchItems[i].num);
    if(!aD2PatchItems[i].fInitOnDemand)
      InstallD2Patchs(aD2PatchItems[i].patch, aD2PatchItems[i].patch+aD2PatchItems[i].num);
  }
  if (fLocalizationSupport) {
    InitD2LocaleMPQ();
    InstallD2Patchs(aD2LocalePatchs, aD2LocalePatchs+ARRAYSIZE(aD2LocalePatchs));
  }

  if (fAutomapPartyDefault != -1) D2CLIENT_SetAutomapParty(fAutomapPartyDefault);
  if (fAutomapNamesDefault != -1) D2CLIENT_SetAutomapNames(fAutomapNamesDefault);

  if (GetModuleHandle("D2LAUNCH.DLL")) {
    InstallPatchs2();
  }

//  KillCellAssert();

  // test only
//  TestAntiDetectionRoutines();

  return TRUE;
}

void RemovePatchs()
{
#ifdef ENABLE_CATCH_EXECEPTION
  SetDbgUnexpectedHandler(FALSE);
#endif

  ResetAutomapCell();
  GameEndPatch();

  for(int i = 0; i < ARRAYSIZE(aD2PatchItems); i++) {
    RemoveD2Patchs(aD2PatchItems[i].patch, aD2PatchItems[i].patch+aD2PatchItems[i].num);
    DeleteD2Patchs(aD2PatchItems[i].patch, aD2PatchItems[i].patch+aD2PatchItems[i].num);
  }
  ReleaseMyFont();
  if(fInstallPatchs2) {
    RemoveD2Patchs(aD2Patchs2, aD2Patchs2+ARRAYSIZE(aD2Patchs2));
    DeleteD2Patchs(aD2Patchs2, aD2Patchs2+ARRAYSIZE(aD2Patchs2));
    RemoveD2Patchs(aD2LocalePatchs2, aD2LocalePatchs2+ARRAYSIZE(aD2LocalePatchs2));
    DeleteD2Patchs(aD2LocalePatchs2, aD2LocalePatchs2+ARRAYSIZE(aD2LocalePatchs2));
    fInstallPatchs2 = 0;
  }
  RemoveD2LocaleMPQ();
}

void InstallPatchs2()
{
  if (!fInstallPatchs2) {
    fInstallPatchs2 = 1;
    DWORD chksum = RelocD2Patchs(aD2Patchs2, aD2Patchs2+ARRAYSIZE(aD2Patchs2));
//    if (chksum == 0x007c0800) {
      SaveD2Patchs(aD2Patchs2, aD2Patchs2+ARRAYSIZE(aD2Patchs2));
      InstallD2Patchs(aD2Patchs2, aD2Patchs2+ARRAYSIZE(aD2Patchs2));
//    }

    if (fLocalizationSupport) {
      chksum = RelocD2Patchs(aD2LocalePatchs2, aD2LocalePatchs2+ARRAYSIZE(aD2LocalePatchs2));
 //     if (chksum == 0x00d5fce8) {
        SaveD2Patchs(aD2LocalePatchs2, aD2LocalePatchs2+ARRAYSIZE(aD2LocalePatchs2));
        InstallD2Patchs(aD2LocalePatchs2, aD2LocalePatchs2+ARRAYSIZE(aD2LocalePatchs2));
 //     }
    }
  }
}

/*
高分辨率支持640X480->1024X768
*/


DWORD HR1024X768_FixMPQ()
{
	D2CLIENT_PrintGameStringAtTopLeft(L"Fixing 1024X768", 8);
	// 修正1024X768下腰带中物品的位置，参考belts.txt，修正第一部分（用于640X480分辨率）
	DWORD *BeltTbl = D2COMMON_pBeltLocationTbl;
	DWORD *p;
	int i,j;
	
	p = BeltTbl;
	for (i=0;i<14;i++)	// 一共需要修正14条记录
	{
		p++;p++;	// 跳过最前面的两个DWORD
		for (j=0;j<16;j++)
		{
			if (*p != 0) (*p) = (*p) + 194;	// Left
			p++;
			if (*p != 0) (*p) = (*p) + 194;	// Right
			p++;
			if (*p != 0) (*p) = (*p) + 285;	// Top
			p++;
			if (*p != 0) (*p) = (*p) + 285;	// Bottom
			p++;
		}
	}
	
	// 修正1024X768下身上物品的位置，参考invertory.txt，修正第一部分（用于640X480分辨率）
	DWORD *InvTbl = D2COMMON_pInvLocationTbl;
	
	p = InvTbl;
	for (i=0;i<16;i++)	// 一共需要修正16条记录，只需要修正各个角色在640X480分辨率下面的参数
	{
		for (j=0;j<12;j++)
		{
			if (*p != -1 && (i<5 || i==14/*Druid*/ || i==15/*Assassin*/)) (*p) = (*p) + 384;	// Left
			p++;
			if (*p != -1 && (i<5 || i==14/*Druid*/ || i==15/*Assassin*/)) (*p) = (*p) + 384;	// Right
			p++;
			if (*p != -1) (*p) = (*p) + 143;	// Top
			p++;
			if (*p != -1) (*p) = (*p) + 143;	// Bottom
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


// 汉字角色名字支持
int __stdcall UnicodeCharNameCheck(void *unk1, void *unk2, char *lpCharName)
{
	char thechar;
	
	thechar = lpCharName[0];
	
// \/:*?"<>|
	if (thechar == '\\' || thechar == '/' || thechar == ':' || thechar == '*' || thechar == '?' || thechar == '"' ||
		  thechar == '<' || thechar == '>' || thechar == '|')
		  return 0;
	return 1;
}