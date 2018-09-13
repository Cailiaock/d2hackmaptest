//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////


#include "stdhdrs.h"
#include <windowsx.h>
#include "ListViewBase.h"

BOOL CListViewBase::InsertColumn( int nCol, LPCTSTR lpszColumnHeading, int nWidth, int nFormat, int nSubItem )
{
	LVCOLUMN lv;
	lv.mask = LVCF_FMT | LVCF_TEXT;
	lv.pszText = (LPTSTR)lpszColumnHeading;
	lv.cchTextMax = _tcslen( lpszColumnHeading ) + sizeof( TCHAR );
	lv.fmt = nFormat;

	if( nWidth != -1 )
	{
		lv.mask |= LVCF_WIDTH;
		lv.cx = nWidth;
	}
	if( nSubItem != -1 )
	{
		lv.mask |= LVCF_SUBITEM;
		lv.iSubItem = nSubItem;
	}

	return ListView_InsertColumn(m_hwnd, nCol, &lv) != -1;
}

BOOL CListViewBase::SetColumn( int nCol, LPCTSTR lpszColumnHeading, int nWidth, int nFormat, int nSubItem )
{
	LVCOLUMN lv;
	lv.mask = LVCF_FMT | LVCF_TEXT;
	lv.pszText = (LPTSTR)lpszColumnHeading;
	lv.cchTextMax = _tcslen( lpszColumnHeading ) + sizeof( TCHAR );
	lv.fmt = nFormat;

	if( nWidth != -1 )
	{
		lv.mask |= LVCF_WIDTH;
		lv.cx = nWidth;
	}
	if( nSubItem != -1 )
	{
		lv.mask |= LVCF_SUBITEM;
		lv.iSubItem = nSubItem;
	}

	return ListView_SetColumn(m_hwnd, nCol, &lv);
}

BOOL CListViewBase::DeleteColumn( int nCol )
{
	return ListView_DeleteColumn( m_hwnd, nCol );
}

BOOL CListViewBase::SetColumnWidth( int nCol, int cx )
{
	return ListView_SetColumnWidth( m_hwnd, nCol, cx );
}

int	CListViewBase::GetColumnWidth( int nCol )
{
	return ListView_GetColumnWidth( m_hwnd, nCol );
}

int CListViewBase::GetColumnCount()
{
	HWND hwnd = ListView_GetHeader( m_hwnd );
	return (int) ::SendMessage(hwnd, HDM_GETITEMCOUNT, 0, 0L);
}

BOOL CListViewBase::InsertItem(int nItem, LPARAM lParam, int nImageIndex )
{
	LV_ITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_PARAM;
	lvi.iItem = nItem;
	lvi.pszText = LPSTR_TEXTCALLBACK;
	lvi.lParam = lParam;
	lvi.iSubItem = 0;
	if( nImageIndex != -1 )
	{
		lvi.mask |= LVIF_IMAGE;
		lvi.iImage = nImageIndex;
	}

	return ListView_InsertItem( m_hwnd, &lvi ) != -1;
}

BOOL CListViewBase::SetItemImage( int nItem, int nImageIndex )
{
	LV_ITEM lvi;
	lvi.mask = LVIF_IMAGE;
	lvi.iItem = nItem;
	lvi.iImage = nImageIndex;

	return ListView_SetItem( m_hwnd, &lvi );
}


BOOL CListViewBase::SortItems( PFNLVCOMPARE pfnCompare, DWORD dwData )
{
	return ListView_SortItems( m_hwnd, pfnCompare, dwData);
}

int CListViewBase::GetNextItem( int nItem, int nFlags )
{
	return ListView_GetNextItem( m_hwnd, nItem, nFlags );
}

int CListViewBase::GetNextSelectedItem( int nItem )
{
	return GetNextItem( nItem, LVNI_SELECTED );
}

void CListViewBase::SelectItem( int nItem )
{
	if( nItem == -1 )
	{
		// clear selected item
		ListView_SetItemState( m_hwnd, GetNextSelectedItem(),
						~(LVIS_SELECTED | LVIS_FOCUSED), LVIS_SELECTED | LVIS_FOCUSED );
	}
	else
		ListView_SetItemState( m_hwnd, nItem, LVIS_SELECTED, LVIS_SELECTED );
}

BOOL CListViewBase::DeleteItem( int nItem )
{
	return ListView_DeleteItem( m_hwnd, nItem );
}

BOOL CListViewBase::DeleteAllItems()
{
	return ListView_DeleteAllItems( m_hwnd );
}

int CListViewBase::GetItemCount()
{
	return ListView_GetItemCount( m_hwnd );
}

DWORD CListViewBase::GetItemData( int nItem )
{
	LVITEM lvi;
	lvi.mask = LVIF_PARAM;
	lvi.iItem = nItem;

	ListView_GetItem( m_hwnd, &lvi);
	return lvi.lParam;
}

BOOL CListViewBase::SetItemData( int nItem, DWORD dwData )
{
	LVITEM lvi;
	lvi.mask = LVIF_PARAM;
	lvi.iItem = nItem;
	lvi.iSubItem = 0;
	lvi.lParam = dwData;
	return ListView_SetItem( m_hwnd, &lvi);
}

BOOL CListViewBase::EnsureVisible( int nItem, BOOL bPartialOK )
{
	return ListView_EnsureVisible( m_hwnd, nItem, bPartialOK );
}

DWORD CListViewBase::SetExtendedStyle(DWORD dwNewStyle)
{
	return ListView_SetExtendedListViewStyle(m_hwnd, dwNewStyle );
}

BOOL CListViewBase::SetRedraw( BOOL bRedraw )
{
	return ::SendMessage( m_hwnd, WM_SETREDRAW, bRedraw, 0 ) == 0;
}

void CListViewBase::RedrawWindow()
{
	::InvalidateRect( m_hwnd, NULL, TRUE );
}
