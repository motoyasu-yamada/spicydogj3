#pragma once

#include "Base64.h"

class CRFC1468 {
public:
	/** ���[��������RFC1468�G���R�[�h���� */
	CString EncodeSubject(LPCSTR in);
	/** ���[���{����RFC1468�G���R�[�h���� */
	CString EncodeBody   (LPCSTR in);
private:
	CBase64 m_base64;
	/** SJIS�������JIS������ɕϊ����� */
	static int convertEncoding(LPCSTR str,CString& out);
	/** SMTP��.�����̍s���I�[�Ɣ��f����̂�..�ɃG�X�P�[�v���� */
	static CString escapeSingleDot(LPCTSTR in);
	/** jis������̕����������؂�o�� */
	static BOOL    subjis  (CString& src,size_t required,CString& out);
	/** subjis�̃w���p�֐� */
	static CString _copyjis(LPCTSTR& src,size_t len,size_t& rest);
	/** subjis�̃w���p�֐� */
	static CString _addjis (LPCTSTR add,size_t& rest) ;
};
