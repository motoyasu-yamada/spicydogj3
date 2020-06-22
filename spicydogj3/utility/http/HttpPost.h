#pragma once

/**
	HTTPプロトコルのフォーム機能のPOSTを利用した
	ファイルアップロードヘルパー
*/
class HttpPost {
private:
	BOOL     m_bCommit;
	CMemFile m_fileData;
	BYTE*    m_pData;
	int      m_nDataLength;
public:
	HttpPost();
	virtual ~HttpPost();
	/** フォームデータの送信 */
	void AddFormData(LPCTSTR,LPCTSTR);
	/** フォームからファイルのアップロード */
	void AddFormFile(LPCTSTR,LPCTSTR,LPCTSTR,LPCTSTR);
	/** アップロード準備処理をコミットする */
	void Commit();
	CString GetHeader() const;
	/** 実データポインタを取得 */
	LPVOID GetData() const;
	/** 実データ長を取得 */
	int GetDataLength() const;
private:
	/** 区切り(BOUNDARY)を出力 */
	void AddBoundary();
	/** HTTPヘッダを取得 */
	virtual CString GetHeaderHelper() const;
	/**	MIMEブロックの区切りを取得 */
	virtual CString GetBoundaryHelper() const;
};
