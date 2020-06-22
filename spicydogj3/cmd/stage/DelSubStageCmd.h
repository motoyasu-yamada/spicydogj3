#pragma once
#include "cmd/Cmd.h"
#include "sdom/OLM.h"

class SubStages;

/** �X�e�[�W�̍폜 */
class DelSubStageCmd : public Cmd {
private:
	COLMOwners& m_owners;
	COLMKeepers m_keepers;
	int         m_pos;

public:
	DelSubStageCmd(SubStages* subStages,int pos);
	virtual void Do();
	virtual void Undo();
};
