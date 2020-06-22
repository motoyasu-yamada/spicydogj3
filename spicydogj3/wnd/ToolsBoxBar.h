#pragma once

#include "utility/wnd/BitmapStatic.h"
#include "utility/wnd/ToolBarWnd.h"
#include "utility/wnd/InToolBar.h"
#include "utility/wnd/ColorBox.h"
#include "ScaleBox.h"
#include "toolkit/secall.h"

/** ロゴ,各種ツール,塗/線色,線 */
class ToolsBoxBar : public SECControlBar {
private:
	BitmapStatic m_wndLogo;
#ifdef USE_INTOOLBAR
	InToolBar m_wndTools;
#else
	ToolBarWnd m_wndTools;
#endif
	ColorBoxEx m_wndPaint;
	ColorBoxEx m_wndLine;
	ScaleBox   m_wndScale;
	CSize      m_sz;

public:
	ToolsBoxBar();
	BOOL Create(CWnd* pParentWnd,UINT nID);
	virtual ~ToolsBoxBar();
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
private:
	virtual CSize CalcDynamicLayout(int nLength, DWORD dwMode);
	BOOL CreateMe(CWnd* pParentWnd,UINT nID);
	afx_msg LRESULT OnClrBoxSelect(WPARAM,LPARAM);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
};