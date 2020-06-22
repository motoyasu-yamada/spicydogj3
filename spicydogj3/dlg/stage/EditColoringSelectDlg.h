#pragma once

#include "utility/wnd/ColorBox.h"

class Coloring;
class EditColoringSelectDlg : public CDialog {
	friend class EditColoringDlg;
	ColorBoxEx  m_colorBox;
	CSliderCtrl	m_applySlider;
	int         m_apply;
	Coloring*   m_coloring;

public:
	EditColoringSelectDlg(Coloring*);
protected:
	afx_msg LRESULT OnSelect(WPARAM,LPARAM);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnOK() {}
	virtual void OnCancel() {}
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};
