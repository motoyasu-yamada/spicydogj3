#pragma once

#include "ActionDlg.h"
class RandomJumpAction;

class RandomJumpActionDlg : public ActionDlg {
public:
	DECLARE_DYNCREATE(RandomJumpActionDlg);
	RandomJumpActionDlg();
protected:
	virtual BOOL Create(CWnd*);
	virtual void SetAction(Appli*,Action*);
	virtual void Apply();
	RandomJumpAction* GetAction();
	void UpdateCtrls();
	void UpdateList();
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnClickCases(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDel();
	afx_msg void OnIns();
	afx_msg void OnMod();
	afx_msg void OnUpdateDel(CCmdUI*);
	afx_msg void OnUpdateIns(CCmdUI*);
	afx_msg void OnUpdateMod(CCmdUI*);
	virtual BOOL OnInitDialog();
private:
	CListCtrl	m_listCases;
	DECLARE_MESSAGE_MAP()
};
