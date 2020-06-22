#include "stdafx.h"
#include "resource.h"
#include "RenSubStageCmd.h"

#include "model/stage/SubStage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

RenSubStageCmd::RenSubStageCmd(SubStage* subStage,LPCTSTR ptsz) :
	Cmd(IDCMD_REN_SUB_STAGE),
	m_subStage(subStage),
	m_strNew(ptsz),
	m_strOld(subStage->GetStageName()) {
}

void RenSubStageCmd::Do() {
	ASSERT_VALID(m_subStage);
	m_subStage->SetStageName(m_strNew);
}

void RenSubStageCmd::Undo() {
	ASSERT_VALID(m_subStage);
	m_subStage->SetStageName(m_strOld);
}
