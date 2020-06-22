#include "model/stage/Animating.h"

class EditAnimatingDlg : public CDialog {
private:
	CComboBox	     m_cbRotateType;
	CEdit	         m_editAccent;
	CSpinButtonCtrl	 m_spinRotateCount;
	CSliderCtrl	     m_scAccent;
	const Animating* m_orgAnimating;
	BOOL             m_resizable;
public:
	Animating editAnimating;
	EditAnimatingDlg(BOOL resizable,Animating*,CWnd* parent);
private:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnUpdate();
	void UpdateControls();
	DECLARE_MESSAGE_MAP()
};
