#pragma once

class PictTrimDlg : public CDialog {
private:
	CSpinButtonCtrl	m_spinW;
	CSpinButtonCtrl	m_spinH;
	int		m_nH;
	int		m_nW;
	int		m_nAlign;
	int m_nOrgW;
	int m_nOrgH;
	BOOL m_bInited;
public:
	PictTrimDlg(int nW,int nH,CWnd* pParent = NULL);
	void GetTrimParam(int& nW,int& nH,int& nAlign) const;

private:
	void UpdateCtrls();
	void SetButtonIcon(UINT,UINT);
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual void OnOK();
	afx_msg void OnReset();
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdate();
	DECLARE_MESSAGE_MAP()
};

