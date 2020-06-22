#pragma once

#include "ActionDlg.H"
#include "utility/wnd/ColorFaceButton.h"

class SetBackgroundColorAction;
class Appli;

class SetBackgroundColorActionDlg : public ActionDlg {
private:
	ColorFaceButton m_btnColor;
public:
	DECLARE_DYNCREATE(SetBackgroundColorActionDlg)
	SetBackgroundColorActionDlg();

private:
	afx_msg void OnBkgnd(CCmdUI*);
	SetBackgroundColorAction* GetAction();
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL Create(CWnd*);
	virtual void SetAction(Appli*,Action*);
	virtual void Apply();
	DECLARE_MESSAGE_MAP()
};
