// MemoryHelper.cpp: implementation of the CMemoryHelper class.
//
//////////////////////////////////////////////////////////////////////
#include "stdhdrs.h"
#include "MemoryHelper.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemoryHelper::CMemoryHelper()
{

}

CMemoryHelper::~CMemoryHelper()
{

}

BOOL CMemoryHelper::OpenProcess(DWORD processId)
{
	m_hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	return m_hProcess != NULL;
}

BOOL CMemoryHelper::AllocMemory(DWORD size)
{
	if(m_hProcess == NULL) return FALSE;
	m_pAllocBase = (LPBYTE)VirtualAllocEx(m_hProcess, NULL, size, MEM_COMMIT, PAGE_READWRITE);
	if(m_pAllocBase == NULL) return FALSE;
	m_totalLen = size;
	m_currAddr = (DWORD)(LPBYTE)m_pAllocBase;
	return TRUE;
}

LPVOID CMemoryHelper::WriteProcessMemory(LPVOID buffer, DWORD size)
{
	if(m_hProcess == NULL || m_pAllocBase == NULL) return NULL;
	if(m_currAddr+size > ((DWORD)(LPBYTE)m_pAllocBase)+m_totalLen) return NULL;
	DWORD cbSize = 0;
	LPVOID addr = (LPVOID)m_currAddr;
	if(!::WriteProcessMemory(m_hProcess, addr, buffer, size, &cbSize))
		return NULL;
	m_currAddr += cbSize;
	return addr;
}

void CMemoryHelper::WriteProcessBYTES(DWORD lpAddress, void* buf, int len)
{
	DWORD oldprot,dummy = 0;
	VirtualProtectEx(m_hProcess, (void*) lpAddress, len, PAGE_READWRITE, &oldprot);
	::WriteProcessMemory(m_hProcess, (void*) lpAddress, buf, len, 0);
	VirtualProtectEx(m_hProcess, (void*) lpAddress, len, oldprot, &dummy);
}

BOOL CMemoryHelper::ReadProcessMemory(LPCVOID lpBaseAddress, LPVOID lpBuffer, DWORD nSize)
{
	if(m_hProcess == NULL) return FALSE;
	DWORD cbSize;
	return ::ReadProcessMemory(m_hProcess, lpBaseAddress, lpBuffer, nSize, &cbSize);
}

void CMemoryHelper::Intercept(int instruction, DWORD lpSource, DWORD lpDest, int len)
{
	BYTE* buffer = new BYTE[len];
	ReadProcessMemory((LPCVOID)lpSource, buffer, len);
	WriteProcessBYTES(lpDest, buffer, len);
	
	buffer[0] = instruction;
	*(DWORD*) (buffer + 1) = lpDest - (lpSource + 5);
	memset(buffer + 5, 0x90, len - 5);		// nops
	
	WriteProcessBYTES(lpSource, buffer, len);
	
	delete [] buffer;
}
