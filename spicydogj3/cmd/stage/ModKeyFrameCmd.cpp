#include "stdafx.h"
#include "resource.h"
#include "ModKeyFrameCmd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


ModKeyFrameCmd::ModKeyFrameCmd(
	KeyFrame* key,
	const KeyFrameState* newState) : 
	Cmd(IDCMD_MOD_KEY_FRAME) {
	m_key = key;
	m_new = *newState;
	key->GetState(&m_old);
}

void ModKeyFrameCmd::Do() {
	ASSERT_VALID(m_key);
	m_key->SetState(&m_new);
	ASSERT_VALID(m_key);
}
void ModKeyFrameCmd::Undo() {
	ASSERT_VALID(m_key);
	m_key->SetState(&m_old);
	ASSERT_VALID(m_key);
}
