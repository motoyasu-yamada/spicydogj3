#include "stdafx.h"
#include "resource.h"
#include "KeyFrameLabelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(KeyFrameLabelDlg, CDialog)
END_MESSAGE_MAP()

KeyFrameLabelDlg::KeyFrameLabelDlg(
	const Appli*         appli,
	const KeyFrameLabel* label,
	CWnd* parent
) : CDialog(IDD_KEY_FRAME_LABEL, parent) {
	m_appli    = appli;
	m_labelOrg = label;
	editLabel  = *label;
}

void KeyFrameLabelDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDCE_COMMENT,editLabel.labelComment);
	DDX_Text(pDX,IDCE_NAME,   editLabel.labelName);
	KeyFrameLabel::DDV_Label(pDX,m_appli,&editLabel,m_labelOrg->labelName);
}

BOOL KeyFrameLabelDlg::OnInitDialog(){
	CDialog::OnInitDialog();
	SendDlgItemMessage(IDCE_NAME,EM_SETSEL,0,-1);	
	return TRUE;
}

void KeyFrameLabelDlg::OnOK() {
	if (UpdateData(TRUE)) {
		CDialog::OnOK();
	}
}

void KeyFrameLabelDlg::OnCancel() {
	CDialog::OnCancel();
}


