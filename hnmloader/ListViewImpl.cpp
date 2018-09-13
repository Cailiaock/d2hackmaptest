//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////


#include "stdhdrs.h"
#include <CommDlg.h>
#include <windowsx.h>
#include "ListViewImpl.h"

#pragma comment(lib, "comctl32.lib")

#ifndef LVS_EX_LABELTIP
#define LVS_EX_LABELTIP 0x00004000
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CListViewImpl::CListViewImpl()
{
	m_ContextMenu.nMenuID = 0;
	m_ContextMenu.nSubMenuID = -1;
	m_SettingsPage.nPageID = 0;
	m_SettingsPage.nListCtrlID = 0;
	m_SettingsPage.fDirtyFlag = FALSE;
}

CListViewImpl::~CListViewImpl()
{
}

void CListViewImpl::LoadImageList()
{
	std::vector< UINT > images;
	int index, count;

	HICON hIconItem;     // icon for list view items 
	HMODULE hInst;
	HIMAGELIST hImageList;   // image list for views 

	// destroy old image list
	hImageList =	ListView_SetImageList( m_hwnd, NULL, LVSIL_SMALL);
	if( hImageList != NULL )
			ImageList_Destroy( hImageList );

	OnSetImageList( images );


	hInst = ::GetModuleHandle( NULL );

	hImageList = ImageList_Create( ::GetSystemMetrics(SM_CXSMICON), 
								::GetSystemMetrics(SM_CYSMICON),
								ILC_MASK,
								1,
								1);

	count = images.size();
	for(index = 0; index < count; index++)
	{
		hIconItem = ::LoadIcon (hInst, MAKEINTRESOURCE(images[index]) );
		ImageList_AddIcon( hImageList, hIconItem );
		::DestroyIcon( hIconItem );
	}

	
	// Assign the image lists to the list view control. 
	ListView_SetImageList( m_hwnd, hImageList, LVSIL_SMALL);
}

void CListViewImpl::OnFreeAllItems()
{
	DeleteAllItems();
}

void CListViewImpl::OnSwitchView()
{
}


void CListViewImpl::OnInitialUpdate(HWND hwnd)
{
	int i, ci, cc, nwidth;
	PCOLUMNINFO pci;

	ASSERT( hwnd != NULL );

	// Initialize member data
	m_hwnd = hwnd;
	m_SortItemStruct.fAscend = TRUE;
	m_SortItemStruct.fInSysTbl = TRUE;
	m_SortItemStruct.nCol = -1;
	m_SortItemStruct.pci = NULL;

	m_autoToolTip.SubClass(hwnd);

	// let header control like a push button
	HWND hwndHeader = ListView_GetHeader( hwnd );
	LONG style = ::GetWindowLong( hwndHeader, GWL_STYLE );
	style |= HDS_BUTTONS;
	::SetWindowLong( hwndHeader, GWL_STYLE, style );


	// modify style
	SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_LABELTIP | LVS_EX_HEADERDRAGDROP );

	// Set Imagelist
	LoadImageList();

	OnFreeAllItems();

	ci = m_index.size();	// count of index
	cc = GetColumnCount();	// count of columns

	// first, modify column info
	for(i = 0, nwidth = 0; i < ci && i < cc; i++)
	{
		pci = GetColumnInfo( i );

		nwidth += pci->nColumnWidth;
		SetColumn(i,
				pci->pcszColumnName,
				pci->nColumnWidth);
	}

	// second, add new columns, if needed
	for(; i < ci; i++)
	{
		pci = GetColumnInfo( i );

		nwidth += pci->nColumnWidth;
		InsertColumn(i,
				pci->pcszColumnName,
				pci->nColumnWidth);
	}

	// now, delete the rest, if needed
	for( ; i <= cc; cc--)
	{
		DeleteColumn( cc );
	}


	//
	// stretch the last column to align with right edge
	//

	// subtract the last column width
	OnEndtrack( NULL );
/*	if( ci > 0 )
	{
		len = nwidth - pci->nColumnWidth;

		RECT rt;
		// view width
		::GetWindowRect(hwnd, &rt);

		
		len = (rt.right-rt.left) - len;
		if( len > 0 )
			SetColumnWidth(--i, len);
	}
*/
	// Update list view
	OnUpdate( m_hwnd );
}

void CListViewImpl::OnUpdate(HWND hwndSender)
{
}

