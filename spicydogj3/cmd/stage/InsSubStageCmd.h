#pragma once
#include "cmd/Cmd.h"
#include "sdom/OLM.h"

class SubStages;
class SubStage;

class InsSubStageCmd : public Cmd {
private:
	COLMOwners& m_owners;
	COLMKeepers m_keepers;
	int m_pos;

public:
	InsSubStageCmd(SubStages*,int pos,SubStage* pTimeline);
	virtual void Do();
	virtual void Undo();
};