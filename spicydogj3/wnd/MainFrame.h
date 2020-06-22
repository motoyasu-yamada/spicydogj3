#pragma once

#include <memory>
#include "model/build/BuildOutput.h"
#include "toolkit/secall.h"
#include "utility/Accel.h"
#include "utility/HtmlHelper.h"
#include "utility/toolbar/DialogSizingBar.h"
#include "wnd/ToolsBoxBar.h"

struct CREATE_DSB;
class MainFrame
:	public SECMDIFrameWnd {
private:
	std::auto_ptr<SECToolBarManager> m_ToolBarMgrPtr;
	std::auto_ptr<SECMDIMenuBar> m_MenuBarPtr;

	SECCustomToolBar   m_wndFixedToolBar;
	DialogSizingBarMgr m_mgrDlgBar;
	SECStatusBar  m_wndStatusBar;
	ToolsBoxBar   m_wndToolsBar;
	MultiAccel    m_accels;
	BuildOutput   m_output;
	HtmlHelper    m_htmlHelp;
	BOOL m_bShowTbr;
	BOOL m_bAppActive;
	BOOL m_bInit;

public:
	DECLARE_DYNAMIC(MainFrame)
	MainFrame();
	HACCEL GetActiveAccelerator();

protected:
	BOOL CreateToolBars();
	BOOL CreateStatusBar();
	BOOL CreateToolBox();
	void ShowOwnedTools(BOOL);
	void OnActiveStateChanged();
	BOOL CreateDialogSizingBar(const CREATE_DSB& dsb);
	virtual ~MainFrame();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEditGrid();
	afx_msg void OnDisplayGrid();
	afx_msg void OnDisplayCenterGrid();
	afx_msg void OnDisplayPixelGrid();
	afx_msg void OnUpdateDisplayGrid(CCmdUI*);
	afx_msg void OnUpdateDisplayCenterGrid(CCmdUI*);
	afx_msg void OnUpdateDisplayPixelGrid(CCmdUI*);
	afx_msg void OnSwitchTbr();
	afx_msg void OnUpdateSwitchTbr(CCmdUI*);
	afx_msg BOOL OnBarCheckEx(UINT nID);
	afx_msg void OnUpdateControlBarMenuEx(CCmdUI* pCmdUI);
	afx_msg BOOL OnWww(UINT);
	afx_msg void OnHelp();
	afx_msg void OnPref();
	afx_msg void OnCustomize();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
	DECLARE_MESSAGE_MAP()
};