void CListViewImpl::OnGetdispinfo(LV_DISPINFO* pDispInfo)
{
	TCHAR szText[1024];
	BOOL bInSysTbl;
	DWORD addr;
	PCOLUMNINFO pci;
	PITEMINFO pItemInfo;

	ASSERT( (size_t)pDispInfo->item.iSubItem < m_index.size() );

	// Reset buffer
	szText[0] = _T('\0');

	pci = GetColumnInfo( pDispInfo->item.iSubItem, &bInSysTbl );

	pItemInfo = (PITEMINFO)pDispInfo->item.lParam;

	if( bInSysTbl )
		addr = pItemInfo->sys_ptr+pci->rva;
	else
		addr = (DWORD)pItemInfo+pci->rva;

	pci->fmtFunc( szText,
			( sizeof(szText)-1 ) / sizeof( szText[0] ),
			addr);

	pDispInfo->item.pszText = szText;
}

void CListViewImpl::OnItemChanged(LPNMLISTVIEW pNMListView)
{
	if( pNMListView->iItem == -1 )
		SelectItem( -1 ); // clear focus
}

void CListViewImpl::OnLClickItem(LPNMITEMACTIVATE pNmItemAct)
{
}

void CListViewImpl::OnRClickItem(LPNMITEMACTIVATE pNmItemAct)
{
	if( m_ContextMenu.nMenuID == 0 || m_ContextMenu.nSubMenuID == -1
	||  pNmItemAct->iItem == -1 )
		return;

	POINT pt;
	GetCursorPos(&pt);

	// load popup menu
	HMENU hMenu = ::LoadMenu( g_hResInstance, MAKEINTRESOURCE(m_ContextMenu.nMenuID) );
	HMENU hSubMenu = ::GetSubMenu( hMenu, m_ContextMenu.nSubMenuID );

	if( hSubMenu != NULL)
		::TrackPopupMenuEx(hSubMenu, TPM_LEFTALIGN | TPM_VERTICAL, pt.x, pt.y, GetParent(m_hwnd), NULL);
	if( hMenu != NULL )
		::DestroyMenu(hMenu);
}

void CListViewImpl::OnDblclk(LPNMITEMACTIVATE pNmItemAct)
{
}

void CListViewImpl::OnColumnclick(NMLISTVIEW* pNMListView)
{
	m_SortItemStruct.pci = GetColumnInfo( pNMListView->iSubItem, &m_SortItemStruct.fInSysTbl );
	if( m_SortItemStruct.nCol != pNMListView->iSubItem )
		m_SortItemStruct.fAscend = TRUE;
	else // reverse flag
		m_SortItemStruct.fAscend = ~m_SortItemStruct.fAscend & 1;

	m_SortItemStruct.nCol = pNMListView->iSubItem;

	ASSERT( m_SortItemStruct.pci != NULL );

	SortItems(CompareFunc, (DWORD)&m_SortItemStruct);
}

void CListViewImpl::OnEndtrack(HD_NOTIFY *phdn)
{
	HWND hwnd = ListView_GetHeader( m_hwnd );
	int i, c, nWidth, len;
	RECT rt;

	if( hwnd == NULL )
		return;
	c = Header_GetItemCount( hwnd );
	if( c <= 0 )
		return;

	for(i = 0, nWidth = 0; i < c; i++)
	{
		nWidth += GetColumnWidth( i );
	}
	
	len = nWidth - GetColumnWidth( c-1 );

	// view width
	::GetWindowRect( m_hwnd, &rt);

	
	len = (rt.right-rt.left) - len;
	if( len > 0 )
		SetColumnWidth( c-1, len);
}

BOOL CListViewImpl::OnContextMenuCommand(UINT nID)
{
	return FALSE;
}

void CListViewImpl::OnInitContextMenuPopup( HMENU hMenu, int pos )
{
}

int CALLBACK CListViewImpl::CompareFunc(LPARAM lp1, LPARAM lp2, LPARAM lp3)
{
	DWORD addr1, addr2;
	PSORTITEMSSTRUCT psis = (PSORTITEMSSTRUCT)lp3;
	PITEMINFO pItem1 = (PITEMINFO)lp1;
	PITEMINFO pItem2 = (PITEMINFO)lp2;

	if( psis->fInSysTbl )
	{
		addr1 = pItem1->sys_ptr+psis->pci->rva;
		addr2 = pItem2->sys_ptr+psis->pci->rva;
	}
	else
	{
		addr1 = (DWORD)pItem1+psis->pci->rva;
		addr2 = (DWORD)pItem2+psis->pci->rva;
	}

	int nResult = psis->pci->cmpFunc(addr1,
									addr2);

	if( !psis->fAscend )
		nResult = -nResult;
	
	return nResult;
}

