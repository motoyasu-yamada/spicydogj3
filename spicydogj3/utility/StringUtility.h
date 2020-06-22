#pragma once

/** �G���[�R�[�h�𕶎���ɕϊ�����
	@param	dwErrorCode GetLastError�Ŏ擾�����G���[�R�[�h
	@return	�G���[�ڍ�
*/
CString FormatError(DWORD dwErrorCode);
/** �N���b�v�{�[�h�Ƀe�L�X�g��ݒ肷�� */
void SetClipText(LPCTSTR text);
/** ������𕶎���ɂ�蕪������ */
void Split(LPCTSTR _src,TCHAR delimit,CStringArray& result);
/** �w�肵�������Ńt�H�[�}�b�g����*/
CString FormatString(LPCTSTR format,...);
/** �w�肵���������\�[�X�������Ƃ݂Ȃ��ăt�H�[�}�b�g����*/
CString FormatString(UINT nID,...);
/** ���\�[�X���當��������[�h */
CString LoadString(UINT);
/**
	AfxGetResourceHandle()�ŕԂ����f�t�H���g���\�[�X�n���h������
	�w�肳�ꂽ�^�C�v�A���O�������\�[�X��
	������Ƃ��ēǂݍ��ށB

	@param	lpName	���\�[�X��
	@param	lpType	���\�[�X�^�C�v
	@return	�Ǎ�����
*/
CString LoadStringResource(LPCTSTR lpName,LPCTSTR lpType = _T("TXT"));
/**
	AfxGetResourceHandle()�ŕԂ����f�t�H���g���\�[�X�n���h������
	�w�肳�ꂽ�^�C�v�A���O�������\�[�X�𕶎���Ƃ��ēǂݍ��ށB

	@param	nID		���\�[�X���lID
	@param	lpType	���\�[�X�^�C�v
	@return	�Ǎ�����
*/
CString LoadStringResource(UINT,LPCTSTR lpType = _T("TXT"));
/**
	�f�t�H���g����𗘗p���Ďw�肳�ꂽ���W���[�����̎w�肳�ꂽ�^�C�v�A
	���O�������\�[�X�𕶎���Ƃ��ēǂݍ��ށB

	@param	hMod	���\�[�X�������Ă��郂�W���[��
	@param	lpName	���\�[�X��
	@param	lpType	���\�[�X�^�C�v
	@return	�Ǎ�����
*/
CString LoadStringResource(HMODULE hMod,LPCTSTR lpType,LPCTSTR lpName);
/**
	�w�肳�ꂽ���W���[�����̎w�肳�ꂽ�^�C�v�A���O�A����������\�[�X��
	������Ƃ��ēǂݍ��ށB

	@param	hMod	���\�[�X�������Ă��郂�W���[��
	@param	lpName	���\�[�X��
	@param	lpType	���\�[�X�^�C�v
	@return	�Ǎ�����
*/
CString LoadStringResourceEx(HMODULE hMod,LPCTSTR lpType,LPCTSTR lpName,WORD wLang);