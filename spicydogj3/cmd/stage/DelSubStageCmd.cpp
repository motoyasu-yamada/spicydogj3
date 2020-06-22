#include "stdafx.h"
#include "resource.h"
#include "DelSubStageCmd.h"
#include "model/stage/SubStages.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DelSubStageCmd::DelSubStageCmd(SubStages* subStages,int pos) 
:	Cmd(IDCMD_DEL_SUB_STAGE),
	m_owners(subStages->m_stagesSub),
	m_pos(pos) {
}

void DelSubStageCmd::Do() {
	m_keepers.InheritedFrom(m_owners,m_pos,1);
}

void DelSubStageCmd::Undo() {
	m_keepers.TransferTo(m_owners,m_pos);
}

