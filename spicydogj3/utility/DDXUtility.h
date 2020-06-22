#pragma once

class DDXUtility {
public:
	/** UTF8������Ƃ��ĕ�������]������ */
	static void DDV_UTF8TextLength(CDataExchange* pDX,CString& sjis,int minLength,int maxLength,
		                           UINT ctrl,UINT errmsg);
	/**	�����񒷂͈͔̔��� */
	static void DDV_TextLength(CDataExchange* pDX,CString& str1,int nMinLen,int nMaxLen,
		                       UINT ctrl,UINT errmsg);
	/** ���͂���Ă���p�X���������t�@�C���p�X���ǂ��� */
	static void DDV_FilePath(CDataExchange* pDX,CString& path,UINT ctrl,UINT errmsg);
	/**	EMAIL�Ƃ��đÓ����ǂ������� */
	static void DDV_Email(CDataExchange* pDX,CString& str1,BOOL required,UINT ctrl,UINT errmsg);
	/** �J���}�ŋ�؂������[�����X�g */
	static void DDV_Emails(CDataExchange* pDX,CString& str1,BOOL required,UINT ctrl,UINT errmsg);
	/** �z�X�g���̑Ó������� */
	static void DDV_Host  (CDataExchange* pDX,CString& str1,BOOL required,UINT ctrl,UINT errmsg);
	/**	HTTP/HTTPS URL �Ƃ��Đ��������ǂ��� */
	static void DDV_HttpURL(CDataExchange* pDX,CString& str1,BOOL required,UINT ctrl,UINT errmsg);
	/** ���͂����e�L�X�g�̃p�[�Z���g�l���Ó��Ȕ͈͂��ǂ��� */
	static void DDV_MinMaxPercent(CDataExchange*,double,double,double,UINT ctrl,UINT errmsg);
	/** ���͂����e�L�X�g���p�[�Z���g�l�Ƃ��đÓ����ǂ��� */
	static void DDX_TextByPercent(CDataExchange* pDX, int nIDC, double& value);
	/** �R���{�{�b�N�X */
	static void DDX_CBValue(
		CDataExchange* pDX,
		CComboBox& combo,
		CString& value,
		const std::list<CString>& keys,
		const std::list<CString>& names);

	/** UpdateData�����s���邪�A�G���[���������Ă��x���_�C�A���O���J���Ȃ� */
	static BOOL UpdateDataEx(CWnd* wnd,BOOL bSaveAndValidate);

private:
	static BOOL SLIENT_DATA_EXCHANGE;
	static void Fail(CDataExchange* pDX,UINT ctrl,UINT errmsg);
	static void MessageBox(CDataExchange* pDX,UINT message);
	static void MessageBox(CDataExchange* pDX,LPCTSTR errmsg);
	static void ActivateCtrl(CDataExchange* pDX,UINT ctrl);
};