#pragma once

#include <afxtempl.h>

/// �A�N�Z�����[�^�e�[�u���T�C�Y���擾
int GetAcceleratorTableSize(HACCEL haccel);

/// �A�N�Z�����[�^�n���h�����b�p�[�N���X
class Accel {
private:
	HACCEL m_haccel;
public:
	Accel();
	~Accel();
	BOOL Attach(HACCEL);
	HACCEL Detach();
	BOOL DeleteAccel();
	HACCEL GetAccel() const;
}; // class Accel

/**
	�A�N�Z�����[�^�e�[�u�����}�[�W����
*/
class MultiAccel {
private:
	/// �}�[�W�σA�N�Z�����[�^
	Accel m_accelMerged;
	/// �}�[�W����K�v�����邩�ǂ���
	BOOL m_bToMerge;
	/// �}�[�W�}�b�v
	CArray<HACCEL,HACCEL&> m_accelMap;
public:
	MultiAccel();
	/// �A�N�Z���ݒ�
	void SetAccel(int nIndex,HACCEL);
	/// �}�[�W�σA�N�Z�����[�^���쐬����
	HACCEL GetMergedAccel();
private:
	/// �}�[�W�����A�N�Z�����[�^�n���h�����쐬����
	void CreateAccel();
}; // class AccelMerge


/**
	�A�N�Z�����[�^�I�[�i�[
*/
class AccelOwner {
private:
	HACCEL m_haccel;
public:
	AccelOwner();
	virtual ~AccelOwner();
	virtual HACCEL GetDefaultAccelerator();
	void SetAccelerators(UINT);
	void SetAccelerators(HACCEL);
}; // class AccelOwner


