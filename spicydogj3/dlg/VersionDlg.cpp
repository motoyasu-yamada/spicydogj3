#include "stdafx.h"
#include "resource.h"
#include "VersionDlg.h"
#include "utility/FileUtility.h"
#include "utility/StringUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(VersionDlg, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_COMMAND_EX(ID_WWW_JPHONE,OnWWW)
	ON_COMMAND_EX(ID_WWW_SPICYSOFT,OnWWW)
	ON_COMMAND_EX(ID_WWW_SPICYDOG,OnWWW)
END_MESSAGE_MAP()

const COLORREF VERSIONDLG_BKGND = RGB(0xFF,0xFF,0xFF);

VersionDlg::VersionDlg() 
:	CDialog(IDD_VERSION)
{
	VERIFY(m_brBkgnd.CreateSolidBrush(VERSIONDLG_BKGND));
	m_strVersion = GetFileVersion(GetAppExePath());
	m_strRemarks = LoadStringResource(IDT_LICENSE_REMARKS);
	m_strStaff   = LoadStringResource(IDT_STAFF);
}

void VersionDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, ID_WWW_JPHONE,    m_bsWwwJphone);
	DDX_Control(pDX, ID_WWW_SPICYSOFT, m_bsWwwSpicysoft);
	DDX_Control(pDX, ID_WWW_SPICYDOG,  m_bsWwwSpicyDog);
	DDX_Text(pDX, IDCS_VERSION, m_strVersion);
	DDX_Text(pDX, IDCE_REMARKS, m_strRemarks);
	DDX_Text(pDX, IDCE_STAFF, m_strStaff);
}

void VersionDlg::OnWWW(UINT id) {
	VERIFY(OpenURL(LoadString(id)));
}

BOOL VersionDlg::OnEraseBkgnd(CDC* pDC) {
	CRect rc;
	GetClientRect(rc);
	pDC->FillRect(rc,&m_brBkgnd);
	return TRUE;
}

HBRUSH VersionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
	return m_brBkgnd;
}

BOOL VersionDlg::OnInitDialog() {
	CDialog::OnInitDialog();
	
	m_bsWwwSpicyDog. Setup(IDC_POINTER,IDB_BN_SPICYDOG);
	m_bsWwwSpicysoft.Setup(IDC_POINTER,IDB_BN_SPICYSOFT);
	m_bsWwwJphone.   Setup(IDC_POINTER,IDB_BN_JPHONE);

	return TRUE;
}
