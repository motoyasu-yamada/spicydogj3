#pragma once

#include "utility/wnd/InPlaceEdit.h"
#include "utility/wnd/InToolBar.h"

class ScaleBox : public CWnd {
private:
	static CFont s_fontTri;
	static CFont s_fontNum;
	enum ScaleMode {modeScale,modeScaleDocumentToFitSize,modeScaleTotalToFitSize};
	int m_nScaleMode;
	double m_scale;
	CRect m_rcTri;
	CRect m_rcNum;
	CRect m_rcCob;
	InPlaceEdit m_editNum;
	InToolBar   m_toolbar;

public:
	DECLARE_DYNAMIC(ScaleBox);
	ScaleBox();
	virtual ~ScaleBox();
	BOOL Create(CWnd*,CPoint ptTopLeft,int cx,UINT nCtrlId);

private:
	static void clinit();
	static BOOL IsValidScale(double);
	static BOOL CompareScale(double,double);
	static CString Scale2String(double);
	BOOL CompareScaleParamWithCurrent(int,double) const;
	void SelectScale(int mode,double);
	void SetScale(int mode,double);
	void EnterSelectScale();
	void EnterEditScale();
	afx_msg void OnScaleMenu(UINT);
	afx_msg void OnUpdateScaleMenu(CCmdUI* pcmd);
	afx_msg void OnScaleToFitSize(UINT);
	afx_msg void OnUpdateScaleToFitSize(CCmdUI* pcmd);
	afx_msg LRESULT OnGetScaleParam(WPARAM,LPARAM);
	afx_msg LRESULT OnSetScaleParam(WPARAM,LPARAM);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnEnable(BOOL bEnable);
	DECLARE_MESSAGE_MAP()
};


