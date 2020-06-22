#pragma once

class CFile;
namespace xercesc_2_2 {
class DOMDocument;
};

using namespace xercesc_2_2;

class SDOMWriter {
public:
	/** DOMをファイルに出力する
		@param file     出力先ファイル
		@param document 出力するドキュメント
		@exception CFileException 低レベルIO例外*/
	void Write(CFile* file,DOMDocument* document);
};
