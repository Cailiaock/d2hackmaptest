//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "ListViewImpl.h"
#include "ModuleWalker.h"
#define WM_UNLOADMODULE WM_USER+101

class CListViewModule  : public CListViewImpl
{
	enum {
		eImagePath = 0,
		eImageName,
		eImageBase,
		eImageSize,
	};
	typedef CModuleWalker::DllInfo DllInfo;
	typedef CModuleWalker::DllList DllList;
	typedef CModuleWalker::DelDllList DelDllList;

public:
	CListViewModule(void);
	~CListViewModule(void);
	virtual void OnUpdate(HWND hwndSender);
	virtual BOOL OnContextMenuCommand(UINT nID);
	void TargettingProcess(DWORD pid) { m_processId = pid; }
protected:
	virtual void OnSetImageList( std::vector<UINT>& images );
	virtual DWORD OnGetItemKey(DWORD item);
	virtual BOOL OnUpdateItem(DWORD iter, DWORD& key, DWORD& data);

	// details
private:
	BOOL PrepareUpdateListView(ItemKeyList& itemsInView);
	void OnUnloadDll();
	DllInfo* GetSelectedDll();
	void ResetStat();
private:
	CModuleWalker m_moduleWalker;
	DWORD m_processId;
};
