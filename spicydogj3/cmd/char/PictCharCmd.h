#pragma once

#include "cmd/Cmd.h"
#include "model/char/PictChar.h"

/**
	�s�N�`���[�̃Z���^�����O�ݒ�̕ύX�R�}���h
*/
class PictSetCenterCmd : public Cmd {
private:
	PictChar* m_pPictChar;
	CPoint m_ptCenterNew;
	CPoint m_ptCenterOld;
public:
	PictSetCenterCmd(PictChar*,CPoint ptNew);
	virtual void Do();
	virtual void Undo();
};


/**
	���o�œK���ݒ��ύX
*/
class PictSetOptParamCmd : public Cmd {
private:
	PictChar* m_pPictChar;
	PictQuantizeParam m_paramOld;
	PictQuantizeParam m_paramNew;
public:
	/**
		- �p�X�X���[�ȊO�˃p�X�X���[�ւ̐ݒ�ύX�͕s��
	*/
	PictSetOptParamCmd(PictChar*,const PictQuantizeParam&);
	virtual void Do();
	virtual void Undo();
};


/**
	(�Ώۑ���)
	�h�b�g�`��,���T�C�Y,�g����,�����[�h,�O���G�f�B�b�g,�G�t�F�N�g
*/
class PictModDataCmd : public Cmd {
private:
	PictChar* m_pPictChar;
	CPoint m_ptCenterOld;
	PictData m_dataNew;
	PictData m_dataOld;
public:
	/**
		@param p    �p�X�X���[���o�ݒ�̃s�N�`���͕ύX�ł��Ȃ�
		@param data �ύX��̃f�[�^
	*/
	PictModDataCmd(UINT resource,PictChar* p,const PictData& data);
	virtual void Do();
	virtual void Undo();
};