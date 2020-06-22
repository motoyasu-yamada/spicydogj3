#pragma once

class CCmdExecListener;

/*
enum	
{
	WINDOW_NORMAL,					// 通常起動
	WINDOW_ICONIC,					// 最小化で起動
	WINDOW_ZOOMED,					// 最大化で起動
};
*/

enum	
{
	GET_STDOUT,						// STDOUT のみを取り込む
	GET_STDERR,						// STDERR のみを取り込む
	GET_STDOUT_STDERR,				// STDOUT / STDERR の両方を取り込む
};

/**
	CCmdExec の管理構造体
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

	int			m_nGetType;				// 取り込みタイプ(0:STDOUT / 1:STDERR / 2:STDOUTとSTDERR)
/*
	int			m_nWindowType;			// 実行時のウィンドウ状態(WINDOW_xxxxx)
	BOOL		m_bExecActive;			// 実行時にアクティブにするか？
*/
	CString		m_strCommand;			// 実行コマンド
	CString		m_strDirectory;			// カレントディレクトリ
	CCmdExecListener* m_pListener;
};

/**
	コマンドライン実行
*/
class	CCmdExec	
{
public:
	// 列挙型の定義
	enum	
	{									// 実行ステータス
		INVALID = -1,					// 無効
		EXEC_OK = 0,					// 正常終了
		CANT_OPEN_PIPE,					// パイプの作成に失敗
		CANT_EXEC_CHILD,				// 
		CANT_RESTORE_STDOUT,			// 標準コンソール出力のリストアに失敗
		CANT_RESTORE_STDERR,			// 標準エラー出力のリストアに失敗
		CANT_CLOSE_WRITE_END,			// 
	};

	BOOL	m_bIsExec;				// スレッドの起動フラグ
	DWORD	m_dwReturnCode;			// リターンコード
	int		m_nState;				// 実行後の情報
	HANDLE	m_hProcess;				// 実行中のプロセスのプロセスハンドル
	DOS_EXEC_PARAM m_param;
	BOOL    m_bCrLfState;
	CWinThread* m_pThread;

public:
	CCmdExec();
	virtual	~CCmdExec();
	/*  概要  : コマンドを実行する
		引数  : m_pParam		= 管理情報
		戻り値: BOOL			= ture:成功 / false:失敗 */
	BOOL Start(const DOS_EXEC_PARAM*);
	BOOL Stop();
	BOOL IsExec() const;
	void WaitForTerm(DWORD);
	
private:
	UINT ThreadExecDos();
	/** 概要  : 子プロセスを実行します
		@param  m_pParam 管理情報
		@return int      実行結果 */
	int  ExecChildProcess();
	
	void NotifyTerm(BOOL);
	void Output(LPCTSTR);
	CString ValidateEol(LPCTSTR);
	static UINT ThreadExecDocsProc(LPVOID);
}; // class CDocExec

/**
	CCmdExecListener
	標準出力/標準エラー出力結果の横取りリスナー
*/
class CCmdExecListener
{
public:
	virtual void OnOutput(LPCTSTR ptszMsg) = 0;
	virtual void OnTerm(BOOL) = 0;
	~ CCmdExecListener() {}
}; // class CDocExecListener

