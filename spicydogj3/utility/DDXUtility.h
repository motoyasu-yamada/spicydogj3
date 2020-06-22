#pragma once

class DDXUtility {
public:
	/** UTF8文字列として文字長を評価する */
	static void DDV_UTF8TextLength(CDataExchange* pDX,CString& sjis,int minLength,int maxLength,
		                           UINT ctrl,UINT errmsg);
	/**	文字列長の範囲判定 */
	static void DDV_TextLength(CDataExchange* pDX,CString& str1,int nMinLen,int nMaxLen,
		                       UINT ctrl,UINT errmsg);
	/** 入力されているパスが正しいファイルパスかどうか */
	static void DDV_FilePath(CDataExchange* pDX,CString& path,UINT ctrl,UINT errmsg);
	/**	EMAILとして妥当かどうか判定 */
	static void DDV_Email(CDataExchange* pDX,CString& str1,BOOL required,UINT ctrl,UINT errmsg);
	/** カンマで区切ったメールリスト */
	static void DDV_Emails(CDataExchange* pDX,CString& str1,BOOL required,UINT ctrl,UINT errmsg);
	/** ホスト名の妥当性判定 */
	static void DDV_Host  (CDataExchange* pDX,CString& str1,BOOL required,UINT ctrl,UINT errmsg);
	/**	HTTP/HTTPS URL として正しいかどうか */
	static void DDV_HttpURL(CDataExchange* pDX,CString& str1,BOOL required,UINT ctrl,UINT errmsg);
	/** 入力したテキストのパーセント値が妥当な範囲かどうか */
	static void DDV_MinMaxPercent(CDataExchange*,double,double,double,UINT ctrl,UINT errmsg);
	/** 入力したテキストがパーセント値として妥当かどうか */
	static void DDX_TextByPercent(CDataExchange* pDX, int nIDC, double& value);
	/** コンボボックス */
	static void DDX_CBValue(
		CDataExchange* pDX,
		CComboBox& combo,
		CString& value,
		const std::list<CString>& keys,
		const std::list<CString>& names);

	/** UpdateDataを実行するが、エラーが発生しても警告ダイアログを開かない */
	static BOOL UpdateDataEx(CWnd* wnd,BOOL bSaveAndValidate);

private:
	static BOOL SLIENT_DATA_EXCHANGE;
	static void Fail(CDataExchange* pDX,UINT ctrl,UINT errmsg);
	static void MessageBox(CDataExchange* pDX,UINT message);
	static void MessageBox(CDataExchange* pDX,LPCTSTR errmsg);
	static void ActivateCtrl(CDataExchange* pDX,UINT ctrl);
};