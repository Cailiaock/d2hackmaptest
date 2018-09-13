//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////


#ifndef __ML_PICTURE_H__
#define __ML_PICTURE_H__

// forward declaration
struct IPicture;

class CPicture  
{
public:
	const HBITMAP GetHandle();
	void Close();
	BOOL SetImage(LPCTSTR lpszPathName);
	CPicture();
	~CPicture();
protected:
	IPicture *m_pPicture;
};

#endif // __ML_PICTURE_H__

