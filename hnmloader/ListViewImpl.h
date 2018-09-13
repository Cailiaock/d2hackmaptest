//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////


#ifndef __LISTVIEW_DEFAULT_IMPL_H__
#define __LISTVIEW_DEFAULT_IMPL_H__

#include "ListViewBase.h"
#include "AutoToolTip.h"
#pragma warning( disable : 4786 )
#include <vector>
#include <set>
#include <map>

class CListViewImpl : public CListViewBase  
{
public:
	CListViewImpl();
	virtual ~CListViewImpl();
public:
	typedef struct _ColumnInfo {
		LPCTSTR	pcszColumnName;
		int		nColumnWidth;
		DWORD	rva;
		void	(CALLBACK *fmtFunc) (LPTSTR, size_t, DWORD);
		int		(CALLBACK *cmpFunc) (DWORD, DWORD);
	}COLUMNINFO, *PCOLUMNINFO;

	typedef struct _ItemInfo {
		DWORD	sys_ptr;			// point to system data
		void (CALLBACK *free)(DWORD); // delete me entry
	}ITEMINFO, *PITEMINFO;

	typedef struct _DispTbl {
		PCOLUMNINFO pSysTbl;
		PCOLUMNINFO pUserTbl;
		int			numSys;
		int			numUser;
	}DISPTBL, *PDISPTBL;

	// Interface default implemention
public:
	virtual void OnInitialUpdate(HWND hwnd);
	virtual void OnUpdate(HWND hwndSender);
	virtual void OnGetdispinfo(LV_DISPINFO* pDispInfo);
	virtual void OnItemChanged(LPNMLISTVIEW pNMListView);
	virtual void OnLClickItem(LPNMITEMACTIVATE pNmItemAct);
	virtual void OnRClickItem(LPNMITEMACTIVATE pNmItemAct);
	virtual void OnDblclk(LPNMITEMACTIVATE pNmItemAct);
	virtual void OnColumnclick(NMLISTVIEW* pNMListView);
	virtual void OnEndtrack(HD_NOTIFY *phdn);
	virtual BOOL OnContextMenuCommand(UINT nID);
	virtual void OnInitContextMenuPopup( HMENU hMenu, int pos );
	virtual void OnFreeAllItems();
	virtual void OnSwitchView();
	

protected:
	virtual void	OnSetImageList( std::vector< UINT >& images ) {}
	virtual DWORD	OnGetItemKey(DWORD item) { return 0; }
	virtual BOOL	OnUpdateItem(DWORD iter, DWORD& key, DWORD& data) { return FALSE; }


	static BOOL CALLBACK OnShowSettingsProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static int CALLBACK CompareFunc(LPARAM lp1, LPARAM lp2, LPARAM lp3);
	void		SetColumnTable( PCOLUMNINFO pSysTbl, int numSys, PCOLUMNINFO pUserTbl = NULL, int numUser = 0 );
	void		LoadImageList();
	PCOLUMNINFO GetColumnInfo( int nCol, BOOL* pIsInSysTbl = NULL );
	void OnShowSettingsInitDialog( HWND hwnd );
	void OnShowSettingsApply( HWND hwnd );

	BOOL OnOpenFile( LPTSTR szFilePath, DWORD cbSize, LPTSTR szFilter, LPCTSTR szDefFilter = NULL, LPTSTR szFileTitle = NULL );

	BOOL UnsupportedMessage();

protected:
	typedef std::set<DWORD> ItemKeyList;

	template<class DelList>
	void RemoveObsoleteItems(const DelList& delItems, ItemKeyList& itemsInView )
	{
		// firstly, iterate items in list view
		DWORD key;
		int count = GetItemCount();
		for(int i = 0; i < count; ++i)
		{
			key = OnGetItemKey(GetItemData(i));
			if( delItems.find((DelList::key_type)key) != delItems.end() )
			{
				DeleteItem(i--);
				--count;
			}
			else
				itemsInView.insert(key);
		}
	}
	template<typename InIt>
	void UpdateListView(InIt first, InIt last, const ItemKeyList& itemsInView )
	{
		BOOL fRedraw = FALSE;
		DWORD key, item;
		// secondly, iterate items in current list
		while(first != last)
		{
			key = 0;
			if(OnUpdateItem((DWORD)&*first, key, item))
				fRedraw = TRUE;
			if(key && itemsInView.find(key) == itemsInView.end() )
			{
				InsertItem( GetItemCount(), item );
			}
			++first;
		}
		if(fRedraw)
			RedrawWindow();
	}
protected:

	struct _SettingsPageID {
		UINT	nPageID;
		UINT	nListCtrlID;
		BOOL	fDirtyFlag;
	}m_SettingsPage;

	struct _ContextMenuID {
		UINT	nMenuID;
		UINT	nSubMenuID;
	}m_ContextMenu;

	typedef struct _SortItemsStruct {
		BOOL			fAscend;
		BOOL			fInSysTbl;
		int				nCol;
		PCOLUMNINFO		pci;
	} SORTITEMSSTRUCT, *PSORTITEMSSTRUCT;

	SORTITEMSSTRUCT		m_SortItemStruct;
	DISPTBL				m_disptbl;
	std::vector<int>	m_index;
	CAutoToolTip		m_autoToolTip;
};


// format functions called by OnGetdispinfo
struct fmtor {
	static void CALLBACK Decimal(LPTSTR pszBuffer, size_t count, DWORD addr);
	static void CALLBACK Hex(LPTSTR pszBuffer, size_t count, DWORD addr);
	static void CALLBACK LargeInt(LPTSTR pszBuffer, size_t count, DWORD addr);
	static void CALLBACK Address(LPTSTR pszBuffer, size_t count, DWORD addr);
	static void CALLBACK Octal(LPTSTR pszBuffer, size_t count, DWORD addr);
	static void CALLBACK INT8(LPTSTR pszBuffer, size_t count, DWORD addr);
	static void CALLBACK INT16(LPTSTR pszBuffer, size_t count, DWORD addr);
	static void CALLBACK Hex16(LPTSTR pszBuffer, size_t count, DWORD addr);
	static void CALLBACK String(LPTSTR pszBuffer, size_t count, DWORD addr);
	static void CALLBACK StlString(LPTSTR pszBuffer, size_t count, DWORD addr);
	static void CALLBACK Time(LPTSTR pszBuffer, size_t count, DWORD addr);
	static void CALLBACK Access(LPTSTR pszBuffer, size_t count, DWORD addr);
};

// compare functions called by CompareFunc by SortItems
struct cmpor {
	static int  CALLBACK UnsignedInteger(DWORD addr1, DWORD addr2);
	static int  CALLBACK UnsignedLargeInt(DWORD addr1, DWORD addr2);
	static int  CALLBACK SignedInteger(DWORD addr1, DWORD addr2);
	static int  CALLBACK UnsignedINT8(DWORD addr1, DWORD addr2);
	static int  CALLBACK UnsignedINT16(DWORD addr1, DWORD addr2);
	static int  CALLBACK String(DWORD addr1, DWORD addr2);
	static int  CALLBACK StlString(DWORD addr1, DWORD addr2);
	static int  CALLBACK Time(DWORD addr1, DWORD addr2);
};

#endif // __LISTVIEW_DEFAULT_IMPL_H__
