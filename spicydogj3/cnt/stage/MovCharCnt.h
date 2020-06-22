#pragma once

#include "StageCnt.h"

class  MovCharCnt : public StageCnt {
public:
	MovCharCnt();
	virtual void OnEnter();

protected:
	virtual void OnLButtonDblClk(UINT,CPoint);
	virtual void OnRButtonDown(UINT,CPoint);
	virtual void OnLButtonDown(UINT,CPoint);
	virtual BOOL OnSetCursor(CWnd*,UINT,UINT);
	virtual void OnDraw(CDC*);

private:
	void TrackMoveSelLayer(CPoint);
	void TrackSelNewLayer(CPoint);
	BOOL TrackMove(CPoint pt);
	void MoveChar(int x,int y);
	void OnLClick(CPoint);
	void OnLDrag(CPoint);

	afx_msg void OnMoveUp();
	afx_msg void OnMoveDown();
	afx_msg void OnMoveRight();
	afx_msg void OnMoveLeft();
	afx_msg void OnUpdateMove(CCmdUI*);

	DECLARE_MESSAGE_MAP()
};