void CListViewImpl::SetColumnTable( PCOLUMNINFO pSysTbl, int numSys, PCOLUMNINFO pUserTbl, int numUser )
{
	m_disptbl.pSysTbl = pSysTbl;
	m_disptbl.numSys = numSys;

	m_disptbl.pUserTbl = pUserTbl;
	m_disptbl.numUser = numUser;
}

CListViewImpl::PCOLUMNINFO CListViewImpl::GetColumnInfo( int nCol, BOOL* pIsInSysTbl )
{
	ASSERT( nCol >= 0 && (size_t)nCol < m_index.size() );

	BOOL bInSysTbl = FALSE;
	PCOLUMNINFO pci;
	int index = m_index[ nCol ];

	ASSERT( index >= 0 );

	if( index < m_disptbl.numSys )
		pci = &m_disptbl.pSysTbl[ index ];
	else if ( index-m_disptbl.numSys < m_disptbl.numUser )
	{
		pci = &m_disptbl.pUserTbl[ index-m_disptbl.numSys ];
		bInSysTbl = FALSE;
	}
	else
	{
		pci = NULL;
		bInSysTbl = FALSE;
	}

	ASSERT( pci != NULL );
	if( pIsInSysTbl != NULL )
		*pIsInSysTbl = bInSysTbl;
	return pci;
}

void CListViewImpl::OnShowSettingsInitDialog( HWND hwnd )
{
	int i, c;
	HWND hListCtrl;

	hListCtrl = ::GetDlgItem( hwnd, m_SettingsPage.nListCtrlID );
	for (i = 0; i < m_disptbl.numSys; i++)
		ListBox_AddString( hListCtrl, m_disptbl.pSysTbl[i].pcszColumnName );
	for (i = 0; i < m_disptbl.numUser; i++)
		ListBox_AddString( hListCtrl, m_disptbl.pUserTbl[i].pcszColumnName );

	c = m_index.size();
	for(i = 0; i < c; i++)
		ListBox_SetSel( hListCtrl, TRUE, m_index[i] );
}

void CListViewImpl::OnShowSettingsApply( HWND hwnd )
{
	int i, c;
	HWND hListCtrl;

	hListCtrl = ::GetDlgItem( hwnd, m_SettingsPage.nListCtrlID );
	m_index.clear();
	c = m_disptbl.numSys+m_disptbl.numUser;
	for(i = 0; i < c; i++)
	{
		if( ListBox_GetSel( hListCtrl, i ) > 0 )
			m_index.push_back( i );
	}

	// Update
	if( m_hwnd != NULL )
		OnInitialUpdate( m_hwnd );
}

BOOL CALLBACK CListViewImpl::OnShowSettingsProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CListViewImpl* _this = (CListViewImpl*)::GetWindowLong( hwnd, GWL_USERDATA );
	switch( uMsg )
	{
	case WM_INITDIALOG:
		{
			PROPSHEETPAGE* psp = (PROPSHEETPAGE*)lParam;

			SetWindowLong(hwnd, GWL_USERDATA, psp->lParam);
			_this = (CListViewImpl*)psp->lParam;
			_this->OnShowSettingsInitDialog( hwnd );
			break;
		}

	case WM_NOTIFY:
		{
			LPPSHNOTIFY lppsn = (LPPSHNOTIFY) lParam;
			switch( lppsn->hdr.code )
			{
			case PSN_APPLY:
				if( _this->m_SettingsPage.fDirtyFlag )
				{
					_this->OnShowSettingsApply( hwnd );
					_this->m_SettingsPage.fDirtyFlag = FALSE;
				}
				break;
			}
			break;
		}

	case WM_COMMAND:
		switch( HIWORD(wParam) )
		{
		case LBN_SELCHANGE:
			_this->m_SettingsPage.fDirtyFlag = TRUE;
			PropSheet_Changed( ::GetParent(hwnd), hwnd );
			break;
		}
		break;
	}
	return 0;
}

BOOL CListViewImpl::UnsupportedMessage()
{
	::MessageBox( m_hwnd, _T("Sorry, not support yet."), _T(":-)"), MB_OK );
	return TRUE;
}

