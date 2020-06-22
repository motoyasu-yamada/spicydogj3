#pragma once

#include "cmd/Cmd.h"
#include "model/action/ActionList.h"

/**
	ModActionListCmd
	アクション一覧に対する一括変更
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