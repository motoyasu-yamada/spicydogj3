#pragma once

class UploadedDlg : public CDialog {
private:
	CBrush  m_outputBkgnd;
	CString m_url;
public:
	UploadedDlg(LPCTSTR url);
private:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSetClipboard();
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};