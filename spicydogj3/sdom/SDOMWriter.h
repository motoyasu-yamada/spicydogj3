#pragma once

class CFile;
namespace xercesc_2_2 {
class DOMDocument;
};

using namespace xercesc_2_2;

class SDOMWriter {
public:
	/** DOM���t�@�C���ɏo�͂���
		@param file     �o�͐�t�@�C��
		@param document �o�͂���h�L�������g
		@exception CFileException �჌�x��IO��O*/
	void Write(CFile* file,DOMDocument* document);
};
