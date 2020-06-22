#pragma once

#include "utility/toolbar/DialogBarDlg.h"
#include "utility/wnd/InToolBar.h"

class Stage;
class SpicydogDoc;

class StagesDlg : public DialogBarDlg {
private:
	CListCtrl	 m_list;
	InToolBar    m_toolbar;
	BOOL         m_bInited;
	Stage*       m_activeStage;
	CImageList   m_images;
	SpicydogDoc* m_activeDocument;
public:
	DECLARE_DYNCREATE(StagesDlg)
	StagesDlg();
	virtual ~StagesDlg();

protected:
	Stage* GetSelStage();
	virtual void OnOK() {}
	virtual void OnCancel() {}
	virtual void OnDocumentActivated(CDocument*);
	virtual void OnDocumentUpdated(CDocument*,LPARAM,CObject*);
	virtual void OnNotify(DWORD,CDocument*,LPARAM,LPVOID);
	void ActivateSelStage();

	afx_msg void OnEdit();
	afx_msg void OnInsSubStage();
	afx_msg void OnDelSubStage();
	afx_msg void OnRenSubStage();
	afx_msg void OnUpdate_ReqActive(CCmdUI*);
	afx_msg void OnUpdate_ReqSel(CCmdUI*);
	afx_msg void OnUpdate_ReqSelSub(CCmdUI*);

	//{{AFX_VIRTUAL(StagesDlg)
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL
	//{{AFX_MSG(StagesDlg)
	afx_msg void OnRclickStages(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkStages(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
