#pragma once

#include "dlg/action/ActionListDlg.H"

const Appli;
class ActionList;

class KeyFrameActionDlg : public CDialog {
private:
	Appli* m_appli;
	const ActionList* m_actionList;
	ActionListDlg m_dlgActionList;
public:
	KeyFrameActionDlg(Appli* appli,const ActionList* actionList,CWnd* parent);
	const ActionList* GetEditActionList() const;
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
};
