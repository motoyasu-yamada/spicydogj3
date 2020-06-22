#pragma once

#include "EditColoringCustomDlg.h"
#include "EditColoringMeidoDlg.h"
#include "EditColoringSelectDlg.h"
#include "model/stage/Coloring.h"
#include "utility/wnd/PreviewStatic.h"

class Char;

class EditColoringDlg 
:	public CDialog,
	public PreviewStatic::PaintEventListener {
private:
	CStatic         m_stChild;
	PreviewStatic	m_psPreview;
	CDialog*         m_pdlgCurrent;
	EditColoringCustomDlg m_dlgCustom;
	EditColoringMeidoDlg  m_dlgMeido;
	EditColoringSelectDlg m_dlgSelect;

	Char*          m_char;
	Coloring*      m_coloring;
	const Coloring m_coloringOrg;
	BOOL           m_bInited;
	int		       m_editType;

public:
	DECLARE_DYNAMIC(EditColoringDlg);
	EditColoringDlg(Char*,Coloring* coloring,CWnd* pParent = NULL);
	static void Invalidate(CDialog* pChild);

private:
	void ChangeSelectType(int nType);
	void ShowChildDialog();
	void ShowChildDialog(CDialog*);
	void InitChildDialog(CDialog&,UINT);
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void OnPSPaint(PreviewStatic&);
	virtual BOOL OnPSEraseBkgnd(PreviewStatic&,CDC&);
	afx_msg void OnDefault();
	afx_msg void OnReset();
	afx_msg void OnSelchangeSeltype();

	DECLARE_MESSAGE_MAP()
};