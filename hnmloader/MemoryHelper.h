// MemoryHelper.h: interface for the CMemoryHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMORYHELPER_H__1DB7257E_7C1B_498E_A164_CA6240DE310B__INCLUDED_)
#define AFX_MEMORYHELPER_H__1DB7257E_7C1B_498E_A164_CA6240DE310B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AutoHandle.h"

class CMemoryHelper  
{
public:
	CMemoryHelper();
	~CMemoryHelper();
	BOOL OpenProcess(DWORD processId);
	BOOL AllocMemory(DWORD size);
	LPVOID WriteProcessMemory(LPVOID buffer, DWORD size);
	BOOL ReadProcessMemory(LPCVOID lpBaseAddress, LPVOID lpBuffer, DWORD nSize);
	DWORD GetCurrentAvailableBase() const { return m_currAddr; }
	void Intercept(int instruction, DWORD lpSource, DWORD lpDest, int len);
	void WriteProcessBYTES(DWORD lpAddress, void* buf, int len);
private:
	AutoHandle<HANDLE> m_hProcess;
	AutoHandle<LPBYTE> m_pAllocBase;
	DWORD m_currAddr;
	DWORD m_totalLen;
};

#endif // !defined(AFX_MEMORYHELPER_H__1DB7257E_7C1B_498E_A164_CA6240DE310B__INCLUDED_)
