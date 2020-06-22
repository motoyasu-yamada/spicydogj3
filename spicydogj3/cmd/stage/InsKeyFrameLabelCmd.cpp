#include "stdafx.h"
#include "resource.h"
#include "InsKeyFrameLabelCmd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

InsKeyFrameLabelCmd::InsKeyFrameLabelCmd (
	KeyFrame*      key,
	const KeyFrameLabel* keyFrameLabel) : 
	Cmd(IDCMD_INS_KEY_FRAME_LABEL) {
	m_key = key;
	m_new = *keyFrameLabel;
	m_key->GetLabel(&m_old);
}

void InsKeyFrameLabelCmd::Do() {
	ASSERT_VALID(m_key);
	m_key->SetLabel(&m_new);
	ASSERT_VALID(m_key);
}

void InsKeyFrameLabelCmd::Undo() {
	ASSERT_VALID(m_key);
	m_key->SetLabel(&m_old);
	ASSERT_VALID(m_key);
}
