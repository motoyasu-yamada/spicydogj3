#pragma once

class RenLayerDlg : public CDialog {
private:
	CEdit	m_editName;
	CString	m_strName;
public:
	RenLayerDlg(LPCTSTR ptsz,CWnd* parent);
	CString GetName() const;
protected:
	afx_msg void OnUpdateOk(CCmdUI*);
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};
