
#include "stdafx.h"
#include "resource.h"
#include "ModBtnCharCmd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

ModBtnCharCmd::ModBtnCharCmd
(
	BtnChar*            btnChar,
	const BtnCharState& stateNew,
	const ActionList&   actionListNew
) : Cmd(IDCMD_MOD_BTN_CHAR) {
	
	ASSERT_VALID(btnChar);
	ASSERT_POINTER(&stateNew,BtnCharState);
	ASSERT_VALID(&actionListNew);

	btnChar->GetState(&m_stateOld);
	m_actionListOld.CopyFrom(&btnChar->actions);
	
	m_char          = btnChar;
	m_stateNew      = stateNew;
	m_actionListNew.CopyFrom(&actionListNew);
}

void ModBtnCharCmd::Do() {
	m_char->SetState(&m_stateNew);
	m_char->actions.CopyFrom(&m_actionListNew);
}

void ModBtnCharCmd::Undo() {
	m_char->SetState(&m_stateOld);
	m_char->actions.CopyFrom(&m_actionListOld);
}
