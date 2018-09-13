//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////


#ifndef __LISTVIEW_INTEGRITY_SCAN_H__
#define __LISTVIEW_INTEGRITY_SCAN_H__

#include "ListViewImpl.h"
#include "IntegrityWalker.h"

class CListViewIntegrityScan  : public CListViewImpl
{
	enum {
		eAddress = 0,
		eDllName,
		eLength,
		eModified,
		eOriginal,
	};
	typedef CIntegrityWalker::ModInfo ModInfo;
	typedef CIntegrityWalker::ModList ModList;
	typedef CIntegrityWalker::DelModList DelModList;
public:
	CListViewIntegrityScan();
	~CListViewIntegrityScan();
	virtual void OnUpdate(HWND hwndSender);
	virtual BOOL OnContextMenuCommand(UINT nID);
	virtual void OnSwitchView() { OnUpdateNow(); }
	void TargettingProcess(DWORD pid) { m_processId = pid; }
	void OnUpdateNow();
protected:
	virtual void OnSetImageList( std::vector<UINT>& images );
	virtual DWORD OnGetItemKey(DWORD item);
	virtual BOOL OnUpdateItem(DWORD iter, DWORD& key, DWORD& data);
	// details
private:
	BOOL PrepareUpdateListView(ItemKeyList& itemsInView);
	void OnRestore();
	void OnScanNow();
	ModInfo* GetSelectedModification();
	void ResetStat();
private:
	CIntegrityWalker m_iscanWalker;
	DWORD m_processId;
	UINT m_interval;
};

#endif // __LISTVIEW_INTEGRITY_SCAN_H__
