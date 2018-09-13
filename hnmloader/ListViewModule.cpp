//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#include "stdhdrs.h"
#include "ListViewModule.h"
#include "resource.h"

inline static DWORD GetShortName(DWORD addr)
{
	const tstring& strPath = *reinterpret_cast<tstring*>(addr);
	return (DWORD)GetFileNameFromPath(strPath.c_str());
}

static void CALLBACK fmtDllName(LPTSTR pszBuffer, size_t count, DWORD addr)
{
	fmtor::String( pszBuffer, count, GetShortName(addr) );
}

static int  CALLBACK cmpDllName(DWORD addr1, DWORD addr2)
{
	return cmpor::String(GetShortName(addr1), GetShortName(addr2));
}

static void CALLBACK fmtAddressRange(LPTSTR pszBuffer, size_t count, DWORD addr)
{
	PDWORD paddr = (DWORD*)addr;
	_sntprintf(pszBuffer, count, _T("0x%08X - 0x%08X"), paddr[0], paddr[0]+paddr[1] );
}

typedef CModuleWalker::DllInfo DI_T_;
static CListViewImpl::COLUMNINFO _pColumnInfo[] = {
	_T("Image Path"),			240, OFFSETOF(DI_T_, imagePath),
	fmtor::StlString,			cmpor::StlString,

	_T("Image Name"),			130, OFFSETOF(DI_T_, imagePath),
	fmtDllName,					cmpDllName,

	_T("Mapped Address"),		150, OFFSETOF(DI_T_, imageBase),
	fmtAddressRange,			cmpor::UnsignedInteger,

	_T("Image Size"),			90, OFFSETOF(DI_T_, imageSize),
	fmtor::Hex,					cmpor::UnsignedInteger,
};

CListViewModule::CListViewModule(void)
				:m_processId(0)
{
	SetColumnTable( _pColumnInfo, sizeof(_pColumnInfo) / sizeof( _pColumnInfo[0] ) );

	m_ContextMenu.nMenuID = IDR_TRAYMENU;
	m_ContextMenu.nSubMenuID = 2;

	m_index.push_back(eImageName);
	m_index.push_back(eImageBase);
	m_index.push_back(eImageSize);
	m_index.push_back(eImagePath);
}

CListViewModule::~CListViewModule(void)
{
}

void CListViewModule::OnSetImageList( std::vector<UINT>& images )
{
	images.push_back( IDI_DLL );
}

DWORD CListViewModule::OnGetItemKey(DWORD item)
{
	return reinterpret_cast<DllInfo*>(item)->imageBase;
}

BOOL CListViewModule::OnUpdateItem(DWORD iter, DWORD& key, DWORD& data)
{
	DllList::value_type& item = *reinterpret_cast<DllList::value_type*>(iter);
	key = item.first;
	data = (DWORD)&item.second;
	return FALSE;
}

void CListViewModule::ResetStat()
{
	m_processId = 0;
	DeleteAllItems();
	m_moduleWalker.m_dlls.clear();
}

BOOL CListViewModule::PrepareUpdateListView(ItemKeyList& itemsInView)
{
	DelDllList delDlls(m_moduleWalker.m_dlls);
	if(!m_moduleWalker.Walk(m_processId, delDlls))
		return FALSE;
	RemoveObsoleteItems(delDlls, itemsInView);
	return TRUE;
}

void CListViewModule::OnUpdate(HWND hwndSender)
{
	if(m_processId)
	{
		ItemKeyList itemsInView;
		if(PrepareUpdateListView(itemsInView))
		{
			UpdateListView(
				m_moduleWalker.m_dlls.begin(),
				m_moduleWalker.m_dlls.end(),
				itemsInView
				);
			return;
		}
	}
	ResetStat();
}

BOOL CListViewModule::OnContextMenuCommand(UINT nID)
{
	switch( nID )
	{
	case ID_DLLMENU_UNLOADDLL:
		OnUnloadDll();
		break;
	}

	return FALSE;
}

CListViewModule::DllInfo* CListViewModule::GetSelectedDll()
{
	int index = GetNextSelectedItem();
	return index != -1 ? (DllInfo*)GetItemData(index) : NULL;
}

void CListViewModule::OnUnloadDll()
{
	DllInfo* pdi = GetSelectedDll();
	if(pdi)
	{
		TCHAR szWarningText[256];
		if(LoadString(IDS_WRN_UNLOAD_DLL, szWarningText, COUNTOF(szWarningText)))
		{
			if(MessageBox(m_hwnd, szWarningText, g_szAppName, MB_YESNO|MB_ICONWARNING) == IDYES)
			{
				// forward to main window due to no enough targeted process information yet (need HWND for win9x/me)
				SendMessage( GetParent(m_hwnd), WM_UNLOADMODULE, 0, pdi->imageBase);
			}
		}
	}
}
