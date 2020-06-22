#include "stdafx.h"
#include "resource.h"
#include "InsSubStageCmd.h"

#include "model/stage/SubStages.h"
#include "model/stage/SubStage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

InsSubStageCmd::InsSubStageCmd(SubStages* stages,int pos,SubStage* stage) :
	Cmd(IDCMD_INS_SUB_STAGE),
	m_owners(stages->m_stagesSub),
	m_pos(pos) {
	ASSERT_VALID(stage);
	m_keepers.OwnNew(stage);
}

void InsSubStageCmd::Do() {
	ASSERT_VALID(&m_owners);
	m_keepers.TransferTo(m_owners,m_pos);
	ASSERT_VALID(&m_owners);
}

void InsSubStageCmd::Undo() {
	ASSERT_VALID(&m_owners);
	m_keepers.InheritedFrom(m_owners,m_pos,1);
	ASSERT_VALID(&m_owners);
}

