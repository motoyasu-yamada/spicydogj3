#include "stdafx.h"
#include "resource.h"
#include "PictTrimDlg.h"
#include "model/char/PictChar.h"
#include "utility/WndUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PictTrimDlg::PictTrimDlg(int nW,int nH,CWnd* pParent)
	: CDialog(IDD_PICT_TRIM, pParent),m_bInited(FALSE) {
	m_nW     = nW;
	m_nH     = nH;
	m_nAlign = 4;
	m_nOrgW  = nW;
	m_nOrgH  = nH;
}

void PictTrimDlg::UpdateCtrls() {
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

void PictTrimDlg::GetTrimParam(int& nW,int& nH,int& nAlign) const {
	ASSERT(m_nW != m_nOrgW);
	ASSERT(m_nH != m_nOrgH);

	nW = m_nW;
	nH = m_nH;
	nAlign = m_nAlign;

	ASSERT(0 < nW);
	ASSERT(0 < nH);
	ASSERT(0 <= nAlign && nAlign < 9);
}

void PictTrimDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PictTrimDlg)
	DDX_Control(pDX, IDCS_W, m_spinW);
	DDX_Control(pDX, IDCS_H, m_spinH);
	DDX_Text(pDX, IDCE_H, m_nH);
	DDX_Text(pDX, IDCE_W, m_nW);
	DDX_Radio(pDX, IDCR_LT, m_nAlign);
	//}}AFX_DATA_MAP
	DDV_MinMaxInt(pDX, m_nH, PICT_MIN_HEIGHT,PICT_MAX_HEIGHT);
	DDV_MinMaxInt(pDX, m_nW, PICT_MIN_WIDTH, PICT_MAX_WIDTH);
}


BEGIN_MESSAGE_MAP(PictTrimDlg, CDialog)
	//{{AFX_MSG_MAP(PictTrimDlg)
	ON_BN_CLICKED(IDCB_RESET, OnReset)
	ON_EN_CHANGE(IDCE_H, OnUpdate)
	ON_BN_CLICKED(IDCR_T, OnUpdate)
	ON_BN_CLICKED(IDCR_RT, OnUpdate)
	ON_BN_CLICKED(IDCR_RB, OnUpdate)
	ON_BN_CLICKED(IDCR_R, OnUpdate)
	ON_BN_CLICKED(IDCR_LT, OnUpdate)
	ON_BN_CLICKED(IDCR_LB, OnUpdate)
	ON_BN_CLICKED(IDCR_L, OnUpdate)
	ON_BN_CLICKED(IDCR_C, OnUpdate)
	ON_BN_CLICKED(IDCR_B, OnUpdate)
	ON_EN_CHANGE(IDCE_W, OnUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void PictTrimDlg::OnOK() {
	if (UpdateData(TRUE)) {
		CDialog::OnOK();
	}
}

BOOL PictTrimDlg::OnInitDialog() {
	CDialog::OnInitDialog();
	
	m_spinW.SetRange32(PICT_MIN_WIDTH, PICT_MAX_WIDTH);
	m_spinH.SetRange32(PICT_MIN_HEIGHT,PICT_MAX_HEIGHT);

	SetButtonIcon(IDCR_LT,IDI_TRIM_LT);
	SetButtonIcon(IDCR_T, IDI_TRIM_T );
	SetButtonIcon(IDCR_RT,IDI_TRIM_RT);

	SetButtonIcon(IDCR_L, IDI_TRIM_L);
	SetButtonIcon(IDCR_C, IDI_TRIM_C);
	SetButtonIcon(IDCR_R, IDI_TRIM_R);

	SetButtonIcon(IDCR_LB,IDI_TRIM_LB);
	SetButtonIcon(IDCR_B, IDI_TRIM_B);
	SetButtonIcon(IDCR_RB,IDI_TRIM_RB);
	
	m_bInited = TRUE;

	UpdateCtrls();
	return TRUE;
}

void PictTrimDlg::SetButtonIcon(UINT nID,UINT nRes)
{
	HICON h = AfxGetApp()->LoadIcon(nRes);
	CButton* p = (CButton*) GetDlgItem(nID);
	if (h == NULL || p == NULL)
		AfxThrowResourceException();
	p->SetIcon(h);
}

void PictTrimDlg::OnUpdate() {
	UpdateCtrls();
}

void PictTrimDlg::OnReset() {
	m_nW = m_nOrgW;
	m_nH = m_nOrgH;
	UpdateData(FALSE);
	UpdateCtrls();
}
