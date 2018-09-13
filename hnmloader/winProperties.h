//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////


#ifndef _WIN_PROPERTIES_
#define _WIN_PROPERTIES_

#include "DialogBase.h"
#include "CoolButton.h"
#include "Picture.h"
#include "ListViewDiabloII.h"
#include "ListViewModule.h"
#include "ListViewIntegrityScan.h"
#include "winTabControl.h"
#include <ShellAPI.h>

class winProperties : public CDialogBase  
{
	enum {eGameView = 0, eDllView = 1, eIScanView = 2};
public:
	winProperties();
	virtual ~winProperties();
	BOOL Init();
protected:
	virtual BOOL OnInitDialog();
	virtual BOOL OnDialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnOK();
	virtual BOOL OnCancel();
	virtual void OnNotify(int idCtrl, LPNMHDR pnmh );

	// command handlers
private:
	void OnNotifyCurrentView(LPNMHDR pnmh);
	void OnNotifyTabConTrol(LPNMHDR pnmh);
	BOOL OnCommand(UINT nCommandID);
	void OnDropFiles(HDROP hDrop);
	BOOL OnHelpInfo(LPHELPINFO lphi);
	HBRUSH OnCtlColor(HDC hdc, HWND hwnd);
	void OnDlgAbout();
	void OnBtnUpgradeConfig();
	void OnBtnInstallUpdateHackmap();
	void OnBtnRemoveHackmap();
	void OnBtnEditConfig();
	void OnBtnRevealAutomapAct();
	void OnTimer(UINT nIDEvent);
	void OnBtnPrompt(int idCtrl);
	void UpdateD2ListView();
	void OnClickTabCtrl(LPNMHDR pnmh);
	BOOL IsMainView() { return m_currView == static_cast<CListViewBase*>(&m_gameView); }

	// helpers
private:
	void SetButtonFace(CCoolButton& btn, UINT nBtnID, UINT nIconID);
	void SwitchView(int index);
private:
	HBRUSH	m_hBrush;
	HBITMAP	m_hBitmap;
	CPicture m_Picture;
	CCoolButton m_btnInstall;
	CCoolButton m_btnRemove;
	CCoolButton m_btnEditConfig;
	CCoolButton m_btnUpgradeConfig;
	CCoolButton m_btnRevealAutomapAct;
	CCoolButton m_btnAbout;
	CCoolButton m_btnExit;
	UINT m_nIdEvent;
	CListViewDiabloII m_gameView;
	CListViewModule m_dllView;
	CListViewIntegrityScan m_iscanView;
	winTabControl m_tabCtrl;
	CListViewBase* m_currView;
	CListViewBase* m_views[3];
};

#endif // _WIN_PROPERTIES_
