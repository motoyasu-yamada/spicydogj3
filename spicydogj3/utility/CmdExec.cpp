/**
	概  要    ： DOS プログラム起動＆コンソール取り込みクラス
	ターゲット： Windows95/98/NT
	処理系    ： Visual C++ Ver 6.0
	作成者    ： MERCURY
	作成日    ： 99/11/22(月)
	著作権表記： Copyright(C) 1999 MERCURY.
*/

#include "stdafx.h"
#include "CmdExec.h"
#include <AfxTempl.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CCmdExec::CCmdExec() {
	m_bIsExec = FALSE;
	m_pThread = NULL;
}

CCmdExec::~CCmdExec() {
	ASSERT(!m_bIsExec);
}

BOOL CCmdExec::Start(const DOS_EXEC_PARAM* pParam) {
	if (m_bIsExec) {
		ASSERT(FALSE);
		return FALSE;
	}

	ASSERT(pParam != NULL);

	m_param      = *pParam;
	m_bIsExec    = TRUE;
	m_nState     = INVALID;
	m_hProcess   = NULL;
	m_bCrLfState = FALSE;
	m_pThread    = NULL;

	m_pThread = AfxBeginThread(ThreadExecDocsProc,(LPVOID)this);
	
	return TRUE;
}

void CCmdExec::WaitForTerm(DWORD mmsecs) {
	ASSERT(m_pThread != NULL);
	WaitForSingleObject(*m_pThread,mmsecs);
}

int CCmdExec::ExecChildProcess() {
	// SECURITY_ATTRIBUTE設定
	SECURITY_ATTRIBUTES securityAttr;
	securityAttr.nLength = sizeof( SECURITY_ATTRIBUTES );
	securityAttr.bInheritHandle = TRUE;
	securityAttr.lpSecurityDescriptor = NULL;

	// 標準出力を待避
	HANDLE hSaveStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hSaveStderr = GetStdHandle(STD_ERROR_HANDLE);
	// 子プロセスへのパイプをつくる。
	HANDLE hChildStdoutWriteEnd;
	HANDLE hChildStdoutReadEnd;
	if (!CreatePipe(&hChildStdoutReadEnd, &hChildStdoutWriteEnd, &securityAttr, 0)) 
	{
		return CANT_OPEN_PIPE;
	}

	// 子プロセス起動(ノンアクティブでアイコン状態で起動)
	PROCESS_INFORMATION procInfo;
	STARTUPINFO supInfo;
	ZeroMemory(&procInfo, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&supInfo, sizeof(STARTUPINFO));
	GetStartupInfo(&supInfo);
	/*
	if (m_param.m_bExecActive) 
	{
		switch(m_param.m_nWindowType) 
		{
		default:
		case WINDOW_NORMAL:				 // 通常起動
			ASSERT(m_param.m_nWindowType == WINDOW_NORMAL);
			supInfo.wShowWindow = SW_SHOWNORMAL;
			break;
		case WINDOW_ICONIC:				 // 最小化で起動
			supInfo.wShowWindow = SW_SHOWMINIMIZED;
			break;
		case WINDOW_ZOOMED:				 // 最大化で起動
			supInfo.wShowWindow = SW_SHOWMAXIMIZED;
			break;
		}
	} 
	else 
	{
		switch(m_param.m_nWindowType) 
		{
		default:
		case WINDOW_NORMAL:				 // 通常起動
			ASSERT(m_param.m_nWindowType == WINDOW_NORMAL);
			supInfo.wShowWindow = SW_SHOWNOACTIVATE;
			break;
		case WINDOW_ICONIC:				 // 最小化で起動
			supInfo.wShowWindow = SW_SHOWMINNOACTIVE;
			break;
		case WINDOW_ZOOMED:				 // 最大化で起動
			supInfo.wShowWindow = SW_SHOWMAXIMIZED;
			break;
		}
	}
	*/
	supInfo.wShowWindow = SW_SHOWDEFAULT;//SW_SHOWMINIMIZED;//SW_HIDE;//SW_SHOWDEFAULT;

	supInfo.dwFlags		= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	switch(m_param.m_nGetType) 
	{
	case GET_STDOUT:			// STDOUT のみを取り込む
		supInfo.hStdOutput	= hChildStdoutWriteEnd;
		break;
	case GET_STDERR:			// STDERR のみを取り込む
		supInfo.hStdError	= hChildStdoutWriteEnd;
		break;
	case GET_STDOUT_STDERR:		// STDOUT / STDERR の両方を取り込む
		supInfo.hStdOutput	= hChildStdoutWriteEnd;
		supInfo.hStdError	= hChildStdoutWriteEnd;
		break;
	}

	CString strDirectory = m_param.m_strDirectory;
	//	ディレクトリ名の最後は必ず '\\' にする
	if (strDirectory.Right(1) != _T("\\"))
		strDirectory += _T("\\");

	LPCTSTR ptszCmdln = m_param.m_strCommand;
	if (!CreateProcess(NULL,
					   (LPTSTR)ptszCmdln, 
					   NULL,
					   NULL,
					   TRUE,
					   0,
					   NULL,
					   strDirectory,
					   &supInfo,
					   &procInfo))
		return CANT_EXEC_CHILD;
	
	// このプロセスの標準出力を元に戻す。
	if (!SetStdHandle(STD_OUTPUT_HANDLE, hSaveStdout)) {
		return CANT_RESTORE_STDOUT;
	}
	if (!SetStdHandle(STD_ERROR_HANDLE, hSaveStderr)) {
		return CANT_RESTORE_STDERR;
	}
	// パイプから読み込む前に書き込み端を閉じる。
	if ( !CloseHandle( hChildStdoutWriteEnd ) ){
		return CANT_CLOSE_WRITE_END;
	}

	// プロセスＩＤを保存
	m_hProcess = procInfo.hProcess;

	// 読み込みのループ
#if defined(_UNICODE) || defined(UNICODE)
	#error not implemented
#else
	const int BUFSIZE = 0xFFFF;
	const int BUFUSESIZE = BUFSIZE - 1 /*\0*/ -1 /*followingbyte*/;
	CArray<BYTE,BYTE&> bufferHolder;
	bufferHolder.SetSize(BUFSIZE);
	
	BYTE* buffer = bufferHolder.GetData();
	for (;;) {
		DWORD dwThisRead = 0;
		if (ReadFile(hChildStdoutReadEnd,buffer,BUFUSESIZE,&dwThisRead, NULL) == 0 || 
			dwThisRead == 0) {
			break;
		}
		if (_istleadbyte(buffer[dwThisRead-1])) {
			DWORD dwRead;
			if (ReadFile(hChildStdoutReadEnd,&buffer[dwThisRead++],1,&dwRead,NULL) == 0 ||
				dwRead == 0){
				ASSERT(FALSE);
				break;
			}
		}
		buffer[dwThisRead] = _T('\0');
		Output((LPCTSTR)buffer);
	}
#endif

	TRACE("***exit***\n");
	GetExitCodeProcess(procInfo.hProcess, &m_dwReturnCode);
	return (EXEC_OK);
}

