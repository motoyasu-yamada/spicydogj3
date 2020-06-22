#pragma once

#if defined(_MT)
#include <afxmt.h>
#endif

/**
	�t�ԍ����O�t���e�I�u�W�F�N�g
*/
class NamingRule {
private:
#if defined(_MT)
	mutable CCriticalSection m_cs;
#endif
	mutable LONG m_nNumber;
public:
	NamingRule();
	/// �V�K���O���擾���� @param nFmtID ���O�t�H�[�}�b�g�����񃊃\�[�X (��):"xxxx %d xxx"
	CString NewIncNumName(UINT nFmtID) const;
	/// �V�K���O���擾 @param ptszFmt ���O�t�H�[�}�b�g (��):"xxxx %d xxx"
	CString NewIncNumName(LPCTSTR ptszFmt) const;
	/// �����������O���擾
	CString DupIncNumName(LPCTSTR ptszOrg) const;
private:
	/// �V�K���O�ԍ����擾����
	LONG GetNewIncNumber() const;
};

