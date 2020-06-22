#pragma once

class AppBugDump;

class AppBugDlg : public CDialog {
private:
	CEdit	m_edit;
	CString	m_strBody;
	CString	m_strSubject;

public:
	AppBugDlg(AppBugDump&);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnOpenMailer();
	virtual BOOL OnInitDialog();
	afx_msg void OnWwwSpicydog();
	DECLARE_MESSAGE_MAP()
};