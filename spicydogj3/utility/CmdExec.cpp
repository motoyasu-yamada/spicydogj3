/**
	�T  �v    �F DOS �v���O�����N�����R���\�[����荞�݃N���X
	�^�[�Q�b�g�F Windows95/98/NT
	�����n    �F Visual C++ Ver 6.0
	�쐬��    �F MERCURY
	�쐬��    �F 99/11/22(��)
	���쌠�\�L�F Copyright(C) 1999 MERCURY.
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
	// SECURITY_ATTRIBUTE�ݒ�
	SECURITY_ATTRIBUTES securityAttr;
	securityAttr.nLength = sizeof( SECURITY_ATTRIBUTES );
	securityAttr.bInheritHandle = TRUE;
	securityAttr.lpSecurityDescriptor = NULL;

	// �W���o�͂�Ҕ�
	HANDLE hSaveStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hSaveStderr = GetStdHandle(STD_ERROR_HANDLE);
	// �q�v���Z�X�ւ̃p�C�v������B
	HANDLE hChildStdoutWriteEnd;
	HANDLE hChildStdoutReadEnd;
	if (!CreatePipe(&hChildStdoutReadEnd, &hChildStdoutWriteEnd, &securityAttr, 0)) 
	{
		return CANT_OPEN_PIPE;
	}

	// �q�v���Z�X�N��(�m���A�N�e�B�u�ŃA�C�R����ԂŋN��)
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
		case WINDOW_NORMAL:				 // �ʏ�N��
			ASSERT(m_param.m_nWindowType == WINDOW_NORMAL);
			supInfo.wShowWindow = SW_SHOWNORMAL;
			break;
		case WINDOW_ICONIC:				 // �ŏ����ŋN��
			supInfo.wShowWindow = SW_SHOWMINIMIZED;
			break;
		case WINDOW_ZOOMED:				 // �ő剻�ŋN��
			supInfo.wShowWindow = SW_SHOWMAXIMIZED;
			break;
		}
	} 
	else 
	{
		switch(m_param.m_nWindowType) 
		{
		default:
		case WINDOW_NORMAL:				 // �ʏ�N��
			ASSERT(m_param.m_nWindowType == WINDOW_NORMAL);
			supInfo.wShowWindow = SW_SHOWNOACTIVATE;
			break;
		case WINDOW_ICONIC:				 // �ŏ����ŋN��
			supInfo.wShowWindow = SW_SHOWMINNOACTIVE;
			break;
		case WINDOW_ZOOMED:				 // �ő剻�ŋN��
			supInfo.wShowWindow = SW_SHOWMAXIMIZED;
			break;
		}
	}
	*/
	supInfo.wShowWindow = SW_SHOWDEFAULT;//SW_SHOWMINIMIZED;//SW_HIDE;//SW_SHOWDEFAULT;

	supInfo.dwFlags		= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	switch(m_param.m_nGetType) 
	{
	case GET_STDOUT:			// STDOUT �݂̂���荞��
		supInfo.hStdOutput	= hChildStdoutWriteEnd;
		break;
	case GET_STDERR:			// STDERR �݂̂���荞��
		supInfo.hStdError	= hChildStdoutWriteEnd;
		break;
	case GET_STDOUT_STDERR:		// STDOUT / STDERR �̗�������荞��
		supInfo.hStdOutput	= hChildStdoutWriteEnd;
		supInfo.hStdError	= hChildStdoutWriteEnd;
		break;
	}

	CString strDirectory = m_param.m_strDirectory;
	//	�f�B���N�g�����̍Ō�͕K�� '\\' �ɂ���
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
	
	// ���̃v���Z�X�̕W���o�͂����ɖ߂��B
	if (!SetStdHandle(STD_OUTPUT_HANDLE, hSaveStdout)) {
		return CANT_RESTORE_STDOUT;
	}
	if (!SetStdHandle(STD_ERROR_HANDLE, hSaveStderr)) {
		return CANT_RESTORE_STDERR;
	}
	// �p�C�v����ǂݍ��ޑO�ɏ������ݒ[�����B
	if ( !CloseHandle( hChildStdoutWriteEnd ) ){
		return CANT_CLOSE_WRITE_END;
	}

	// �v���Z�X�h�c��ۑ�
	m_hProcess = procInfo.hProcess;

	// �ǂݍ��݂̃��[�v
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
	�T�v  : �c�n�r�R�}���h���s�p�X���b�h
	����  : m_pParam		= �Ǘ����
	�߂�l: UINT			= ��� 0
*/
UINT CCmdExec::ThreadExecDocsProc(LPVOID pvTHIS)
{
	CCmdExec* pTHIS = (CCmdExec*)pvTHIS;
	ASSERT(pTHIS != NULL);
	return pTHIS->ThreadExecDos();
}

/**
	CCmdExec::ThreadExecDosProc
	�T�v  : DOS�R�}���h���s�p�X���b�h
	�߂�l: UINT			= ��� 0
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