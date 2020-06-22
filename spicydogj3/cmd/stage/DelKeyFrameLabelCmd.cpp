#include "stdafx.h"
#include "resource.h"
#include "DelKeyFrameLabelCmd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DelKeyFrameLabelCmd::DelKeyFrameLabelCmd(KeyFrame* key)
:	Cmd(IDCMD_DEL_KEY_FRAME_LABEL),
	m_key(key) {
	ASSERT_VALID(m_key);
	m_key->GetLabel(&m_old);
}

void DelKeyFrameLabelCmd::Do() {
	ASSERT_VALID(m_key);
	KeyFrameLabel label;
	label.useLabel = FALSE;
	m_key->SetLabel(&label);
	ASSERT_VALID(m_key);
}

void DelKeyFrameLabelCmd::Undo() {
	ASSERT_VALID(m_key);
	m_key->SetLabel(&m_old);
	ASSERT_VALID(m_key);
}

