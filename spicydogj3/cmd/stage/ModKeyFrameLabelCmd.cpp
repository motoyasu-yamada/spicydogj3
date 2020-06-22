#include "stdafx.h"
#include "resource.h"
#include "ModKeyFrameLabelCmd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

ModKeyFrameLabelCmd::ModKeyFrameLabelCmd(
	KeyFrame* key,
	const KeyFrameLabel* keyFrameLabel) :
	Cmd(IDCMD_MOD_KEY_FRAME_LABEL)
{
	ASSERT_VALID(key);
	m_key = key;
	m_new = *keyFrameLabel;
	key->GetLabel(&m_old);
}

void ModKeyFrameLabelCmd::Do() {
	ASSERT_VALID(m_key);
	m_key->SetLabel(&m_new);
	ASSERT_VALID(m_key);
}

void ModKeyFrameLabelCmd::Undo() {
	ASSERT_VALID(m_key);
	m_key->SetLabel(&m_old);
	ASSERT_VALID(m_key);
}
