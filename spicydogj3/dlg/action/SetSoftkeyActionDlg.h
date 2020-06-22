#pragma once
#include "ActionDlg.H"

class SetSoftkeyAction;
class Appli;

class SetSoftkeyActionDlg : public ActionDlg {
public:
	DECLARE_DYNCREATE(SetSoftkeyActionDlg)
	SetSoftkeyActionDlg();

private:
	virtual BOOL Create(CWnd*);
	virtual void SetAction(Appli*,Action*);
	virtual void Apply();
	SetSoftkeyAction* GetAction();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	CString	m_label;
	int		m_which;

	DECLARE_MESSAGE_MAP()
};
