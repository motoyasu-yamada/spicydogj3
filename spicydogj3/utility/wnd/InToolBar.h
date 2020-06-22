#pragma once

#include <vector>

/**
	InToolBarItem
	InToolBarの各アイテム情報クラス
*/
struct InToolBarItem {
public:
	/// 左から順に数えたときのアイテム位置インデックス
	int m_nIndex;
	/// アイテムのコマンドID
	UINT m_nID;
	/// アイテム状態(選択状態、有効状態)の保持
	int m_nState;
	/// アイテム矩形
	CRect m_rc;
}; // struct InToolBarItem

/**
	InToolBar
*/
class InToolBar : public CWnd {
private:
	typedef std::vector<InToolBarItem> ITEMS;
	mutable CString	m_strMsg;
	CSize			m_szBitmap;
	CSize			m_szButton;
	CSize			m_szFrame;
	CImageList		m_imagelistButtons;
	ITEMS			m_items;
	int				m_nHover;
	int				m_nColCount;
	BOOL m_bBmpBorder;
	CSize m_szBmpBorder;
	BOOL m_bBorder;
	CSize m_szBorder;
	CToolTipCtrl m_tooltip;
	mutable CString m_strTooltip;

public:
	DECLARE_DYNAMIC(InToolBar);
	InToolBar(BOOL bBorder = FALSE,CSize = CSize(2,2),BOOL bBmpBorder = FALSE,CSize = CSize(1,1));
	virtual ~InToolBar();
	BOOL Create(UINT nResID,CWnd* pParentWnd,int nColCount = -1,DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP, UINT nID = AFX_IDW_TOOLBAR);
	int GetCount() const;
	void SetButtonStyle(int nIndex,UINT);
	UINT GetButtonStyle(int nIndex) const;
	UINT GetButtonID(int nIndex) const;
	void GetButtonRect(int nIndex,LPRECT) const;
	afx_msg void OnIdleUpdateCmdUI();
	afx_msg void OnUpdateCmdUI(CWnd*,BOOL b);

private:
	BOOL Init();
	UINT GetCurrentPosID() const;
	BOOL IsRealyEnabled() const;
	int HitTest(CPoint ptScreen) const;
	int FindIndex(UINT nID);
	void DrawFrame(CDC&);
	void DrawButton(CDC&,const InToolBarItem&);
	void RedrawButton(int nIndex);
	void GetButtonItem(int nIndex,InToolBarItem&) const;
	void SetButtonItem(int nIndex,const InToolBarItem&);
	BOOL LoadToolbar(UINT nIDToolbar);

	//{{AFX_VIRTUAL(InToolBar)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL
	afx_msg BOOL OnNeedText(UINT id,NMHDR * pTTTStruct,LRESULT * pResult);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//{{AFX_MSG(InToolBar)
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnEnable(BOOL bEnable);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
