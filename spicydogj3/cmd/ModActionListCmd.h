#pragma once

#include "cmd/Cmd.h"
#include "model/action/ActionList.h"

/**
	ModActionListCmd
	�A�N�V�����ꗗ�ɑ΂���ꊇ�ύX
*/
class ModActionListCmd : public Cmd {
private:
	ActionList* m_actionList;
	ActionList  m_newActionList;
	ActionList  m_oldActionList;
public:
	ModActionListCmd(ActionList*,const ActionList* newActionList);
private:
	virtual void Do();
	virtual void Undo();
};