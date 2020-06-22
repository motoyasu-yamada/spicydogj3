#pragma once

#include "cnt/Cnt.h"
#include "utility/Tracker.h"

class PictChar;
class PictCharView;
class SpicydogDoc;

/**
	PictViewコントローラー規定クラス
*/
class PictViewCnt : public TViewControler<PictCharView,SpicydogDoc> {
protected:
	PictViewCnt();

protected:
	const PictChar* GetPictChar() const;
	PictChar* GetPictChar();
	BOOL IsEditable() const;
	LRESULT Default();
	DECLARE_MESSAGE_MAP()
}; // class PictViewCnt


/**
	デフォルトコントローラー
*/
class SelPictViewCnt : public PictViewCnt {
private:
	ResizeTracker m_tracker;
	HCURSOR m_hcur;

public:
	SelPictViewCnt();

protected:
	virtual BOOL OnSetCursor(CWnd*,UINT,UINT);
	virtual void OnRButtonDown(UINT,CPoint);
	virtual void OnLButtonDown(UINT,CPoint);
	afx_msg void OnPictOpt();
	afx_msg void OnUpdatePictOpt(CCmdUI*);
	DECLARE_MESSAGE_MAP()
}; // class SelPictViewCnt


/**
	矩形選択ツール
*/
class SelRectPictViewCnt : public PictViewCnt {
private:
	CRectTracker m_tracker;
	static HCURSOR s_hcurReset;
	static HCURSOR s_hcurSel;

public:
	static void clinit();
	SelRectPictViewCnt();
	virtual void OnEnter();

protected:
	virtual BOOL OnSetCursor(CWnd*,UINT,UINT);
	virtual void OnRButtonDown(UINT,CPoint);
	virtual void OnLButtonDown(UINT,CPoint);
	afx_msg void OnUpdateCmdUI(CCmdUI*);
	DECLARE_MESSAGE_MAP()
}; // class SelPictViewCnt


/**
	センタリングツール
*/
class CenterPictViewCnt : public PictViewCnt
{
private:
	CRectTracker m_tracker;
	HCURSOR m_hcur;
	HCURSOR m_hcurCenter;
	
public:
	CenterPictViewCnt();
protected:
	virtual BOOL OnSetCursor(CWnd*,UINT,UINT);
	virtual void OnRButtonDown(UINT,CPoint);
	virtual void OnLButtonDown(UINT,CPoint);
	
	DECLARE_MESSAGE_MAP()
}; // class SelPictViewCnt

/**
	ドット描画ツール
*/
class DotPictViewCnt : public PictViewCnt
{
private:
	struct DRAWDOT {
		CPoint m_pt;
		COLORREF m_cr;
	};
	std::list<DRAWDOT> m_drawdots;
	HCURSOR m_hcur;
	HCURSOR m_hcurNo;
	CPoint m_ptDocLastDot;
	BOOL m_bDown;

public:
	DotPictViewCnt();
	virtual void OnEnter();
	virtual void OnExit();

protected:
	afx_msg void OnUpdateCmdUI(CCmdUI*);
	void BeginDraw();
	void EndDraw();
	void Draw(CPoint ptView);
	void DrawDot(CPoint ptDoc,COLORREF);
	virtual void OnMouseMove(UINT nFlags,CPoint point);
	virtual void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnDraw(CDC*);
	virtual BOOL OnSetCursor(CWnd*,UINT,UINT);
	virtual void OnLButtonDown(UINT,CPoint);
	virtual void OnLButtonUp(UINT,CPoint);
	DECLARE_MESSAGE_MAP()
}; // class DotPictViewCnt

