#pragma once

#include "model/build/BuildOutput.h"
#include "utility/toolbar/DialogBarDlg.h"

class BuildOutputDlg : public DialogBarDlg {
private:
	enum SORTTYPE { SORT_BY_TYPE=0,SORT_BY_LOC=1,SORT_BY_MSG=2};
	CListCtrl m_listOutput;
	BuildOutput::STATE m_state;
	CImageList m_images;
	SORTTYPE m_SortType;
	const BuildOutput* m_pBuildOutput;
	BOOL m_bInited;
public:
	DECLARE_DYNCREATE(BuildOutputDlg)
	BuildOutputDlg();
	virtual ~BuildOutputDlg();
protected:
	virtual void OnOK() {}
	virtual void OnCancel() {}
	int OnSortList(int nL,int nR);
	void SortList(SORTTYPE st);
	void SetupList();
	virtual void OnDocumentActivated(CDocument*);
	virtual void OnNotify(DWORD dwNotifyCode,CDocument*,LPARAM,LPVOID);
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnColumnclickOutput(NMHDR* pNMHDR, LRESULT* pResult);
	static int CALLBACK SortListCallback(LPARAM lParam1, LPARAM lParam2,LPARAM lParamSort);
	DECLARE_MESSAGE_MAP()
};
