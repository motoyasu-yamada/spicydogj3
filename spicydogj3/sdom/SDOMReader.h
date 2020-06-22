#pragma once

class CFile;
namespace xercesc_2_2 {
class DOMDocument;
};
using namespace xercesc_2_2;

/** �t�@�C����ǂݍ���XMLDOM���\�z���郊�[�_�[�@*/
class SDOMReader {
public:
	/** �ǂݍ��ނ��Ƃ��ł���t�@�C���`�����ǂ����H
		@param  �ǂݍ��ނ��Ƃ��ł��邩�ǂ������肷��t�@�C���̃|�C���^
		@return    true  �ǂݍ��ނ��Ƃ��ł���
	               false �ǂݍ��߂Ȃ�
	    @exception ��O�𓊂��Ă͂����Ȃ�
	*/
	virtual BOOL CanRead(CFile* p) = 0;
	/** �t�@�C����ǂݍ���DOM���\�z����
		@param  �ǂݍ��ރt�@�C��
		@return �t�@�C����ǂݍ��ݍ\�z����DOM
		@exception CFileException    �჌�x����IO��O
	               CArchiveException �t�@�C���̔j����
	*/
	virtual DOMDocument* Read(CFile* p) = 0;
};
