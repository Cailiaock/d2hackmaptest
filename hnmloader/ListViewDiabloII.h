//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "ListViewImpl.h"
#include "GameWalker.h"

class CListViewDiabloII : public CListViewImpl
{
	enum {
		eProcessId = 0,
		eDiabloIITitle,
		eGameServerIp,
		eMHVersionInfo,
	};
	typedef CGameWalker::GameInfo GameInfo;
	typedef CGameWalker::GameList GameList;
	typedef CGameWalker::DelGameList DelGameList;
public:
	CListViewDiabloII(void);
	~CListViewDiabloII(void);
public:
	DWORD GetSelectedGameId();
	void OnInstallUpdateHackmap();
	void OnRemoveHackmap();
	void OnLoadOtherModule();
	void OnRevealAutomapAct();
	void LoadModule(LPCTSTR lpszModuleName);
	void UnloadModule(HMODULE hModule);
	virtual void OnUpdate(HWND hwndSender);
	virtual BOOL OnContextMenuCommand(UINT nID);
	virtual void OnInitContextMenuPopup( HMENU hMenu, int pos );
protected:
	virtual void OnSetImageList( std::vector<UINT>& images );
	virtual DWORD OnGetItemKey(DWORD item);
	virtual BOOL OnUpdateItem(DWORD iter, DWORD& key, DWORD& data);

	// details
private:
	BOOL PrepareUpdateListView(ItemKeyList& itemsInView);
	GameInfo* GetSelectedGame();
	void OnInitMenuPopupInstallUpdate( HMENU hMenu, BOOL fLoaded );
	void OnInitMenuPopupRemove( HMENU hMenu, BOOL fLoaded );
	BOOL SecurityCheck(DWORD processId, DWORD flag);
private:
	CGameWalker	m_gameWalker;
	UINT		m_interval;
};
