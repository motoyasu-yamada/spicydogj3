#pragma once


class Coloring;
class EditColoringMeidoDlg : public CDialog {
	friend class EditColoringDlg;
private:
	CSliderCtrl	m_scMeido;
	CEdit	    m_editMeido;
	int		    m_nMeido;
	BOOL        m_bInited;
	BOOL        m_bUpdateLock;
	Coloring*   m_coloring;

public:
	EditColoringMeidoDlg(Coloring*);
protected:
	void UpdateMeidoText();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK() {}
	virtual void OnCancel() {}
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_MESSAGE_MAP()
};

