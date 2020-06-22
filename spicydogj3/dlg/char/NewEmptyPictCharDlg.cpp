#include "stdafx.h"
#include "resource.h"
#include "NewEmptyPictCharDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int DEFAULT_H = 32;
const int DEFAULT_W = 32;
const int MIN_H = 1;
const int MAX_H = 120;
const int MIN_W = 1;
const int MAX_W = 120;

BEGIN_MESSAGE_MAP(NewEmptyPictCharDlg, CDialog)
END_MESSAGE_MAP()

NewEmptyPictCharDlg::NewEmptyPictCharDlg(CWnd* pParent)
:	CDialog(IDD_NEW_EMPTY_PICT, pParent) {
	m_nH = DEFAULT_W;
	m_nW = DEFAULT_H;
}

void NewEmptyPictCharDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NewEmptyPictCharDlg)
	DDX_Control(pDX, IDCS_W, m_spinW);
	DDX_Control(pDX, IDCS_H, m_spinH);
	DDX_Text(pDX, IDCE_H, m_nH);
	DDX_Text(pDX, IDCE_W, m_nW);
	//}}AFX_DATA_MAP
	DDV_MinMaxInt(pDX, m_nW, MIN_W, MAX_W);
	DDV_MinMaxInt(pDX, m_nH, MIN_H, MAX_H);
}

BOOL NewEmptyPictCharDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_spinW.SetRange(MIN_W,MAX_W);
	m_spinH.SetRange(MIN_H,MAX_H);

	return TRUE;
}

void NewEmptyPictCharDlg::OnOK() 
{
	if (UpdateData(TRUE))	
		CDialog::OnOK();
}

CSize NewEmptyPictCharDlg::GetSize() const
{
	return CSize(m_nW,m_nH);
}
