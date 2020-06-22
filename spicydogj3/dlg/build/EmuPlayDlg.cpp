#include "stdafx.h"
#include "resource.h"
#include "EmuPlayDlg.h"

#include "Jphone.h"
#include "model/build/Builder.h"
#include "SpicydogPref.h"
#include "utility/DDXUtility.h"
#include "utility/FileUtility.h"
#include "utility/WndUtility.h"
#include "utility/RegUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const UINT WM_USER_ON_TERM_EMUPLAY = WM_USER + 1000;
static const CString JAVA_REGPATH                 = _T("SOFTWARE\\JavaSoft\\Java Runtime Environment");
static const CString JAVA_VALNAME_CURRENT_VERSION = _T("CurrentVersion");
static const CString JAVA_VALNAME_JAVAHOME        = _T("JavaHome");
static const CString JAVA_RPATH_JAVAEXE           = _T("bin\\javaw.exe");
static const CString JAVA_RPATH_JAVAEXE_DEFAULT   = _T("javaw.exe");

BEGIN_MESSAGE_MAP(EmuPlayDlg, CDialog)
	ON_BN_CLICKED(IDCB_RUN,OnRun)
	ON_BN_CLICKED(IDCB_CLEAR,OnClear)
	ON_BN_CLICKED(IDCB_DETECT,OnDetect)
	ON_BN_CLICKED(IDCB_BROWSE,OnBrowse)
	ON_WM_CTLCOLOR()
	ON_MESSAGE_VOID(WM_USER_ON_TERM_EMUPLAY,OnEmuTerm)
END_MESSAGE_MAP()

EmuPlayDlg::EmuPlayDlg(LPCTSTR temp,Builder* builder,BOOL bImediate)
:	CDialog(IDD_EMU_PLAY) {
	m_temp    = temp;
	m_builder = builder;
	VERIFY(m_outputBkgnd.CreateStockObject(WHITE_BRUSH));
}

void EmuPlayDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control (pDX,IDCE_OUTPUT,  m_editOutput);
	DDX_Text    (pDX,IDCE_EMU_PATH,ACTIVE_PREF.EmuPath);
	DDXUtility::DDV_FilePath(pDX,ACTIVE_PREF.EmuPath,IDCE_EMU_PATH,IDS_ERR_INVALID_EMU_PATH);
}

void EmuPlayDlg::OnCancel() {
	if (IsRunning()) {
		ASSERT(FALSE);
		return;
	}
	DDXUtility::UpdateDataEx(this,TRUE);
	CDialog::OnCancel();
}

void EmuPlayDlg::OnChanged() {
	UpdateCtrlEx(IsRunning());
}

BOOL EmuPlayDlg::IsRunning() const {
	return m_exec.IsExec();
}

void EmuPlayDlg::OnOutput(LPCTSTR ptsz) {
	m_editOutput.SetSel(-1,0);
	m_editOutput.ReplaceSel(ptsz);
}

void EmuPlayDlg::OnEmuTerm() {
	UpdateCtrlEx(FALSE);
}

void EmuPlayDlg::OnTerm(BOOL b) {
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	PostMessage(WM_USER_ON_TERM_EMUPLAY);
}

void EmuPlayDlg::OnBrowse() {
	if (IsRunning()) {
		ASSERT(FALSE);
		return;
	}
	DDXUtility::UpdateDataEx(this,TRUE);
	if (Jphone::BrowseEmuPath(ACTIVE_PREF.EmuPath)) {
		UpdateData(FALSE);
	}
}

void EmuPlayDlg::OnDetect() {
	if (IsRunning()) {
		ASSERT(FALSE);
		return;
	}
	DDXUtility::UpdateDataEx(this,TRUE);
	if (Jphone::DetectEmuPath(ACTIVE_PREF.EmuPath)) {
		UpdateData(FALSE);
	} else {
		AfxMessageBox(IDS_ERR_CAN_NOT_DETECT_EMU_PATH,MB_OK | MB_ICONSTOP);
	}
}

HBRUSH EmuPlayDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
	if (pWnd->GetDlgCtrlID() == IDCE_OUTPUT) {
		return m_outputBkgnd;
	}
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void EmuPlayDlg::OnClear() {
	if (IsRunning()) {
		ASSERT(FALSE);
		return;
	}
	m_editOutput.SetWindowText(NULL);
}

void EmuPlayDlg::OnRun() {
	if (IsRunning()) {
		ASSERT(FALSE);
		return;
	}
	if (!UpdateData(TRUE)) {
		return;
	}
	Run();
}

void EmuPlayDlg::UpdateCtrlEx(BOOL running) {
	EnableDlgItem(this,IDCB_BROWSE,!running);
	EnableDlgItem(this,IDCB_DETECT,!running);
	EnableDlgItem(this,IDCB_CLEAR, !running);
	EnableDlgItem(this,IDCB_RUN,   !running);
	EnableDlgItem(this,IDCE_EMU_PATH,!running);
	EnableDlgItem(this,IDC_STATIC1,!running);
	EnableDlgItem(this,IDC_STATIC2,!running);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND | (!running ? MF_ENABLED : (MF_DISABLED | MF_GRAYED)));
}

BOOL EmuPlayDlg::OnInitDialog() {
	CDialog::OnInitDialog();
	UpdateData(FALSE);
	return TRUE;
}

CString GetJavaPath() {
	CString strCurrentVer;
	strCurrentVer = QueryRegValue(HKEY_LOCAL_MACHINE,JAVA_REGPATH,JAVA_VALNAME_CURRENT_VERSION,NULL);
	if (strCurrentVer.IsEmpty()) {
		ASSERT(FALSE);
		return (LPCTSTR)JAVA_RPATH_JAVAEXE_DEFAULT;
	}

	CString strJavaHome;
	strJavaHome = QueryRegValue(HKEY_LOCAL_MACHINE,
								JAVA_REGPATH + _T("\\") + strCurrentVer,
								JAVA_VALNAME_JAVAHOME,
								NULL);
	if (strJavaHome.IsEmpty()) {
		ASSERT(FALSE);
		return (LPCTSTR)JAVA_RPATH_JAVAEXE_DEFAULT;
	}
	return CatPath(strJavaHome,JAVA_RPATH_JAVAEXE);
}

void EmuPlayDlg::Run() {
	CString saved   = GetCurrentDirectory();
	VERIFY(SetCurrentDirectory(m_temp));
	CString jadPath = CatPath(m_temp,"sdp.jad");
	m_builder->GenerateJARFile(CatPath(m_temp,"sdp.jar"));	
	m_builder->GenerateADFFile(jadPath,"sdp.jar");
	VERIFY(SetCurrentDirectory(saved));

	CString command;
	command.Format("\"%s\" -jar \"%s\" -DirectExec \"%s\"",
		           GetJavaPath(),ACTIVE_PREF.EmuPath,jadPath);
	TRACE("%s\n",command);
	OnOutput(command + _T("\r\n"));

	DOS_EXEC_PARAM param;
	param.m_nGetType   = GET_STDOUT_STDERR;
	param.m_pListener  = this;
	param.m_strCommand = command;
	param.m_strDirectory = GetFileParent(ACTIVE_PREF.EmuPath);
	UpdateCtrlEx(TRUE);
	if (!m_exec.Start(&param)) {
		AfxMessageBox(IDS_ERR_CAN_NOT_RUN_EMU);
		UpdateCtrlEx(FALSE);
	}
}

