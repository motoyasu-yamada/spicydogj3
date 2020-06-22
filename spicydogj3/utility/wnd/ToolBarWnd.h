#pragma once

#include <AfxTempl.h>

class ToolBarWnd : public CWnd
{
private:
	CToolBarCtrl m_ctrl;
	int m_nCols;

public:
	DECLARE_DYNAMIC(ToolBarWnd)
	ToolBarWnd();
	void Create(DWORD dwCtrlStyle,CPoint,CWnd* pParent,UINT nID);
	void LoadToolBar(UINT,int ncol,int cx,int cy);
	CToolBarCtrl& GetToolBarCtrl() const;
protected:
	void AdjustSize(int,int,int,int);
	void LoadItems(WORD*,int,CArray<UINT,UINT&>&);
	void DeleteAllButtons();
	void SetButtons(const UINT*,int nCount);
	void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	void SetOwner(CWnd* pOwnerWnd);
	virtual void AssertValid() const;
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg void OnIdleUpdateCmdUI();
	afx_msg void OnNeedText(NMHDR*,LRESULT*);
	LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM);

	DECLARE_MESSAGE_MAP()
};