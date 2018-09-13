//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#include "stdhdrs.h"
#include "Picture.h"
#include <olectl.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPicture::CPicture()
		:m_pPicture(NULL)
{
}

CPicture::~CPicture()
{
	Close();
}

BOOL CPicture::SetImage(LPCTSTR lpszPathName)
{
	if(lpszPathName == NULL) return FALSE;

	LPPICTURE pPicture = NULL;

	useconv_towcstr(szPath, lpszPathName);

	HRESULT hr = ::OleLoadPicturePath(const_cast<LPOLESTR>((szPath)),
									  NULL,
									  0,
									  0,
									  IID_IPicture,
									  reinterpret_cast<LPVOID *>(&pPicture));
	if (SUCCEEDED(hr) && pPicture != NULL)
	{
		Close();
		m_pPicture = pPicture;
		return TRUE;
	}
	return FALSE;
}

void CPicture::Close()
{
	if (m_pPicture)
		m_pPicture->Release(), m_pPicture = NULL;
}

const HBITMAP CPicture::GetHandle()
{
	HBITMAP hbmp = NULL;
	if(m_pPicture)
		m_pPicture->get_Handle((OLE_HANDLE*)&hbmp);
	return hbmp;
}

