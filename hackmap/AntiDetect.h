//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include <map>

class CAntiDetect
{
	struct MirrorModule
	{
		MirrorModule():mirroredImageBase(0){}
		DWORD mirroredImageBase;
		DWORD originalImageBase;
		DWORD imageSize;
		~MirrorModule();
	};
	typedef std::map<DWORD, MirrorModule> MirrorModuleList;
public:
	CAntiDetect(void);
	~CAntiDetect(void);
	DWORD GetOriginalData(DWORD addr);
private:
	MirrorModule* FindModuleFromAddress(DWORD addr);
	DWORD LoadOriginalImage(LPCTSTR lpFileName, LPVOID pOldDLLImage);
	static BOOL MapDLLFromImage(void *pDLLFileImage, void *pMemoryImage, void *pOldDLLImage);
	static BOOL FixupDLLImage(void *pMemoryImage, void* pOldDLLImage);
private:
	MirrorModuleList m_mirrorModules;
};
