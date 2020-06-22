#pragma once
#include "utility/Accel.h"
#include "toolkit/secall.h"

class DialogSizingBarMgr;
class DialogSizingBar : public SECControlBar,public AccelOwner {
	friend class DialogSizingBarMgr;
	friend class CDialogBarDlg;
private:
	CDialog* m_pDialog;
	DialogSizingBarMgr* m_pMgr;
public:
	DECLARE_DYNAMIC(DialogSizingBar)
	DialogSizingBar();
	void AttachDialog(CDialog*);
	virtual ~DialogSizingBar();

protected:
	void AdjustDialog();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual void OnUpdateCmdUI(CFrameWnd* pFrameWnd,BOOL b);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PostNcDestroy();
	afx_msg LRESULT OnSizeParent(WPARAM,LPARAM);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnWindowPosChanged(WINDOWPOS*);
	DECLARE_MESSAGE_MAP()
};

class DialogSizingBarMgr {
	friend class DialogSizingBar;
private:
	typedef DialogSizingBar* BARPTR;
	typedef std::list<BARPTR> BARS; 
	BARS m_listBars;
	DialogSizingBar* m_pActiveBar;
public:
	DialogSizingBarMgr();
	virtual HACCEL GetDefaultAccelerator();
	void Append(DialogSizingBar*);
	BOOL IsAnyBarActive() const;
	BOOL OnCmdMsg1(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHI);
	BOOL OnCmdMsg2(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHI);
	BOOL OnCommand1(WPARAM,LPARAM);
	BOOL OnCommand2(WPARAM,LPARAM);
private:
	void Remove(DialogSizingBar*);
	DialogSizingBar* FindActiveBar();
};