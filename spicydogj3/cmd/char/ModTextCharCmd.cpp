#include "stdafx.h"
#include "resource.h"
#include "ModTextCharCmd.H"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

ModTextCharCmd::ModTextCharCmd(TextChar* pCharSrc,const TextCharState& state)
:	Cmd(IDCMD_MOD_TEXT_CHAR),
	m_pText(pCharSrc),
	m_stateNew(state)
{
	m_pText->GetState(m_stateOld);
}

void ModTextCharCmd::Do() {
	m_pText->SetState(m_stateNew);
}

void ModTextCharCmd::Undo() {
	m_pText->SetState(m_stateOld);
}
