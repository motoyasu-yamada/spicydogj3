#pragma once

class Action;
class Appli;

/** 各アクションのプロパティー */
class ActionDlg : public CDialog {
public:
	DECLARE_DYNCREATE(ActionDlg)
	ActionDlg();
	virtual ~ActionDlg();
	virtual void SetAction(Appli*,Action*);
	virtual BOOL Create(CWnd*);
	virtual void Apply();
	
private:
	BOOL IsInited() const;
	virtual afx_msg void OnOK();
	virtual afx_msg void OnCancel();
	
protected:
	Action* m_action;
	Appli*  m_appli;

	DECLARE_MESSAGE_MAP()
};