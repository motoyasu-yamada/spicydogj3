#pragma once

class CCmdExecListener;

/*
enum	
{
	WINDOW_NORMAL,					// �ʏ�N��
	WINDOW_ICONIC,					// �ŏ����ŋN��
	WINDOW_ZOOMED,					// �ő剻�ŋN��
};
*/

enum	
{
	GET_STDOUT,						// STDOUT �݂̂���荞��
	GET_STDERR,						// STDERR �݂̂���荞��
	GET_STDOUT_STDERR,				// STDOUT / STDERR �̗�������荞��
};

/**
	CCmdExec �̊Ǘ��\����
*/
struct	DOS_EXEC_PARAM	
{
	DOS_EXEC_PARAM()
	:	m_nGetType(GET_STDOUT_STDERR),
		//m_nWindowType(WINDOW_NORMAL),
		//m_bExecActive(TRUE),
		m_pListener(NULL)
	{
	}

	int			m_nGetType;				// ��荞�݃^�C�v(0:STDOUT / 1:STDERR / 2:STDOUT��STDERR)
/*
	int			m_nWindowType;			// ���s���̃E�B���h�E���(WINDOW_xxxxx)
	BOOL		m_bExecActive;			// ���s���ɃA�N�e�B�u�ɂ��邩�H
*/
	CString		m_strCommand;			// ���s�R�}���h
	CString		m_strDirectory;			// �J�����g�f�B���N�g��
	CCmdExecListener* m_pListener;
};

/**
	�R�}���h���C�����s
*/
class	CCmdExec	
{
public:
	// �񋓌^�̒�`
	enum	
	{									// ���s�X�e�[�^�X
		INVALID = -1,					// ����
		EXEC_OK = 0,					// ����I��
		CANT_OPEN_PIPE,					// �p�C�v�̍쐬�Ɏ��s
		CANT_EXEC_CHILD,				// 
		CANT_RESTORE_STDOUT,			// �W���R���\�[���o�͂̃��X�g�A�Ɏ��s
		CANT_RESTORE_STDERR,			// �W���G���[�o�͂̃��X�g�A�Ɏ��s
		CANT_CLOSE_WRITE_END,			// 
	};

	BOOL	m_bIsExec;				// �X���b�h�̋N���t���O
	DWORD	m_dwReturnCode;			// ���^�[���R�[�h
	int		m_nState;				// ���s��̏��
	HANDLE	m_hProcess;				// ���s���̃v���Z�X�̃v���Z�X�n���h��
	DOS_EXEC_PARAM m_param;
	BOOL    m_bCrLfState;
	CWinThread* m_pThread;

public:
	CCmdExec();
	virtual	~CCmdExec();
	/*  �T�v  : �R�}���h�����s����
		����  : m_pParam		= �Ǘ����
		�߂�l: BOOL			= ture:���� / false:���s */
	BOOL Start(const DOS_EXEC_PARAM*);
	BOOL Stop();
	BOOL IsExec() const;
	void WaitForTerm(DWORD);
	
private:
	UINT ThreadExecDos();
	/** �T�v  : �q�v���Z�X�����s���܂�
		@param  m_pParam �Ǘ����
		@return int      ���s���� */
	int  ExecChildProcess();
	
	void NotifyTerm(BOOL);
	void Output(LPCTSTR);
	CString ValidateEol(LPCTSTR);
	static UINT ThreadExecDocsProc(LPVOID);
}; // class CDocExec

/**
	CCmdExecListener
	�W���o��/�W���G���[�o�͌��ʂ̉���胊�X�i�[
*/
class CCmdExecListener
{
public:
	virtual void OnOutput(LPCTSTR ptszMsg) = 0;
	virtual void OnTerm(BOOL) = 0;
	~ CCmdExecListener() {}
}; // class CDocExecListener

