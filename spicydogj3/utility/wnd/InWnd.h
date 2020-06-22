#pragma once

class InWndOwner;

/**
	ウインドウ内の描画&操作用子ウインドウ
*/
class InWnd : public CWnd
{
private:
	InWndOwner* m_pOwner;

public:
	InWnd();
	virtual BOOL Create(InWndOwner* p,
						CWnd* pParentWnd,
						LPCRECT prc = NULL,
						UINT nID    = AFX_IDW_PANE_FIRST,
						DWORD dwStyle = WS_CHILD | WS_VISIBLE,
						DWORD dwExStyle = 0);

public:
	virtual ~InWnd();
protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg UINT OnGetDlgCode();
	DECLARE_MESSAGE_MAP()

	friend class InWndOwner;
};

/**
	InWndの描画イベントキャッチインターフェース
*/
class InWndOwner {
public:
	InWndOwner();
	virtual void OnPaint(InWnd*);
	virtual void OnLButtonDblClk(InWnd*,UINT nFlags, CPoint point);
	virtual void OnLButtonDown(InWnd*,UINT nFlags, CPoint point);
	virtual void OnLButtonUp(InWnd*,UINT nFlags, CPoint point);
	virtual void OnRButtonDblClk(InWnd*,UINT nFlags, CPoint point);
	virtual void OnRButtonDown(InWnd*,UINT nFlags, CPoint point);
	virtual void OnRButtonUp(InWnd*,UINT nFlags, CPoint point);
	virtual BOOL OnSetCursor(InWnd*,CWnd* pWnd, UINT nHitTest, UINT message);
protected:
	virtual ~InWndOwner();
};