BOOL CListViewImpl::OnOpenFile( LPTSTR szFilePath, DWORD cbSize, LPTSTR szFilter, LPCTSTR szDefFilter, LPTSTR szFileTitle )
{
	ASSERT(szFilePath);
	szFilePath[0] = _T('\0');
	if( szFilter == NULL )
		return FALSE;


	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn)); // initialize structure to 0/NULL

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hwnd;
	ofn.lpstrFile = szFilePath;
	ofn.nMaxFile = cbSize-1;
	ofn.lpstrDefExt = szDefFilter;
	ofn.lpstrFileTitle = szFileTitle;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_READONLY | OFN_EXPLORER;

	// Translate filter into commdlg format (lots of \0)
	LPTSTR pch = szFilter; // modify the buffer in place
	while ((pch = _tcschr(pch, _T('|')) ) != NULL)
		*pch++ = _T('\0');
	ofn.lpstrFilter = szFilter;

	return ::GetOpenFileName(&ofn);
}

///////////////////////////////////////////////////////////////////
//	format values with deferent type to string
///////////////////////////////////////////////////////////////////
void CALLBACK fmtor::Decimal(LPTSTR pszBuffer, size_t count, DWORD addr)
{
	_sntprintf(pszBuffer, count, _T("%d"), *( (int*)addr) );
}

void CALLBACK fmtor::Hex(LPTSTR pszBuffer, size_t count, DWORD addr)
{
	_sntprintf(pszBuffer, count, _T("0x%X"), *( (DWORD*)addr) );
}

void CALLBACK fmtor::LargeInt(LPTSTR pszBuffer, size_t count, DWORD addr)
{
	_sntprintf(pszBuffer, count, _T("0x%08X%08X"), ((LARGE_INTEGER*)addr)->HighPart, ((LARGE_INTEGER*)addr)->LowPart);
}

void CALLBACK fmtor::Address(LPTSTR pszBuffer, size_t count, DWORD addr)
{
	_sntprintf(pszBuffer, count, _T("0x%08X"), *( (DWORD*)addr) );
}

void CALLBACK fmtor::Octal(LPTSTR pszBuffer, size_t count, DWORD addr)
{
	_sntprintf(pszBuffer, count, _T("%o"), *( (UINT*)addr) );
}

void CALLBACK fmtor::INT8(LPTSTR pszBuffer, size_t count, DWORD addr)
{
	_sntprintf(pszBuffer, count, _T("%d"), *( (UCHAR*)addr) );
}

void CALLBACK fmtor::INT16(LPTSTR pszBuffer, size_t count, DWORD addr)
{
	_sntprintf(pszBuffer, count, _T("%d"), *( (USHORT*)addr) );
}

void CALLBACK fmtor::Hex16(LPTSTR pszBuffer, size_t count, DWORD addr)
{
	_sntprintf(pszBuffer, count, _T("0x%X"), *( (USHORT*)addr) );
}

void CALLBACK fmtor::String(LPTSTR pszBuffer, size_t count, DWORD addr)
{
	_tcsncpy( pszBuffer, (LPCTSTR)addr, count);
}

void CALLBACK fmtor::StlString(LPTSTR pszBuffer, size_t count, DWORD addr)
{
	const tstring& str = *(tstring*)addr;
	if( !str.empty() )
		_tcsncpy( pszBuffer, str.c_str(), count);
}

static DWORD UnitaryTime(FILETIME time)
{
	const DWORD msecondsPerSecond = 1000;
	const DWORD msecondsPerMinute = msecondsPerSecond * 60;
	const DWORD msecondsPerHour = msecondsPerMinute * 60;

	SYSTEMTIME st;
	::FileTimeToSystemTime(&time,&st);

	float TotMS = st.wMilliseconds;
	TotMS += (msecondsPerSecond * st.wSecond);
	TotMS += (msecondsPerMinute * st.wMinute);
	TotMS += (msecondsPerHour * st.wHour);
	return 0;
}

void CALLBACK fmtor::Time(LPTSTR pszBuffer, size_t count, DWORD addr)
{
	FILETIME time = *(FILETIME*)addr;
	SYSTEMTIME st;

	::FileTimeToSystemTime(&time,&st);

	_sntprintf(pszBuffer, count,
				_T("%02d:%02d:%02d.%03d"),
				st.wHour, st.wMinute,
				st.wSecond, st.wMilliseconds);
}

