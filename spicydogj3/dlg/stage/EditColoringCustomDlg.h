#pragma once

class Coloring;
class EditColoringCustomDlg : public CDialog {
	friend class EditColoringDlg;

	Coloring*       m_coloring;
	CSpinButtonCtrl	m_spinRedMult;
	CSpinButtonCtrl	m_spinRedAdd;
	CSpinButtonCtrl	m_spinGreenMult;
	CSpinButtonCtrl	m_spinGreenAdd;
	CSpinButtonCtrl	m_spinBlueMult;
	CSpinButtonCtrl	m_spinBlueAdd;
	BOOL m_bInited;
	BOOL m_bUpdateLock;

public:
	EditColoringCustomDlg(Coloring*);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdateValue();

	DECLARE_MESSAGE_MAP()
	virtual void OnOK() {}
	virtual void OnCancel() {}
};