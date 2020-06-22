#pragma once

#include "ActionDlg.h"
#include "model/stage/Label.h"

class DelayJumpAction;
class DelayJumpActionDlg : public ActionDlg {
public:
	DECLARE_DYNCREATE(DelayJumpActionDlg)
	DelayJumpActionDlg();

private:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL Create(CWnd*);
	virtual void SetAction(Appli*,Action*);
	virtual void Apply();
	DelayJumpAction* GetAction();

	enum { IDD = IDD_ACTDLG_DELAY_JUMP };
	CComboBox	    m_combo;
	int		        m_delay;
	LABEL_NAME_LIST m_labels;

	DECLARE_MESSAGE_MAP()
};

