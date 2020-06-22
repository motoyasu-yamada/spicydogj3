#pragma once

/**
	HTTP�v���g�R���̃t�H�[���@�\��POST�𗘗p����
	�t�@�C���A�b�v���[�h�w���p�[
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
	/** �t�H�[���f�[�^�̑��M */
	void AddFormData(LPCTSTR,LPCTSTR);
	/** �t�H�[������t�@�C���̃A�b�v���[�h */
	void AddFormFile(LPCTSTR,LPCTSTR,LPCTSTR,LPCTSTR);
	/** �A�b�v���[�h�����������R�~�b�g���� */
	void Commit();
	CString GetHeader() const;
	/** ���f�[�^�|�C���^���擾 */
	LPVOID GetData() const;
	/** ���f�[�^�����擾 */
	int GetDataLength() const;
private:
	/** ��؂�(BOUNDARY)���o�� */
	void AddBoundary();
	/** HTTP�w�b�_���擾 */
	virtual CString GetHeaderHelper() const;
	/**	MIME�u���b�N�̋�؂���擾 */
	virtual CString GetBoundaryHelper() const;
};
