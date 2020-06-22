#pragma once


//	スプリット位置変更を親ウインドウに通知する
#define WM_SAFX_SPLITV_MOVE		(WM_USER + 0x1100)
#define WM_SAFX_SPLITH_MOVE		(WM_USER + 0x1101)

//	親ウインドウのスプレッド位置変更通知ハンドラ―
#define ON_WM_SAFX_SPLITV_MOVE()	\
	{ WM_SAFX_SPLITV_MOVE,0,0,0,AfxSig_vv,\
(AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(void))&OnSplitVMove},

#define ON_WM_SAFX_SPLITH_MOVE()	\
	{ WM_SAFX_SPLITH_MOVE,0,0,0,AfxSig_vv,\
(AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(void))&OnSplitHMove},


/**
	スプリッターコントロールの基底クラス
*/
class SplitterCtrl : public CStatic {
private:
	UINT m_nWmMsg;
	HCURSOR m_hcur;
	UINT m_nPane1MinSize;
	UINT m_nPane2MinSize;

	CRect m_rcDragOrg;
	CRect m_rcDragCur;
	CPoint m_ptDragOrg;
	CPoint m_ptDragCur;
	CSize  m_szDragBorder;

public:
	SplitterCtrl(UINT,HCURSOR);
	virtual ~SplitterCtrl();
protected:
	void SetPane1MinSize(UINT n);
	void SetPane2MinSize(UINT n);
	virtual LONG& GetPosFromPoint(CPoint& pt) = 0;
	virtual void GetDragSplitRect(CRect& rc) const = 0;
	void GetParentClientRect(LPRECT) const;
private:
	void NotifySplitterMove();
	BOOL DoSplitterMove();
	UINT GetSplitterPos() const;
	void GetTrackClipRect(CRect& rc);
	BOOL Track(CDC&);
	const LONG& GetPosFromPointC(const CPoint&) const;
	static void GetPointFromLPARAM(CPoint&,LPARAM);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
};

/**	垂直方向のスプリッター */
class SplitterVCtrl : public SplitterCtrl {
public:
	SplitterVCtrl();
	virtual ~ SplitterVCtrl();
	void SetUpperPaneMinHeight(UINT n);
	void SetLowerPaneMinHeight(UINT n);

protected:
	virtual LONG& GetPosFromPoint(CPoint& pt);
	virtual void GetDragSplitRect(CRect& rc) const;
};

/**	垂直方向のスプリッター */
class SplitterHCtrl : public SplitterCtrl {
public:
	SplitterHCtrl();
	virtual ~ SplitterHCtrl();
	void SetLeftPaneMinWidth(UINT n);
	void SetRightPaneMinWidth(UINT n);

protected:
	virtual LONG& GetPosFromPoint(CPoint& pt);
	virtual void GetDragSplitRect(CRect& rc) const;
};

