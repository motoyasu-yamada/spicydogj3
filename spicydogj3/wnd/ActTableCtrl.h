#pragma once

#include "model/type/Act.h"

class ActTableCtrl : public CWnd {
private:
	BOOL m_bHasAct;
	Act  m_actCopied;
	BOOL m_bHasTrans;
	COLORREF m_crTrans;
	int m_line;
	int m_page;
	int m_cell;
	UINT m_nToolTipFormat;
	CToolTipCtrl m_tooltip;
	mutable CString m_strTooltip;

public:
	ActTableCtrl();
	void SetAct(const Act*);
	void ResetAct();
	void SetTransColor(BOOL b,COLORREF = 0xFFFFFFFF);
	void SetToolTipFormat(UINT);

public:
	virtual ~ActTableCtrl();
protected:
	void OnPrepareDC(CDC&);
	void RecalcLayout();
	void Init();
	BOOL OnNeedText(UINT id,NMHDR * pTTTStruct,LRESULT * pResult);
	BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll);
	BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll);
	CPoint GetDeviceScrollPosition() const;
	//{{AFX_VIRTUAL(ActTableCtrl)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
	//{{AFX_MSG(ActTableCtrl)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/// 指定したSTATICコントロールクラスと同位置にコントロールを作成しSTATICコントロールを破棄する
void DDX_ActTableCtrl(CDataExchange* pDX, int nIDC,ActTableCtrl& ctrl);

//{{AFX_INSERT_LOCATION}}
