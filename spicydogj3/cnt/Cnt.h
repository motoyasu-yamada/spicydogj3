#pragma once
#include "utility/Accel.h"

class CntMgr;

class Cnt : public CCmdTarget,public AccelOwner {
private:
	BOOL m_bContinueMsgRouting;
	CntMgr* m_pMgr;
public:
	virtual void OnEnter();
	virtual void OnExit();
	virtual void OnDraw(CDC* pDC);
	virtual void OnLButtonDblClk(UINT,CPoint);
	virtual void OnLButtonDown(UINT,CPoint);
	virtual	void OnLButtonUp(UINT,CPoint);
	virtual void OnRButtonDblClk(UINT,CPoint);
	virtual void OnRButtonDown(UINT,CPoint);
	virtual void OnRButtonUp(UINT,CPoint);
	virtual void OnMouseMove(UINT,CPoint);
	virtual void OnKeyDown(UINT,UINT,UINT);
	virtual void OnKeyUp(UINT,UINT,UINT);
	virtual BOOL OnSetCursor(CWnd*,UINT,UINT);
public:
	virtual	~Cnt();
protected:
	CntMgr* GetCntMgr();
	const CntMgr* GetCntMgr() const;
	void RejectEnter();
private:
	friend class CntMgr;
	void AttachControlerMgr(CntMgr*);
	void ContinueMsgRouting(BOOL);
	BOOL IsContinueMsgRouting() const;
};

template <typename _VIEWT,typename _DOCT>
class TViewControler : public Cnt {
protected:
	inline      _VIEWT*  GetViewT()           { return dynamic_cast<      _VIEWT*>(GetCntMgr());}
	inline const _VIEWT*  GetViewT() const    { return dynamic_cast<const _VIEWT*>(GetCntMgr());}
	inline _DOCT*  GetDocumentT()             { return dynamic_cast<      _DOCT*>(GetViewT()->CView::GetDocument());}
	inline const _DOCT*  GetDocumentT() const { return dynamic_cast<const _DOCT*>(GetViewT()->CView::GetDocument());}
};

