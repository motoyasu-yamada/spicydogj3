#include "stdafx.h"
#include "resource.h"
#include "SpicydogPref.h"
#include "GridEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(GridEditDlg, CDialog)
	//{{AFX_MSG_MAP(GridEditDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

GridEditDlg::GridEditDlg(CWnd* pParent)
:	CDialog(IDD_GRID_EDIT, pParent) {
}


void GridEditDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDCB_PIXEL_GRID_COLOR, m_btnPixelGridColor);
	DDX_Control(pDX, IDCB_GRID_COLOR, m_btnGridColor);
	DDX_Control(pDX, IDCB_CENTER_GRID_COLOR, m_btnCenterGridColor);
	DDX_Control(pDX, IDCS_GRID_INTERVAL, m_spinGridInterval);

	if (pDX->m_bSaveAndValidate) {
		ACTIVE_PREF.DisplayGrid  = IsDlgButtonChecked(IDCC_DISPLAY_GRID) == 1;
		ACTIVE_PREF.GridInterval = m_spinGridInterval.GetPos();
		ACTIVE_PREF.GridColor    = m_btnGridColor.GetColor();
		ACTIVE_PREF.DisplayCenterGrid = IsDlgButtonChecked(IDCC_DISPLAY_CENTER_GRID) == 1;
		ACTIVE_PREF.CenterGridColor   = m_btnCenterGridColor.GetColor();
		ACTIVE_PREF.DisplayPixelGrid  = IsDlgButtonChecked(IDCC_DISPLAY_PIXEL_GRID) == 1;
		ACTIVE_PREF.PixelGridColor    = m_btnPixelGridColor.GetColor();
	} else {
		CheckDlgButton(IDCC_DISPLAY_GRID,ACTIVE_PREF.DisplayGrid ? 1 : 0);
		m_spinGridInterval.SetPos(ACTIVE_PREF.GridInterval);
		m_btnGridColor.SetColor(ACTIVE_PREF.GridColor);	
		CheckDlgButton(IDCC_DISPLAY_CENTER_GRID,ACTIVE_PREF.DisplayCenterGrid ? 1 : 0);
		m_btnCenterGridColor.SetColor(ACTIVE_PREF.CenterGridColor);
		CheckDlgButton(IDCC_DISPLAY_PIXEL_GRID,ACTIVE_PREF.DisplayPixelGrid ? 1 : 0);
		m_btnPixelGridColor.SetColor(ACTIVE_PREF.PixelGridColor);
	}
}

void GridEditDlg::OnOK() 
{
	CDialog::OnOK();
}

BOOL GridEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_spinGridInterval.SetRange(2,32);
	return TRUE;
}
