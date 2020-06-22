#include "stdafx.h"
#include "resource.h"
#include "RenSubStageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(RenSubStageDlg, CDialog)
END_MESSAGE_MAP()

RenSubStageDlg::RenSubStageDlg(CWnd* pParent) :
	CDialog(IDD_REN_SUBSTAGE, pParent) {
	m_strName = _T("");
}

void RenSubStageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDCE_NAME, m_strName);
}

void RenSubStageDlg::OnOK()  {
	if (Save()) {
		CDialog::OnOK();
	}
}

BOOL RenSubStageDlg::Save() {
	if (!UpdateData(TRUE)) {
		return FALSE;
	}
	return !m_strName.IsEmpty();
}
