//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#include "stdhdrs.h"
#include "ListViewDiabloII.h"
#include "IntegrityWalker.h"
#include "ModuleWalker.h"
#include "RevealMapWorker.h"
#include "resource.h"
#include "AutoHandle.h"
#include "MaphackChecker.h"
#include "ConfigLoader.h"

static void CALLBACK fmtMaphackVersion(LPTSTR pszBuffer, size_t count, DWORD addr)
{
	switch(*(int*)addr)
	{
	case MH_HACKMAP:
		_tcsncpy(pszBuffer, _T("Sting's Hackmap"), count );
		break;
	case MH_51EDITED_MAPHACK:
		_tcsncpy(pszBuffer, _T("Sting's 5.1 edited MH"), count );
		break;
	case MH_NOT_LOADED:
		_tcsncpy(pszBuffer, _T("Not loaded"), count );
		break;
	default:
		_tcsncpy(pszBuffer, _T("Unkown"), count );
		break;
	}
}

typedef CGameWalker::GameInfo GI_T_;
static CListViewImpl::COLUMNINFO _pColumnInfo[] = {
	_T("PID"),			60, OFFSETOF(GI_T_, processId),
	fmtor::Decimal,		cmpor::UnsignedInteger,

	_T("Title"),		200, OFFSETOF(GI_T_, gameTitle),
	fmtor::StlString,	cmpor::StlString,

	_T("Game Server"),	125, OFFSETOF(GI_T_, gameServerIp),
	fmtor::StlString,	cmpor::StlString,

	_T("Maphack"),		90, OFFSETOF(GI_T_, mhVersionInfo),
	fmtMaphackVersion,	cmpor::UnsignedInteger,
};

CListViewDiabloII::CListViewDiabloII(void)
				:m_interval(0)
{
	SetColumnTable( _pColumnInfo, sizeof(_pColumnInfo) / sizeof( _pColumnInfo[0] ) );

	m_ContextMenu.nMenuID = IDR_TRAYMENU;
	m_ContextMenu.nSubMenuID = 1;

	m_index.push_back(eProcessId);
	m_index.push_back(eDiabloIITitle);
	m_index.push_back(eGameServerIp);
	m_index.push_back(eMHVersionInfo);
}

CListViewDiabloII::~CListViewDiabloII(void)
{
}

void CListViewDiabloII::OnSetImageList( std::vector<UINT>& images )
{
	images.push_back( IDI_DIABLOII );
}

DWORD CListViewDiabloII::OnGetItemKey(DWORD item)
{
	return reinterpret_cast<GameInfo*>(item)->processId;
}

BOOL CListViewDiabloII::OnUpdateItem(DWORD iter, DWORD& key, DWORD& data)
{
	GameList::value_type& item = *reinterpret_cast<GameList::value_type*>(iter);
	key = item.first;
	data = (DWORD)&item.second;

	GameInfo& gi = item.second;
	HMODULE hModule = CMaphackChecker::CheckMaphack(gi.processId, gi.mhVersionInfo);
	CRevealMapWorker rmWorker;
	DWORD oldId = gi.gameServerId;
	rmWorker.GetGameServerInfo(gi.processId, gi.hwnd, gi.gameServerId, gi.gameServerIp);
	BOOL fRedraw = (hModule != gi.hModuleHackmap) || (oldId != gi.gameServerId);
	gi.hModuleHackmap = hModule;
	
	if(g_config.fAutoLoadHackmap && !gi.hModuleHackmap && SecurityCheck(gi.processId, g_config.fAutoLoadHackmap))
	{
		gi.hModuleHackmap = m_gameWalker.LoadModule( gi.processId, gi.hwnd, _T("d2hackmap.dll") );
		fRedraw = gi.hModuleHackmap != NULL;
		gi.mhVersionInfo = gi.hModuleHackmap ? MH_HACKMAP : MH_NOT_LOADED;
	}
	if(g_config.fAutoRevealMap)
	{
		DWORD flag = 0;
		switch(g_config.fAutoRevealMap)
		{
		case 1:
			flag |= RM_INTEGRITY_SCAN|RM_NOWARNINGS;
			if(g_config.fScanGameDllsOnly) flag |= RM_SCAN_GAMEDLLS_ONLY;
			break;
		case 2:
			flag |= RM_NOWARNINGS;
			break;
		default:
			flag = -1;
			break;
		}
		if(flag != -1)
		{
			if((m_interval++)%g_config.nAutoRevealMapFreq == 0)
			{
				if(m_interval%(g_config.nAutoRevealMapFreq*2) == 0)
				{
					m_interval = 0;
					if(g_config.fReportIScanInGame)
						flag |= RM_EXTRA_INTEGRITY_CHECK|RM_REPORT_ISCAN_TOGAME;
				}
				CRevealMapWorker worker;
				worker.AutoRevealAutomapAct(gi.processId, gi.hwnd, gi.gameStat, flag);
			}
		}
	}
	return fRedraw;
}

