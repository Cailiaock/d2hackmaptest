//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////


#include "stdhdrs.h"
#include "ListViewIntegrityScan.h"
#include "ConfigLoader.h"
#include "AutoHandle.h"

static void CALLBACK fmtDllName(LPTSTR pszBuffer, size_t count, DWORD addr)
{
	fmtor::String( pszBuffer, count,
		(DWORD)GetFileNameFromPath((*reinterpret_cast<CIntegrityWalker::DllInfo**>(addr))->imagePath.c_str())
		);
}

static int  CALLBACK cmpDllName(DWORD addr1, DWORD addr2)
{
	return cmpor::String(
		(DWORD)GetFileNameFromPath((*reinterpret_cast<CIntegrityWalker::DllInfo**>(addr1))->imagePath.c_str()),
		(DWORD)GetFileNameFromPath((*reinterpret_cast<CIntegrityWalker::DllInfo**>(addr2))->imagePath.c_str())
		);
}

typedef CIntegrityWalker::ModInfo MI_T_;
static CListViewImpl::COLUMNINFO _pColumnInfo[] = {
	_T("Address"),				110, OFFSETOF(MI_T_, address),
	fmtor::Address,				cmpor::UnsignedInteger,
	
	_T("In Dll"),				130, OFFSETOF(MI_T_, pdi),
	fmtDllName,					cmpDllName,
	
	_T("Length"),				30, OFFSETOF(MI_T_, len),
	fmtor::Decimal,				cmpor::UnsignedInteger,
	
	_T("Patched Code"),			100, OFFSETOF(MI_T_, strModified),
	fmtor::StlString,			cmpor::StlString,

	_T("Original Code"),		94, OFFSETOF(MI_T_, strOriginal),
	fmtor::StlString,			cmpor::StlString,
};

CListViewIntegrityScan::CListViewIntegrityScan()
					:m_processId(0)
					,m_interval(0)
{
	SetColumnTable( _pColumnInfo, sizeof(_pColumnInfo) / sizeof( _pColumnInfo[0] ) );
	
	m_ContextMenu.nMenuID = IDR_TRAYMENU;
	m_ContextMenu.nSubMenuID = 3;
	
	m_index.push_back(eAddress);
	m_index.push_back(eDllName);
	m_index.push_back(eLength);
	m_index.push_back(eModified);
	m_index.push_back(eOriginal);
}

CListViewIntegrityScan::~CListViewIntegrityScan()
{

}

void CListViewIntegrityScan::OnSetImageList( std::vector<UINT>& images )
{
	images.push_back( IDI_ISCAN );
}

DWORD CListViewIntegrityScan::OnGetItemKey(DWORD item)
{
	return reinterpret_cast<ModInfo*>(item)->address;
}

BOOL CListViewIntegrityScan::OnUpdateItem(DWORD iter, DWORD& key, DWORD& data)
{
	ModList::value_type& item = *reinterpret_cast<ModList::value_type*>(iter);
	key = item.first;
	data = (DWORD)&item.second;
	return FALSE;
}

BOOL CListViewIntegrityScan::PrepareUpdateListView(ItemKeyList& itemsInView)
{
	DelModList delMods(m_iscanWalker.m_mods);
	if(!m_iscanWalker.Walk(m_processId, g_config.fScanGameDllsOnly, delMods))
		return FALSE;
	RemoveObsoleteItems(delMods, itemsInView);
	return TRUE;
}

void CListViewIntegrityScan::ResetStat()
{
	m_processId = 0;
	DeleteAllItems();
	m_iscanWalker.m_mods.clear();
	m_interval = 0;
}

void CListViewIntegrityScan::OnUpdate(HWND hwndSender)
{
	if((m_interval++)%g_config.nIScanFreq != 0)
		return;
	OnUpdateNow();
}

void CListViewIntegrityScan::OnUpdateNow()
{
	if(m_processId)
	{
		ItemKeyList itemsInView;
		if(PrepareUpdateListView(itemsInView))
		{
			UpdateListView(
				m_iscanWalker.m_mods.begin(),
				m_iscanWalker.m_mods.end(),
				itemsInView
				);
			return;
		}
	}
	ResetStat();
}

BOOL CListViewIntegrityScan::OnContextMenuCommand(UINT nID)
{
	switch( nID )
	{
	case ID_ISCNMENU_RESTORE:
		OnRestore();
		break;
	case ID_ISCANMENU_SCANNOW:
		OnUpdateNow();
	}
	
	return FALSE;
}

CListViewIntegrityScan::ModInfo* CListViewIntegrityScan::GetSelectedModification()
{
	int index = GetNextSelectedItem();
	return index != -1 ? (ModInfo*)GetItemData(index) : NULL;
}

static inline BOOL MyWriteProcessMemory(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T * lpNumberOfBytesWritten)
{
	DWORD oldprot;
	BOOL retval = VirtualProtectEx(hProcess, lpBaseAddress, nSize, PAGE_EXECUTE_READWRITE, &oldprot);
	if (retval)
	{
		retval = WriteProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesWritten);
		VirtualProtectEx(hProcess, lpBaseAddress, nSize, oldprot, NULL);
	}
	return retval;
}

void CListViewIntegrityScan::OnRestore()
{
	ModInfo* pmi = GetSelectedModification();
	if(pmi)
	{
		TCHAR szWarningText[256];
		if(LoadString(IDS_WRN_UNPATCH_MODULE, szWarningText, COUNTOF(szWarningText)))
		{
			if(MessageBox(m_hwnd, szWarningText, g_szAppName, MB_YESNO|MB_ICONWARNING) == IDYES)
			{
				AutoHandle<HANDLE> hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_processId);
				if(hProcess != NULL )
					if(MyWriteProcessMemory(hProcess, (LPBYTE)pmi->address, &pmi->abOriginal[0], pmi->len, NULL ))
						OnUpdateNow();
			}
		}
	}
}
