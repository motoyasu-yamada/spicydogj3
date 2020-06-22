#pragma once

#include "ActionDlg.H"
#include "model/stage/Label.h"

class JumpAction;
class JumpActionDlg : public ActionDlg {
	CComboBox       m_cbLabelList;
	LABEL_NAME_LIST m_labels;

public:
	DECLARE_DYNCREATE(JumpActionDlg);
	JumpActionDlg();

private:
	void UpdateLabelList();
	JumpAction* GetAction();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL Create(CWnd*);
	virtual void SetAction(Appli*,Action*);
	virtual void Apply();
	afx_msg void OnSelchangeLabellist();

	DECLARE_MESSAGE_MAP()
};
