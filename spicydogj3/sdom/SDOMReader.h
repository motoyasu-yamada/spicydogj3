#pragma once

class CFile;
namespace xercesc_2_2 {
class DOMDocument;
};
using namespace xercesc_2_2;

/** ファイルを読み込みXMLDOMを構築するリーダー　*/
class SDOMReader {
public:
	/** 読み込むことができるファイル形式かどうか？
		@param  読み込むことができるかどうか判定するファイルのポインタ
		@return    true  読み込むことができる
	               false 読み込めない
	    @exception 例外を投げてはいけない
	*/
	virtual BOOL CanRead(CFile* p) = 0;
	/** ファイルを読み込みDOMを構築する
		@param  読み込むファイル
		@return ファイルを読み込み構築したDOM
		@exception CFileException    低レベルのIO例外
	               CArchiveException ファイルの破損等
	*/
	virtual DOMDocument* Read(CFile* p) = 0;
};
