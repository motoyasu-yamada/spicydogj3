#pragma once
#include <AfxOle.h>

class CharListDlg;
class CharListCtrlDropTarget : public COleDropTarget {
private:
	CharListDlg* m_pDictionaryView;
public:
	CharListCtrlDropTarget();
	void Register(CWnd*,CharListDlg*);
	virtual DROPEFFECT OnDragEnter(CWnd* pWnd,COleDataObject*,DWORD,CPoint);
	virtual DROPEFFECT OnDragOver(CWnd* pWnd,COleDataObject*,DWORD,CPoint);
	virtual BOOL OnDrop(CWnd*,COleDataObject*,DROPEFFECT,CPoint);
};

class CharListCtrl : public CListCtrl {
private:
	CharListCtrlDropTarget m_Target;
	COleDataSource m_Source;
	CharListDlg* m_pDictionaryView;
public:
	CharListCtrl();
	virtual ~CharListCtrl();
	void Register(CharListDlg*);
protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};