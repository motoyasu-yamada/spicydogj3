#pragma once

/** エラーコードを文字列に変換する
	@param	dwErrorCode GetLastErrorで取得したエラーコード
	@return	エラー詳細
*/
CString FormatError(DWORD dwErrorCode);
/** クリップボードにテキストを設定する */
void SetClipText(LPCTSTR text);
/** 文字列を文字列により分割する */
void Split(LPCTSTR _src,TCHAR delimit,CStringArray& result);
/** 指定した書式でフォーマットする*/
CString FormatString(LPCTSTR format,...);
/** 指定した文字リソースを書式とみなしてフォーマットする*/
CString FormatString(UINT nID,...);
/** リソースから文字列をロード */
CString LoadString(UINT);
/**
	AfxGetResourceHandle()で返されるデフォルトリソースハンドルから
	指定されたタイプ、名前をもつリソースを
	文字列として読み込む。

	@param	lpName	リソース名
	@param	lpType	リソースタイプ
	@return	読込結果
*/
CString LoadStringResource(LPCTSTR lpName,LPCTSTR lpType = _T("TXT"));
/**
	AfxGetResourceHandle()で返されるデフォルトリソースハンドルから
	指定されたタイプ、名前をもつリソースを文字列として読み込む。

	@param	nID		リソース数値ID
	@param	lpType	リソースタイプ
	@return	読込結果
*/
CString LoadStringResource(UINT,LPCTSTR lpType = _T("TXT"));
/**
	デフォルト言語を利用して指定されたモジュール内の指定されたタイプ、
	名前をもつリソースを文字列として読み込む。

	@param	hMod	リソースを持っているモジュール
	@param	lpName	リソース名
	@param	lpType	リソースタイプ
	@return	読込結果
*/
CString LoadStringResource(HMODULE hMod,LPCTSTR lpType,LPCTSTR lpName);
/**
	指定されたモジュール内の指定されたタイプ、名前、言語をもつリソースを
	文字列として読み込む。

	@param	hMod	リソースを持っているモジュール
	@param	lpName	リソース名
	@param	lpType	リソースタイプ
	@return	読込結果
*/
CString LoadStringResourceEx(HMODULE hMod,LPCTSTR lpType,LPCTSTR lpName,WORD wLang);