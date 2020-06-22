#pragma once

class Placing;
class EditPlacingDlg : public CDialog {
private:
	Placing& m_frame;
	CSpinButtonCtrl	m_spinX;
	CSpinButtonCtrl	m_spinY;
	int		m_x;
	int		m_y;
	BOOL    m_sizing;

public:
	EditPlacingDlg(BOOL bSizing,Placing&,CWnd* pParent = NULL);
	virtual void DoDataExchange(CDataExchange* pDX);
protected:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnKickIdle();
	afx_msg void OnUpdateSizing(CCmdUI*);
	afx_msg void OnUpdateTranslate(CCmdUI*);
	DECLARE_MESSAGE_MAP()
};