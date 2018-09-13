//keydown vars

BYTE vkRevealAct = VK_SUBTRACT;
BYTE vkRevealLevel = -1;
BYTE vkCenterMap = VK_PAUSE;
BYTE vkExitGame = -1;
BYTE vkQuickNextGame = -1;
BYTE vkBackToTown = VK_BACK;
BYTE vkAutoDropItem = VK_OEM_3;


#define pTogglesListStart tToggleFullVisuals

ToggleVar tToggleFullVisuals = {1, VK_MULTIPLY};
ToggleVar tToggleSleepy = {0, -1, "Low CPU Usage Toggle"};
ToggleVar tToggleTrueResist = {0, -1, "Show Resist Toggle"};
ToggleVar tToggleAreaLevel = {0, -1, "Area Level Toggle"};
ToggleVar tToggleULCMask = {0, -1, "ULC Mask Toggle"};
ToggleVar tToggleShowPacket = {0, -1, "Show Packet Toggle"};
ToggleVar tToggleShowExp = {0, -1, "Show Exp Toggle"};
ToggleVar tToggleShowPetExp = {0, -1, "Show PET Exp Toggle"};
ToggleVar tToggleShowPing = {0, -1, "Show Ping Toggle"};
ToggleVar tToggleAutoParty = {0, -1, "Auto Party Toggle"};
ToggleVar tToggleAutoMap = {0, -1, "Auto Map Toggle"};
ToggleVar tToggleBugKM = {0, -1, "Bug KM Toggle"};
ToggleVar tToggleBugKD = {0, -1, "Bug KD Toggle",NULL,0};
ToggleVar tToggleBugKB = {0, -1, "Bug KB Toggle",NULL,0};
//ToggleVar vkAutoDropItem = { 0, -1, "Auto Drop Item Key" };


// new
ToggleVar tToggleInfravision = {1, VK_MULTIPLY};
ToggleVar tToggleLightRadiu = {1, VK_MULTIPLY};
ToggleVar tToggleScreenshake = {1, VK_MULTIPLY};
ToggleVar tToggleHiddenCorpse = {0, VK_MULTIPLY};
// end

ToggleVar tToggleAutomapMonsters = {1, VK_DIVIDE};
ToggleVar tToggleAutomapLevelNames = {1, VK_DIVIDE};
ToggleVar tToggleAutomapChests = {0, VK_DIVIDE};

ToggleVar tToggleAutomapItems = {1, VK_DIVIDE, "Show automap items"};
ToggleVar tToggleAutomapMissiles = {1, VK_DIVIDE};

// new
ToggleVar tToggleAutomapCorpse = {0, -1};
ToggleVar tToggleAutomapActiveRoom = {0, -1};
ToggleVar tToggleAutomapReadyRoom = {0, -1};
// end

ToggleVar tToggleHiddenItems = {0, VK_ADD, "Show hidden items"};
ToggleVar tToggleScrollMap = {0, VK_SCROLL};

// new
ToggleVar tToggleAutomapScreenArea = {0, VK_SCROLL};
ToggleVar tToggleMonsterLevel = {0, -1, "Monster Level"};
// end

ToggleVar tTogglePermShowItems = {0, -1, "Perm show items"};

// new
ToggleVar tToggleGameTime = {1, -1, "Game Time"};
ToggleVar tToggleClock = {0, -1, "Game Clock"};
ToggleVar tTogglePermShowOrb = {0, -1, "Perm Show Orbs"};
ToggleVar tToggleInputLine = {0, -1, "New input line"};
ToggleVar tToggleOutTownSelect = {0, -1, "Out of town select"};
// end

ToggleVar tToggleMiniShrine = {0, -1};
ToggleVar tToggleDiagonalScroll = {1, -1, "Diagonal scroll"};
ToggleVar tToggleMonsterTC = {0, -1};
ToggleVar tToggleMonsterResists = {0, -1};

// new
ToggleVar tToggleChickenLife = {1, -1, "Chicken life"};
ToggleVar tToggleChickenHostile = {1, -1, "Chicken hostile"};
ToggleVar tToggleChickenHostileNearby = {1, -1, "Chicken hostile nearby"};
// end

ToggleVar tToggleSocketProtect = {0, -1, "Socket protect"};
ToggleVar tToggleRuneNumbers = {1, -1, "Rune numbers"};
ToggleVar tToggleSocketNumbers = {1, -1, "Socket numbers"};

// new, added by sting
ToggleVar tToggleKeepGameWindow = {0, -1, "Keep game window", &KeepGameWindow};
ToggleVar tToggleMinimap = {0, -1, "Minimap"};
ToggleVar tToggleServerIp = {1, -1};
ToggleVar tToggleChickenDangerousMonster = {0, -1, "Chicken Dangerous Monster"};
ToggleVar tToggleRightClickSwap = {1, -1, "Right Click Swap"};
ToggleVar tToggleUseCustomFont = {0, -1, "Use Custom Font"};
ToggleVar tToggleLayerLevelNo = {0, -1, "Layer And Level No"};

  
// new
ToggleVar tToggleItemLevel = {0, -1};
ToggleVar tToggleItemValue = {0, -1};
ToggleVar tToggleItemIndex = {0, -1};
ToggleVar tToggleUnitNumber = {0, -1};
ToggleVar tToggleViewSocketable = {0, -1, "View socketables"};
ToggleVar tToggleItemBasicStat = {0, -1, "Item basic stat"};
// end



#define pTogglesListEnd tToggleItemBasicStat
