#include "stdafx.h"
#include "resource.h"
#include "RenLayerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

RenLayerDlg::RenLayerDlg(LPCTSTR ptsz,CWnd* pParent)
:	CDialog(IDD_REN_LAYER, pParent) {
	m_strName = ptsz;
}

	
CString RenLayerDlg::GetName() const
{
	return m_strName;
}

void RenLayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RenLayerDlg)
	DDX_Control(pDX, IDCE_NAME, m_editName);
	DDX_Text(pDX, IDCE_NAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RenLayerDlg, CDialog)
	//{{AFX_MSG_MAP(RenLayerDlg)
	ON_UPDATE_COMMAND_UI(IDOK,OnUpdateOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void RenLayerDlg::OnOK() 
{
	UpdateData(TRUE);
	CDialog::OnOK();
}

void RenLayerDlg::OnUpdateOk(CCmdUI* pCmd)
{
	BOOL bValid = FALSE;
	if (UpdateData(TRUE) && 1 < m_strName.GetLength())
		bValid = TRUE;

	pCmd->Enable(bValid);
}

BOOL RenLayerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_editName.SetSel(0,-1);
	m_editName.SetFocus();

	return FALSE;
}
