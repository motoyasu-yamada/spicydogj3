#pragma once

#include "Cnt.h"
#include "wnd/ScrollViewEx.H"

class  ScrollViewMoveCnt : public TViewControler<ScrollViewEx,CDocument> {
private:
	CPoint m_ptPrev;
public:
	ScrollViewMoveCnt();
private:
	virtual void OnMouseMove(UINT,CPoint);
	virtual BOOL OnSetCursor(CWnd*,UINT,UINT);

	afx_msg void OnCancle();
	virtual void OnLButtonDown(UINT,CPoint);
	virtual void OnRButtonDown(UINT,CPoint);

	DECLARE_MESSAGE_MAP()
};


class  ScrollViewZoomCnt : public TViewControler<ScrollViewEx,CDocument> {
public:
	ScrollViewZoomCnt();
	virtual void OnLButtonDown(UINT,CPoint pt);
	virtual void OnRButtonDown(UINT,CPoint);
	virtual BOOL OnSetCursor(CWnd*,UINT,UINT);

	afx_msg void OnCancle();
private:
	void SetScale(double scale,CPoint);
	DECLARE_MESSAGE_MAP()
};