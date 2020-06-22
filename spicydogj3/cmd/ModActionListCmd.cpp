#include "stdafx.h"
#include "resource.h"
#include "ModActionListCmd.h"

#if defined(RUN_UNITTEST)

#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

ModActionListCmd::ModActionListCmd(ActionList* actionList,const ActionList* newActionList)
:	Cmd(IDCMD_MOD_ACTION_LIST) {
	m_actionList = actionList;
	m_oldActionList.CopyFrom(m_actionList);
	m_newActionList.CopyFrom(newActionList);
}

void ModActionListCmd::Do() {
	ASSERT_VALID(m_actionList);
	m_actionList->CopyFrom(&m_newActionList);
	ASSERT_VALID(m_actionList);
}

void ModActionListCmd::Undo() {
	ASSERT_VALID(m_actionList);
	m_actionList->CopyFrom(&m_oldActionList);
	ASSERT_VALID(m_actionList);
}
