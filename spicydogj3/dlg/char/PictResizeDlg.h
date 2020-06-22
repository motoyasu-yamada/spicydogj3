#pragma once

#include <ltwrappr.h>

class PictResizeDlg : public CDialog {
	CSpinButtonCtrl	m_spinW;
	CSpinButtonCtrl	m_spinH;
	int		m_nH;
	int		m_nW;
	const LBitmapBase& m_bmp;
	BOOL m_bWBase;
	BOOL m_bInited;
	BOOL m_bReenter;
	int m_nOrgW;
	int m_nOrgH;

public:
	PictResizeDlg(const LBitmapBase& bmp,CWnd* pParent = NULL);   
	CSize GetNewSize() const;

private:
	void UpdateCtrls();
	void KeepRatio();
	BOOL IsToKeepRatio() const;
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	afx_msg void OnUpdateH();
	afx_msg void OnUpdateW();
	afx_msg void OnKeep();
	virtual BOOL OnInitDialog();
	afx_msg void OnReset();
	DECLARE_MESSAGE_MAP()
};
