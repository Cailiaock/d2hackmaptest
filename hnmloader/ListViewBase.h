//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////


#ifndef __LISTVIEW_BASE_H__
#define __LISTVIEW_BASE_H__

#pragma warning( disable : 4786 )
#include <CommCtrl.h>

class CListViewBase  
{
public:
	virtual void OnInitialUpdate(HWND hwnd) = 0;
	virtual void OnUpdate(HWND hwndSender) = 0;
	virtual void OnGetdispinfo(LV_DISPINFO* pDispInfo) = 0;
	virtual void OnItemChanged(LPNMLISTVIEW pNMListView) = 0;
	virtual void OnLClickItem(LPNMITEMACTIVATE pNmItemAct) = 0;
	virtual void OnRClickItem(LPNMITEMACTIVATE pNmItemAct) = 0;
	virtual void OnDblclk(LPNMITEMACTIVATE pNmItemAct) = 0;
	virtual void OnColumnclick(NMLISTVIEW* pNMListView) = 0;
	virtual void OnEndtrack(HD_NOTIFY *phdn) = 0;
	virtual BOOL OnContextMenuCommand(UINT nID) = 0;
	virtual void OnInitContextMenuPopup( HMENU hMenu, int pos ) = 0;
	virtual void OnInactive() { m_hwnd = NULL; }
	virtual void OnFreeAllItems() = 0;
	virtual void OnSwitchView() = 0;

	HWND GetSafeHwnd() const { return m_hwnd; }
protected:
	CListViewBase() :m_hwnd(NULL) {}

	// helpers, used by derived class only
	BOOL	InsertColumn( int nCol, LPCTSTR lpszColumnHeading, int nWidth = -1, int nFormat = LVCFMT_LEFT, int nSubItem = -1 );
	BOOL	SetColumn( int nCol, LPCTSTR lpszColumnHeading, int nWidth = -1, int nFormat = LVCFMT_LEFT, int nSubItem = -1 );
	BOOL	DeleteColumn( int nCol );
	BOOL	SetColumnWidth( int nCol, int cx );
	int		GetColumnWidth( int nCol );
	int		GetColumnCount();
	BOOL	InsertItem( int nItem, LPARAM lParam, int nImageIndex = -1 );
	BOOL	SetItemImage( int nItem, int nImageIndex );
	BOOL	SortItems( PFNLVCOMPARE pfnCompare, DWORD dwData );
	int		GetNextItem( int nItem, int nFlags );
	int		GetNextSelectedItem( int nItem = -1 );
	void	SelectItem( int nItem );
	BOOL	DeleteItem( int nItem );
	BOOL	DeleteAllItems();
	int		GetItemCount();
	DWORD	GetItemData( int nItem );
	BOOL	SetItemData( int nItem, DWORD dwData );
	BOOL	EnsureVisible( int nItem, BOOL bPartialOK = FALSE );
	DWORD	SetExtendedStyle( DWORD dwNewStyle );
	BOOL	SetRedraw( BOOL bRedraw );
	void	RedrawWindow();

	// give access right to derived class
protected:
	HWND	m_hwnd;
};

#endif // __LISTVIEW_BASE_H__
