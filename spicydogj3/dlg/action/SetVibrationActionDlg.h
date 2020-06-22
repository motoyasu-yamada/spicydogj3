#pragma once
#include "ActionDlg.H"

class SetVibrationAction;

class SetVibrationActionDlg : public ActionDlg {
public:
	DECLARE_DYNCREATE(SetVibrationActionDlg)
	SetVibrationActionDlg();

protected:
	SetVibrationAction* GetAction();
	virtual BOOL Create(CWnd*);
	virtual void SetAction(Appli*,Action*);
	virtual void Apply();
	virtual void DoDataExchange(CDataExchange* pDX);

	int		m_vibration;

	DECLARE_MESSAGE_MAP()
};

