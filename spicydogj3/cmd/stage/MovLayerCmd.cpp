#include "stdafx.h"
#include "resource.h"
#include "MovLayerCmd.h"
#include "model/stage/Stage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

MovLayerCmd::MovLayerCmd(Stage* stage,int dst,int src) : 
	Cmd(IDCMD_MOD_KEY_FRAME_LABEL) {
	m_stage  = stage;
	m_dstPos = dst;
	m_srcPos = src;
	m_noMove = (src == dst);
	ASSERT_VALID(m_stage);
}

void MovLayerCmd::Do() {
	ASSERT_VALID(m_stage);
	if (!m_noMove) {
		m_stage->GetOwners().Move(m_dstPos,m_srcPos);	
	}
	ASSERT_VALID(m_stage);
}

void MovLayerCmd::Undo() {
	ASSERT_VALID(m_stage);
	if (!m_noMove) {
		m_stage->GetOwners().Move(m_srcPos,m_dstPos);
	}
	ASSERT_VALID(m_stage);
}
