#pragma once

#include "model/char/PictChar.h"
#include "utility/wnd/PreviewStatic.h"
#include "utility/wnd/ColorBox.h"
#include "wnd/ActTableCtrl.h"

class PictChar;

/* 画像最適化ツールバー */
class EditPictQuantizeDlg : public CDialog,public PreviewStatic::PaintEventListener {
	CComboBox	  m_cbColorCount;
	CComboBox	  m_cbActType;
	CSliderCtrl   m_sliderJpgCompress;
	CStatic	      m_stColors;
	CScrollBar	  m_scV;
	PreviewStatic m_psPreview;
	ActTableCtrl  m_ctrlAct;
	ColorBoxEx    m_colorTransparency;
	BOOL m_bReenterLock;
	BOOL m_bInited;
	BOOL m_bPalette;
	PictQuantizeParam m_paramOrg;
	PictData     m_data;
	PictOptCache m_cache;
	PictQuantizeParam m_param;
public:
	EditPictQuantizeDlg(PictChar*);
	const PictQuantizeParam& GetQuantizeParam() const;
	virtual ~EditPictQuantizeDlg();

private:
	BOOL SaveData();
	void LoadData();
	void DoOptimize();
	void OnParamUpdated();
	void UpdateCtrls();

	virtual void OnPSPaint(PreviewStatic& ps);
	virtual BOOL OnPSEraseBkgnd(PreviewStatic&,CDC&);
	void DrawPreview(CDC*);

	//{{AFX_VIRTUAL(CPictOptDlg)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

	afx_msg void OnTransparencyColor();
	LRESULT OnClrBoxDropdown(WPARAM,LPARAM);
	//{{AFX_MSG(CPictOptDlg)
	afx_msg void OnBuildTypeChanged();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeActtype();
	afx_msg void OnTransparency();
	afx_msg void OnSelchangeColorbits();
	afx_msg void OnClickDither();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnReset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
