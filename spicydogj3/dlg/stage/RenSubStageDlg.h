#pragma once

class RenSubStageDlg : public CDialog {
public:
	CString	m_strName;
	RenSubStageDlg(CWnd* pParent = NULL);

protected:
	BOOL Save();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
};