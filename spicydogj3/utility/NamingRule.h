#pragma once

#if defined(_MT)
#include <afxmt.h>
#endif

/**
	付番号名前付け親オブジェクト
*/
class NamingRule {
private:
#if defined(_MT)
	mutable CCriticalSection m_cs;
#endif
	mutable LONG m_nNumber;
public:
	NamingRule();
	/// 新規名前を取得する @param nFmtID 名前フォーマット文字列リソース (例):"xxxx %d xxx"
	CString NewIncNumName(UINT nFmtID) const;
	/// 新規名前を取得 @param ptszFmt 名前フォーマット (例):"xxxx %d xxx"
	CString NewIncNumName(LPCTSTR ptszFmt) const;
	/// 複製した名前を取得
	CString DupIncNumName(LPCTSTR ptszOrg) const;
private:
	/// 新規名前番号を取得する
	LONG GetNewIncNumber() const;
};

