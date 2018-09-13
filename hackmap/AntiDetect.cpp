//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "ModuleWalker.h"
#include "AntiDetect.h"

#define NTSIGNATURE(ptr) ((LPVOID)((BYTE *)(ptr) + ((PIMAGE_DOS_HEADER)(ptr))->e_lfanew))
#define SIZE_OF_NT_SIGNATURE (sizeof(DWORD))
#define PEFHDROFFSET(ptr) ((LPVOID)((BYTE *)(ptr)+((PIMAGE_DOS_HEADER)(ptr))->e_lfanew+SIZE_OF_NT_SIGNATURE))
#define OPTHDROFFSET(ptr) ((LPVOID)((BYTE *)(ptr)+((PIMAGE_DOS_HEADER)(ptr))->e_lfanew+SIZE_OF_NT_SIGNATURE+sizeof(IMAGE_FILE_HEADER)))
#define SECHDROFFSET(ptr) ((LPVOID)((BYTE *)(ptr)+((PIMAGE_DOS_HEADER)(ptr))->e_lfanew+SIZE_OF_NT_SIGNATURE+sizeof(IMAGE_FILE_HEADER)+sizeof(IMAGE_OPTIONAL_HEADER)))
#define RVATOVA(base,offset) ((LPVOID)((DWORD)(base)+(DWORD)(offset)))
#define VATORVA(base,offset) ((LPVOID)((DWORD)(offset)-(DWORD)(base)))

#pragma pack(push,1)

typedef struct {
	DWORD	dwPageRVA;
	DWORD	dwBlockSize;
} IMAGE_FIXUP_BLOCK, *PIMAGE_FIXUP_BLOCK;

typedef struct {
	WORD	offset:12;
	WORD	type:4;
} IMAGE_FIXUP_ENTRY, *PIMAGE_FIXUP_ENTRY;

#pragma pack(pop)

CAntiDetect::CAntiDetect(void)
{
}

CAntiDetect::~CAntiDetect(void)
{
}

CAntiDetect::MirrorModule::~MirrorModule()
{
	if(mirroredImageBase) VirtualFree((LPVOID)mirroredImageBase, 0, MEM_RELEASE);
}

DWORD __stdcall GlobalGetOriginalData(DWORD addr)
{
	static CAntiDetect ad;
	return ad.GetOriginalData(addr);
}

DWORD CAntiDetect::GetOriginalData(DWORD addr)
{
	MirrorModule* pmm = FindModuleFromAddress(addr);
	if(pmm != NULL)
		return *(LPDWORD)(pmm->mirroredImageBase + (addr-pmm->originalImageBase));

	// to raise exeception if checking address is invalid
	// no worry, d2client will catch it.
	DWORD oldval = *(LPDWORD)addr;

	CModuleWalker dllWalker;
	if(!dllWalker.Walk(GetCurrentProcessId()))
		return oldval;

	CModuleWalker::DllInfo *pdi = dllWalker.FindModuleByAddress(addr);
	if(!pdi) return oldval;

	DWORD mirroredImageBase = LoadOriginalImage(pdi->imagePath.c_str(), (LPVOID)pdi->imageBase);
	if(mirroredImageBase)
	{
		MirrorModule& mm = m_mirrorModules[pdi->imageBase];
		mm.mirroredImageBase = mirroredImageBase;
		mm.originalImageBase = pdi->imageBase;
		mm.imageSize = pdi->imageSize;
	}
	
	return mirroredImageBase ? *(LPDWORD)(mirroredImageBase + (addr-pdi->imageBase)) : oldval;
}

CAntiDetect::MirrorModule* CAntiDetect::FindModuleFromAddress(DWORD addr)
{
	MirrorModuleList::iterator cit = m_mirrorModules.begin();
	MirrorModuleList::iterator end = m_mirrorModules.end();
	for(; cit != end; ++cit)
	{
		MirrorModule& mm = cit->second;
		if(mm.originalImageBase < addr && mm.originalImageBase+mm.imageSize > addr)
		{
			return &mm;
		}
	}
	return NULL;
}

