#include "stdafx.h"
#include "resource.h"

#include "ViewADFDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(ViewADFDlg, CDialog)
	//{{AFX_MSG_MAP(ViewADFDlg)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

ViewADFDlg::ViewADFDlg(CWnd* pParent)
:	CDialog(IDD_VIEWADF, pParent),
	m_brush(::GetSysColor(COLOR_WINDOW)) {
}

void ViewADFDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCE_ADF, m_edit);
}

void ViewADFDlg::OnSize(UINT nType, int cx, int cy) {
	CDialog::OnSize(nType, cx, cy);	
	if (m_edit.GetSafeHwnd() == NULL) {
		return;
	}
	m_edit.SetWindowPos(NULL,0,0,cx,cy,SWP_NOACTIVATE);
}

HBRUSH ViewADFDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
	return m_brush;
}

BOOL ViewADFDlg::OnInitDialog() {
	CDialog::OnInitDialog();
	m_edit.SetWindowText(m_strJam);	
	m_edit.SetSel(-1,-1);
	return TRUE;
}
