#include "stdafx.h"
#include "resource.h"
#include "PictResizeDlg.h"
#include "model/char/PictChar.h"
#include "utility/WndUtility.h"
#include "utility/ReenterLock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PictResizeDlg::PictResizeDlg(const LBitmapBase& bmp,CWnd* pParent /*=NULL*/)
:	CDialog(IDD_PICT_RESIZE, pParent),
	m_bmp(bmp),
	m_bWBase(TRUE),
	m_bReenter(FALSE),
	m_bInited(FALSE)
{
	m_nOrgW = const_cast<LBitmapBase&>(m_bmp).GetWidth();
	m_nOrgH = const_cast<LBitmapBase&>(m_bmp).GetHeight();

	//{{AFX_DATA_INIT(PictResizeDlg)
	m_nH = m_nOrgH;
	m_nW = m_nOrgW;
	//}}AFX_DATA_INIT
}


void PictResizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PictResizeDlg)
	DDX_Control(pDX, IDCS_W, m_spinW);
	DDX_Control(pDX, IDCS_H, m_spinH);
	DDX_Text(pDX, IDCE_H, m_nH);
	DDX_Text(pDX, IDCE_W, m_nW);
	//}}AFX_DATA_MAP
	DDV_MinMaxInt(pDX, m_nH,PICT_MIN_HEIGHT,PICT_MAX_HEIGHT);
	DDV_MinMaxInt(pDX, m_nW,PICT_MIN_WIDTH, PICT_MAX_WIDTH );
}

void PictResizeDlg::UpdateCtrls() {
	if (!m_bInited)
		return;

	m_nH = GetDlgItemInt(IDCE_H,NULL,FALSE);
	m_nW = GetDlgItemInt(IDCE_W,NULL,FALSE);

	BOOL bValid = TRUE;
	bValid &= PICT_MIN_HEIGHT <= m_nH && m_nH <= PICT_MAX_HEIGHT;
	bValid &= PICT_MIN_WIDTH  <= m_nW && m_nW <= PICT_MAX_WIDTH;
	bValid &= !(m_nOrgW == m_nW && m_nOrgH == m_nH);
	EnableDlgItem(this,IDOK,bValid);
}

BEGIN_MESSAGE_MAP(PictResizeDlg, CDialog)
	//{{AFX_MSG_MAP(PictResizeDlg)
	ON_EN_UPDATE(IDCE_H, OnUpdateH)
	ON_EN_UPDATE(IDCE_W, OnUpdateW)
	ON_BN_CLICKED(IDCC_KEEP, OnKeep)
	ON_BN_CLICKED(IDCB_RESET, OnReset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void PictResizeDlg::OnOK() {
	if (UpdateData(TRUE)) {
		CDialog::OnOK();
	}
}


BOOL PictResizeDlg::IsToKeepRatio() const {
	return IsDlgButtonChecked(IDCC_KEEP);
}

void PictResizeDlg::OnUpdateH() {
	m_bWBase = FALSE;

	if (IsToKeepRatio())
		KeepRatio();
	UpdateCtrls();
}

void PictResizeDlg::OnUpdateW() {
	m_bWBase = TRUE;
	if (IsToKeepRatio())
		KeepRatio();
	UpdateCtrls();
}

void PictResizeDlg::OnKeep() {
	if (IsToKeepRatio())
		KeepRatio();
	UpdateCtrls();
}

BOOL PictResizeDlg::OnInitDialog() {
	CDialog::OnInitDialog();
	
	CheckDlgButton(IDCC_KEEP,1);
	m_spinW.SetRange32(PICT_MIN_WIDTH, PICT_MAX_WIDTH);
	m_spinH.SetRange32(PICT_MIN_HEIGHT,PICT_MAX_HEIGHT);
	m_bInited = TRUE;
	UpdateCtrls();
	return TRUE;
}

void PictResizeDlg::KeepRatio() {
	if (!m_bInited) {
		return;
	}
	ReenterLock lock(m_bReenter);
	if (!lock.Lock()) {
		return;
	}

	m_nW = GetDlgItemInt(IDCE_W,NULL,FALSE);
	m_nH = GetDlgItemInt(IDCE_H,NULL,FALSE);

	if (m_bWBase) {
		if (m_nW == m_nOrgW) {
			m_nH = m_nOrgH;
		} else {
			m_nH = (m_nOrgH * m_nW) / m_nOrgW;
		}
		SetDlgItemInt(IDCE_H,m_nH,FALSE);
	} else {
		if (m_nH == m_nOrgH) {
			m_nW = m_nOrgW;
		} else {
			m_nW = (m_nOrgW * m_nH) / m_nOrgH;
		}
		SetDlgItemInt(IDCE_W,m_nW,FALSE);
	}
}

CSize PictResizeDlg::GetNewSize() const {
	return CSize(m_nW,m_nH);
}

void PictResizeDlg::OnReset() {
	m_nW = m_nOrgW;
	m_nH = m_nOrgH;
	UpdateData(FALSE);
	UpdateCtrls();
}
