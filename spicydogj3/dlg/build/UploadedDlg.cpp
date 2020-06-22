#include "stdafx.h"
#include "resource.h"
#include "UploadedDlg.h"
#include "utility/StringUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(UploadedDlg,CDialog)
	ON_BN_CLICKED(IDCB_COPY,OnSetClipboard)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

UploadedDlg::UploadedDlg(LPCTSTR url) :
	CDialog(IDD_UPLOADED) {
	ASSERT(AfxIsValidString(url));
	m_url = url;
}

BOOL UploadedDlg::OnInitDialog() {
	if (!CDialog::OnInitDialog()) {
		ASSERT(FALSE);
		return FALSE;
	}
	SetDlgItemText(IDCE_URL,m_url);
	return TRUE;
}

void UploadedDlg::OnSetClipboard() {
	SetClipText(m_url);
	AfxMessageBox(IDCS_SET_DOWNLOAD_URL_TO_CLIP,MB_OK);
}

HBRUSH UploadedDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
	if (pWnd->GetDlgCtrlID() == IDCE_URL) {
		return m_outputBkgnd;
	}
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}