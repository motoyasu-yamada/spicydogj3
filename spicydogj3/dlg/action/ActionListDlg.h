#pragma once

#include <memory>
#include <model/action/ActionList.h>

class Appli;
class ActionDlg;

/** アクション一覧ダイアログ */
class ActionListDlg : public CDialog {
private:
	typedef std::auto_ptr<ActionDlg>     DIALOGPTR;
private:
	ActionList m_editActionList;
	Action*    m_selectAction;
	Appli*     m_appli;
	CButton	   m_wndActionName;
	CStatic	   m_stNoProp;
	CStatic	   m_stPropDlg;
	CListBox   m_lbActions;
	DIALOGPTR  m_dialog;

public:
	ActionListDlg();
	virtual ~ActionListDlg();
	virtual BOOL Create(Appli* applil,const ActionList*,CWnd*,LPCRECT = NULL);
	const ActionList& GetEditActionList() const;
	void UpdateCurrentData();

private:
	virtual void OnOK();
	virtual void OnCancel();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	void HidePrevPropDlg();
	void DisplayPropDlg();
	void DisplayNonePropDlg();
	void ChangeActionOrder(int,int);
	void InsAction(Action* action);
	void UpdateActionList();
	void UpdateCtrls();
	BOOL IsSelected() const;
	BOOL IsValidActDown() const;
	BOOL IsValidActUp() const;
	afx_msg void OnActAdd();
	afx_msg void OnActDel();
	afx_msg void OnActDown();
	afx_msg void OnActUp();
	afx_msg void OnInsPlay();
	afx_msg void OnInsStop();
	afx_msg void OnInsQuit();
	afx_msg void OnInsJump();
	afx_msg void OnInsSoundPlay();
	afx_msg void OnInsSoundStop();
	afx_msg void OnInsSetBacklight();
	afx_msg void OnInsSetBackgroundColor();
	afx_msg void OnInsSetVibration();
	afx_msg void OnInsSetSoftkey();
	afx_msg void OnInsDelayJump();
	afx_msg void OnInsRandomJump();
	afx_msg void OnSelchangeAction();
	afx_msg void OnDestroy();	
	DECLARE_MESSAGE_MAP()
};