void CALLBACK fmtor::Access(LPTSTR pszBuffer, size_t count, DWORD addr)
{
#define DELETE                           (0x00010000L)
#define READ_CONTROL                     (0x00020000L)
#define WRITE_DAC                        (0x00040000L)
#define WRITE_OWNER                      (0x00080000L)
#define SYNCHRONIZE                      (0x00100000L)
#define ACCESS_SYSTEM_SECURITY           (0x01000000L)
#define MAXIMUM_ALLOWED                  (0x02000000L)
#define GENERIC_READ                     (0x80000000L)
#define GENERIC_WRITE                    (0x40000000L)
#define GENERIC_EXECUTE                  (0x20000000L)
#define GENERIC_ALL                      (0x10000000L)

	DWORD Access = *( (DWORD*)addr );
	
	LPTSTR p = pszBuffer;
	for( DWORD i = 0; i < count-1; i++)
		*(p++) = _T('-');
	*p = _T('\0');
	p = pszBuffer;

	if( Access & GENERIC_ALL )
	{
		*(p++) = _T('E');
		*(p++) = _T('W');
		*(p++) = _T('R');
	}
	else
	{
		if( Access & GENERIC_EXECUTE )	*p = _T('E');
		p++;
		if( Access & GENERIC_WRITE )	*p = _T('W');
		p++;
		if( Access & GENERIC_READ )		*p = _T('R');
		p++;
	}
	*(p++) = _T(' ');

	if( Access & DELETE )		*p = _T('D');
	p++;
	if( Access & READ_CONTROL )	*p = _T('R');
	p++;
	if( Access & WRITE_DAC )	*p = _T('W');
	p++;
	if( Access & WRITE_OWNER )	*p = _T('W');
	p++;
	if( Access & SYNCHRONIZE )	*p = _T('S');
	p++;

	*(p++) = _T(' ');

	if( Access & ACCESS_SYSTEM_SECURITY )	*p = _T('S');
	p++;
	if( Access & MAXIMUM_ALLOWED )			*p = _T('M');
	p++;

	*p = _T('\0');
}

// compare function
int  CALLBACK cmpor::UnsignedInteger(DWORD addr1, DWORD addr2)
{
	DWORD d1 = *(DWORD*)addr1;
	DWORD d2 = *(DWORD*)addr2;

	return d1-d2;
}

int  CALLBACK cmpor::UnsignedLargeInt(DWORD addr1, DWORD addr2)
{
	LARGE_INTEGER d1 = *(LARGE_INTEGER*)addr1;
	LARGE_INTEGER d2 = *(LARGE_INTEGER*)addr2;

	int c1 = d1.HighPart - d2.HighPart;
	int c2 = d1.LowPart - d2.LowPart;
	if( c1 != 0 ) return c1;
	return c2;
}

int  CALLBACK cmpor::SignedInteger(DWORD addr1, DWORD addr2)
{
	int d1 = *(int*)addr1;
	int d2 = *(int*)addr2;

	// avoid overflow
	// e.g., 2^31 - (-2^31) will overflow
	if( d1 < d2 )
		return -1;
	else if ( d1 > d2 )
		return 1;
	else
		return 0;
}

int  CALLBACK cmpor::UnsignedINT8(DWORD addr1, DWORD addr2)
{
	int d1 = *(UCHAR*)addr1;
	int d2 = *(UCHAR*)addr2;

	return d1-d2;
}

int  CALLBACK cmpor::UnsignedINT16(DWORD addr1, DWORD addr2)
{
	int d1 = *(USHORT*)addr1;
	int d2 = *(USHORT*)addr2;

	return d1-d2;
}

int  CALLBACK cmpor::String(DWORD addr1, DWORD addr2)
{
	return _tcscmp( (LPCTSTR)addr1, (LPCTSTR)addr2 );
}

int  CALLBACK cmpor::StlString(DWORD addr1, DWORD addr2)
{
	tstring* pstr1 = (tstring*)addr1;
	return pstr1->compare( ((tstring*)addr2)->c_str() );
}

int  CALLBACK cmpor::Time(DWORD addr1, DWORD addr2)
{
	LARGE_INTEGER d1 = *(LARGE_INTEGER*)addr1;
	LARGE_INTEGER d2 = *(LARGE_INTEGER*)addr2;
	if( d1.HighPart < d2.HighPart )
		return -1;
	else if( d1.HighPart > d2.HighPart )
		return 1;
	else
	{
		if( d1.LowPart < d2.LowPart )
			return -1;
		else if( d1.LowPart > d2.LowPart )
			return 1;
		else
			return 0;
	}
}
