#pragma once

#include "utility/toolbar/DialogBarDlg.h"
#include "utility/wnd/InToolBar.h"
class CmdHistory;

class HistoryDlg : public DialogBarDlg {
private:
	int         m_curCmd;
	CImageList	m_imagesBg;
	CImageList	m_imagesCmd;
	CPen		m_penBorder;
	CDocument*  m_activeDocument;
	CmdHistory* m_history;
	InToolBar   m_tools;
	BOOL        m_inited;
	CListCtrl	m_list;
public:
	DECLARE_DYNCREATE(HistoryDlg)
	HistoryDlg();
	~HistoryDlg();
private:
	void AdjustSize();
	void AdjustListColumnSize();
	virtual void OnDocumentUpdated(CDocument* pDoc,LPARAM,CObject*);
	virtual void OnDocumentActivated(CDocument*);
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDblclkHistory(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOwnerData(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	DECLARE_MESSAGE_MAP()
};
