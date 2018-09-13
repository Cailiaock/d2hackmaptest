//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "DialogBase.h"
#include "CoolButton.h"

class winAbout : public CDialogBase
{
public:
	winAbout(void);
	~winAbout(void);
	void SetBGBrush(HBRUSH br) {m_hbrBG = br;}
protected:
	virtual BOOL OnInitDialog();
	virtual BOOL OnDialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void SetButtonFace(CCoolButton& btn, UINT nBtnID, UINT nIconID);
	HBRUSH OnCtlColor(HDC hdc, HWND hwnd);
private:
	CCoolButton m_btnOK;
	HBRUSH		m_hbrBG;
};