DWORD CAntiDetect::LoadOriginalImage(LPCTSTR lpFileName, LPVOID pOldDLLImage)
{
	HANDLE hfile=CreateFile(lpFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if(hfile==INVALID_HANDLE_VALUE) return 0;

	//-------------------------------------------------
	// Create a file mapping
	//-------------------------------------------------
	HANDLE hmapping;
	hmapping=CreateFileMapping(hfile, NULL, PAGE_READONLY, 0, 0, NULL);

	//-------------------------------------------------
	// Close file handle since we don't need it anymore
	//-------------------------------------------------
	CloseHandle(hfile);


	//-------------------------------------------------
	// Map file mapping object to memory image
	//-------------------------------------------------
	void *baseaddr;
	baseaddr=MapViewOfFile(hmapping,FILE_MAP_READ,0,0,0);
	if(baseaddr==NULL) {
		CloseHandle(hmapping);
		return 0;
	}
	PIMAGE_OPTIONAL_HEADER poh;

	poh=(PIMAGE_OPTIONAL_HEADER) OPTHDROFFSET(baseaddr);

	LPVOID pMemoryImage = VirtualAlloc(NULL, poh->SizeOfImage, MEM_COMMIT, PAGE_READWRITE);
	if(pMemoryImage)
	{
		if(!MapDLLFromImage(baseaddr, pMemoryImage, pOldDLLImage) || !FixupDLLImage(pMemoryImage, pOldDLLImage))
		{
			VirtualFree(pMemoryImage, 0, MEM_RELEASE);
			pMemoryImage = NULL;
		}
	}

	UnmapViewOfFile(baseaddr);
	CloseHandle(hmapping);

	return (DWORD)pMemoryImage;
}

BOOL CAntiDetect::MapDLLFromImage(void *pDLLFileImage, void *pMemoryImage, void *pOldDLLImage)
{
	//------------------------------------------------------------------------------
	// Get Number of Sections
	//------------------------------------------------------------------------------
	PIMAGE_FILE_HEADER pfh;
	int nSectionCount;

	pfh=(PIMAGE_FILE_HEADER) PEFHDROFFSET(pDLLFileImage);
	nSectionCount=pfh->NumberOfSections;

	//------------------------------------------------------------------------------
	// Get PE Header Length + Section Header Length
	//------------------------------------------------------------------------------
	PIMAGE_OPTIONAL_HEADER poh;
	DWORD hdrlen;

	poh=(PIMAGE_OPTIONAL_HEADER) OPTHDROFFSET(pDLLFileImage);
	hdrlen=poh->SizeOfHeaders;

	memcpy(pMemoryImage, pOldDLLImage, poh->SizeOfImage);
	memcpy(pMemoryImage,pDLLFileImage,hdrlen);

	//------------------------------------------------------------------------------
	// Copy Sections one by one
	//------------------------------------------------------------------------------
	int i;
	PIMAGE_SECTION_HEADER psh;

	psh=(PIMAGE_SECTION_HEADER) SECHDROFFSET(pDLLFileImage);
	for(i=0;i<nSectionCount;i++) {
		void *secMemAddr, *secFileAddr;

		//------------------------------------------------------------------------------
		// Copy Section data
		//------------------------------------------------------------------------------
		secMemAddr  = (char *)pMemoryImage + psh->VirtualAddress;
		secFileAddr = (char *)pDLLFileImage + psh->PointerToRawData;

		// only recovery code section here
		// can check more section if neccessary
		if((psh->Characteristics & IMAGE_SCN_MEM_WRITE)==0)//IMAGE_SCN_MEM_EXECUTE)
		{
			memcpy(secMemAddr,secFileAddr,psh->SizeOfRawData);
		}

		psh++;
	}

	return TRUE;
}

BOOL CAntiDetect::FixupDLLImage(void *pMemoryImage, void* pOldDLLImage)
{
	//-----------------------------------------------
	// Get headers
	//-----------------------------------------------
	PIMAGE_OPTIONAL_HEADER   poh;
	PIMAGE_SECTION_HEADER    psh;
	poh = (PIMAGE_OPTIONAL_HEADER)OPTHDROFFSET (pMemoryImage);
	psh = (PIMAGE_SECTION_HEADER)SECHDROFFSET (pMemoryImage);


	//-----------------------------------------------
	// Get number of image directories in list
	//-----------------------------------------------
	int nDirCount;
	nDirCount=poh->NumberOfRvaAndSizes;
	if(nDirCount<16) return FALSE;

	//-----------------------------------------------
	// Process import table
	//-----------------------------------------------
	if(poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size!=0)
	{
		PIMAGE_IMPORT_DESCRIPTOR pid;
		pid=(IMAGE_IMPORT_DESCRIPTOR *)RVATOVA(pMemoryImage,poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

		PIMAGE_IMPORT_DESCRIPTOR pid2;
		pid2=(IMAGE_IMPORT_DESCRIPTOR *)RVATOVA(pOldDLLImage,poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

		//---------------------------------------------------
		// For all imported DLLs
		// OriginalFirstThunk is always zero for image generated by borland's linker
		//---------------------------------------------------
		while( 1 )
		{
			if ( (pid->TimeDateStamp==0 ) && (pid->Name==0) )
				break;

			char *svDllName;
			svDllName=(char *) RVATOVA(pMemoryImage,pid->Name);

			//-------------------------------------------------------------
			// Map library into address space (could also use LoadDLL())
			//-------------------------------------------------------------
			HMODULE hDll;
			hDll=GetModuleHandleA(svDllName);
			if(hDll==NULL) hDll=LoadLibraryA(svDllName);

			if(hDll==NULL)
				return FALSE;


			//-------------------------------------------------------------
			// Store DLL infoz
			//-------------------------------------------------------------
			pid->ForwarderChain=pid2->ForwarderChain;

			//-------------------------------------------------------------
			// This is bullshit cuz I don't want to call libc.
			//-------------------------------------------------------------
			pid->TimeDateStamp=pid2->TimeDateStamp;

			//-------------------------------------------------------------
			// Fill in Import Address Table
			//-------------------------------------------------------------
			PIMAGE_THUNK_DATA ptd_in,ptd_out;
			ptd_in=(PIMAGE_THUNK_DATA) RVATOVA(pMemoryImage, pid->OriginalFirstThunk);
			ptd_out=(PIMAGE_THUNK_DATA) RVATOVA(pMemoryImage, pid->FirstThunk);
			if( pid->OriginalFirstThunk == 0 ) // No OriginalFirstThunk field?
			{
				if( pid->FirstThunk == 0 ) // No FirstThunk field?  Ooops!!!
					break;

				//-------------------------------------------------------------
				// Yes! Gotta have a non-zero FirstThunk field then.
				//-------------------------------------------------------------
				ptd_in = ptd_out;
			}

			while(ptd_in->u1.Function!=NULL)
			{
				FARPROC func;

				//-------------------------------------------------------------
				// Determine if ordinal or name pointer
				//-------------------------------------------------------------
				if(ptd_in->u1.Ordinal & 0x80000000) {
					// Ordinal
					func=GetProcAddress(hDll,MAKEINTRESOURCEA(ptd_in->u1.Ordinal));
				} else {
					// Function name
					PIMAGE_IMPORT_BY_NAME pibn;
					pibn=(PIMAGE_IMPORT_BY_NAME) RVATOVA(pMemoryImage,ptd_in->u1.AddressOfData);
					func=GetProcAddress(hDll,(char *)pibn->Name);
				}

				if(func==NULL)
					return FALSE;

				//-------------------------------------------------------------
				// Write address to appropriate location
				//-------------------------------------------------------------
				*(FARPROC*)&ptd_out->u1.Function = func;

				ptd_in++;
				ptd_out++;
			}

			pid++;
			pid2++;
		}
	}


	//-------------------------------------------------------------
	// Process relocation tables if necessary
	//-------------------------------------------------------------


	//-------------------------------------------------------------
	// Calculate fixup delta
	//-------------------------------------------------------------
	DWORD delta;
	delta=(DWORD)pOldDLLImage - (DWORD)poh->ImageBase;

	if((delta!=0) && (poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size!=0))
	{
		PIMAGE_FIXUP_BLOCK pfb;
		pfb=(PIMAGE_FIXUP_BLOCK)RVATOVA(pMemoryImage,poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);

		PIMAGE_FIXUP_BLOCK pfb2;
		pfb2=(PIMAGE_FIXUP_BLOCK)RVATOVA(pOldDLLImage,poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);

		//-------------------------------------------------------------
		// For each fixup block
		//-------------------------------------------------------------
		while(pfb->dwPageRVA!=0)
		{
			PIMAGE_FIXUP_ENTRY pfe;
			int i,count;

			count=(pfb->dwBlockSize-sizeof(IMAGE_FIXUP_BLOCK))/sizeof(IMAGE_FIXUP_ENTRY);
			pfe=(PIMAGE_FIXUP_ENTRY)((char *)pfb + sizeof(IMAGE_FIXUP_BLOCK));

			PIMAGE_FIXUP_ENTRY pfe2;
			pfe2=(PIMAGE_FIXUP_ENTRY)((char *)pfb2 + sizeof(IMAGE_FIXUP_BLOCK));

			//-------------------------------------------------------------
			// For each fixup entry, fill with checking image data
			//-------------------------------------------------------------
			for(i=0;i<count;i++)
			{
				void *fixaddr;

				fixaddr=RVATOVA(pMemoryImage,pfb->dwPageRVA + pfe->offset);
				void *fixaddr2;
				fixaddr2=RVATOVA(pOldDLLImage,pfb2->dwPageRVA + pfe2->offset);
				switch(pfe->type)
				{
				case IMAGE_REL_BASED_ABSOLUTE:
					break;
				case IMAGE_REL_BASED_HIGH:
				case IMAGE_REL_BASED_LOW:
					*((WORD *)fixaddr) = *((WORD *)fixaddr2);
					break;
				case IMAGE_REL_BASED_HIGHLOW:
					// bullshit, d2common doesnt fix all rebase addresses, i dont know why
					if( *((DWORD *)fixaddr) != *((DWORD *)fixaddr2))
						*((DWORD *)fixaddr) += delta;
					break;
				case IMAGE_REL_BASED_HIGHADJ: // This one's really fucked up.
					*((WORD *)fixaddr) = *((WORD *)fixaddr2);
					pfe++;
					pfe2++;
					break;
				default:
					return FALSE;
				}

				pfe++;
				pfe2++;
			}

			pfb=(PIMAGE_FIXUP_BLOCK)((char *)pfb + pfb->dwBlockSize);
			pfb2=(PIMAGE_FIXUP_BLOCK)((char *)pfb2 + pfb2->dwBlockSize);
		}
	}
	else
	{
		//------------------------------------------------------------------------------
		// if image base has changed but we cann't find the relocation table,
		// we failed to fixup this image,
		// which usually happens when this image is a release-version executable file.
		//------------------------------------------------------------------------------
		if( delta !=0 && poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size == 0 )
			return FALSE;
	}

	return TRUE;
}
