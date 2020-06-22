#include "stdafx.h"
#include "resource.h"
#include "EditColoringDlg.h"
#include "model/char/Char.h"
#include "model/AffineMatrix.h"
#include "utility/PaintMemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(EditColoringDlg, CDialog)
	//{{AFX_MSG_MAP(EditColoringDlg)
	ON_BN_CLICKED(IDCB_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDCB_RESET, OnReset)
	ON_CBN_SELCHANGE(IDCC_SELTYPE, OnSelchangeSeltype)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(EditColoringDlg,CDialog)
EditColoringDlg::EditColoringDlg(Char* pchar,Coloring* coloring,CWnd* pParent /*=NULL*/)
:	CDialog(IDD_EDIT_COLORING, pParent),
	m_dlgCustom(coloring),
	m_dlgSelect(coloring),
	m_dlgMeido (coloring) {

	m_bInited = FALSE;
	m_char    = pchar;
	const_cast<Coloring&>(m_coloringOrg) = *coloring;
	m_coloring    = coloring;
	m_pdlgCurrent = NULL;
	m_editType    = m_coloring->editType;
	m_psPreview.SetListener(this);

	ASSERT_VALID(m_char);
	ASSERT(m_char->GetCharType().canModifyColoring);
}

void EditColoringDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCS_CHILD, m_stChild);
	DDX_Control(pDX, IDCS_PREVIEW, m_psPreview);
	DDX_CBIndex(pDX, IDCC_SELTYPE, m_editType);
	
	class CDlgRev : public CDialog {
		friend class EditColoringDlg;
	};
	if (m_pdlgCurrent != NULL) {
		((CDlgRev*)m_pdlgCurrent)->DoDataExchange(pDX);
	} else {
		if (pDX->m_bSaveAndValidate) {
			Coloring d;
			*m_coloring = d;
		}
	}
	if (pDX->m_bSaveAndValidate) {
		m_coloring->editType = m_editType;
	}
}

void EditColoringDlg::OnDefault() {
	Coloring d;
	*m_coloring = d;
	m_editType = Coloring::EDIT_TYPE_NONE;

	UpdateData(FALSE);
	ChangeSelectType(m_editType);
	UpdateData(FALSE);

	m_psPreview.Invalidate();
}

void EditColoringDlg::OnReset() 
{
	*m_coloring = m_coloringOrg;
	m_editType = m_coloringOrg.editType;

	UpdateData(FALSE);
	ChangeSelectType(m_editType);
	UpdateData(FALSE);

	m_psPreview.Invalidate();
}

void EditColoringDlg::OnOK() {
	if (UpdateData(TRUE)) {
		CDialog::OnOK();
	}
}

void EditColoringDlg::OnPSPaint(PreviewStatic& ps) {
	PaintMemDC dc(&ps);

	CRect rc;
	ps.GetClientRect(rc);
	if (IsWindowEnabled()) {

		dc.FillSolidRect(rc,GetSysColor(COLOR_APPWORKSPACE));
		AffineMatrix matrix;
		matrix.x = (rc.right  - rc.left) / 2;
		matrix.y = (rc.bottom - rc.top ) / 2;
		m_char->Draw(&dc,&matrix,m_coloring);
	} else {
		dc.FillSolidRect(rc,GetSysColor(COLOR_APPWORKSPACE));
	}
}

BOOL EditColoringDlg::OnPSEraseBkgnd(PreviewStatic&,CDC&) {
	return TRUE;
}

void EditColoringDlg::Invalidate(CDialog* pChild) {
	ASSERT_VALID(pChild);
	CWnd* pParent = pChild->GetParent();
	ASSERT_VALID(pParent);
	pParent = CWnd::FromHandlePermanent(pParent->GetSafeHwnd());
	EditColoringDlg* pTHIS = (EditColoringDlg*)DYNAMIC_DOWNCAST(EditColoringDlg,pParent);
	ASSERT_VALID(pTHIS);
	pTHIS->m_psPreview.Invalidate();
}

BOOL EditColoringDlg::OnInitDialog() {
	CDialog::OnInitDialog();
	
	InitChildDialog(m_dlgCustom,IDD_EDIT_COLORING_CUSTOM);
	InitChildDialog(m_dlgSelect,IDD_EDIT_COLORING_SELECT);
	InitChildDialog(m_dlgMeido, IDD_EDIT_COLORING_MEIDO);
	m_bInited = TRUE;

	ChangeSelectType(m_editType);

	return TRUE;
}

void EditColoringDlg::InitChildDialog(CDialog& dlg,UINT nID)
{
	CRect rc;
	m_stChild.GetWindowRect(rc);
	ScreenToClient(rc);

	VERIFY(dlg.Create(nID,this));
	dlg.SetDlgCtrlID(nID);
	dlg.MoveWindow(rc,FALSE);
	dlg.ShowWindow(SW_HIDE);
	dlg.UpdateWindow();
}

void EditColoringDlg::OnSelchangeSeltype() {
	VERIFY(UpdateData(TRUE));
	ChangeSelectType(m_editType);
}

void EditColoringDlg::ChangeSelectType(int nType) {
	if (m_pdlgCurrent != NULL) {
		m_pdlgCurrent->UpdateData(TRUE);
	}

	switch (nType) {
	case 0:
		m_pdlgCurrent = NULL;
		break;
	case 1:
		m_pdlgCurrent = &m_dlgSelect;
		break;
	case 2:
		m_pdlgCurrent = &m_dlgMeido;
		break;
	case 3:
		m_pdlgCurrent = &m_dlgCustom;
		break;
	}
	
	ShowChildDialog();
	if (m_pdlgCurrent != NULL)
	{
		ASSERT_VALID(m_pdlgCurrent);
		m_pdlgCurrent->UpdateData(FALSE);
		Invalidate(m_pdlgCurrent);
	}
}

void EditColoringDlg::ShowChildDialog()
{
	ShowChildDialog(&m_dlgCustom);
	ShowChildDialog(&m_dlgMeido);
	ShowChildDialog(&m_dlgSelect);
}

void EditColoringDlg::ShowChildDialog(CDialog* pdlg)
{
	ASSERT_VALID(pdlg);
	BOOL bShow = m_pdlgCurrent == pdlg;
	pdlg->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	pdlg->EnableWindow(bShow);
	pdlg->UpdateWindow();
}

