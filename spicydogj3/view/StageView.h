#pragma once

#include <AfxOle.h>
#include "SpicydogDoc.h"
#include "cnt/CntMgr.h"
#include "model/char/Char.h"
#include "view/CharView.h"
#include "wnd/ScrollViewEx.H"

class Stage;
class KeyFrame;

class StageView : 
	public TWndControlerMgr<ScrollViewEx>,
	public CharView {
	friend class MovCharCnt;
	friend class ScrollViewMoveCnt;
	friend class ScrollViewZoomCnt;
private:
	static BOOL s_bHideCenter;
	COleDropTarget m_Target;
	BOOL           m_bMaskView;
	CSize          m_szStage;
	BOOL           m_bNeedToScrollToCenter;
protected:
	DECLARE_DYNCREATE(StageView)
	StageView();		
public:
	virtual BOOL GetViewCaption(CString& str) const;

protected:
	virtual ~StageView();
	DROPEFFECT OnDragEnter(COleDataObject*,DWORD,CPoint);
	DROPEFFECT OnDragOver(COleDataObject*,DWORD,CPoint);
	BOOL OnDrop(COleDataObject*,DROPEFFECT,CPoint);

private:
	Stage* GetTargetStage();
	const Stage* GetTargetStage() const;
	SpicydogDoc* GetDocument();
	const SpicydogDoc* GetDocument() const;
	void DrawChar(CDC* pDC);
	virtual CString GetCaption() const;
	virtual int GetIconIndex() const;
	virtual BOOL IsClosable() const;
	virtual void OnDraw(CDC* pDC);
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

	BOOL IsValidMove() const;
	BOOL IsValidSizing() const;
	BOOL IsValidEditChar(Char*&);
	BOOL IsValidPlaceProp(KeyFrame*& pKey);
	BOOL IsValidEditColoring(KeyFrame*& pKey);
	BOOL IsValidEditPlacing (KeyFrame*& pKey);

	afx_msg BOOL IsValidDelLayer() const;
	afx_msg BOOL OnMaskView(UINT);
	afx_msg void OnClose();
	afx_msg void OnDelLayer();
	afx_msg void OnEditChar();
	afx_msg void OnPlayFirst();
	afx_msg void OnPlayLast();
	afx_msg void OnPlayNext();
	afx_msg void OnPlayPlay();
	afx_msg void OnPlayPrev();
	afx_msg void OnPlayStop();
	afx_msg void OnEditPlacing();
	afx_msg void OnEditColoring();
	afx_msg void OnShowCenter();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUpdateEditPlacing(CCmdUI*);
	afx_msg void OnUpdateEditColoring(CCmdUI*);
	afx_msg void OnUpdateDelLayer(CCmdUI* pCmd);
	afx_msg void OnUpdateEditChar(CCmdUI*);
	afx_msg void OnUpdateInfoMouse(CCmdUI*);
	afx_msg void OnUpdateMaskView(CCmdUI*);
	afx_msg void OnUpdatePlayFirst(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePlayLast(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePlayNext(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePlayPlay(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePlayPrev(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePlayStop(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePlaceProp(CCmdUI*);
	afx_msg void OnUpdatePublishLast(CCmdUI*);
	afx_msg void OnUpdateShowCenter(CCmdUI*);

	DECLARE_MESSAGE_MAP()
};
