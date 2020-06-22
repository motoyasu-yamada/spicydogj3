#pragma once

#include "Base64.h"

class CRFC1468 {
public:
	/** メール件名をRFC1468エンコードする */
	CString EncodeSubject(LPCSTR in);
	/** メール本文をRFC1468エンコードする */
	CString EncodeBody   (LPCSTR in);
private:
	CBase64 m_base64;
	/** SJIS文字列をJIS文字列に変換する */
	static int convertEncoding(LPCSTR str,CString& out);
	/** SMTPは.だけの行を終端と判断するので..にエスケープする */
	static CString escapeSingleDot(LPCTSTR in);
	/** jis文字列の部分文字列を切り出す */
	static BOOL    subjis  (CString& src,size_t required,CString& out);
	/** subjisのヘルパ関数 */
	static CString _copyjis(LPCTSTR& src,size_t len,size_t& rest);
	/** subjisのヘルパ関数 */
	static CString _addjis (LPCTSTR add,size_t& rest) ;
};
