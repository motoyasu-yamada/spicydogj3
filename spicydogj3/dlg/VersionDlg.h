#pragma once
#include "utility/wnd/BitmapStatic.h"

/**	アプリケーションのバージョン情報で使われるダイアログ */
class VersionDlg : public CDialog {
	BitmapStatic	m_bsWwwJphone;
	BitmapStatic	m_bsWwwSpicysoft;
	BitmapStatic	m_bsWwwSpicyDog;
	CString	m_strVersion;
	CString	m_strRemarks;
	CString	m_strStaff;
	CBrush m_brBkgnd;
public:
	VersionDlg();
protected:
	afx_msg void OnWWW(UINT);
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};
