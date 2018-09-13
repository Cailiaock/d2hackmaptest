//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////


#ifndef __ML_TOOLTIP_H__
#define __ML_TOOLTIP_H__

class winToolTip  
{
public:
	void Hide();
	void Show(LPTSTR lpszText, LPRECT lpRect = NULL);
	BOOL Create(HWND hwndParent, HINSTANCE hAppInst);
	static BOOL Init();
	winToolTip();
	~winToolTip();
protected:
	HINSTANCE m_hAppInst;
	HWND m_hwndParent;
	HWND m_hwndTT;
};

#endif // __ML_TOOLTIP_H__
