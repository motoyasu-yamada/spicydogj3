#pragma once

class NewEmptyPictCharDlg : public CDialog {
	CSpinButtonCtrl	m_spinW;
	CSpinButtonCtrl	m_spinH;
	int		m_nH;
	int		m_nW;

public:
	NewEmptyPictCharDlg(CWnd* pParent = NULL);
	CSize GetSize() const;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
};