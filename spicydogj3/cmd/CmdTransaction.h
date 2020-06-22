#pragma once

#include "CmdSet.h"

/**
	�����̏������܂Ƃ߂ăR�}���h������
	CommandBag
		�S�Ă̏������ꊇ�ADo/Uno/Redo
	CommandTransaction
		���[�U�̑���ɉ����Ċe�������e��������Do
		�ꊇUndo
		�ꊇRedo
*/
class CmdTransaction : public CmdSet {
private:
	BOOL m_commited;
	BOOL m_firstDo;
public:
	CmdTransaction(UINT resource);
	virtual void AddCommand(Cmd*);
	virtual void Commit();
	virtual void Do();
	virtual void Undo();
};
