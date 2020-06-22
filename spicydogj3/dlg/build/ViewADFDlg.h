#pragma once

class ViewADFDlg : public CDialog {
private:
	CBrush m_brush;
	CEdit  m_edit;
public:
	ViewADFDlg(CWnd* pParent = NULL);
	CString	m_strJam;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
};