BOOL CListViewDiabloII::PrepareUpdateListView(ItemKeyList& itemsInView)
{
	DelGameList delGames(m_gameWalker.m_games);
	m_gameWalker.Walk(delGames);
	RemoveObsoleteItems(delGames, itemsInView);
	return TRUE;
}

void CListViewDiabloII::OnUpdate(HWND hwndSender)
{
	ItemKeyList itemsInView;
	if(PrepareUpdateListView(itemsInView))
	{
		UpdateListView(
			m_gameWalker.m_games.begin(),
			m_gameWalker.m_games.end(),
			itemsInView
			);
	}
}


CListViewDiabloII::GameInfo* CListViewDiabloII::GetSelectedGame()
{
	int index = GetNextSelectedItem();
	if(index == -1)
	{
		if( GetItemCount() == 1 )
			index = 0;
		else
		{
			GlobalReportStatus(IDS_ERR_SELECT_GAME_FIRST);
			return NULL;
		}
	}
	return (GameInfo*)GetItemData(index);
}

void CListViewDiabloII::OnInstallUpdateHackmap()
{
	GameInfo* pgi = GetSelectedGame();
	if(pgi)
	{
		if(pgi->hModuleHackmap)
			m_gameWalker.UnloadModule(pgi->processId, pgi->hwnd, pgi->hModuleHackmap);
		m_gameWalker.LoadModule(pgi->processId, pgi->hwnd, _T("d2hackmap.dll"));
		RedrawWindow();
	}
}

void CListViewDiabloII::OnRemoveHackmap()
{
	GameInfo* pgi = GetSelectedGame();
	if(pgi)
	{
		if(pgi->hModuleHackmap)
			m_gameWalker.UnloadModule(pgi->processId, pgi->hwnd, pgi->hModuleHackmap);
		RedrawWindow();
	}
}

void CListViewDiabloII::OnLoadOtherModule()
{
	GameInfo* pgi = GetSelectedGame();
	if(!pgi) return;
	
	static TCHAR szFilter[] = _T("Dll Files (*.dll)|*.dll|All Files (*.*)|*.*||");
	TCHAR szPath[MAX_PATH];
	
	if( !OnOpenFile( szPath, COUNTOF(szPath), szFilter, _T("dll"), _T("Select a File") ) )
		return;
	
	HMODULE h = m_gameWalker.LoadModule(pgi->processId, pgi->hwnd, szPath);
	if(h)
		GlobalFormatAndReportStatus(IDS_LOAD_SUCCESS, szPath, pgi->processId, h);
	else
		GlobalFormatAndReportStatus(IDS_LOAD_FAIL, szPath);
}

void CListViewDiabloII::OnRevealAutomapAct()
{
	GameInfo* pgi = GetSelectedGame();
	if(!pgi) return;
	CRevealMapWorker worker;
	DWORD flag = RM_INTEGRITY_SCAN;
	if(g_config.fScanGameDllsOnly) flag |= RM_SCAN_GAMEDLLS_ONLY;
	worker.RevealAutomapAct(pgi->processId, pgi->hwnd, flag);
}

