#pragma once

/** Undo / Redo �T�|�[�g�ׂ̈̒��ۊ��N���X */
class Cmd {
private:
	CString m_name;
	int     m_iconIndex;
protected:
	Cmd(UINT resourceID);
public:
	virtual void Do()   = 0;
	virtual void Undo() = 0;
	virtual ~Cmd();
	/** �R�}���h�� */
	CString GetName() const;
	/** �A�C�R���摜�̃��\�[�XID */
	int GetIconIndex() const;
	/** @param resid - -1:     ���݂� name�������t�H�[�}�b�g�Ƃ��Ďg�p
	                   -1�ȊO: �w�肵��ID�̕����񃊃\�[�X�������t�H�[�}�b�g�Ƃ��Ďg�p */
	void SetCmdName(UINT resid,...);
};
