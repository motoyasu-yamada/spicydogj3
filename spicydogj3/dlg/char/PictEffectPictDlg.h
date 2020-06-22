#pragma once
#include "model/char/PictChar.h"
#include "utility/wnd/PreviewStatic.h"

class PictEffectPictDlg
:	public CDialog,
	public PreviewStatic::PaintEventListener {

	PICT_EFFECT_PROC m_proc;
	PreviewStatic	 m_psPreview;

	CSliderCtrl	m_scParam;
	LBitmapBase m_bmpSrc;
	LBitmapBase m_bmpDst;
	int m_nMin;
	int m_nMax;
	int m_nDefault;
	int		m_nParam;
	CString	m_strParamName;
	CString	m_strUnit;
	CString	m_strEffectName;

public:
	PictEffectPictDlg(const LBitmapBase&,
				   UINT nEffectNameID,
				   UINT nParamNameID,
				   UINT nUnitNameID,
				   int nMin,
				   int nMax,
				   int nDefault,
				   PICT_EFFECT_PROC proc,
				   CWnd* pParent = NULL);
	int GetEffectParam() const;

protected:
	void UpdatePreview();
	virtual void OnPSPaint(PreviewStatic&);
	virtual BOOL OnPSEraseBkgnd(PreviewStatic&,CDC&);
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};