#include "stdafx.h"
#include "resource.h"
#include "UploadingDlg.h"
#include "utility/StringUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(UploadingDlg,CDialog)
END_MESSAGE_MAP()

UploadingDlg::UploadingDlg() :
	CDialog() {
}

void UploadingDlg::Create() {
	VERIFY(CDialog::Create(IDD_UPLOADING));
	ShowWindow(SW_SHOW);
	UpdateWindow();
	CenterWindow();
	SetProgress(IDS_PREPAREING);
}

void UploadingDlg::SetProgress(UINT msg) {
	SetDlgItemText(IDCS_PROGRESS,LoadString(msg));
	UpdateWindow();
}
