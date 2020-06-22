#pragma once

#include "cnt/CntMgr.h"
#include "model/char/PictChar.h"
#include "model/type/Act.h"
#include "view/CharView.h"
#include "wnd/ScrollViewEx.H"

class PictChar;
class SpicydogDoc;

/** PictCharï“èWÉrÉÖÅ[ */
class PictCharView 
:	public TWndControlerMgr<ScrollViewEx>,
	public TCharView<PictChar>
{
	friend class PictViewCnt;
	friend class ScrollViewMoveCnt;
	friend class ScrollViewZoomCnt;

private:
	CRect m_rcSel;
	COLORREF m_crLine;
	COLORREF m_crFill;
	static BOOL s_bPreview;

public:
	DECLARE_DYNCREATE(PictCharView)
	PictCharView();
	void GetSelRect(CRect&) const;
	void SetSelRect(const CRect&);
	void ResetSelRect();
	int HitResizeHandle(CPoint ptView) const;
	COLORREF GetLineColor() const;
	COLORREF GetFillColor() const;
	
	BOOL IsEditable() const;

protected:
	friend class SelPictViewCnt;
	friend class SelRectPictViewCnt;
	friend class CenterPictViewCnt;
	friend class DotPictViewCnt;
	afx_msg void OnEditByExe();
	afx_msg void OnEditview();
	afx_msg void OnInvertH();
	afx_msg void OnInvertV();
	afx_msg void OnLineColorChanged();
	afx_msg void OnPEAverage();
	afx_msg void OnPEMedian();
	afx_msg void OnPEMosaic();
	afx_msg void OnPEOilify();
	afx_msg void OnPEPosterize();
	afx_msg void OnPESharpness();
	afx_msg void OnPESolarize();
	afx_msg void OnPictMouseTrim();
	afx_msg void OnPictOpt();
	afx_msg void OnPictResize();
	afx_msg void OnPictTrim();
	afx_msg void OnPreview();
	afx_msg void OnReload();
	afx_msg void OnResetSelRect();
	afx_msg void OnUpdateEditableCmd(CCmdUI*);
	afx_msg void OnUpdateEditview(CCmdUI*);
	afx_msg void OnUpdateLineColor(CCmdUI*);
	afx_msg void OnUpdatePictMouseTrim(CCmdUI*);
	afx_msg void OnUpdatePreview(CCmdUI*);
	afx_msg void OnUpdateResetSelRect(CCmdUI*);
	afx_msg void OutputPreview2File();
	const PictChar* GetPictChar() const;
	const SpicydogDoc* GetDocument() const;
	PictChar* GetPictChar();
	SpicydogDoc* GetDocument();
	virtual void OnDraw(CDC* pDC);
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual ~PictCharView();
	void AdjustSelRect();
	void DoEffect(UINT,UINT,UINT,int,int,int,PICT_EFFECT_PROC proc);
	void DrawBkgnd(CDC*) const;
	void DrawChar(CDC*) const;
	void DrawResizeHandle(CDC*) const;
	void DrawSelRect(CDC*) const;
	void GetPictBmpRect(CRect& rcLP) const;
	void UpdateCommandModes();

	DECLARE_MESSAGE_MAP()
};