/**
	CCmdExec::ThreadExecDosProc
	概要  : ＤＯＳコマンド実行用スレッド
	引数  : m_pParam		= 管理情報
	戻り値: UINT			= 常に 0
*/
UINT CCmdExec::ThreadExecDocsProc(LPVOID pvTHIS)
{
	CCmdExec* pTHIS = (CCmdExec*)pvTHIS;
	ASSERT(pTHIS != NULL);
	return pTHIS->ThreadExecDos();
}

/**
	CCmdExec::ThreadExecDosProc
	概要  : DOSコマンド実行用スレッド
	戻り値: UINT			= 常に 0
*/
UINT CCmdExec::ThreadExecDos() {
	int nResult = ExecChildProcess();
	m_bIsExec = FALSE;
	switch(nResult) {
	case EXEC_OK:
		NotifyTerm(TRUE);
		break;
	case CANT_OPEN_PIPE:
	case CANT_EXEC_CHILD:
	case CANT_RESTORE_STDOUT:
	case CANT_CLOSE_WRITE_END:
		NotifyTerm(FALSE);
		break;
	default:
		ASSERT(FALSE);
		NotifyTerm(FALSE);
		break;
	}
	return(0);
}

BOOL CCmdExec::Stop(void) {
	if (IsExec() && m_hProcess) {
		TerminateProcess(m_hProcess, 0);
		return TRUE;
	}
	return FALSE;
}

BOOL CCmdExec::IsExec() const {
	return m_bIsExec;
}

const LPCTSTR OUTPUTEOL = _T("\r\n");
CString CCmdExec::ValidateEol(LPCTSTR ptsz) {
	ASSERT(AfxIsValidString(ptsz));
	CString str;
	if (m_bCrLfState) {
		if (*ptsz == '\n') {
			ptsz++;
		}
		m_bCrLfState = FALSE;
	}

	ASSERT(AfxIsValidString(ptsz));
	TCHAR tc;
	while ((tc = *ptsz++) != '\0') {
		if (isleadbyte(tc)) {
			str += tc;
			ASSERT(AfxIsValidString(ptsz));
			str += (tc = *ptsz++);
		} else if (tc == '\n') {
			str += OUTPUTEOL;
		} else if (tc == '\r') {
			ASSERT(AfxIsValidString(ptsz));
			TCHAR tcp = *ptsz;
			if (tcp == '\n') {
				ptsz++;
			} else if (tcp == '\0') {
				m_bCrLfState = TRUE;
			}
			str += OUTPUTEOL;
		} else {
			str += tc;
		}
		ASSERT(AfxIsValidString(ptsz));
	}
	return str;
}

void CCmdExec::Output(LPCTSTR ptsz) {
	ASSERT(AfxIsValidString(ptsz));
	if (m_param.m_pListener == NULL)
		return;
	ASSERT(AfxIsValidAddress(this,sizeof CCmdExec));
	ASSERT(AfxIsValidAddress(m_param.m_pListener,sizeof CCmdExecListener));
	m_param.m_pListener->OnOutput(ValidateEol(ptsz));
}

void CCmdExec::NotifyTerm(BOOL b) {
	ASSERT(m_param.m_pListener != NULL);
	if (m_param.m_pListener != NULL) {
		m_param.m_pListener->OnTerm(b);
	}
}