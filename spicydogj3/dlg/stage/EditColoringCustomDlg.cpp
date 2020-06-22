#include "stdafx.h"
#include "resource.h"
#include "EditColoringCustomDlg.h"

#include "dlg/stage/EditColoringDlg.h"
#include "model/stage/Coloring.h"
#include "utility/ReenterLock.h"
#include "utility/ChildDataExchange.h"
#include "utility/FileUtility.h"
#include "utility/DDXUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(EditColoringCustomDlg, CDialog)
	ON_EN_UPDATE(IDCE_BLUEADD, OnUpdateValue)
	ON_EN_UPDATE(IDCE_BLUEMULT, OnUpdateValue)
	ON_EN_UPDATE(IDCE_GREENADD, OnUpdateValue)
	ON_EN_UPDATE(IDCE_GREENMULT, OnUpdateValue)
	ON_EN_UPDATE(IDCE_REDADD, OnUpdateValue)
	ON_EN_UPDATE(IDCE_REDMULT, OnUpdateValue)
END_MESSAGE_MAP()

EditColoringCustomDlg::EditColoringCustomDlg(Coloring* ctform)
:	m_coloring(ctform),
	m_bInited(FALSE)
{
	ASSERT_POINTER(m_coloring,Coloring);
	ReenterLock::InitReenterLock(m_bUpdateLock);
}


void EditColoringCustomDlg::DoDataExchange(CDataExchange* pDX) {
	ReenterLock lock(m_bUpdateLock);
	if (!lock.Lock()) {
		return;
	}
	ASSERT_POINTER(&m_coloring,Coloring);

	ChildDataExchange cde(this,pDX);
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCS_REDMULT, m_spinRedMult);
	DDX_Control(pDX, IDCS_REDADD, m_spinRedAdd);
	DDX_Control(pDX, IDCS_GREENMULT, m_spinGreenMult);
	DDX_Control(pDX, IDCS_GREENADD, m_spinGreenAdd);
	DDX_Control(pDX, IDCS_BLUEMULT, m_spinBlueMult);
	DDX_Control(pDX, IDCS_BLUEADD, m_spinBlueAdd);
	DDX_Text(pDX, IDCE_REDADD,m_coloring->addRed);
	DDX_Text(pDX, IDCE_GREENADD,m_coloring->addGreen);
	DDX_Text(pDX, IDCE_BLUEADD,m_coloring->addBlue);
	DDXUtility::DDX_TextByPercent(pDX, IDCE_REDMULT,m_coloring->mulRed);
	DDXUtility::DDX_TextByPercent(pDX, IDCE_GREENMULT,m_coloring->mulGreen);
	DDXUtility::DDX_TextByPercent(pDX, IDCE_BLUEMULT,m_coloring->mulBlue);
	DDV_MinMaxInt(pDX,m_coloring->addRed,-255,255);
	DDV_MinMaxInt(pDX,m_coloring->addGreen,-255,255);
	DDV_MinMaxInt(pDX,m_coloring->addBlue,-255,255);
	DDXUtility::DDV_MinMaxPercent(pDX,m_coloring->mulRed,0,10,IDCE_REDMULT,IDS_ERR_INVALID_COLOR_MUL);
	DDXUtility::DDV_MinMaxPercent(pDX,m_coloring->mulGreen,0,10,IDCE_GREENMULT,IDS_ERR_INVALID_COLOR_MUL);
	DDXUtility::DDV_MinMaxPercent(pDX,m_coloring->mulBlue,0,10,IDCE_BLUEMULT,IDS_ERR_INVALID_COLOR_MUL);
}

BOOL EditColoringCustomDlg::OnInitDialog() {
	CDialog::OnInitDialog();
	
	m_spinRedMult.SetRange32(0,1000);
	m_spinGreenMult.SetRange32(0,1000);
	m_spinBlueMult.SetRange32(0,1000);
	m_spinRedAdd.SetRange32(-255,255);
	m_spinGreenAdd.SetRange32(-255,255);
	m_spinBlueAdd.SetRange32(-255,255);
	
	m_bInited = TRUE;

	return TRUE;
}

void EditColoringCustomDlg::OnUpdateValue()  {
	if (!m_bInited) {
		return;
	}
	ReenterLock lock(m_bUpdateLock);
	if (!lock.Lock()) {
		return;
	}

	UpdateData(TRUE);
	EditColoringDlg::Invalidate(this);

	lock.Unlock();
}

