#include "stdafx.h"
#include "resource.h"
#include "InsLayerCmd.h"
#include "model/stage/Stage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


InsLayerCmd::InsLayerCmd(
	Stage* stage,
	int pos,
	Layer* layer) :
	Cmd(IDCMD_INS_LAYER){

	m_stage = stage;
	m_pos   = pos;
	m_keepers.OwnNew(layer);
	ASSERT_VALID(m_stage);
}

void InsLayerCmd::Do() {
	ASSERT_VALID(m_stage);
	m_keepers.TransferTo(m_stage->GetOwners(),m_pos);
	ASSERT_VALID(m_stage);
}

void InsLayerCmd::Undo() {
	ASSERT_VALID(m_stage);
	m_keepers.InheritedFrom(m_stage->GetOwners(),m_pos,1);
	ASSERT_VALID(m_stage);
}