void CListViewDiabloII::LoadModule(LPCTSTR lpszModuleName)
{
	GameInfo* pgi = GetSelectedGame();
	if(!pgi) return;

	HMODULE h = m_gameWalker.LoadModule(pgi->processId, pgi->hwnd, lpszModuleName);
	if(h)
		GlobalFormatAndReportStatus(IDS_LOAD_SUCCESS, lpszModuleName, pgi->processId, h);
	else
		GlobalFormatAndReportStatus(IDS_LOAD_FAIL, lpszModuleName);
}

BOOL CListViewDiabloII::OnContextMenuCommand(UINT nID)
{
	switch( nID )
	{
	case ID_D2MENU_LOADHACKMAPMANUALLY:
		OnInstallUpdateHackmap();
		break;
		
	case ID_D2MENU_UNLOADHACKMAP:
		OnRemoveHackmap();
		break;

	case ID_D2MENU_LOADOTHERMODULE:
		OnLoadOtherModule();
		break;
	}
	
	return FALSE;
}

void CListViewDiabloII::OnInitContextMenuPopup( HMENU hMenu, int pos )
{
	if(pos == 0)
	{
		GameInfo* pgi = GetSelectedGame();
		if(!pgi) return;
		BOOL fLoaded = CMaphackChecker::TestHackmapLoaded(pgi->processId) != NULL;
		OnInitMenuPopupInstallUpdate( hMenu, fLoaded );
		OnInitMenuPopupRemove( hMenu, fLoaded );
	}
}

inline
void CListViewDiabloII::OnInitMenuPopupInstallUpdate( HMENU hMenu, BOOL fLoaded )
{
	MENUITEMINFO mii;
	TCHAR szTitle[256];
	if (LoadString(fLoaded ? IDS_UPDATEHACKMAP : IDS_LOADHACKMAP, szTitle, COUNTOF(szTitle)-1))
		mii.dwTypeData = szTitle;
	else
	{
		if(fLoaded)
			mii.dwTypeData = _T("Update Hackmap");
		else
			mii.dwTypeData = _T("Install Hackmap");
	}
	
	mii.cbSize = sizeof(mii);
	mii.fMask = MIIM_TYPE;
	mii.fType = MFT_STRING;
	
	mii.cch = (_tcslen( mii.dwTypeData )+1)*sizeof(TCHAR);
	::SetMenuItemInfo( hMenu, ID_D2MENU_LOADHACKMAPMANUALLY, FALSE, &mii );
}

inline
void CListViewDiabloII::OnInitMenuPopupRemove( HMENU hMenu, BOOL fLoaded )
{
	UINT fEnable = fLoaded ? MF_ENABLED : MF_GRAYED;
	EnableMenuItem(hMenu, ID_D2MENU_UNLOADHACKMAP, fEnable|MF_BYCOMMAND);
}

DWORD CListViewDiabloII::GetSelectedGameId()
{
	GameInfo* pgi = GetSelectedGame();
	return pgi ? pgi->processId : 0;
}

void CListViewDiabloII::UnloadModule(HMODULE hModule)
{
	GameInfo* pgi = GetSelectedGame();
	if(pgi)
		CGameWalker::UnloadModule(pgi->processId, pgi->hwnd, hModule);
}

int CListViewDiabloII::SecurityCheck(DWORD processId, DWORD flag)
{
	CModuleWalker dllWalker;
	if(!dllWalker.Walk(processId)) return FALSE;
	if(dllWalker.FindModule(_T("Extrawork.dll")) != NULL )
		return FALSE;
	if(dllWalker.FindModule(_T("IX86ver"), TRUE, FALSE) != NULL)
		return FALSE;
	if(flag != 2)
	{
		CIntegrityWalker iscan;
		if(!iscan.Walk(processId, g_config.fScanGameDllsOnly) || !iscan.m_mods.empty())
		{
			g_config.fAutoLoadHackmap = 0;
			return FALSE;
		}
	}
	return TRUE;
}
