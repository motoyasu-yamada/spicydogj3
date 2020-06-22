#pragma once

#include "ActionDlg.h"
class SetBacklightAction;
class Appli;

class SetBacklightActionDlg : public ActionDlg {
public:
	DECLARE_DYNCREATE(SetBacklightActionDlg)
	SetBacklightActionDlg();

private:
	virtual BOOL Create(CWnd*);
	virtual void SetAction(Appli*,Action*);
	virtual void Apply();
	SetBacklightAction* GetAction();
	virtual void DoDataExchange(CDataExchange* pDX);

	int m_enabled;

	DECLARE_MESSAGE_MAP()
};
