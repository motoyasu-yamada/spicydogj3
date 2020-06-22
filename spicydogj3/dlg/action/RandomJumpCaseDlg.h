#pragma once

#include "model/stage/Label.h"
class RandomJumpCase;
class Appli;
class RandomJumpCaseDlg : public CDialog {
public:
	RandomJumpCaseDlg(Appli* appli,RandomJumpCase*,CWnd* wnd);
protected:
	void UpdateCtrls();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnChanged();

private:
	LABEL_NAME_LIST m_labels;
	RandomJumpCase* m_case;
	CComboBox	    m_cbLabelList;
	int		        m_probability;

	DECLARE_MESSAGE_MAP()
};
