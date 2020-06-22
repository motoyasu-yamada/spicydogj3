#pragma once

#include "utility/wnd/InToolBar.h"
#include "utility/wnd/PreviewStatic.h"
#include "utility/wnd/SplitterCtrl.h"
#include "utility/toolbar/DialogBarDlg.h"
#include "wnd/CharListCtrl.h"

class Char;
class CharList;
class CharListDlg;
class SpicydogDoc;
class AppliFrameWnd;

class CharListDlg 
:	public DialogBarDlg,
	public PreviewStatic::PaintEventListener {
	friend class CharListCtrlDropTarget;
	friend class CharListCtrl;

private:
	enum VIEW_MODE { VIEW_MODE_LIST,VIEW_MODE_PREVIEW };
	VIEW_MODE m_viewMode;

	CharListCtrl	m_listPreview;
	CharListCtrl	m_listList;
	PreviewStatic   m_stPreview;
	SplitterVCtrl   m_splitter;
	InToolBar       m_itb;
	CImageList      m_imageList;
	SpicydogDoc*    m_document;
	CharList*       m_charList;
	CPen   m_penPreviewGrid;
	CFont  m_fontPreviewList;
	BOOL   m_bInit;

public:
	DECLARE_DYNCREATE(CharListDlg)
	CharListDlg();
	virtual ~CharListDlg();

private:
	const CListCtrl& GetActiveList() const;
	int GetSelectedChars(std::list<Char*>&);
	int GetSingleSelection() const;
	AppliFrameWnd* GetAppliFrameWnd();
	BOOL InsChar(Char*);
	void ModChar(int nEdit);
	void AdjustSize();
	void InvalidateView();
	void InvalidatePreview();
	void SwitchViewMode(VIEW_MODE,BOOL bForce = FALSE);
	BOOL IsSingleSelected() const;
	BOOL IsSelected() const;
	BOOL CanAttachChar();
	void SetupPreviewListCtrl();
	void SetupListListCtrl();
	void SetupPreviewStatic();
	void SetupResources();
	BOOL AcceptOle(COleDataObject*,BOOL bDnD,DROPEFFECT de = DROPEFFECT_NONE,CPoint pt = CPoint(0,0));
	BOOL SubmitOle(COleDataSource*,BOOL bDnD);

	virtual void OnDocumentUpdated(CDocument*,LPARAM,CObject*);
	virtual void OnDocumentActivated(CDocument*);
	virtual void OnNotify(DWORD,CDocument*,LPARAM,LPVOID);
	virtual BOOL OnInitDialog();
	virtual void OnPSPaint(PreviewStatic&);
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnSplitVMove();
	afx_msg void OnInsCharMenu();
	afx_msg void OnInsPictChar();
	afx_msg void OnInsEmptyPictChar();
	afx_msg void OnInsTextChar();
	afx_msg void OnInsBtnChar();
	afx_msg void OnInsSndChar();
	afx_msg void OnRenameChar();
	afx_msg void OnEditChar();
	afx_msg void OnDupChar();
	afx_msg void OnAttachChar();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnUpdateEditPaste(CCmdUI*);
	afx_msg void OnClickCharList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkCharList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickCharList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReturnCharList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateAttachChar(CCmdUI*);
	afx_msg void OnUpdateRequireSingleSelected(CCmdUI*);
	afx_msg void OnUpdateRequireSelected(CCmdUI*);
	afx_msg void OnUpdateExistActiveDocument(CCmdUI*);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnGetCharInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPreviewView();
	afx_msg void OnListView();
	afx_msg void OnUpdateView(CCmdUI*);
	DECLARE_MESSAGE_MAP()
};


