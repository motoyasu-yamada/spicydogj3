#pragma once

#include "model/stage/Stage.h"
#include "utility/toolbar/DialogBarDlg.H"
#include "utility/wnd/PreviewStatic.h"
#include "utility/wnd/InWnd.h"
#include "utility/wnd/InToolbar.h"
#include "utility/wnd/SplitterCtrl.h"

class SpicydogDoc;
class Stage;
class KeyFrame;

class TimelineDlg :
	public DialogBarDlg,
	public InWndOwner {
private:
	enum HIT_LAYER {HL_ICON,HL_EYE,HL_LOCK,HL_NAME};
public:
	CScrollBar	m_sbV;
	CScrollBar	m_sbH;
	SplitterHCtrl	m_Splitter;
	InWnd     m_wndLayer;
	InWnd     m_wndFrame;
	InToolBar m_itbLayer;
	InToolBar m_itbFrame;

	CRect m_rcLayerHdr;
	CRect m_rcFrameHdr;
	CRect m_rcLayerList;
	CRect m_rcFrameList;
	CRect m_rcLayers;
	CRect m_rcFrames;

	int m_nFrameMax;
	int m_nFramePage;
	int m_nLayerMax;
	int m_nLayerPage;
	int m_nStartFrame;
	int m_nStartLayer;
	BOOL m_bInited;
	COleDropTarget m_Target;
	COleDataSource m_Source;
	CRect m_rcDrag;
	SpicydogDoc* m_pDocument;

public:
	DECLARE_DYNCREATE(TimelineDlg)
	TimelineDlg();
	static void clinit();
	virtual ~TimelineDlg();
	virtual void OnDocumentActivated(CDocument*);
	virtual void OnDocumentUpdated(CDocument*,LPARAM lHint,CObject* pHint);

private:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActionsProp();
	afx_msg void OnDelFrame();
	afx_msg void OnDelLabel();
	afx_msg void OnDelLayer();
	afx_msg void OnDupLayer();
	afx_msg void OnHideOtherLayers();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnInsEmptyKeyFrame();
	afx_msg void OnInsFrame();
	afx_msg void OnInsKeyFrame();
	afx_msg void OnInsLabel();
	afx_msg void OnInsLayer();
	afx_msg void OnLockOtherLayers();
	afx_msg void OnModLabel();
	afx_msg void OnMoveDown();
	afx_msg void OnMoveLeft();
	afx_msg void OnMoveRight();
	afx_msg void OnMoveUp();
	afx_msg void OnOrderDown();
	afx_msg void OnOrderUp();
	afx_msg void OnRenLayer();
	afx_msg void OnShowAllLayers();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSplitHMove();
	afx_msg void OnTweenCreate();
	afx_msg void OnTweenProperty();
	afx_msg void OnUpdateActionsProp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDelFrame(CCmdUI*);
	afx_msg void OnUpdateDelLabel(CCmdUI*);
	afx_msg void OnUpdateDelLayer(CCmdUI*);
	afx_msg void OnUpdateDupLayer(CCmdUI*);
	afx_msg void OnUpdateHideOtherLayers(CCmdUI*);
	afx_msg void OnUpdateInsEmptyKeyFrame(CCmdUI*);
	afx_msg void OnUpdateInsFrame(CCmdUI*);
	afx_msg void OnUpdateInsKeyFrame(CCmdUI*);
	afx_msg void OnUpdateInsLabel(CCmdUI*);
	afx_msg void OnUpdateInsLayer(CCmdUI*);
	afx_msg void OnUpdateLockOtherLayers(CCmdUI*);
	afx_msg void OnUpdateModLabel(CCmdUI*);
	afx_msg void OnUpdateMoveDown(CCmdUI*);
	afx_msg void OnUpdateMoveLeft(CCmdUI*);
	afx_msg void OnUpdateMoveRight(CCmdUI*);
	afx_msg void OnUpdateMoveUp(CCmdUI*);
	afx_msg void OnUpdateOrderDown(CCmdUI*);
	afx_msg void OnUpdateOrderUp(CCmdUI*);
	afx_msg void OnUpdateRenLayer(CCmdUI*);
	afx_msg void OnUpdateShowAllLayers(CCmdUI*);
	afx_msg void OnUpdateTweenCreate(CCmdUI*);
	afx_msg void OnUpdateTweenProperty(CCmdUI*);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	BOOL GetSelKeyFrame(KeyFrame const*& infoFrame,BOOL bExact) const;
	BOOL IsFrameWnd(CWnd*) const;
	BOOL IsLayerWnd(CWnd*) const;
	BOOL IsValidActionsProp(KeyFrame const*& infoFrame);
	BOOL IsValidDelFrame();
	BOOL IsValidDelLabel(KeyFrame const*& infoFrame) const;
	BOOL IsValidDelLayer() const;
	BOOL IsValidDupLayer() const;
	BOOL IsValidHideOtherLayers() const;
	BOOL IsValidInsEmptyKeyFrame();
	BOOL IsValidInsFrame();
	BOOL IsValidInsKeyFrame();
	BOOL IsValidInsLabel(KeyFrame const*& infoFrame) const;
	BOOL IsValidInsLayer() const;
	BOOL IsValidLockOtherLayers() const;
	BOOL IsValidModLabel(KeyFrame const*& infoFrame) const;
	BOOL IsValidMoveDown();
	BOOL IsValidMoveLeft();
	BOOL IsValidMoveRight();
	BOOL IsValidMoveUp();
	BOOL IsValidOrderDown() const;
	BOOL IsValidOrderUp() const;
	BOOL IsValidRenLayer() const;
	BOOL IsValidShowAllLayers() const;
	BOOL IsValidTweenCreate(KeyFrame const*& infoFrame);
	BOOL IsValidTweenProperty(KeyFrame const*& infoFrame);
	BOOL SelectFrame(CPoint point,BOOL bLDown);
	BOOL SelectLayer(CPoint point);
	const Stage* GetTargetStage() const;
	HIT_LAYER GetLayerItemFromX(int) const;
	int GetDrawFrameCount() const;
	int GetFrameIndexFromX(int) const;
	int GetLayerIndexFromY(int) const;
	SpicydogDoc* GetDocument();
	Stage* GetTargetStage();
	virtual BOOL OnDrop(COleDataObject*,DROPEFFECT,CPoint);
	virtual BOOL OnInitDialog();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual DROPEFFECT OnDragEnter(COleDataObject*,DWORD,CPoint);
	virtual DROPEFFECT OnDragOver(COleDataObject*,DWORD,CPoint);
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnDragLeave(CWnd*);
	virtual void OnLButtonDblClk(InWnd*,UINT,CPoint);
	virtual void OnLButtonDown(InWnd*,UINT nFlags, CPoint point);
	virtual void OnPaint(InWnd*);
	virtual void OnRButtonDown(InWnd*,UINT nFlags, CPoint point);
	void AdjustSize();
	void CalcDragRect(int nSel);
	void DrawFrameHdr(CDC&);
	void DrawFrameList(CDC&);
	void DrawLayerHdr(CDC&);
	void DrawLayerList(CDC&);
	void GetCell(CPoint&,int nLayer,int nFrame);
	void UpdateFrameScroll();
	void UpdateLayerScroll();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
