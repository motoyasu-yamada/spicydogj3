#include "stdafx.h"
#include "resource.h"
#include "model/stage/Placing.h"
#include "EditPlacingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//	•ÒW‰Â”\’l”ÍˆÍ
const int ROTATEOFFSET_MAX	= +360;
const int ROTATEOFFSET_MIN	= -360;
const int SCALE_MAX			= 10000;
const int SCALE_MIN			= 1;
const int TRANSLATE_MAX		= +256;
const int TRANSLATE_MIN		= -256;

BEGIN_MESSAGE_MAP(EditPlacingDlg, CDialog)
	//{{AFX_MSG_MAP(EditPlacingDlg)
	ON_MESSAGE_VOID(WM_KICKIDLE,OnKickIdle)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDCS_SIZING1,OnUpdateSizing)
	ON_UPDATE_COMMAND_UI(IDCS_SIZING2,OnUpdateSizing)
	ON_UPDATE_COMMAND_UI(IDCS_SIZING3,OnUpdateSizing)
	ON_UPDATE_COMMAND_UI(IDCS_SIZING4,OnUpdateSizing)
	ON_UPDATE_COMMAND_UI(IDCS_SIZING5,OnUpdateSizing)
	ON_UPDATE_COMMAND_UI(IDCS_SIZING6,OnUpdateSizing)
	ON_UPDATE_COMMAND_UI(IDCS_SIZING7,OnUpdateSizing)
	ON_UPDATE_COMMAND_UI(IDCS_SIZING8,OnUpdateSizing)
	ON_UPDATE_COMMAND_UI(IDCE_TRANSLATEX,OnUpdateTranslate)
	ON_UPDATE_COMMAND_UI(IDCE_TRANSLATEY,OnUpdateTranslate)
	ON_UPDATE_COMMAND_UI(IDCS_TRANSLATEX,OnUpdateTranslate)
	ON_UPDATE_COMMAND_UI(IDCS_TRANSLATEY,OnUpdateTranslate)	
END_MESSAGE_MAP()

EditPlacingDlg::EditPlacingDlg(BOOL bSizing,Placing& frame,CWnd* pParent /*=NULL*/)
:	CDialog(IDD_EDIT_PLACING, pParent),
	m_frame(frame),
	m_sizing(bSizing)
{
	m_x	= m_frame.x;
	m_y	= m_frame.y;
}

void EditPlacingDlg::OnUpdateTranslate(CCmdUI* pcmd) {
	pcmd->Enable(TRUE);
}

void EditPlacingDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCS_TRANSLATEY,	m_spinY);
	DDX_Control(pDX, IDCS_TRANSLATEX,	m_spinX);
	DDX_Text(pDX, IDCE_TRANSLATEX, m_x);
	DDX_Text(pDX, IDCE_TRANSLATEY, m_y);
	DDV_MinMaxInt(pDX, m_x,	TRANSLATE_MIN,TRANSLATE_MAX);
	DDV_MinMaxInt(pDX, m_y,	TRANSLATE_MIN,TRANSLATE_MAX);
}

void EditPlacingDlg::OnUpdateSizing(CCmdUI* pcmd) {
	pcmd->Enable(m_sizing);
}

void EditPlacingDlg::OnKickIdle()
{
	UpdateDialogControls(this,FALSE);
}

void EditPlacingDlg::OnOK()  {
	UpdateData(TRUE);

	m_frame.x	= m_x;
	m_frame.y	= m_y;

	CDialog::OnOK();
}

BOOL EditPlacingDlg::OnInitDialog()  {
	CDialog::OnInitDialog();
	
	m_spinX.SetRange32(TRANSLATE_MIN,TRANSLATE_MAX);
	m_spinY.SetRange32(TRANSLATE_MIN,TRANSLATE_MAX);

	return TRUE;
}
